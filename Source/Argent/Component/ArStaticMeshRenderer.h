#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <memory>

#include <unordered_map>
#include "ArRenderer.h"
#include "../Graphic/Dx12/ArConstantBuffer.h"
#include "../Resource/ArTexture.h"
#include "../Resource/ArStaticMesh.h"

namespace Argent::Component::Renderer
{
	class ArStaticMeshRenderer:
//<<<<<<< HEAD
		public ArRenderer
	{
	public:
		enum class RootParameterIndex
		{
			cbScene,
			cbObject,
			cbMaterial,
			txAlbedo,
			txNormal,
		};

		struct Constants
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4 color;
		};

		struct Material
		{
			enum class TextureType
			{
				Diffuse,
				Specular,
				Ambient,
				Normal,
				Max
			};
			struct Constant
			{
				DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };
				DirectX::XMFLOAT4 ka{ 0.2f, 0.2f, 0.2f, 1.0f };
				DirectX::XMFLOAT4 kd{ 0.2f, 0.2f, 0.2f, 1.0f };
				DirectX::XMFLOAT4 ks{ 0.2f, 0.2f, 0.2f, 1.0f };
				float shininess = 128;
			};
			std::string name;

			static constexpr int NumTextures = static_cast<int>(TextureType::Max);
			std::shared_ptr<Argent::Texture::ArTexture> textures[NumTextures];
			std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constant>> constantBuffer;
			Constant constant{};
			ArColor color;
			void CreateTexture(const char* filePath, TextureType type);

			void SetOnCommand(ID3D12GraphicsCommandList* cmdList) const
			{
				//todo ‚È‚ñ‚Æ‚©”’
				constantBuffer->SetOnCommandList(cmdList, static_cast<UINT>(RootParameterIndex::cbMaterial));
				textures[static_cast<int>(TextureType::Diffuse)]->Render(cmdList, static_cast<UINT>(RootParameterIndex::txAlbedo));
				textures[static_cast<int>(TextureType::Normal)]->Render(cmdList, static_cast<UINT>(RootParameterIndex::txNormal));
			}

#ifdef _DEBUG
			void DrawDebug()
			{
				if (ImGui::TreeNode(name.c_str()))
				{
					ImGui::DragFloat3("Ka", &constant.ka.x, 0.001f, 0, 1.0f);
					ImGui::DragFloat3("Kd", &constant.kd.x, 0.001f, 0, 1.0f);
					ImGui::DragFloat3("Ks", &constant.ks.x, 0.001f, 0, 1.0f);
					ImGui::DragFloat("Shininess", &constant.shininess, 1.0f, 0, FLT_MAX);
					color.DrawDebug();
					ImGui::TreePop();
				}
			}
#endif
		};

	public:
		ArStaticMeshRenderer(ID3D12Device* device, const char* fileName, std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes,
			std::unordered_map<uint64_t, Material>& materials);
		~ArStaticMeshRenderer() override = default;

		void Render(ID3D12GraphicsCommandList* cmdList,
			const DirectX::XMFLOAT4X4& world) const;

		void CreateRootSignatureAndPipelineState();


		void Render() const override;
		void Update() override;

#ifdef _DEBUG
		void DrawDebug() override;
#endif

		void CreateComObject(ID3D12Device* device);

	private:
		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constants>> constantBuffer;

	protected:
		std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes;
		std::unordered_map<uint64_t, Material> materials;
		
	public:
	};
}


//=======
//		public ArRenderer
//	{
//	public:
//		ArStaticMeshRenderer();
//		~ArStaticMeshRenderer() override = default;
//
//		void SetOnCommandList() const override;
//
//	private:
//		std::shared_ptr<Argent::Resource::Mesh::ArStaticMesh> meshData;
//	};
//}
//>>>>>>> GinNote
