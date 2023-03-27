#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include <unordered_map>
#include "ArRenderer.h"
#include "../Graphic/Dx12/ArConstantBuffer.h"
#include "../Resource/ArStaticMesh.h"
#include "../Resource/ArMaterial.h"

namespace Argent::Component::Renderer
{
	class ArStaticMeshRenderer:
		public ArRenderer
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
		ArStaticMeshRenderer(ID3D12Device* device, const char* fileName, std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials);
		~ArStaticMeshRenderer() override = default;

		ArStaticMeshRenderer(const ArStaticMeshRenderer&) = delete;
		ArStaticMeshRenderer(const ArStaticMeshRenderer&&) = delete;
		ArStaticMeshRenderer operator=(const ArStaticMeshRenderer&) = delete;
		ArStaticMeshRenderer operator=(const ArStaticMeshRenderer&&) = delete;

		void Render(ID3D12GraphicsCommandList* cmdList,
			const DirectX::XMFLOAT4X4& world) const;

		void CreateRootSignatureAndPipelineState();


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
