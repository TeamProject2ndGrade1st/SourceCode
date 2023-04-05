#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Renderer.h"
#include "../Graphic/Dx12/ConstantBuffer.h"
#include "../Resource/ArSkinnedMesh.h"
#include "../Resource/Material.h"
#include "../Resource/ArAnimation.h"


//todo�@�{�[�����邢�̓A�j���[�V�����������Ă��Ȃ��ꍇ�̓����_�����O�ł��Ȃ�����
//�N���X�𕪂������@
//�{�[���������Ă���== skinnedMesh �����Ă��Ȃ�== mesh�@�Ƃ�������
//�Ȃ̂�fbx���[�_���O���ō���ă{�[���̐����擾�@��������ǂ����ɂ��邩������
//���\�[�X�}�l�[�W���[�ɂł������Ƃ��Ⴂ�������ɂȂ�ł���
//�R���X�g���N�^�̈�����ύX����̂͂��肶��Ȃ�
//�{�[���ƃ��b�V�����ɂ���̂͑S�R����@�A�j���[�V�������u���Ƃ��Ă�������
//unity�݂����ɂ��������A�j���[�V�����𕪗����ĕʂ̏ꏊ�ɒu���ƊǗ�����ς���

namespace Argent::Component::Renderer
{
	class ArSkinnedMeshRenderer:
		public Argent::Component::Renderer::ArRenderer
	{
	public:
		enum class RootParameterIndex
		{
			cbScene,
			cbObject,
			cbMesh,
			cbMaterial,
			txAlbedo,
			txNormal,
		};
		static constexpr int MaxBoneInfluences{ 4 };
		
		struct Vertex			
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texcoord;
		};

		struct VertexBone
		{
			float boneWeights[MaxBoneInfluences]{1, 0, 0, 0};
			uint32_t boneIndices[MaxBoneInfluences]{};
		};

		static const int MaxBones{ 256 };

		struct Constants
		{
			DirectX::XMFLOAT4X4 world;
		};


	public:
		ArSkinnedMeshRenderer(ID3D12Device* device, const char* fileName,
			std::vector<std::shared_ptr<Resource::Mesh::ArSkinnedMesh>>& meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials,
			std::vector<Resource::Animation::ArAnimation>& animation);
		ArSkinnedMeshRenderer(ID3D12Device* device, const char* fileName,
			std::shared_ptr<Resource::Mesh::ArSkinnedMesh> meshes,
			std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial>& materials,
			std::vector<Resource::Animation::ArAnimation>& animation);

		~ArSkinnedMeshRenderer() override = default;

		ArSkinnedMeshRenderer(const ArSkinnedMeshRenderer&) = delete;
		ArSkinnedMeshRenderer(const ArSkinnedMeshRenderer&&) = delete;
		ArSkinnedMeshRenderer operator=(const ArSkinnedMeshRenderer&) = delete;
		ArSkinnedMeshRenderer operator=(const ArSkinnedMeshRenderer&&) = delete;

		void Initialize() override;
		void Render(ID3D12GraphicsCommandList* cmdList,
			const DirectX::XMFLOAT4X4& world,
			const Resource::Animation::ArAnimation::Keyframe* keyframe) const;

		void Render() const override;

		void Update() override;

	#ifdef _DEBUG
		void DrawDebug() override;
	#endif

		void CreateComObject(ID3D12Device* device);

	private:
		std::unique_ptr<Argent::Dx12::ArConstantBuffer<Constants>> objectConstantBuffer;

		std::shared_ptr<Argent::Resource::Mesh::ArSkinnedMesh> skinnedMesh;
		std::vector<std::shared_ptr<Argent::Resource::Mesh::ArSkinnedMesh>> skinnedMeshes;
		std::unordered_map<uint64_t, Argent::Material::ArMeshMaterial> materials;
		int clipIndex{};
		float frameIndex{};
		std::vector<Resource::Animation::ArAnimation> animationClips;

	private:
		void CreateRootSignatureAndPipelineState();
	};
}