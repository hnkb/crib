
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

model model::pyramid()
{
	// pyramid has a point on top (a), four at the base (b, c, d, e)

	XMFLOAT3 colors[] = {
		XMFLOAT3(.9f, .3f, .1f),
		XMFLOAT3(.8f, .7f, .5f),
		XMFLOAT3(.6f, .6f, .6f),
		XMFLOAT3(.1f, .9f, .9f)
	};

	std::pair<XMVECTOR, int> a(XMVectorSet( 0.f,  .5f,  0.f, 1.f), 0);
	std::pair<XMVECTOR, int> b(XMVectorSet(-.5f, -.5f, -.5f, 1.f), 1);
	std::pair<XMVECTOR, int> c(XMVectorSet( .5f, -.5f, -.5f, 1.f), 2);
	std::pair<XMVECTOR, int> d(XMVectorSet(-.5f, -.5f,  .5f, 1.f), 1);
	std::pair<XMVECTOR, int> e(XMVectorSet( .5f, -.5f,  .5f, 1.f), 3);


	std::vector<pipeline::vertex_format> vertices;
	std::vector<unsigned short> indices;

	auto add_triangle = [&](const std::vector<std::pair<XMVECTOR, int>> vv, const XMVECTOR n)
	{
		unsigned short offset = (unsigned short)vertices.size();
		unsigned short idx[] = { 0,1,2 };
		for (auto& i : idx) indices.push_back(i + offset);

		XMFLOAT3 normal(n.m128_f32);
		for (auto& v : vv) vertices.push_back({ XMFLOAT3(v.first.m128_f32), colors[v.second], normal });
	};


	add_triangle({ c,b,d }, XMVectorSet(0, -1.f, 0, 1.f));
	add_triangle({ c,d,e }, XMVectorSet(0, -1.f, 0, 1.f));
	add_triangle({ a,e,d }, XMVectorSet(                      0, XMScalarSin(XM_PIDIV4),  XMScalarCos(XM_PIDIV4), 1.f));
	add_triangle({ a,b,c }, XMVectorSet(                      0, XMScalarSin(XM_PIDIV4), -XMScalarCos(XM_PIDIV4), 1.f));
	add_triangle({ a,d,b }, XMVectorSet(-XMScalarCos(XM_PIDIV4), XMScalarSin(XM_PIDIV4),                       0, 1.f));
	add_triangle({ a,c,e }, XMVectorSet( XMScalarCos(XM_PIDIV4), XMScalarSin(XM_PIDIV4),                       0, 1.f));


	return model(std::move(vertices), std::move(indices));
}
