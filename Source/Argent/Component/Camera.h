#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Transform.h"

class GameObject;

class Camera final:
	public Argent::Component::BaseComponent
{
public:
	Camera(bool isSceneCamera, float width, float height, float nearZ = 0.01f, float farZ = 5000.0f, float fov = 60.0f,
		const Transform* target = nullptr);
	~Camera() override = default;

	void Reset() override;
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;

	void DrawDebug() override;

	[[nodiscard]] DirectX::XMMATRIX GetViewProjectionMatrix() const;
	[[nodiscard]] DirectX::XMMATRIX GetViewMatrix() const;
	[[nodiscard]] DirectX::XMMATRIX GetProjectionMatrix() const;

	bool GetIsSceneCamera() const { return isSceneCamera; }

	void OnSceneCamera() { isSceneCamera = true; }
	void OffSceneCamera() { isSceneCamera = false; }

	DirectX::XMFLOAT3 GetForward() const { return forward; }
	DirectX::XMFLOAT3 GetRight() const { return right; }
	DirectX::XMFLOAT3 GetUp() const { return up; }

	void SetFov(float f) { fov = f; }
	void SetWidth(float f) { width = f; }
	void SetHeight(float f) { height = f; }


	DirectX::XMFLOAT4 GetMaxRotation() const { return maxRotation; }
	void SetMaxRotation(const DirectX::XMFLOAT4& mRot) { maxRotation = mRot; useMaxRotation = true; }
	DirectX::XMFLOAT4 GetMinRotation() const { return minRotation; }
	void SetMinRotation(const DirectX::XMFLOAT4& mRot) { minRotation = mRot;  useMinRotation = true; }

private:
	float width;
	float height;
	float nearZ;
	float farZ;
	float fov;
	float aspectRatio;

	bool isSceneCamera;
	DirectX::XMFLOAT4 maxRotation;
	DirectX::XMFLOAT4 minRotation;
	bool useMaxRotation = false;
	bool useMinRotation = false;

	DirectX::XMFLOAT3 forward;
	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 up;
	static constexpr DirectX::XMVECTOR ForwardVec = { 0, 0, 1 };
	static constexpr DirectX::XMVECTOR RightVec = { 1, 0, 0 };
	static constexpr DirectX::XMVECTOR UpVec = { 0, 1, 0 };
};


class CameraController:
	public Argent::Component::BaseComponent
{
public:
	CameraController();
	~CameraController() override = default;

	void Update();
#ifdef _DEBUG
	void DrawDebug() override;
#endif
private:
	float rotationSpeed;
	float vertAndHorMoveSpeed;
	float depthMoveSpeed;
};