#pragma once
#include <memory>
#include <d3d12.h>

#include "Component.h"
#include "../Graphic/RenderingPipeline.h"
#include "../Resource/Material.h"

//描画用のコンポーネント
namespace Argent::Component
{
	namespace Renderer
	{
		class BaseRenderer : public BaseComponent
		{
		public:
			BaseRenderer(const std::string& name);
			virtual ~BaseRenderer() override;


			void Initialize() override {}

			/**
			 * \brief 継承して使う　元の関数は一番上で呼び出すこと
			 * \param cmdList 
			 */
			virtual void Render(ID3D12GraphicsCommandList* cmdList) const;
			void Render() const override;

#ifdef _DEBUG
			void DrawDebug() override{}
#endif
		protected:
			std::vector<std::shared_ptr<Material::ArMaterial>> materials;
			std::shared_ptr<Argent::Graphics::RenderingPipeline::BaseRenderingPipeline> renderingPipeline;
		};
	}
}
