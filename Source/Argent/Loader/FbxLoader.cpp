#include "FbxLoader.h"
#include <fbxsdk.h>
#include "../Resource/ArResourceManager.h"

//todo
#include "../Component/ArStaticMeshRenderer.h"

namespace Argent::Loader::Fbx
{
	static constexpr int MaxBoneInfluences{ 256 };

	//データの読み込みとアニメーションのアップデートに使ってる
	struct ArFbxScene
	{
		struct Node
		{
			uint64_t id{};
			std::string name;
			FbxNodeAttribute::EType attribute{};
			int64_t parentIndex{ -1 };
		};
		std::vector<Node> nodes{};

		//todo 何のやつでしょう？
		int64_t IndexOf(uint64_t id) const  // NOLINT(modernize-use-nodiscard)
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

	/**
	 * \brief すべてのノードを取ってくる
	 * \param fbxNode 
	 * \param sceneView 
	 */
	void Traverse(FbxNode* fbxNode, ArFbxScene& sceneView)
	{
		ArFbxScene::Node& node{ sceneView.nodes.emplace_back() };
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


	struct TmpFbxMesh
	{
		int64_t nodeIndex;
		std::vector<SkinnedMeshVertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<ArSubset> subsets;
		ArSkeleton bindPose;
		DirectX::XMFLOAT4X4 defaultGlobalTransform
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
	};

	void FetchMesh(FbxScene* fbxScene,const ArFbxScene& sceneView, std::vector<TmpFbxMesh>& meshes);
	void FetchMaterial(FbxScene* fbxScene, const ArFbxScene& sceneView, const char* fbxFilePath, std::unordered_map<uint64_t, Component::Renderer::ArStaticMeshRenderer::Material>& materials);
	void FetchSkeleton(FbxMesh* fbxMesh, ArSkeleton& bindPose, const ArFbxScene& sceneView);
	void FetchAnimation(FbxScene* fbxScene, std::vector<ArAnimation>& animationClips, 
			float samplingRate, const ArFbxScene& sceneView);
	void UpdateAnimation(ArAnimation::ArKeyframe& keyframe, const ArFbxScene& sceneView);
	void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<ArBoneInfluence>>& boneInfluences);


	Argent::Component::ArComponent* LoadFbx(const char* filePath, bool triangulate)
	{
		FbxManager* manager{ FbxManager::Create() };
		FbxScene* fbxScene{ FbxScene::Create(manager, "") };

		FbxImporter* importer{ FbxImporter::Create(manager, "") };
		bool importState{ FALSE };
		importState = importer->Initialize(filePath);
		_ASSERT_EXPR_A(importState, importer->GetStatus().GetErrorString());

		importState = importer->Import(fbxScene);
		_ASSERT_EXPR_A(importState, importer->GetStatus().GetErrorString());

		FbxGeometryConverter converter(manager);
		if (triangulate)
		{
			converter.Triangulate(fbxScene, true, false);
			converter.RemoveBadPolygonsFromMeshes(fbxScene);
		}

		ArFbxScene sceneView{};

		Traverse(fbxScene->GetRootNode(), sceneView);

		std::vector<TmpFbxMesh> tmpMeshes;
		std::unordered_map<uint64_t, Component::Renderer::ArStaticMeshRenderer::Material> materials;

		FetchMesh(fbxScene, sceneView, tmpMeshes);
		FetchMaterial(fbxScene, sceneView, filePath, materials);

		
		std::vector<ArAnimation> animationClips;

		std::vector<std::shared_ptr<Resource::Mesh::ArStaticMesh>> meshes;
		meshes.resize(tmpMeshes.size());
		for(size_t i = 0; i < meshes.size(); ++i)
		{
			size_t size = tmpMeshes.at(i).vertices.size();
			std::vector<Argent::Resource::Mesh::Vertex> vertices(size);
			for(size_t j = 0; j < vertices.size(); ++j)
			{
				vertices.at(j).position = tmpMeshes.at(i).vertices.at(j).position;
				vertices.at(j).normal = tmpMeshes.at(i).vertices.at(j).normal;
				vertices.at(j).texcoord = tmpMeshes.at(i).vertices.at(j).texcoord;
			}
			meshes.at(i) = std::make_shared<Resource::Mesh::ArStaticMesh>(vertices, tmpMeshes.at(i).indices, reinterpret_cast<std::vector<Argent::Resource::Mesh::ArStaticMesh::Subset>&>(tmpMeshes.at(i).subsets));
		}

		Component::Renderer::ArStaticMeshRenderer* ret = new Component::Renderer::ArStaticMeshRenderer(Argent::Graphics::ArGraphics::Instance()->GetDevice(),
		filePath, meshes, materials);

		return ret;
	}


