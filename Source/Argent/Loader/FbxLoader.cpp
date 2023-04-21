#include "FbxLoader.h"
#include <fbxsdk.h>
#include "../Resource/ResourceManager.h"
#include <fstream>

//todo
#include "../Graphic/Graphics.h"
#include "../Component/MeshRenderer.h"
#include "../Component/SkinnedMeshRenderer.h"
#include "../Resource/SkinnedMesh.h"
#include "../Resource/Material.h"
#include "../Resource/Animation.h"


namespace Argent::Loader::Fbx
{
	static constexpr int MaxBoneInfluences{ 256 };

	//データの読み込みとアニメーションのアップデートに使ってる
	/**
	 * \brief すべてのノードを取ってくる
	 * \param fbxNode 
	 * \param sceneView 
	 */
	void Traverse(FbxNode* fbxNode, ArFbxScene& sceneView)
	{
		auto& node = sceneView.nodes.emplace_back();
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

	void FetchMesh(FbxScene* fbxScene,const ArFbxScene& sceneView, std::vector<TmpFbxMesh>& meshes);
	void FetchMaterial(FbxScene* fbxScene, const ArFbxScene& sceneView, const char* fbxFilePath, std::unordered_map<uint64_t, std::shared_ptr<Material::MeshMaterial>>& materials);
	void FetchSkeleton(FbxMesh* fbxMesh, Argent::Resource::Mesh::Skeleton& bindPose, const ArFbxScene& sceneView);
	void FetchAnimation(FbxScene* fbxScene, std::vector<Resource::Animation::AnimationClip>& animationClips, 
			float samplingRate, const ArFbxScene& sceneView);
	void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<ArBoneInfluence>>& boneInfluences);


	//ローダー
	std::vector<Component::BaseComponent*> LoadFbx(const char* filePath, bool triangulate)
	{
		ArFbxScene sceneView{};
		FbxResource fbxResource;

		std::vector<Component::BaseComponent*> ret{};
		//シリアライズ
		std::filesystem::path cerealFileName(filePath);
		cerealFileName.replace_extension("cereal");
		if(std::filesystem::exists(cerealFileName.c_str()))
		{
			std::ifstream ifs(cerealFileName.c_str(), std::ios::binary);
			cereal::BinaryInputArchive deserialization(ifs);
			deserialization(sceneView, fbxResource);

			for(auto& mesh : fbxResource.tmpMeshes)
			{
				for(auto& s : mesh.subsets)
				{
					for(int i = 0; i < Material::MeshMaterial::NumTextures; ++i)
					{
						s.material->CreateTexture(s.material->textureNames[i].c_str(), static_cast<Material::MeshMaterial::TextureType>(i));
					}
				}
			}
		}
		else
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

			Traverse(fbxScene->GetRootNode(), sceneView);

			FetchMaterial(fbxScene, sceneView, filePath, fbxResource.materials);
			FetchMesh(fbxScene, sceneView, fbxResource.tmpMeshes);
			

			FetchAnimation(fbxScene, fbxResource.animationClips, 0, sceneView);

			manager->Destroy();
			std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
			cereal::BinaryOutputArchive serialization(ofs);
			serialization(sceneView, fbxResource);
		}


		bool hasBone = false;
		for (const auto& m : fbxResource.tmpMeshes)
		{
			if (m.bindPose.bones.size() > 0)
			{
				hasBone = true;
			}
		}

		ID3D12Device* device = Argent::Graphics::Graphics::Instance()->GetDevice();

