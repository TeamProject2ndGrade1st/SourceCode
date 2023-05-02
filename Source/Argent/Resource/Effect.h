#pragma once
#include "ArResource.h"
#include "Effekseer.h"
#include <DirectXMath.h>

namespace Argent::Resource::Effect
{
	class EffekseerResource:
		public Resource::ArImportedResource
	{
	public:
		EffekseerResource(const char* filePath, const char* materialPath);
		~EffekseerResource() override = default;

		void Play(const DirectX::XMFLOAT3& position, 
				const DirectX::XMFLOAT3& scale,
				const DirectX::XMFLOAT4& rotate,
			float startFrame = 0);
		void Update(const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& scale,
			const DirectX::XMFLOAT4& rotate,
			const DirectX::XMFLOAT4& color);
		void Stop() const;
		void Pause();
		bool IsExist() const;
	private:
		float startFrame;
		Effekseer::EffectRef effect{};
		Effekseer::Handle handle;
	};
	
}

