#include "FbxLoader.h"
#include <fbxsdk.h>
#include "../Resource/ArResourceManager.h"

namespace Argent::Loader::Fbx
{
	//�f�[�^�̓ǂݍ��݂ƃA�j���[�V�����̃A�b�v�f�[�g�Ɏg���Ă�
	struct SkinnedScene
	{
		struct Node
		{
			uint64_t id{};
			std::string name;
			FbxNodeAttribute::EType attribute{};
			int64_t parentIndex{ -1 };
		};
		std::vector<Node> nodes;

		//todo ���̂�ł��傤�H
		int64_t IndexOf(uint64_t id) const
		{
			int64_t index{};
			for(const Node& node : nodes)
			{
				if(node.id == id) return index;
				++index;
			}
			return -1;
		}
	};

	void Traverse(FbxNode* fbxNode, SkinnedScene& sceneView)
	{
		SkinnedScene::Node& node{ sceneView.nodes.emplace_back() };
		node.attribute = fbxNode->GetNodeAttribute() ?
			fbxNode->GetNodeAttribute()->GetAttributeType() :
			FbxNodeAttribute::EType::eUnknown;
		
		node.name = fbxNode->GetName();
		node.id = fbxNode->GetUniqueID();
		node.parentIndex = sceneView.IndexOf(fbxNode->GetParent() ?
			fbxNode->GetParent()->GetUniqueID() : 0);
		for(int childIndex = 0; childIndex < fbxNode->GetChildCount(); ++childIndex)
		{
			Traverse(fbxNode->GetChild(childIndex), sceneView);
		}
	}

	void CreateSkinnedMeshFromFBX()
	{

	}

	void LoadFbx(const char* fileName, bool triangulate)
	{
		//FbxManager* mgr{ FbxManager::Create() };
		//FbxScene* scene{ FbxScene::Create(mgr, "") };
		//FbxImporter* importer{ FbxImporter::Create(mgr, "") };
		//SkinnedScene sceneView{};
		//bool result{};
		//result = importer->Initialize(fileName);
		//_ASSERT_EXPR(result, L"import Initialize failed");

		//result = importer->Import(scene);
		//_ASSERT_EXPR(result, L"Import failed");

		////todo �O�p�����邩�ǂ�����sdk�̊֐��ł������i�֐����邩���m��Ȃ��j
		//if(triangulate)
		//{
		//	FbxGeometryConverter converter(mgr);
		//	converter.Triangulate(scene, true);
		//	converter.RemoveBadPolygonsFromMeshes(scene);
		//}

		////�ċA�֐��@�V�[�����ɑ��݂��邷�ׂẴm�[�h����荞��
		////todo null�m�[�h�Ƃ�����炵�������������̂��Ĕ�΂����Ⴞ�߂Ȃ̂��H
		//Traverse(scene->GetRootNode(), sceneView);

		//mgr->Destroy();

		//std::vector<Argent::Mesh::ArStaticMesh::Vertex> vertices;
		//std::vector<uint32_t> indices;

		//Argent::Resource::ArResourceManager::Instance().AddMesh("DemoFBX", std::make_unique<Argent::Mesh::ArStaticMesh>(vertices, indices));
		////Argent::Resource::ArResourceManager::Instance().AddMesh(fileName, std::make_unique<Argent::Mesh::ArStaticMesh>(vertices, indices));

	}

}
