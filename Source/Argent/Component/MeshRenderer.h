#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include <unordered_map>

#include "Renderer.h"
#include "../Graphic/Dx12/ConstantBuffer.h"
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"

namespace Argent::Component::Renderer
{
	class MeshRenderer :
		public BaseRenderer
	{
	public:
		struct Constants
		{
			DirectX::XMFLOAT4X4 world;
		};


	public:
		MeshRenderer(ID3D12Device* device, const char* fileName, std::shared_ptr<Resource::Mesh::ArMesh> meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials);
		~MeshRenderer() override = default;

		MeshRenderer(const MeshRenderer&) = delete;
		MeshRenderer(const MeshRenderer&&) = delete;
		MeshRenderer operator=(const MeshRenderer&) = delete;
		MeshRenderer operator=(const MeshRenderer&&) = delete;

		void Render(ID3D12GraphicsCommandList* cmdList,
			const DirectX::XMFLOAT4X4& world) const;


		void Initialize() override;
		void Render() const override;
		void Update() override;

#ifdef _DEBUG
		void DrawDebug() override;
#endif

		void CreateComObject(ID3D12Device* device);

	private:
		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constants>> constantBuffer{};

	protected:
		std::shared_ptr<Resource::Mesh::ArMesh> mesh{};
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial> materials{};
		
	public:
	};
}
