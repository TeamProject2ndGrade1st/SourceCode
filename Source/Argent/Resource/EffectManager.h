#pragma once
#include "Effekseer.h"
#include "EffekseerRendererDX12.h"
//#include "../EffekseerRendererDX12/EffekseerRendererDX12.h"
#include "Effect.h"

namespace Argent::Resource::Effect
{
	class EffectManager
	{
	public:

		EffectManager(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, UINT numBackBuffers);


	public:
		~EffectManager() = default;

		void Update();
		void Render() const;

		Effekseer::ManagerRef GetManager() const { return efkManager;  }  // NOLINT(modernize-use-nodiscard)
		EffekseerRenderer::RendererRef GetRenderer() const { return efkRenderer;  }  // NOLINT(modernize-use-nodiscard)

		static EffectManager* Instance() { return instance;  }
		
	private:
		EffekseerRenderer::RendererRef efkRenderer{};
		Effekseer::ManagerRef efkManager{};
		Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> efkMemoryPool;
		Effekseer::RefPtr<EffekseerRenderer::CommandList> efkCmdList{};

		static EffectManager* instance;
	};
}
