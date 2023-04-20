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
	Transform& operator=(const Transform& t);

	void Initialize() override;
	void Update() override;

	void DrawDebug() override;


	void Reset() override;
	void SetWorld(const DirectX::XMFLOAT4X4& w);

	[[nodiscard]] DirectX::XMMATRIX CalcWorldMatrix();

	[[nodiscard]] DirectX::XMFLOAT4X4 GetWorld()
	{
		DirectX::XMFLOAT4X4 tmp{};
		DirectX::XMStoreFloat4x4(&tmp, CalcWorldMatrix());
		return tmp;
	}

	DirectX::XMFLOAT3 GetPosition() const { return position; }
	DirectX::XMFLOAT3 GetScale() const { return scale; }
	DirectX::XMFLOAT4 GetRotation() const { return rotation; }

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
	 * \brief 正規化された前方ベクトルを返す(回転値が(0, 0, 0)の時の前方ベクトルは(0, 0, 1))
	 */
	DirectX::XMFLOAT3 CalcForward();
	/**
	 * \brief 
	 * \return 正規化済み上方ベクトルを返す（回転値(0, 0, 0)の場合は(0, 1, 0)
	 */
	DirectX::XMFLOAT3 CalcUp();
	DirectX::XMFLOAT3 CalcRight();
private:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT4 postRotation;
	DirectX::XMFLOAT4 orientation;

	float scaleFactor;

	int coordinateSystem;
	DirectX::XMFLOAT4X4 defaultWorld
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

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
