#include "Light.h"

void Light::Reset()
{
	color.Reset();
	BaseComponent::Reset();
}
#ifdef _DEBUG
void Light::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		color.DrawDebug();
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
#endif
