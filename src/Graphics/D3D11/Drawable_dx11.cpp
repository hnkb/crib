
#include <Crib/Graphics/Direct3D11.h>

using Crib::Graphics::D3D11::Drawable;
using Crib::Platform::Windows::ThrowOnFail;


Drawable::Drawable(const Graphics::Drawable& drawable, ID3D11Device* dev)
{
	D3D11_SUBRESOURCE_DATA initData = {};

	initData.pSysMem = drawable.getVertexData();
	ThrowOnFail(dev->CreateBuffer(&CD3D11_BUFFER_DESC(drawable.getVertexBytes(), D3D11_BIND_VERTEX_BUFFER), &initData, &vertex), "Create vertex buffer");

	initData.pSysMem = drawable.getIndexData();
	ThrowOnFail(dev->CreateBuffer(&CD3D11_BUFFER_DESC(drawable.getIndexBytes(), D3D11_BIND_INDEX_BUFFER), &initData, &index), "Create index buffer");

	getVertexStride = drawable.getVertexStride();
	idxCount = drawable.getIndexCount();
}

void Drawable::draw(ID3D11DeviceContext2* ctx)
{
	UINT stride = getVertexStride, offset = 0;

	ctx->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
	ctx->IASetIndexBuffer(index, DXGI_FORMAT_R16_UINT, 0);
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ctx->DrawIndexed(idxCount, 0, 0);
}
