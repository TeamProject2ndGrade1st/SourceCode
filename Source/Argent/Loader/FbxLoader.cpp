//#include "FbxLoader.h"
//#include <fbxsdk.h>
//#include "../Resource/ArResourceManager.h"
//
//namespace Argent::Loader::Fbx
//{
//	//データの読み込みとアニメーションのアップデートに使ってる
//	struct ArFbxScene
//	{
//		struct Node
//		{
//			uint64_t id{};
//			std::string name;
//			FbxNodeAttribute::EType attribute{};
//			int64_t parentIndex{ -1 };
//		};
//		std::vector<Node> nodes{};
//
//		//todo 何のやつでしょう？
//		int64_t IndexOf(uint64_t id) const  // NOLINT(modernize-use-nodiscard)
//		{
//			int64_t index{};
//			for(const Node& node : nodes)
//			{
//				if(node.id == id) return index;
//				++index;
//			}
//			return -1;
//		}
//	};
//
//	/**
//	 * \brief すべてのノードを取ってくる
//	 * \param fbxNode 
//	 * \param sceneView 
//	 */
//	void Traverse(FbxNode* fbxNode, ArFbxScene& sceneView)
//	{
//		ArFbxScene::Node& node{ sceneView.nodes.emplace_back() };
//		node.attribute = fbxNode->GetNodeAttribute() ?
//			fbxNode->GetNodeAttribute()->GetAttributeType() :
//			FbxNodeAttribute::EType::eUnknown;
//		
//		node.name = fbxNode->GetName();
//		node.id = fbxNode->GetUniqueID();
//		node.parentIndex = sceneView.IndexOf(fbxNode->GetParent() ?
//			fbxNode->GetParent()->GetUniqueID() : 0);
//		for(int childIndex = 0; childIndex < fbxNode->GetChildCount(); ++childIndex)
//		{
//			Traverse(fbxNode->GetChild(childIndex), sceneView);
//		}
//	}
//
//	void FetchMesh(FbxScene* fbxScene,const ArFbxScene& sceneView);
//	void FetchMaterial(FbxScene* fbxScene, ArFbxScene& sceneView, const char* fbxFilePath);
//	void FetchSkeleton(FbxMesh* fbxMesh, ArSkeleton& bindPose, const ArFbxScene& sceneView);
//	void FetchAnimation(FbxScene* fbxScene, std::vector<ArAnimation>& animationClips, 
//			float samplingRate, const ArFbxScene& sceneView);
//	void UpdateAnimation(ArAnimation::ArKeyframe& keyframe, const ArFbxScene& sceneView);
//	void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<BoneInfluence>>& boneInfluences);
//
//
//	void LoadFbx(const char* fileName, bool triangulate)
//	{
//		
//	}
//
//	void FetchMesh(FbxScene* fbxScene, const ArFbxScene& sceneView)
//	{
//		for(const ArFbxScene::Node& node : sceneView.nodes)
//		{
//			if(node.attribute != FbxNodeAttribute::EType::eMesh) continue;
//
//			FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };
//			FbxMesh* fbxMesh{ fbxNode->GetMesh() };
//
//			ArSkinnedMeshRenderer::Mesh& mesh{ meshes.emplace_back() };
//			mesh.uniqueId = fbxMesh->GetNode()->GetUniqueID();
//			mesh.name = fbxMesh->GetNode()->GetName();
//			mesh.nodeIndex = sceneView.IndexOf(mesh.uniqueId);
//			mesh.defaultGlobalTransform = Argent::Helper::FBX::ToFloat4x4(fbxMesh->GetNode()->EvaluateGlobalTransform());
//
//			std::vector<boneInfluencesPerControlPoint> boneInfluences;
//			SkinnedMesh::FetchBoneInfluences(fbxMesh, boneInfluences);
//			SkinnedMesh::FetchSkeleton(fbxMesh, mesh.bindPose, sceneView);
//
//			std::vector<ArSkinnedMeshRenderer::Mesh::Subset>& subsets{ mesh.subsets };
//			const int MaterialCount{ fbxMesh->GetNode()->GetMaterialCount() };
//			subsets.resize(MaterialCount > 0 ? MaterialCount : 1);
//			for(int materialIndex = 0; materialIndex < MaterialCount; ++materialIndex)
//			{
//				const FbxSurfaceMaterial* fbxMaterial{ fbxMesh->GetNode()->GetMaterial(materialIndex) };
//				subsets.at(materialIndex).materialUniqueId = fbxMaterial->GetUniqueID();
//			}
//
//			if(MaterialCount > 0)
//			{
//				const int  polygonCount{ fbxMesh->GetPolygonCount() };
//				for(int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
//				{
//					const int materialIndex{fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) };
//					subsets.at(materialIndex).indexCount += 3;
//				}
//				uint32_t offset{};
//				for(ArSkinnedMeshRenderer::Mesh::Subset& subset : subsets)
//				{
//					subset.startIndexLocation = offset;
//					offset += subset.indexCount;
//					subset.indexCount = 0;
//				}
//			}
//
//			const int polygonCount{ fbxMesh->GetPolygonCount() };
//			mesh.vertices.resize(polygonCount * 3LL);
//			mesh.indices.resize(polygonCount * 3LL);
//
//			FbxStringList uvNames;
//			fbxMesh->GetUVSetNames(uvNames);
//			const FbxVector4* controlPoints{ fbxMesh->GetControlPoints() };
//			for(int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
//			{
//				const int materialIndex{ MaterialCount > 0 ? 
//					fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) : 0 };
//				ArSkinnedMeshRenderer::Mesh::Subset& subset{ subsets.at(materialIndex) };
//				const uint32_t offset{ subset.startIndexLocation + subset.indexCount };
//
//				for(int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
//				{
//					const int vertexIndex{ polygonIndex * 3 + positionInPolygon };
//
//					ArSkinnedMeshRenderer::Vertex vertex;
//					const int polygonVertex{ fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon) };
//					vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
//					vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
//					vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);
//
//
//					//bone
//					const boneInfluencesPerControlPoint& influencesPerControlPoint{ boneInfluences.at(polygonVertex) };
//					for(size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
//					{
//						if(influenceIndex < ArSkinnedMeshRenderer::MaxBoneInfluences)
//						{
//							vertex.boneWeights[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneWeight;
//							vertex.boneIndices[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex;
//						}
//					}
//
//					if(fbxMesh->GetElementNormalCount() > 0)
//					{
//						FbxVector4 normal;
//						fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
//						vertex.normal.x = static_cast<float>(normal[0]);
//						vertex.normal.y = static_cast<float>(normal[1]);
//						vertex.normal.z = static_cast<float>(normal[2]);
//					}
//					if(fbxMesh->GetElementUVCount() > 0)
//					{
//						FbxVector2 uv;
//						bool unmappedUv;
//						fbxMesh->GetPolygonVertexUV(polygonIndex	, positionInPolygon, 
//							uvNames[0], uv, unmappedUv);
//						vertex.texcoord.x = static_cast<float>(uv[0]);
//						vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
//					}
//
//					mesh.vertices.at(vertexIndex) = std::move(vertex);
//					mesh.indices.at(static_cast<size_t>(offset) + positionInPolygon) = vertexIndex;
//					++subset.indexCount;
//				}
//			}
//		}
//
//	}
//
//
//}
