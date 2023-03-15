#pragma once
#include <memory>
#include <d3d12.h>

#include "ArComponent.h"
#include "../Graphic/ArRenderingPipeline.h"
#include "../Resource/ArMaterial.h"

//�`��p�̃R���|�[�l���g
namespace Argent::Component
{
	namespace Renderer
	{
		class ArRenderer : public ArComponent
		{
		public:
			ArRenderer(const std::string& name);
			virtual ~ArRenderer() override;


			void Initialize() override {}

			/**
			 * \brief �p�����Ďg���@���̊֐��͈�ԏ�ŌĂяo������
			 * \param cmdList 
			 */
			virtual void Render(ID3D12GraphicsCommandList* cmdList) const;
			void Render() const override;

#ifdef _DEBUG
			void DrawDebug() override{}
#endif
		protected:
			std::vector<std::shared_ptr<Material::ArMaterial>> materials;
			std::shared_ptr<Argent::Graphics::RenderingPipeline::ArBaseRenderingPipeline> renderingPipeline;
		};
	}
}
