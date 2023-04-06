#include "Effect.h"

#include "ArResource.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "../Other/Helper.h"

namespace Argent::Resource::Effect
{
	EffectResource::EffectResource(const char* filePath, const char* materialPath):
		ArImportedResource(filePath, ResourceType::Effect)
	{
		std::wstring fPath = Helper::String::GetWideStringFromString(filePath);
		std::wstring mPath = Helper::String::GetWideStringFromString(materialPath);
		effect = Effekseer::Effect::Create(Argent::Resource::Effect::EffectManager::Instance()->GetManager(), (const EFK_CHAR*)(fPath.c_str()),
			1.0f, (const EFK_CHAR*)(mPath.c_str()));
		if (!effect.Get()) _ASSERT_EXPR(FALSE, L"failepath missing");

	}

	void EffectResource::Play(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT4& rotate, float startFrame)
	{
		handle = EffectManager::Instance()->GetManager()->Play(effect, Helper::Effect::ToVector3D(position), static_cast<int32_t>(startFrame));
	}

	void EffectResource::Update(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT4& rotate, const DirectX::XMFLOAT4& color)
	{
		EffectManager::Instance()->GetManager()->SetRotation(handle, rotate.x, rotate.y, rotate.z);
		EffectManager::Instance()->GetManager()->SetScale(handle, scale.x, scale.y, scale.z);
		EffectManager::Instance()->GetManager()->SetLocation(handle, position.x, position.y, position.z);

		Effekseer::Color col{static_cast<unsigned char>(color.x * 255),
			static_cast<unsigned char>(color.y * 255),
		static_cast<unsigned char>(color.z * 255),
		static_cast<unsigned char>(color.w * 255)};
		EffectManager::Instance()->GetManager()->SetAllColor(handle, col);
	}

	void EffectResource::Stop() const
	{
		EffectManager::Instance()->GetManager()->StopEffect(handle);
	}

	bool EffectResource::IsExist() const
	{
		return EffectManager::Instance()->GetManager()->Exists(handle);
	}
}
