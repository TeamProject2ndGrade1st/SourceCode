#include "ImguiCtrl.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "imgui_internal.h"

void ImguiCtrl::Begin(const char* windowName)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_::ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 500.0f), ImGuiCond_::ImGuiCond_Once);
	ImGui::Begin(windowName);
}

void ImguiCtrl::End(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* heap)
{
	ImGui::End();
	ImGui::Render();
	cmdList->SetDescriptorHeaps(1, &heap);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

void ImguiCtrl::CallBeforeSwap(ID3D12GraphicsCommandList* cmdList)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault(NULL, (void*)cmdList);
	}
}

void ::ImguiCtrl::Initialize(HWND hWnd, ID3D12Device* device, ID3D12DescriptorHeap* heap)
{
	IMGUI_CHECKVERSION();
	if(!ImGui::CreateContext()) assert(0);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/migu-1m.ttf", 
		16.0f, nullptr);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX12_Init(device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, 
		heap, heap->GetCPUDescriptorHandleForHeapStart(),
		heap->GetGPUDescriptorHandleForHeapStart());
}

void ImguiCtrl::Terminate()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