		if (!hasBone)
		{
			std::vector<std::shared_ptr<Resource::Mesh::Mesh>> meshes;
			meshes.resize(fbxResource.tmpMeshes.size());
			for (size_t i = 0; i < meshes.size(); ++i)
			{
				meshes.at(i) = Resource::ResourceManager::Instance().GetMesh(fbxResource.tmpMeshes.at(i).name.c_str());

				if(!meshes.at(i))
				{
					meshes.at(i) = std::make_shared<Resource::Mesh::Mesh>(
						fbxResource.tmpMeshes.at(i).name.c_str(),
						fbxResource.tmpMeshes.at(i).meshResource,
						fbxResource.tmpMeshes.at(i).subsets,
						fbxResource.tmpMeshes.at(i).defaultGlobalTransform);

					Resource::ResourceManager::Instance().RegisterMesh(meshes.at(i));
				}
			}

			ret.resize(meshes.size());
			for(size_t i = 0; i < meshes.size(); ++i)
			{
				ret.at(i) = new Component::Renderer::MeshRenderer(device,
					filePath, meshes.at(i));
			}
		}
		else
		{
			std::vector<std::shared_ptr<Resource::Mesh::SkinnedMesh>> skinnedMeshes;
			skinnedMeshes.resize(fbxResource.tmpMeshes.size());
			for (size_t i = 0; i < skinnedMeshes.size(); ++i)
			{
				skinnedMeshes.at(i) = Resource::ResourceManager::Instance().GetSkinnedMesh(fbxResource.tmpMeshes.at(i).name.c_str());
				if(!skinnedMeshes.at(i))
				{
					skinnedMeshes.at(i) = std::make_shared<Resource::Mesh::SkinnedMesh>(
						fbxResource.tmpMeshes.at(i).name.c_str(), fbxResource.tmpMeshes.at(i).meshResource,
						fbxResource.tmpMeshes.at(i).vertexBones,
						fbxResource.tmpMeshes.at(i).subsets, fbxResource.tmpMeshes.at(i).bindPose);
					Resource::ResourceManager::Instance().RegisterSkinnedMesh(skinnedMeshes.at(i));
				}
			}

			ret.resize(skinnedMeshes.size());
			for (size_t i = 0; i < skinnedMeshes.size(); ++i)
			{
				ret.at(i) = new Component::Renderer::SkinnedMeshRenderer(device, filePath,
					skinnedMeshes.at(i), fbxResource.animationClips);

			}
		}

