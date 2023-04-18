#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_5.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "Dx12/Descriptor.h"
#include "Dx12/Command.h"


namespace Argent::Graphics
{
	struct SceneConstant
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 lightColor;
		DirectX::XMFLOAT3 lightPosition;
		DirectX::XMFLOAT3 cameraPosition;
	};

	enum class RenderType
	{
		Sprite,
		Mesh,
		PostEffect,
		demo1,
		demo2,
		demo3,
		demo4,
		demo5,
	};

	class FrameResource
	{
	public:
		FrameResource(ID3D12Device* device, IDXGISwapChain4* swapChain, UINT backBufferIndex, 
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

		void Begin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect, float clearColor[4]) const;
		void UpdateSceneConstant(const SceneConstant& sceneConstant) const;
		void SetSceneConstant(UINT rootParameterIndex = 0);
		void SetBarrier(D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;
		void Reset();
		Dx12::Descriptor* GetDsv() const { return dsv;  }

		std::vector<std::unique_ptr<Dx12::ArCommandBundle>> cmdBundle;
		Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource;
		Dx12::Descriptor* rtv;
		Dx12::Descriptor* dsv;
		Dx12::Descriptor* cbv;
		UINT backBufferIndex;
		SceneConstant*	cbScene;
	public:
	};
}
			