#pragma once
#include <d3d12.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "ArTexture.h"
#include "ArResource.h"
#include "../Component/ArColor.h"
#include "../Graphic/Dx12/ArConstantBuffer.h"

#include "../Other/CerealHelper.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>


namespace Argent::Material
{
	class ArMaterial
	{
	public:
		ArMaterial(const char* textureFilepath);

		/**
		 * \brief 
		 * \param cmdList 
		 * \param RootParameterIndex 
		 */
		void Render(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const;
#ifdef _DEBUG
		void DrawDebug();
#endif

		std::vector<std::shared_ptr<Texture::ArTexture>> textures;
		ArColor color;
		std::string name;
		DirectX::XMFLOAT4 ka{ 0.2f, 0.2f, 0.2f, 1.0f };
		DirectX::XMFLOAT4 kd{ 0.2f, 0.2f, 0.2f, 1.0f };
		DirectX::XMFLOAT4 ks{ 0.2f, 0.2f, 0.2f, 1.0f };
	};

	struct ArMeshMaterial
	{
	public:
		template<class T>
		void serialize(T& archive)
		{
			archive(name, textureNames, constant);
		}
		enum class TextureType
		{
			Diffuse,
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

			template<class T>
			void serialize(T& archive)
			{
				archive(color, ka, kd, ks, shininess);
			}
		};
		std::string name;

		static constexpr int NumTextures = static_cast<int>(TextureType::Max);
		std::shared_ptr<Argent::Texture::ArTexture> textures[NumTextures];
		std::string textureNames[NumTextures];
		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constant>> constantBuffer;
		Constant constant{};
		ArColor color;
		void CreateTexture(const char* filePath, TextureType type);

		void SetOnCommand(ID3D12GraphicsCommandList* cmdList, UINT cbIndex, UINT diffuseIndex, UINT normalIndex) const
		{
			//todo ‚È‚ñ‚Æ‚©”’
			constantBuffer->SetOnCommandList(cmdList, cbIndex);
			textures[static_cast<int>(TextureType::Diffuse)]->Render(cmdList, diffuseIndex);
			textures[static_cast<int>(TextureType::Normal)]->Render(cmdList, normalIndex);
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
}