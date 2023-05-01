#include "FbxResource.h"
#include "../../../imgui/imgui.h"


void Argent::Resource::Fbx::FbxResource::DrawDebug()
{
	if(ImGui::TreeNode(filePath.c_str()))
	{
		int i = 0;

		for(auto& mat : materials)
		{
			if(!isResize)
			{
				mat.second->replace.diffuse.resize(128);
				mat.second->replace.normal.resize(128);
				isResize = true;
			}
			auto& m = mat.second;
			

			ImGui::Text(m->textureNames[0].c_str());
			ImGui::InputText("Diffuse", &m->replace.diffuse.at(0), m->replace.diffuse.size());

			ImGui::Text(m->textureNames[1].c_str());
			ImGui::InputText("Normal", &m->replace.normal.at(0), m->replace.normal.size());

			//’u‚«Š·‚¦‚é‚Æ‚±‚ë
			if(ImGui::Button("Accept Diffuse"))
			{
				m->textureNames[0] = m->replace.diffuse;
			}
			if(ImGui::Button("Accept Normal"))
			{
				m->textureNames[1] = m->replace.normal;
			}
			++i;

		}
		ImGui::TreePop();
	}

}
