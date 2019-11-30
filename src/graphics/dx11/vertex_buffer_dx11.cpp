
#include "stdafx.h"
#include "vertex_buffer.h"
#include "../../core/utility/windows_error.h"

using crib::graphics::dx11::vertex_buffer;
using crib::core::utility::throw_if_failed;


vertex_buffer::vertex_buffer(const crib::geometry::mesh& mesh, ID3D11Device* dev)
{
	D3D11_SUBRESOURCE_DATA initData = {};

	initData.pSysMem = mesh.vertex_data();
	throw_if_failed(dev->CreateBuffer(&CD3D11_BUFFER_DESC(mesh.vertex_size_bytes(), D3D11_BIND_VERTEX_BUFFER), &initData, &vertex), "Create vertex buffer");

	initData.pSysMem = mesh.index_data();
	throw_if_failed(dev->CreateBuffer(&CD3D11_BUFFER_DESC(mesh.index_size_bytes(), D3D11_BIND_INDEX_BUFFER), &initData, &index), "Create index buffer");

	vertex_stride = mesh.vertex_stride();
	idx_count = mesh.index_count();
}

void vertex_buffer::draw(ID3D11DeviceContext2* ctx)
{
	UINT stride = vertex_stride, offset = 0;

	ctx->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
	ctx->IASetIndexBuffer(index, DXGI_FORMAT_R16_UINT, 0);
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ctx->DrawIndexed(idx_count, 0, 0);
}
