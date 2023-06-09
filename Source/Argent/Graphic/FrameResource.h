#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_5.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "Dx12/Descriptor.h"
#include "Dx12/Command.h"

#include "SceneConstant.h"


namespace Argent::Graphics
{
	enum class RenderType
	{
		Sprite,
		Mesh,
		PostRendering,
		Count
	};

	class FrameResource
	{
	public:
		FrameResource(ID3D12Device* device, IDXGISwapChain3* swapChain, UINT backBufferIndex, 
		              Dx12::Descriptor* rtv, Dx12::Descriptor* dsv, Dx12::Descriptor* cbv, 
		              UINT NumCmdLists);
		~FrameResource() = default;
		FrameResource operator=(const FrameResource&) = delete;
		FrameResource operator=(const FrameResource&&) = delete;
		FrameResource(const FrameResource&) = delete;
		FrameResource(const FrameResource&&) = delete;

		[[nodiscard]] D3D12_RESOURCE_DESC GetBackBufferDesc() const { return backBuffer->GetDesc(); }

		[[nodiscard]] ID3D12GraphicsCommandList* GetCmdList(RenderType type) const { return cmdBundle.at(static_cast<int>(type))->cmdList.Get(); }

		DirectX::XMFLOAT4X4 GetSceneView() const { return cbScene->view;  }
		DirectX::XMFLOAT4X4 GetSceneProjection() const { return cbScene->projection;  }

		void Begin() const;
		void End() const;
		void SetRenderTarget(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect, float clearColor[4]);
		void UpdateSceneConstant(const SceneConstant& sceneConstant) const;
		void SetSceneConstant(UINT rootParameterIndex = 0);
		void SetBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;
		void Reset();
		void Terminate(Dx12::CommandQueue* cmdQueue);
		Dx12::Descriptor* GetDsv() const { return dsv;  }

		Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource;
		Dx12::Descriptor* rtv;
		Dx12::Descriptor* dsv;
		Dx12::Descriptor* cbv;
		UINT backBufferIndex;
		SceneConstant*	cbScene;
		uint64_t fenceValue;

		struct CommandQueue;
		void WaitForEvent(Dx12::CommandQueue* cmdQueue);
		std::vector<std::unique_ptr<Dx12::CommandBundle>> cmdBundle;
	public:
	};
}
			