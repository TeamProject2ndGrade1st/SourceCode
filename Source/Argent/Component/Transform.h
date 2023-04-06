#pragma once
#include "Component.h"
#include <DirectXMath.h>

class Transform final :
	public Argent::Component::BaseComponent
{
public:
	enum class CoordinateSystem
	{
		cRightYup,
		cLeftYup,
		cRightZup,
		cLeftZup,
		cNone,
	};


	Transform():
		BaseComponent("Transform")
	,	position(DirectX::XMFLOAT3())
	,	scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
	,	rotation(DirectX::XMFLOAT4())
	,	scaleFactor(1.0f)
	,	coordinateSystem(static_cast<int>(CoordinateSystem::cRightYup))
	{}

	~Transform() override = default;
	Transform(const Transform&) = default;
	Transform& operator+=(const Transform& t);
	Transform operator=(const Transform& t);
#ifdef _DEBUG
	void DrawDebug() override;
#endif

	void Reset() override;
	void SetWorld(const DirectX::XMFLOAT4X4& w);

	[[nodiscard]] DirectX::XMMATRIX GetMatrix() const
	{
		const DirectX::XMMATRIX C = { DirectX:: XMLoadFloat4x4(&CoordinateSystemTransforms[coordinateSystem]) * 
			DirectX:: XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
		const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)};
		const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.z))};
		const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(position.x,position.y, position.z)};
		return C * S * R * T;
	}

	[[nodiscard]] DirectX::XMFLOAT4X4 GetWorld() const
	{
		DirectX::XMFLOAT4X4 tmp{};
		DirectX::XMStoreFloat4x4(&tmp, GetMatrix());
		return tmp;
	}

	[[nodiscard]] DirectX::XMFLOAT3 GetPosition() const { return position; }
	[[nodiscard]] DirectX::XMFLOAT3 GetScale() const { return scale; }
	[[nodiscard]] DirectX::XMFLOAT4 GetRotation() const { return rotation; }

	void SetPosition(const DirectX::XMFLOAT3& pos) { position = pos; }
	void SetScale(const DirectX::XMFLOAT3& scl) { scale = scl; }
	void SetRotation(const DirectX::XMFLOAT4& rot) { rotation = rot; }

	void AddPosition(const DirectX::XMFLOAT3& pos)
	{
		position.x += pos.x;
		position.y += pos.y;
		position.z += pos.z;
	}

	Transform AdjustParentTransform() const;

	float GetScaleFactor() const { return scaleFactor;  }
	void SetScaleFactor(float f) { scaleFactor = f;  }

	void SetTransform(const Transform& t);

	/**
	 * \brief ���K�����ꂽ�O���x�N�g����Ԃ�(��]�l��(0, 0, 0)�̎��̑O���x�N�g����(0, 0, 1))
	 */
	DirectX::XMFLOAT3 CalcForward();
private:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotation;
	float scaleFactor;

	int coordinateSystem;

	const DirectX::XMFLOAT4X4 CoordinateSystemTransforms[static_cast<int>(CoordinateSystem::cNone)]
	{
		{
			-1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1,
		},

		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		},

		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		},
		{
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1
		}
	};

	const std::string coordinateSystemName[static_cast<int>(CoordinateSystem::cNone)]
	{
		"Right Hand, Y up",
		"Left Hand, Y up",
		"Right Hand, Z up",
		"Left Hand, Z up"
	};
};

//Transform AdjustParentTransform(const Transform& t);
