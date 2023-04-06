#include "Renderer.h"
#include "../Resource/Material.h"
#include "../Graphic/Graphics.h"
//todo 成瀬さんの置物

//using ID = size_t;
//
//inline ID GenerateID()
//{
//	static ID lastID{};
//	return lastID++;
//}
//
//template<typename>
//inline ID GetID()
//{
//	static ID id = GenerateID();
//	return id;
//}

/**
 * \brief 描画用の基底クラス
 */
namespace Argent::Component::Renderer
{
	BaseRenderer::BaseRenderer(const std::string& name):
		BaseComponent(name)
	{}

	BaseRenderer::~BaseRenderer()
	{

	}


	void BaseRenderer::Render(ID3D12GraphicsCommandList* cmdList) const
	{
		renderingPipeline->SetOnCommandList(cmdList);
	}

	void BaseRenderer::Render() const
	{
		ID3D12GraphicsCommandList* cmdList = Argent::Graphics::ArGraphics::Instance()->GetCommandList();
		Render(cmdList);
	}
}
