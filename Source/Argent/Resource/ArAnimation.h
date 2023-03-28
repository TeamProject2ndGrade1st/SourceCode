#pragma once
#include "ArResource.h"

namespace Argent::Resource::Animation
{
	class ArAnimation final:
		public Resource::ArResource
	{
	public:
		ArAnimation():
			ArResource(ResourceType::Animation)
		{}

	private:
		ArAnimation(uint64_t uniqueId, const char* name):
			ArResource(uniqueId, name, ResourceType::Animation)
		{}
	public:
		~ArAnimation() override = default;

		ArAnimation operator=(const ArAnimation& anim)
		{
			ArAnimation ret(anim.GetUniqueId(), anim.GetName());
			ret.samplingRate = anim.samplingRate;
			ret.sequence = anim.sequence;

			return ret;
		}

		float samplingRate{};

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
			};
			std::vector<Node> nodes{};
		};
		std::vector<Keyframe> sequence{};
	};
}
