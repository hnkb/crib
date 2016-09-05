
#include "stdafx.h"
#include "model.h"

using crib::scene::hello3d::model;
using namespace DirectX;


model model::cube()
{
	// To create cube programatically:
	//    define one face (here with a,b,c,d vertices and two triangles)
	//    here in this face z value is the same for all, so
	//    rotate it around X and Y axis to generate other faces

	XMVECTOR a = XMVectorSet(-.5f,  .5f, .5f, 1.f);
	XMVECTOR b = XMVectorSet( .5f,  .5f, .5f, 1.f);
	XMVECTOR c = XMVectorSet( .5f, -.5f, .5f, 1.f);
	XMVECTOR d = XMVectorSet(-.5f, -.5f, .5f, 1.f);

	XMVECTOR n = XMVectorSet(0.f, 0.f, 1.f, 1.f);

	unsigned short idx[] = { 0,1,2 , 0,2,3 };


	std::vector<pipeline::vertex_format> vertices;
	std::vector<unsigned short> indices;

	auto add_face = [&](const XMMATRIX& M)
	{
		unsigned short offset = (unsigned short)vertices.size();
		for (auto&t : idx) indices.push_back(t + offset);

		XMFLOAT3 normal(XMVector4Transform(n, M).m128_f32);

		vertices.push_back({ XMFLOAT3(XMVector4Transform(a, M).m128_f32), XMFLOAT3(.8f, .7f, .5f), normal });
		vertices.push_back({ XMFLOAT3(XMVector4Transform(b, M).m128_f32), XMFLOAT3(.6f, .6f, .6f), normal });
		vertices.push_back({ XMFLOAT3(XMVector4Transform(c, M).m128_f32), XMFLOAT3(.1f, .9f, .9f), normal });
		vertices.push_back({ XMFLOAT3(XMVector4Transform(d, M).m128_f32), XMFLOAT3(1.f, 1.f, 1.f), normal });
	};


	// 0, 90, 180, 270 degrees rotation around Y
	add_face(XMMatrixRotationY(-XM_PIDIV2));
	add_face(XMMatrixIdentity());
	add_face(XMMatrixRotationY(XM_PIDIV2));
	add_face(XMMatrixRotationY(XM_PI));

	// 90, 270 degrees rotation around X (180 around X is same face as 180 around Y)
	add_face(XMMatrixRotationX(-XM_PIDIV2));
	add_face(XMMatrixRotationX(XM_PIDIV2));


	return model(std::move(vertices), std::move(indices));
}
