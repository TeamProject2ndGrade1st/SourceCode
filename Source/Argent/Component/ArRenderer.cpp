#include "ArRenderer.h"
#include "../Resource/ArMaterial.h"
//��������̒u��

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
 * \brief �`��p�̊��N���X
 */
namespace Argent::Component::Renderer
{
	ArRenderer::ArRenderer(const std::string& name):
		ArComponent(name)
	{}

	ArRenderer::~ArRenderer()
	{

	}


	void ArRenderer::Render(ID3D12GraphicsCommandList* cmdList) const
	{
		renderingPipeline->SetOnCommandList(cmdList);
	}

	void ArRenderer::Render() const
	{
		ID3D12GraphicsCommandList* cmdList = Argent::Graphics::ArGraphics::Instance()->GetCommandList();
		Render(cmdList);
	}
}
