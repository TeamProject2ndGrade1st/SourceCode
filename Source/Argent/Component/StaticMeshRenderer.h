#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include <unordered_map>

#include "Renderer.h"
#include "../Graphic/Dx12/ConstantBuffer.h"
#include "../Resource/StaticMesh.h"
#include "../Resource/Material.h"

namespace Argent::Component::Renderer
{
	class StaticMeshRenderer:
		public BaseRenderer
	{
	public:
		enum class RootParameterIndex
		{
			cbScene,
			cbObject,
			cbMaterial,
			txAlbedo,
			txNormal,
		};

		struct Constants
		{
			DirectX::XMFLOAT4X4 world;
		};


	public:
		StaticMeshRenderer(ID3D12Device* device, const char* fileName, std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials);
		StaticMeshRenderer(ID3D12Device* device, const char* fileName, std::shared_ptr<Resource::Mesh::ArStaticMesh> meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials);
		~StaticMeshRenderer() override = default;

		StaticMeshRenderer(const StaticMeshRenderer&) = delete;
		StaticMeshRenderer(const StaticMeshRenderer&&) = delete;
		StaticMeshRenderer operator=(const StaticMeshRenderer&) = delete;
		StaticMeshRenderer operator=(const StaticMeshRenderer&&) = delete;

		void Render(ID3D12GraphicsCommandList* cmdList,
			const DirectX::XMFLOAT4X4& world) const;

		void CreateRootSignatureAndPipelineState();

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
		std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes{};
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial> materials{};
		
	public:
	};
}
