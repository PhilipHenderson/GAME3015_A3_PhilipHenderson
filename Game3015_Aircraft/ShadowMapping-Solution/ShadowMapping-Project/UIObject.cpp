#include "UIObject.hpp"
#include "Game.hpp"

UIObject::UIObject(std::string sprite, Game* game) : Entity(game), mSprite(sprite), isActive(true)
{
	
}

void UIObject::drawCurrent() const
{
	if (isActive)
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

}

void UIObject::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["shapeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["grid"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["grid"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["grid"].BaseVertexLocation;


	//Uncomment for shadows
	//game->mRitemLayer[(int)RenderLayer::Opaque].push_back(render.get());

	game->getRenderItems().push_back(std::move(render));
}

unsigned int UIObject::getCategory() const
{
	return Category::Scene;
}