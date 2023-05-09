#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>

 
//todo カメラとかライトとかを引数にしないように
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTex.lib")

//自作クラス
#include "FrameBuffer.h"
#include "FrameResource.h"
#include "Dx12/Command.h"
#include "Dx12/DescriptorHeap.h"

namespace Argent::Graphics
{
	class Graphics
	{
	public:
		enum class CoordinateSystem
		{
			RightYup,
			LeftYup,
			RightZup,
			LeftZup,
			CoordCount,
		};
	public:
		HWND hWnd;
		Graphics(HWND hWnd);
		virtual ~Graphics()
		{
			//renderingQueue->SetFence(1);
			
		}
		Graphics(const Graphics&) = delete;
		Graphics(const Graphics&&) = delete;
		Graphics operator=(const Graphics&) = delete;
		Graphics operator=(const Graphics&&) = delete;
		
		void Initialize();
		void Terminate();
		void Begin();
		void End();
		
		HRESULT CreateWhiteTexture(ID3D12Resource** resource);
		HRESULT CreateBlackTexture(ID3D12Resource** resource);
		HRESULT CreateGrayGradationTexture(ID3D12Resource** resource);
		HRESULT CreateNoiseTexture(ID3D12Resource** resource);
		
		void SetSceneConstant(UINT rootParameterIndex = 0);
	public:
		static Graphics* Instance() { return instance; }
		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12GraphicsCommandList* GetCommandList(RenderType type) const { return curFrameResource->GetCmdList(type); }
		
		float GetWidth() const { return windowWidth; }
		float GetHeight() const { return windowHeight; }
		
		[[nodiscard]] ID3D12DescriptorHeap* GetGUIHeap() const { return imGuiHeap->GetHeapPointer(); }

		D3D12_VIEWPORT GetViewport() const { return viewport; }
		D3D12_RECT GetRect() const { return scissorRect; }

		float* GetClearColor() { return clearColor; }

		[[nodiscard]] Dx12::DescriptorHeap* GetHeap(D3D12_DESCRIPTOR_HEAP_TYPE type) const;

		Dx12::DescriptorHeap* GetImGuiHeap() const { return imGuiHeap.get(); }
		Dx12::CommandBundle* GetResourceCmdBundle() const { return resourceCmdBundle.get(); }
		Dx12::CommandQueue* GetResourceCmdQueue() const { return resourceQueue.get(); }


		const FrameResource* GetCurrentFrameResource() const { return curFrameResource;  }

		ID3D12CommandQueue* GetCommandQueue() const { return renderingQueue->cmdQueue.Get();  }

		static int GetNumBackBuffers() { return NumBackBuffers;  }

		//シーンコンスタント用
		void SetViewMatrix(const DirectX::XMMATRIX& m){ DirectX::XMStoreFloat4x4(&sceneConstant.view, m); }
		void SetProjectionMatrix(const DirectX::XMMATRIX& m) { DirectX::XMStoreFloat4x4(&sceneConstant.projection, m); }
		void SetCameraPosition(const DirectX::XMFLOAT3& p) { sceneConstant.cameraPosition = p; }
		void SetDirectionalLight(const DirectionalLight& d) { sceneConstant.directionalLight = d; }
		void SetPointLight(const PointLight& p, int index) { sceneConstant.pointLight = p; }
	private:
		static Graphics* instance;
		static constexpr int NumBackBuffers = 3;
		static constexpr int NumCmdLists = static_cast<int>(RenderType::Count);
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<ID3D12DebugDevice> debugDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
		std::vector<std::unique_ptr<FrameResource>> frameResources;
		FrameResource* curFrameResource;

		std::unique_ptr<Dx12::CommandQueue> resourceQueue;
		std::unique_ptr<Dx12::CommandBundle> resourceCmdBundle;
		std::unique_ptr<Dx12::CommandQueue> renderingQueue;
		std::unique_ptr<Dx12::DescriptorHeap> rtvHeap;
		std::unique_ptr<Dx12::DescriptorHeap> srvCbvHeap;
		std::unique_ptr<Dx12::DescriptorHeap> dsvHeap;
		std::unique_ptr<Dx12::DescriptorHeap> imGuiHeap;


		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
		
		
		float clearColor[4];
		float windowWidth;
		float windowHeight;
		SceneConstant sceneConstant;
	
		UINT backBufferIndex = -1;
	public:
		std::unique_ptr<FrameBuffer> frameBuffer[8];
	};

	HRESULT CreateDevice(IDXGIFactory6* factory, ID3D12Device** device);

	HRESULT CreateSwapChain(HWND hWnd, IDXGIFactory2* factory, UINT windowWidth, UINT windowHeight, 
	                        UINT NumBackBuffers, ID3D12CommandQueue* cmdQueue, IDXGISwapChain3** ppSwapChain);
}