		return ret;
	}

	void FetchMesh(FbxScene* fbxScene, const ArFbxScene& sceneView, Argent::Resource::Mesh::MeshResource& mResource)
	{
		for (const ArFbxScene::Node& node : sceneView.nodes)
		{
			if (node.attribute != FbxNodeAttribute::EType::eMesh) continue;

			FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };
			FbxMesh* fbxMesh{ fbxNode->GetMesh() };

			const int polygonCount{ fbxMesh->GetPolygonCount() };
			mResource.vertices.resize(polygonCount * 3LL);
			mResource.indices.resize(polygonCount * 3LL);

			FbxStringList uvNames;
			fbxMesh->GetUVSetNames(uvNames);
			const FbxVector4* controlPoints{ fbxMesh->GetControlPoints() };
			for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
			{
				for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
				{
					const int vertexIndex{ polygonIndex * 3 + positionInPolygon };

					Resource::Mesh::Vertex vertex;

					const int polygonVertex{ fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon) };
					vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
					vertex.position.y = static_cast<float>(controlPoints[polygonVertex][2]);
					vertex.position.z = static_cast<float>(-controlPoints[polygonVertex][1]);

					if (fbxMesh->GetElementNormalCount() > 0)
					{
						FbxVector4 normal;
						fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
						vertex.normal.x = static_cast<float>(normal[0]);
						vertex.normal.y = static_cast<float>(normal[2]);
						vertex.normal.z = -static_cast<float>(normal[1]);
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
					mResource.vertices.at(vertexIndex) = std::move(vertex);
					mResource.indices.at(vertexIndex) = vertexIndex;
				}
			}
		}
	}

	void LoadDebug(const char* filePath)
	{
		//シリアライズ
		std::filesystem::path cerealFileName(filePath);
		cerealFileName.replace_extension("cereal");

		ArFbxScene sceneView{};
		Argent::Resource::Mesh::MeshResource mResource;

		FbxManager* manager{ FbxManager::Create() };
		FbxScene* fbxScene{ FbxScene::Create(manager, "") };

		FbxImporter* importer{ FbxImporter::Create(manager, "") };
		bool importState{ FALSE };
		importState = importer->Initialize(filePath);
		_ASSERT_EXPR_A(importState, importer->GetStatus().GetErrorString());

		importState = importer->Import(fbxScene);
		_ASSERT_EXPR_A(importState, importer->GetStatus().GetErrorString());

		Traverse(fbxScene->GetRootNode(), sceneView);

		FetchMesh(fbxScene, sceneView, mResource);

		manager->Destroy();
		std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive serialization(ofs);
		serialization(mResource);
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
			mesh.name = fbxNode->GetName();

			std::vector<std::vector<ArBoneInfluence>> boneInfluences;
			FetchBoneInfluences(fbxMesh, boneInfluences);
			FetchSkeleton(fbxMesh, mesh.bindPose, sceneView);

			auto& subsets{ mesh.subsets };
			const int MaterialCount{ fbxMesh->GetNode()->GetMaterialCount() };
			subsets.resize(MaterialCount > 0 ? MaterialCount : 1);
			for (int materialIndex = 0; materialIndex < MaterialCount; ++materialIndex)
			{
				const FbxSurfaceMaterial* fbxMaterial{ fbxMesh->GetNode()->GetMaterial(materialIndex) };
				subsets.at(materialIndex).materialUniqueId = fbxMaterial->GetUniqueID();
				subsets.at(materialIndex).materialName = fbxMaterial->GetName();
				
				subsets.at(materialIndex).material = Resource::ResourceManager::Instance().GetMaterial(fbxMaterial->GetName());
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
				for (auto& subset : subsets)
				{
					subset.startIndexLocation = offset;
					offset += subset.indexCount;
					subset.indexCount = 0;
				}
			}

			const int polygonCount{ fbxMesh->GetPolygonCount() };
			mesh.meshResource.vertices.resize(polygonCount * 3LL);
			mesh.meshResource.indices.resize(polygonCount * 3LL);
			mesh.vertexBones.resize(polygonCount * 3LL);

			FbxStringList uvNames;
			fbxMesh->GetUVSetNames(uvNames);
			const FbxVector4* controlPoints{ fbxMesh->GetControlPoints() };
			for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
			{
				const int materialIndex{ MaterialCount > 0 ?
					fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) : 0 };
				auto& subset{ subsets.at(materialIndex) };
				const uint32_t offset{ subset.startIndexLocation + subset.indexCount };

				for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
				{
					const int vertexIndex{ polygonIndex * 3 + positionInPolygon };

					Argent::Resource::Mesh::Vertex vertex;
					Argent::Resource::Mesh::BoneVertex bone;

					const int polygonVertex{ fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon) };
					vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
					vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
					vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);


					//bone
					const auto& influencesPerControlPoint{ boneInfluences.at(polygonVertex) };
					for (size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
					{
						if (influenceIndex < MaxBoneInfluences)
						{
							bone.boneWeights[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneWeight;
							bone.boneIndices[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex;
						}
					}
					
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

					mesh.meshResource.vertices.at(vertexIndex) = std::move(vertex);
					mesh.meshResource.indices.at(static_cast<size_t>(offset) + positionInPolygon) = vertexIndex;
					mesh.vertexBones.at(vertexIndex) = std::move(bone);
					++subset.indexCount;
				}
			}
		}
	}

	void FetchSurfaceMaterial(const FbxSurfaceMaterial* surfaceMaterial, const char* pName, const char* fbxFilePath, std::shared_ptr<Material::MeshMaterial> material)
	{
		const FbxProperty fbxProp = surfaceMaterial->FindProperty(pName);

		const std::string PName = pName;
		if (fbxProp.IsValid())
		{
			if(PName == FbxSurfaceMaterial::sDiffuse)
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material->constant.kd.x = static_cast<float>(color[0]);
				material->constant.kd.y = static_cast<float>(color[1]);
				material->constant.kd.z = static_cast<float>(color[2]);
				material->constant.kd.w = 1.0f;

				const FbxFileTexture* fbxTexture{ fbxProp.GetSrcObject<FbxFileTexture>() };
				const char* tmpFilePath = fbxTexture ? fbxTexture->GetRelativeFileName() : "";
				std::filesystem::path path(fbxFilePath);
				path.replace_filename(tmpFilePath);

				material->CreateTexture(path.generic_string().c_str(), Material::MeshMaterial::TextureType::Diffuse);
			}
			if(PName == FbxSurfaceMaterial::sSpecular) 
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material->constant.ks.x = static_cast<float>(color[0]);
				material->constant.ks.y = static_cast<float>(color[1]);
				material->constant.ks.z = static_cast<float>(color[2]);
				material->constant.ks.w = 1.0f;
			}
			if(PName == FbxSurfaceMaterial::sAmbient)
			{
				const FbxDouble3 color{ fbxProp.Get<FbxDouble3>() };
				material->constant.ka.x = static_cast<float>(color[0]);
				material->constant.ka.y = static_cast<float>(color[1]);
				material->constant.ka.z = static_cast<float>(color[2]);
				material->constant.ka.w = 1.0f;
			}
			if(pName == FbxSurfaceMaterial::sNormalMap)
			{
				const FbxFileTexture* fbxTexture{ fbxProp.GetSrcObject<FbxFileTexture>() };
				const char* tmpFilePath = fbxTexture ? fbxTexture->GetRelativeFileName() : "";

				std::filesystem::path path(fbxFilePath);
				path.replace_filename(tmpFilePath);

				material->CreateTexture(path.generic_string().c_str(), Material::MeshMaterial::TextureType::Normal);
			}
			
			
			//if(PName == FbxSurfaceMaterial::sBump)
			//{
			//	const FbxFileTexture* fbxTexture{ fbxProp.GetSrcObject<FbxFileTexture>() };
			//	const char* tmpFilePath = fbxTexture ? fbxTexture->GetRelativeFileName() : "";

			//	//tmpFilePath = "lambert1_Normal_OpenGL.png";
			//	std::filesystem::path path(fbxFilePath);
			//	path.replace_filename(tmpFilePath);

			//	material.CreateTexture(path.generic_string().c_str(), Material::MeshMaterial::TextureType::Normal);
			//}
		}
		else
		{
			if(PName == FbxSurfaceMaterial::sDiffuse)
			{
				material->CreateTexture("", Material::MeshMaterial::TextureType::Diffuse);
			}
			if(pName == FbxSurfaceMaterial::sNormalMap)
			{
				material->CreateTexture("", Material::MeshMaterial::TextureType::Normal);
			}
		}
	}

	void SetDummySurfaceMaterial(std::shared_ptr<Material::MeshMaterial> material)
	{
		material->CreateTexture("", Material::MeshMaterial::TextureType::Diffuse);
		material->CreateTexture("", Material::MeshMaterial::TextureType::Normal);
	}

	void FetchMaterial(FbxScene* fbxScene, const ArFbxScene& sceneView, const char* fbxFilePath, std::unordered_map<uint64_t, std::shared_ptr<Material::MeshMaterial>>& materials)
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
					const char* name = fbxMaterial->GetName();
					std::shared_ptr<Material::MeshMaterial> material = std::make_shared<Material::MeshMaterial>(name);
					UINT64 materialUniqueId = fbxMaterial->GetUniqueID();

					/// スペキュラー　アンビエント　ディフューズ　ノーマルだけ読み込む
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sDiffuse, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sSpecular, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sAmbient, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sNormalMap, fbxFilePath, material);
					FetchSurfaceMaterial(fbxMaterial, FbxSurfaceMaterial::sBump, fbxFilePath, material);

					materials.emplace(materialUniqueId, material);
					Resource::ResourceManager::Instance().RegisterMaterial(material);
				}
			}
			else
			{
				
			}
		}

		if(materials.size() <= 0)
		{
			const char* name = "Dummy";
			auto material = std::make_shared<Material::MeshMaterial>(name);
			SetDummySurfaceMaterial(material);
			materials.emplace(0, material);
		}
	}

	void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<std::vector<ArBoneInfluence>>& boneInfluences)
	{
		const int controlPointsCount{ fbxMesh->GetControlPointsCount() };
		boneInfluences.resize(controlPointsCount);

		const int skinCount{ fbxMesh->GetDeformerCount(FbxDeformer::eSkin) };
		for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)
		{
			const FbxSkin* fbxSkin{ static_cast<FbxSkin*>(fbxMesh->GetDeformer(skinIndex, FbxDeformer::eSkin)) };
			const int clusterCount{ fbxSkin->GetClusterCount() };
			for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				const FbxCluster* fbxCluster{ fbxSkin->GetCluster(clusterIndex) };
				const int controlPointIndicesCount{ fbxCluster->GetControlPointIndicesCount() };
				for (int controlPointIndicesIndex = 0; controlPointIndicesIndex < controlPointIndicesCount;
					++controlPointIndicesIndex)
				{
					const int controlPointIndex{ fbxCluster->GetControlPointIndices()[controlPointIndicesIndex] };
					const double controlPointWeight{ fbxCluster->GetControlPointWeights()[controlPointIndicesIndex] };
					auto& boneInfluence{ boneInfluences.at(controlPointIndex).emplace_back() };
					boneInfluence.boneIndex = static_cast<uint32_t>(clusterIndex);
					boneInfluence.boneWeight = static_cast<float>(controlPointWeight);
				}
			}
		}
	}



	void FetchSkeleton(FbxMesh* fbxMesh, Argent::Resource::Mesh::Skeleton& bindPose, const ArFbxScene& sceneView)
	{
		const int deformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
		{
			FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
			const int clusterCount = skin->GetClusterCount();
			bindPose.bones.resize(clusterCount);
			for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
			{
				FbxCluster* cluster = skin->GetCluster(clusterIndex);

				auto& bone{ bindPose.bones.at(clusterIndex) };
				bone.name = cluster->GetLink()->GetName();
				bone.uniqueId = cluster->GetLink()->GetUniqueID();
				bone.parentIndex = bindPose.indexOf(cluster->GetLink()->GetParent()->GetUniqueID());
				bone.nodeIndex = sceneView.IndexOf(bone.uniqueId);

				FbxAMatrix referenceGlobalInitPosition;
				cluster->GetTransformMatrix(referenceGlobalInitPosition);

				FbxAMatrix clusterGlobalInitPosition;
				cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
				bone.offsetTransform = Argent::Helper::FBX::ToFloat4x4(clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition);
			}
		}
	}

	void FetchAnimation(FbxScene* fbxScene, std::vector<Resource::Animation::AnimationClip>& animationClips,
		float samplingRate, const ArFbxScene& sceneView)
	{
		FbxArray<FbxString*> animationStackNames;
		fbxScene->FillAnimStackNameArray(animationStackNames);
		const int animationStackCount{ animationStackNames.GetCount() };
		for(int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
		{
			auto& animationClip{ animationClips.emplace_back() };
			const char* name = 
			/*animationClip.name =*/ animationStackNames[animationStackIndex]->Buffer();
			animationClip.name = name;

			FbxAnimStack* animationStack{ fbxScene->FindMember<FbxAnimStack>(animationClip.name.c_str()) };
			fbxScene->SetCurrentAnimationStack(animationStack);

			const FbxTime::EMode timeMode{ fbxScene->GetGlobalSettings().GetTimeMode() };
			FbxTime oneSecond{};
			oneSecond.SetTime(0, 0, 1, 0, 0, timeMode);
			animationClip.samplingRate = samplingRate > 0 ? samplingRate : static_cast<float>(oneSecond.GetFrameRate(timeMode));
			const FbxTime samplingInterval{ static_cast<FbxLongLong>(static_cast<float>(oneSecond.Get()) / animationClip.samplingRate) };
			const FbxTakeInfo* takeInfo{ fbxScene->GetTakeInfo(animationClip.name.c_str()) };
			const FbxTime startTime{ takeInfo->mLocalTimeSpan.GetStart() };
			const FbxTime stopTime{ takeInfo->mLocalTimeSpan.GetStop() };
			for(FbxTime time = startTime; time < stopTime; time += samplingInterval)
			{
				auto& keyframe{ animationClip.sequence.emplace_back() };

				const size_t nodeCount{ sceneView.nodes.size() };
				keyframe.nodes.resize(nodeCount);
				for(size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
				{
					FbxNode* fbxNode{ fbxScene->FindNodeByName(sceneView.nodes.at(nodeIndex).name.c_str()) };
					if(fbxNode)
					{
						auto& node{ keyframe.nodes.at(nodeIndex) };
						node.globalTransform = Argent::Helper::FBX::ToFloat4x4(fbxNode->EvaluateGlobalTransform(time));

						const FbxAMatrix& localTransform{ fbxNode->EvaluateLocalTransform(time) };
						node.scaling = Argent::Helper::FBX::Tofloat3(localTransform.GetS());
						node.rotation = Argent::Helper::FBX::ToFloat4(localTransform.GetQ());
						node.translation = Argent::Helper::FBX::Tofloat3(localTransform.GetT());

					}
				}
			}
		}
		for(int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
		{
			delete animationStackNames[animationStackIndex];
		}
	}
}
