#pragma once
#include "ArResource.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>



namespace Argent::Resource::Animation
{
	class AnimationClip final
		//public Resource::ArResource
	{
	public:
		AnimationClip()
			//ArResource(ResourceType::Animation)
		{}

	private:
		AnimationClip(uint64_t uniqueId, const char* name)
			//ArResource(uniqueId, name, ResourceType::Animation)
		{}
	public:
		~AnimationClip() = default;

		AnimationClip operator=(const AnimationClip& anim)
		{
			AnimationClip ret{};
			ret.samplingRate = anim.samplingRate;
			ret.sequence = anim.sequence;

			return ret;
		}

		float samplingRate{};
		std::string name;
		struct Keyframe
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

				template<class T>
				void serialize(T& archive)
				{
					archive(globalTransform, scaling, rotation, translation);
				}
			};
			std::vector<Node> nodes{};

			template<class T>
			void serialize(T& archive)
			{
				archive(nodes);
			}
		};
		std::vector<Keyframe> sequence{};

		template<class T>
		void serialize(T& archive)
		{
			archive(samplingRate, name, sequence);
		}
	};
}
