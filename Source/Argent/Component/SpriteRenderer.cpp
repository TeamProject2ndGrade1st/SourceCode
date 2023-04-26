#include "SpriteRenderer.h"
#include "../Graphic/Graphics.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Material.h"
#include "../Other/Helper.h"
#include "../Math/MathHelper.h"

namespace Argent::Component::Renderer
{
	SpriteRenderer::SpriteRenderer(const char* filePath):
		BaseRenderer("Sprite Renderer")
	{
		sprite = std::make_unique<Argent::Resource::Sprite::Sprite>();
		material = std::make_shared<Material::SpriteMaterial>(filePath);
		renderingPipeline = Argent::Graphics::RenderingPipeline::CreateDefaultSpritePipeline();
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
		sprite->UpdateVertexMap(transform->GetPosition() + offset, 
			transform->GetScale() * scale,
			texPos, transform->GetRotation().z, 
			material->texture->GetWidth(), material->texture->GetHeight(),
			material->color.color);
	}

	void SpriteRenderer::Render() const
	{
		auto cmdList = Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Sprite);
		Render(cmdList);
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
			ImGui::DragFloat3("Offset", &offset.x, 0.05f, -FLT_MAX, FLT_MAX);
			ImGui::DragFloat3("Scale", &scale.x, 0.05f, 0.0f, 10.0f);
			ImGui::DragFloat2("TexPos", &texPos.x, 0.05f, -FLT_MAX, FLT_MAX);
			ImGui::TreePop();
		}
	}

}
