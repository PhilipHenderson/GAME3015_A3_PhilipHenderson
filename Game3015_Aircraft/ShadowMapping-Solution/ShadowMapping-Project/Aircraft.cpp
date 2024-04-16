#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
	, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "skullMat";
		break;
	case (Raptor):
		mSprite = "mirror0";
		break;
	default:
		mSprite = "skullMat";
		break;
	}
}

void Aircraft::drawCurrent() const
{	
	game->getCommandList()->SetPipelineState(game->mPSOs["opaque"].Get());

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

void Aircraft::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	XMStoreFloat4x4(&renderer->World, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["skullGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["skull"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["skull"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["skull"].BaseVertexLocation;

	game->mRitemLayer[(int)RenderLayer::Opaque].push_back(render.get());

	game->getRenderItems().push_back(std::move(render));
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case Eagle:
		return Category::PlayerAircraft;

	default:
		return Category::EnemyAircraft;
	}
}
