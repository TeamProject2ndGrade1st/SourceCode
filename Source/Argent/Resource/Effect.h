#pragma once
#include "ArResource.h"
#include "Effekseer.h"
#include <DirectXMath.h>

namespace Argent::Resource::Effect
{
	class EffectResource:
		public Resource::ArImportedResource
	{
	public:
		EffectResource(const char* filePath, const char* materialPath);
		~EffectResource() override = default;

		void Play(const DirectX::XMFLOAT3& position, 
				const DirectX::XMFLOAT3& scale, 
				const DirectX::XMFLOAT4& rotate, float startFrame = 0);
		void Update(const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& scale,
			const DirectX::XMFLOAT4& rotate,
			const DirectX::XMFLOAT4& color);
		void Stop() const;
		bool IsExist() const;
	private:

		Effekseer::EffectRef effect{};
		Effekseer::Handle handle;
	};
	
}
