
#include "Context.h"
#include <glad/glad.h>
#include <algorithm>
#include <vector>
#include <map>

#include "../Text-Layout/Font.h"

using namespace crib;
using Graphics::OpenGL::Context;


extern Graphics::Camera camera;
extern std::string text;


GLuint theProgram = 0;


GLuint CreateShader(GLenum eShaderType, const std::string& strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char* strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch (eShaderType)
		{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint>& shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}



const std::string strVertexShader(
	"#version 330 core\n"
	"layout(location = 0) in vec2 position;"
	"uniform vec2 offset;"
	"uniform vec2 scale;"
	"void main()"
	"{"
	"   gl_Position = vec4((position+offset)*scale, 0, 1.0);"
	"}");

const std::string strFragmentShader(
	"#version 330 core\n"
	"out vec4 outputColor;"
	"void main()"
	"{"
	//"   outputColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
	"}");

void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

//GLuint positionBufferObject;
//GLuint vao;



class Buffer
{
public:
	GLuint vertex;
	GLuint index;
} buffer;


struct DrawableObject
{
	void* start;
	GLsizei numPoints;
	GLuint objectId;
};
std::map<wchar_t, DrawableObject> objects;


Graphics::ModelPack fontMesh;


void InitializeVertexBuffer()
{
	fontMesh = Graphics::loadMeshFromTTF(
		std::filesystem::current_path().parent_path().parent_path()
		/ "assets/Overpass-Bold.ttf");


	//////////////////// create buffers and copy data //////////////////////////

	glGenBuffers(1, &buffer.vertex);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.vertex);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(fontMesh.buffer.vertex[0]) * fontMesh.buffer.vertex.size(),
		fontMesh.buffer.vertex.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &buffer.index);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.index);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(fontMesh.buffer.index[0]) * fontMesh.buffer.index.size(),
		fontMesh.buffer.index.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	////////////////////// create objects for each glyph ///////////////////////

	for (auto& mesh: fontMesh.meshes)
	{
		auto& obj = objects[mesh.first];
		obj.numPoints = mesh.second.indexCount;
		obj.start = (void*)(mesh.second.startIndex * sizeof(GLushort));

		auto vertOffset = (void*)(mesh.second.startVertex * sizeof(fontMesh.buffer.vertex[0]));

		glGenVertexArrays(1, &obj.objectId);
		glBindVertexArray(obj.objectId);

		glBindBuffer(GL_ARRAY_BUFFER, buffer.vertex);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertOffset);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.index);

		glBindVertexArray(0);
	}
}

GLint offsetLocation;
GLint scaleLocation;

void initGL_3()
{
	InitializeProgram();
	InitializeVertexBuffer();

	offsetLocation = glGetUniformLocation(theProgram, "offset");
	scaleLocation = glGetUniformLocation(theProgram, "scale");
}



struct Entity
{
	int modelId;  // or uint64_t or void*

	// model transformation
	float2 pos;
	// float scale;

	// TODO:
	//  - proper trafo matrix
	//  - hierarchy
	//    - with caching (isDirty)
	//    - with flattening
};

std::vector<Entity> scene;

void updateScene(const std::string& text)
{
	static std::string cachedText;
	if (cachedText == text)
		return;

	scene.clear();
	cachedText = text;


	float xStart = -2;
	float2 textPos = { xStart, -xStart - .9f };

	for (auto symbol: text)
	{
		if (symbol == ' ')
			textPos.x += .2f;
		else if (symbol == '\n' || symbol == '\r')
		{
			textPos.x = xStart;
			textPos.y -= 1.1f;
		}
		else
		{
			auto& e = scene.emplace_back();

			e.modelId = fontMesh.meshes.find(symbol) == fontMesh.meshes.end() ? 175 : symbol;
			e.pos = textPos;
			textPos.x += fontMesh.meshes.at(e.modelId).font.advance;
		}
	}
}


#include <crib/File>

void Context::onResize(int2 dims)
{
	glViewport(0, 0, dims.x, dims.y);
	camera.setViewport(dims);
}

void Context::readDeviceDescription(int swapInterval)
{
	std::string profile;
	{
		GLint mask;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);
		if (mask & GL_CONTEXT_CORE_PROFILE_BIT)
			profile = "Core";
		else if (mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
			profile = "Compatibility";
	}

	std::string sync;
	{
		if (swapInterval == 1)
			sync = " (V-Sync)";
		else if (swapInterval == -1)
			sync = " (adaptive sync)";
	}

	description =
		std::string("GL ") + (char*)glGetString(GL_VERSION) + "  " + profile + "  GLSL "
		+ (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) + "  |  "
		+ (char*)glGetString(GL_RENDERER) + sync;
}

void Context::drawPlatformIndependent()
{
	if (theProgram == 0)
		initGL_3();

	//File txt(__FILE__, "rb");
	//char buffer[5000];
	//fread(buffer, 1, sizeof(buffer),txt);
	
	//updateScene(__FILE__);
	//updateScene(buffer);
	updateScene(text + "_");


	glClearColor(0.6f, 0.2f, 0.15f, 0.0f);
	//glClearColor(1.f, 0.75f, 0.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

	float2 scale = camera.scaleWithAR();
	glUniform2fv(scaleLocation, 1, (float*)&scale);

	for (auto e: scene)
	{
		auto& obj = objects.at(e.modelId);
		auto pos = camera.view.offset + e.pos;
		glBindVertexArray(obj.objectId);
		glUniform2fv(offsetLocation, 1, (float*)&pos);
		glDrawElements(GL_TRIANGLES, obj.numPoints, GL_UNSIGNED_SHORT, obj.start);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
