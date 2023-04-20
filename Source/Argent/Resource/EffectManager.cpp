#include "EffectManager.h"
#include "../Graphic/Graphics.h"


namespace Argent::Resource::Effect
{
	EffectManager* EffectManager::instance = nullptr;
	EffectManager::EffectManager(ID3D12Device* device, ID3D12CommandQueue* cmdQueue, UINT numBackBuffers)
	{
		//todo spell miss ?
		if (instance) _ASSERT_EXPR(FALSE, L"already instanciated");
		instance = this;
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

		//���W�n�̃Z�b�g
		efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

		//�`��@�\�̃Z�b�g ������ł���炵�������̂܂܎g��
		efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
		efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
		efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
		efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
		efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

		//���[�_�[�̃Z�b�g�@�����������\
		efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
		efkManager->SetModelLoader(efkRenderer->CreateModelLoader());


		//dx12���L�̏���
		efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(efkRenderer->GetGraphicsDevice());
		if (!efkMemoryPool.Get()) _ASSERT_EXPR(FALSE, "Error");
		efkCmdList = EffekseerRenderer::CreateCommandList(efkRenderer->GetGraphicsDevice(), efkMemoryPool);
		if (!efkCmdList.Get()) _ASSERT_EXPR(FALSE, "Error");

		efkRenderer->SetCommandList(efkCmdList);
	}


	void Argent::Resource::Effect::EffectManager::Update()
	{
		
		efkManager->Update();
	}

	void Argent::Resource::Effect::EffectManager::Render() const
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



		//�`��ꏊ���w��@�}���`�����_�[�^�[�Q�b�g�ꍇ�͈ꖇ�ڂł�邱��
		efkMemoryPool->NewFrame();
		EffekseerRendererDX12::BeginCommandList(efkCmdList, Graphics::Graphics::Instance()->GetCommandList(Graphics::RenderType::Mesh));

		efkRenderer->BeginRendering();
		efkManager->Draw();
		efkRenderer->EndRendering();

		EffekseerRendererDX12::EndCommandList(efkCmdList);
	}
}