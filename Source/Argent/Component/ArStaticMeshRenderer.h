#pragma once

#include "ArRenderer.h"
#include "../Resource/ArStaticMesh.h"

namespace Argent::Component::Renderer
{
	class ArStaticMeshRenderer:
		public ArRenderer
	{
	public:
		ArStaticMeshRenderer();
		~ArStaticMeshRenderer() override = default;

		void Render() const override;

	private:
		std::shared_ptr<Argent::Resource::Mesh::ArStaticMesh> meshData;
	};
}