	void FetchMesh(FbxScene* fbxScene, const ArFbxScene& sceneView, std::vector<TmpFbxMesh>& meshes)
	{
		for (const ArFbxScene::Node& node : sceneView.nodes)
		{
			if (node.attribute != FbxNodeAttribute::EType::eMesh) continue;

			FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };
			FbxMesh* fbxMesh{ fbxNode->GetMesh() };

			TmpFbxMesh& mesh{ meshes.emplace_back() };
			mesh.nodeIndex = sceneView.IndexOf(fbxMesh->GetNode()->GetUniqueID());
			mesh.defaultGlobalTransform = Argent::Helper::FBX::ToFloat4x4(fbxMesh->GetNode()->EvaluateGlobalTransform());

			//std::vector<std::vector<ArBoneInfluence>> boneInfluences;
			//FetchBoneInfluences(fbxMesh, boneInfluences);
			//FetchSkeleton(fbxMesh, mesh.bindPose, sceneView);

			std::vector<ArSubset>& subsets{ mesh.subsets };
			const int MaterialCount{ fbxMesh->GetNode()->GetMaterialCount() };
			subsets.resize(MaterialCount > 0 ? MaterialCount : 1);
			for (int materialIndex = 0; materialIndex < MaterialCount; ++materialIndex)
			{
				const FbxSurfaceMaterial* fbxMaterial{ fbxMesh->GetNode()->GetMaterial(materialIndex) };
				subsets.at(materialIndex).materialUniqueId = fbxMaterial->GetUniqueID();
			}

			if (MaterialCount > 0)
			{
				const int  polygonCount{ fbxMesh->GetPolygonCount() };
				for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
				{
					const int materialIndex{ fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) };
					subsets.at(materialIndex).indexCount += 3;
				}
				uint32_t offset{};
				for (ArSubset& subset : subsets)
				{
					subset.startIndexLocation = offset;
					offset += subset.indexCount;
					subset.indexCount = 0;
				}
			}

			const int polygonCount{ fbxMesh->GetPolygonCount() };
			mesh.vertices.resize(polygonCount * 3LL);
			mesh.indices.resize(polygonCount * 3LL);

			FbxStringList uvNames;
			fbxMesh->GetUVSetNames(uvNames);
			const FbxVector4* controlPoints{ fbxMesh->GetControlPoints() };
			for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
			{
				const int materialIndex{ MaterialCount > 0 ?
					fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) : 0 };
				ArSubset& subset{ subsets.at(materialIndex) };
				const uint32_t offset{ subset.startIndexLocation + subset.indexCount };

				for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
				{
					const int vertexIndex{ polygonIndex * 3 + positionInPolygon };

					SkinnedMeshVertex vertex;
					const int polygonVertex{ fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon) };
					vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
					vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
					vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);


					//bone
					/*const auto& influencesPerControlPoint{ boneInfluences.at(polygonVertex) };
					for (size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
					{
						if (influenceIndex < MaxBoneInfluences)
						{
							vertex.boneWeights[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneWeight;
							vertex.boneIndices[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex;
						}
					}*/
					
					if (fbxMesh->GetElementNormalCount() > 0)
					{
						FbxVector4 normal;
						fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
						vertex.normal.x = static_cast<float>(normal[0]);
						vertex.normal.y = static_cast<float>(normal[1]);
						vertex.normal.z = static_cast<float>(normal[2]);
					}
					if (fbxMesh->GetElementUVCount() > 0)
					{
						FbxVector2 uv;
						bool unmappedUv;
						fbxMesh->GetPolygonVertexUV(polygonIndex, positionInPolygon,
							uvNames[0], uv, unmappedUv);
						vertex.texcoord.x = static_cast<float>(uv[0]);
						vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
					}

