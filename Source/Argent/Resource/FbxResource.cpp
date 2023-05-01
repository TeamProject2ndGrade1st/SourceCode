#include "FbxResource.h"
#include "../../../imgui/imgui.h"


void Argent::Resource::Fbx::FbxResource::DrawDebug()
{
	if(!isResize)
	{
		replaceTextureFileName.resize(materials.size());
		for(auto& r : replaceTextureFileName)
		{
			r.replace[0].resize(128);
			r.replace[1].resize(128);
		}
		isResize = true;
	}
	if(ImGui::TreeNode(filePath.c_str()))
	{
		int i = 0;

		if(ImGui::TreeNode("Material"))
		{
			for(auto& m : materials)
			{
				//if(ImGui::TreeNode("Diffuse"))
				{
					ImGui::Text(m.second->textureNames[0].c_str());
					ImGui::InputText("Diffuse", &replaceTextureFileName[i].replace[0].at(0), replaceTextureFileName[i].replace->size());
					//ImGui::TreePop();				
				}
				//if(ImGui::TreeNode("Diffuse"))
				{
					ImGui::Text(m.second->textureNames[1].c_str());
					ImGui::InputText("Normal", &replaceTextureFileName[i].replace[1].at(0), replaceTextureFileName[i].replace->size());
					//ImGui::TreePop();
				}
				if(ImGui::Button("Accept Diffuse"))
				{
					m.second->textureNames[0] = replaceTextureFileName.at(i).replace[0];
				}
				if(ImGui::Button("Accept Normal"))
				{
					m.second->textureNames[1] = replaceTextureFileName.at(i).replace[1];
				}
				++i;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

}
