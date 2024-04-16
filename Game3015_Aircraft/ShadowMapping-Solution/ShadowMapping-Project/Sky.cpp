#include "Sky.hpp"
#include "Game.hpp"

Sky::Sky(Game* game) : Entity(game)
{
	
}

void Sky::drawCurrent() const
{	

	game->getCommandList()->SetPipelineState(game->mPSOs["sky"].Get());

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	auto objectCB = game->getCurrFrameResource()->ObjectCB->Resource();

	auto ri = renderer;
	auto vbv = ri->Geo->VertexBufferView();
	auto ibv = ri->Geo->IndexBufferView();

	game->getCommandList()->IASetVertexBuffers(0, 1, &vbv);
	game->getCommandList()->IASetIndexBuffer(&ibv);
	game->getCommandList()->IASetPrimitiveTopology(ri->PrimitiveType);

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;

	game->getCommandList()->SetGraphicsRootConstantBufferView(0, objCBAddress);

	game->getCommandList()->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);

}

void Sky::buildCurrent()
{

	auto skyRitem = std::make_unique<RenderItem>();
	renderer = skyRitem.get();
	XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f));
	renderer->TexTransform = MathHelper::Identity4x4();
	renderer->ObjCBIndex = 0;
	renderer->Mat = game->getMaterials()["sky"].get();
	renderer->Geo = game->getGeometries()["shapeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	renderer->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	renderer->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;

	game->getRenderItems().push_back(std::move(skyRitem));
}
