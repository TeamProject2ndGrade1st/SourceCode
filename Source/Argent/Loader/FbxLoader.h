#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "../Resource/ArTexture.h"

namespace Argent::Component
{
	class ArComponent;
}

namespace Argent::Loader
{
	namespace Fbx
	{
		struct ArFbxScene;

		struct ArBoneInfluence
		{
			uint32_t boneIndex;
			float boneWeight;
		};

		struct ArAnimation
		{
			std::string name;
			float samplingRate{};

			struct ArKeyframe
			{
				struct Node
				{
					DirectX::XMFLOAT4X4 globalTransform
					{
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					};

					DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
					DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 };
					DirectX::XMFLOAT3 translation{ 0, 0, 0 };
				};
				std::vector<Node> nodes;
			};
			std::vector<ArKeyframe> sequence;
		};

		struct ArSubset
		{
			uint64_t materialUniqueId{};
			uint32_t startIndexLocation{};
			uint32_t indexCount{};
		};

		struct ArMaterial
		{
			enum class TextureType
			{
				Diffuse,
				Specular,
				Ambient,
				Normal,
				Height,
				Max,
			};
			struct Constant
			{
				DirectX::XMFLOAT4 ka{ 0.2f, 0.2f, 0.2f, 1.0f };
				DirectX::XMFLOAT4 kd{ 0.2f, 0.2f, 0.2f, 1.0f };
				DirectX::XMFLOAT4 ks{ 0.2f, 0.2f, 0.2f, 1.0f };
				float shininess = 128;
			};
			std::string name;

			Constant constant{};
			void CreateTexture(const char* filePath, TextureType type);
			std::shared_ptr<Argent::Texture::ArTexture> textures[static_cast<int>(TextureType::Max)];
		};


		Argent::Component::ArComponent* LoadFbx(const char* filePath, bool triangulate = false);
		

	}
}
