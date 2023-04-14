#include "SpriteRenderer.h"
#include "../Graphic/Graphics.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Material.h"
#include "../Other/Helper.h"

namespace Argent::Component::Renderer
{
	SpriteRenderer::SpriteRenderer(const char* filePath):
		BaseRenderer("Sprite Renderer")
	{
		sprite = std::make_unique<Argent::Resource::Sprite::Sprite>();
		material = std::make_shared<Material::SpriteMaterial>(filePath);
		renderingPipeline = Argent::Graphics::RenderingPipeline::CreateDefaultSpritePipeLine();
	}


	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{
		BaseRenderer::Initialize();
	}

	void SpriteRenderer::Finalize()
	{
		BaseRenderer::Finalize();
	}

	void SpriteRenderer::Begin()
	{
		BaseRenderer::Begin();
	}

	void SpriteRenderer::End()
	{
		BaseRenderer::End();
	}

	void SpriteRenderer::Update()
	{
		const Transform* transform = GetOwner()->GetTransform();

		//todo Center‚Ì’l‚ð‚Ç‚Á‚©‚Å’è‹`‚·‚é‚±‚Æ
		sprite->UpdateVertexMap(transform->GetPosition(), transform->GetScale(), DirectX::XMFLOAT2(), transform->GetRotation().z, 
			material->texture->GetWidth(), material->texture->GetHeight(),
			material->color.color);
	}

	void SpriteRenderer::Render(ID3D12GraphicsCommandList* cmdList) const
	{
		BaseRenderer::Render(cmdList);
		material->Render(cmdList, 0);
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		sprite->SetOnCommandList(cmdList);
		cmdList->DrawInstanced(4, 1, 0, 0);
		//sprite->DrawCall(cmdList, 4);
	}


	void SpriteRenderer::DrawDebug()
	{
		if(ImGui::TreeNode(GetName().c_str()))
		{
			BaseRenderer::DrawDebug();
			material->DrawDebug();
			ImGui::TreePop();
		}
	}

}
