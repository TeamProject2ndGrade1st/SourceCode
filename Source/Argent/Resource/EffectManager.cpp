#include "EffectManager.h"
#include "../Graphic/Graphics.h"
#include "../Core/Timer.h"


namespace Argent::Resource::Effect
{
	EffectManager* EffectManager::instance = nullptr;
	EffectManager::EffectManager(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, UINT numBackBuffers)
	{
		//todo spell miss ?
		if (instance) _ASSERT_EXPR(FALSE, L"already instanciated");
		instance = this;
		DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		efkRenderer = EffekseerRendererDX12::Create(
			device,
			cmdQueue,
			numBackBuffers,
			&format,
			1,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			false,
			10000
		);

		if (!efkRenderer.Get()) _ASSERT_EXPR(FALSE, "Error");

		efkManager = Effekseer::Manager::Create(10000);
		if (!efkManager.Get()) _ASSERT_EXPR(FALSE, "Error");

		//座標系のセット
		efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

		//描画機能のセット 自作もできるらしいがそのまま使う
		efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
		efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
		efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
		efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
		efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

		//ローダーのセット　こちらも自作可能
		efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
		efkManager->SetModelLoader(efkRenderer->CreateModelLoader());


		//dx12特有の処理
		efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer->GetGraphicsDevice());
		if (!efkMemoryPool.Get()) _ASSERT_EXPR(FALSE, "Error");
		efkCmdList = EffekseerRenderer::CreateCommandList(efkRenderer->GetGraphicsDevice(), efkMemoryPool);
		if (!efkCmdList.Get()) _ASSERT_EXPR(FALSE, "Error");

		efkRenderer->SetCommandList(efkCmdList);
	}


	void EffectManager::Update()
	{
		
		efkManager->Update(0.5f);
	}

	void EffectManager::Render() const
	{
		Effekseer::Matrix44 fkViewMat;
		Effekseer::Matrix44 fkProjMat;

		const auto view = Graphics::Graphics::Instance()->GetCurrentFrameResource()->GetSceneView();
		const auto projection = Graphics::Graphics::Instance()->GetCurrentFrameResource()->GetSceneProjection();

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				fkViewMat.Values[i][j] = view.m[i][j];
				fkProjMat.Values[i][j] = projection.m[i][j];
			}
		}
		efkRenderer->SetCameraMatrix(fkViewMat);
		efkRenderer->SetProjectionMatrix(fkProjMat);



		//描画場所を指定　マルチレンダーターゲット場合は一枚目でやること
		efkMemoryPool->NewFrame();
		EffekseerRendererDX12::BeginCommandList(efkCmdList, Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Mesh));

		efkRenderer->BeginRendering();
		efkManager->Draw();
		efkRenderer->EndRendering();

		EffekseerRendererDX12::EndCommandList(efkCmdList);
	}
}