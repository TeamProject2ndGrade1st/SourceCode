#pragma once
#include <d3d12.h>
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Texture.h"
#include "ArResource.h"
#include "../Component/Color.h"
#include "../Graphic/Dx12/ConstantBuffer.h"

#include "../Other/CerealHelper.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

#include "ArResource.h"


namespace Argent::Material
{
	class SpriteMaterial
	{
	public:
		SpriteMaterial(const char* textureFilepath);

		/**
		 * \brief 
		 * \param cmdList 
		 * \param RootParameterIndex 
		 */
		void Render(ID3D12GraphicsCommandList* cmdList, UINT RootParameterIndex) const;

		void DrawDebug();


		std::shared_ptr<Resource::Texture> texture;
		Color color;
		std::string name;
	};


	struct ReplaceFileName
	{
		std::string diffuse;
		std::string normal;
	};


	class MeshMaterial:
		public Resource::ArResource
	{
	public:
		MeshMaterial(const char* name):
			ArResource(name, ResourceType::Mesh)
		{}

		MeshMaterial():
			ArResource("Material", ResourceType::Material)
		{}
		template<class T>
		void serialize(T& archive)
		{
			archive(name, textureNames, constant);
		}
		enum class TextureUsage
		{
			Diffuse,
			Normal,
			Max
		};

		struct Constant
		{
			DirectX::XMFLOAT4 color{ 1, 1, 1, 1 };
			DirectX::XMFLOAT3 emissiveColor{ 1, 1, 1 };
			float emissivePower = 0.0f;
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

		static constexpr int NumTextures = static_cast<int>(TextureUsage::Max);
		uint64_t textureUniqueId[NumTextures];
		std::string textureNames[NumTextures];
		std::unique_ptr<Argent::Dx12::ConstantBuffer<Constant>> constantBuffer{};
		Constant constant{};
		Color color;
		void CreateTexture(const char* filePath, TextureUsage type);

		void SetOnCommand(ID3D12GraphicsCommandList* cmdList, UINT cbIndex, UINT diffuseIndex, UINT normalIndex) const;

		void DrawDebug();
		

		ReplaceFileName replace;
	};
}