					mesh.vertices.at(vertexIndex) = std::move(vertex);
					mesh.indices.at(static_cast<size_t>(offset) + positionInPolygon) = vertexIndex;
					++subset.indexCount;
				}
			}
		}
	}

	void FetchSurfaceMaterial(const FbxSurfaceMaterial* surfaceMaterial, const char* pName, const char* fbxFilePath, Component::Renderer::ArStaticMeshRenderer::Material& material)
	{
		const FbxProperty fbxProp = surfaceMaterial->FindProperty(pName);

		const std::string PName = pName;
		if (fbxProp.IsValid())
		{
			if(PName == FbxSurfaceMaterial::sDiffuse)
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material.constant.kd.x = static_cast<float>(color[0]);
				material.constant.kd.y = static_cast<float>(color[1]);
				material.constant.kd.z = static_cast<float>(color[2]);
				material.constant.kd.w = 1.0f;

				const FbxFileTexture* fbxTexture{ fbxProp.GetSrcObject<FbxFileTexture>() };
				const char* tmpFilePath = fbxTexture ? fbxTexture->GetRelativeFileName() : "";
				tmpFilePath = "lambert1_Base_color.png";
				std::filesystem::path path(fbxFilePath);
				path.replace_filename(tmpFilePath);

				material.CreateTexture(path.generic_string().c_str(), Component::Renderer::ArStaticMeshRenderer::Material::TextureType::Diffuse);
			}
			if(PName == FbxSurfaceMaterial::sSpecular) 
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material.constant.ks.x = static_cast<float>(color[0]);
				material.constant.ks.y = static_cast<float>(color[1]);
				material.constant.ks.z = static_cast<float>(color[2]);
				material.constant.ks.w = 1.0f;
			}
			if(PName == FbxSurfaceMaterial::sAmbient)
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material.constant.ka.x = static_cast<float>(color[0]);
				material.constant.ka.y = static_cast<float>(color[1]);
				material.constant.ka.z = static_cast<float>(color[2]);
				material.constant.ka.w = 1.0f;
			}
			if(PName == FbxSurfaceMaterial::sBump)
			{
				const FbxFileTexture* fbxTexture{ fbxProp.GetSrcObject<FbxFileTexture>() };
				const char* tmpFilePath = fbxTexture ? fbxTexture->GetRelativeFileName() : "";

				tmpFilePath = "lambert1_Normal_OpenGL.png";
				std::filesystem::path path(fbxFilePath);
				path.replace_filename(tmpFilePath);

				material.CreateTexture(path.generic_string().c_str(), Component::Renderer::ArStaticMeshRenderer::Material::TextureType::Normal);
			}
		}
	}

	void FetchMaterial(FbxScene* fbxScene, const ArFbxScene& sceneView, const char* fbxFilePath, std::unordered_map<uint64_t, Component::Renderer::ArStaticMeshRenderer::Material>& materials)
	{
		const size_t nodeCount{ sceneView.nodes.size() };
		for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
		{
			const ArFbxScene::Node& node{ sceneView.nodes.at(nodeIndex) };
			const FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };

			const int materialCount{ fbxNode->GetMaterialCount() };
			if (materialCount > 0)
			{
				for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
				{
					const FbxSurfaceMaterial* fbxMaterial{ fbxNode->GetMaterial(materialIndex) };

					Component::Renderer::ArStaticMeshRenderer::Material material;
					material.name = fbxMaterial->GetName();
					UINT64 materialUniqueId = fbxMaterial->GetUniqueID();

					/// スペキュラー　アンビエント　ディフューズ　ノーマルだけ読み込む
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sDiffuse, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sSpecular, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sAmbient, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sBump, fbxFilePath, material);

					materials.emplace(materialUniqueId, std::move(material));
				}
			}
		}
	}

}
