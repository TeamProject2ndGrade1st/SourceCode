#pragma once
#include <vector>

#include "Renderer.h"
#include "../Resource/StaticMesh.h"


namespace Argent::Component::Renderer
{
	struct Subset
	{
		uint32_t startVertex{};
		uint32_t vertexCount{};
		uint32_t startIndex{};
		uint32_t indexCount{};
	};


	class ArMeshRenderer :
		public BaseRenderer
	{
	public:
		ArMeshRenderer(Argent::Resource::Mesh::ArStaticMesh* data);

		void Initialize() override;
		void Finalize() override;
		void Begin() override;
		void End() override;
		void Update() override;
		void Render() const override;

#ifdef _DEBUG
		void DrawDebug() override;
#endif


	protected:
		
		std::vector<Subset> subsets{};
		Argent::Resource::Mesh::ArStaticMesh* mesh{};
	};
}
