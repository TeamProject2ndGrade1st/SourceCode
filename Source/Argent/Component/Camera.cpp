#include "Camera.h"
#include "../Graphic/Graphics.h"
#include "../Input/Keyboard.h"
#include "../GameObject/GameObject.h"
#include "../Math/MathHelper.h"

Camera::Camera(bool isSceneCamera, float width, float height, float nearZ, float farZ, 
               float fov, const Transform* target):
	BaseComponent("Camera")
,	isSceneCamera(isSceneCamera)
,	width(width)
,	height(height)
,	nearZ(nearZ)
,	farZ(farZ)
,	fov(fov)
,	aspectRatio(width / height)
,	forward(DirectX::XMFLOAT3(0, 0, 1))
,	right(DirectX::XMFLOAT3(1, 0, 0))
,	up(DirectX::XMFLOAT3(0, 1, 0))
{}

void Camera::Reset()
{
	Argent::Component::BaseComponent::Reset();
}

void Camera::Initialize()
{
	DirectX::XMFLOAT3 pos =  GetOwner()->GetTransform()->GetPosition();
	pos.z = -10.0f;
	GetOwner()->GetTransform()->SetPosition(pos);
	Argent::Component::BaseComponent::Initialize();
}

void Camera::Update()
{
	if(!isSceneCamera) return;

	Transform* transform = GetOwner()->GetTransform();

	if (!transform) _ASSERT_EXPR(FALSE, "Missing Transform Component");
	DirectX::XMFLOAT4 rotate = transform->GetRotation();
	const DirectX::XMFLOAT3 pos = transform->GetPosition();
	DirectX::XMVECTOR focus;

	focus = DirectX::XMLoadFloat3(&forward);
	
	DirectX::XMVECTOR q = DirectX::XMVector3AngleBetweenVectors(DirectX::XMVector3Normalize(focus), DirectX::XMVector3Normalize(ForwardVec));

	DirectX::XMFLOAT4 rot{};
	DirectX::XMStoreFloat4(&rot, q);

	rot.x = DirectX::XMConvertToDegrees(rot.x);
	rot.y = DirectX::XMConvertToDegrees(rot.y);
	rot.z = DirectX::XMConvertToDegrees(rot.z);
	rot.w = 0;

	//transform->SetRotation(rot);


	const DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationMatrix(
		DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rotate.x), 
			DirectX::XMConvertToRadians(rotate.y), 
			DirectX::XMConvertToRadians(rotate.z)));

	//float angle;
	//DirectX::XMStoreFloat(&angle, DirectX::XMVector3Dot(ForwardVec, focus));
	//angle = acosf(angle);


	//�^�[�Q�b�g�����Ɍ�������
	{
		const DirectX::XMFLOAT3 normX{ 1, 0, 0 };
		const DirectX::XMFLOAT3 normY{ 0, 1, 0 };
		const DirectX::XMFLOAT3 normZ{ 0, 0, 1 };
		//x ����
		{
			DirectX::XMFLOAT3 focusX{};
			DirectX::XMStoreFloat3(&focusX, focus);
			focusX.x = 0;
			//�P�ʃx�N�g����
			DirectX::XMStoreFloat3(&focusX, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&focusX)));

			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&focusX), DirectX::XMLoadFloat3(&normZ));
			DirectX::XMVECTOR Corss = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&focusX), DirectX::XMLoadFloat3(&normZ));
			DirectX::XMFLOAT3 cross;
			DirectX::XMStoreFloat3(&cross, Corss);

			float angle{};
			DirectX::XMStoreFloat(&angle, Dot);
			angle = acosf(angle);
			if(cross.x > 0)
			{
				angle = -angle;
			}
			rotate.x = DirectX::XMConvertToDegrees(angle);
		}

		{
			DirectX::XMFLOAT3 focusY{};
			DirectX::XMStoreFloat3(&focusY, focus);
			focusY.y = 0;
			//�P�ʃx�N�g����
			DirectX::XMStoreFloat3(&focusY, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&focusY)));

			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&focusY), DirectX::XMLoadFloat3(&normZ));
			DirectX::XMVECTOR Corss = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&focusY), DirectX::XMLoadFloat3(&normZ));
			DirectX::XMFLOAT3 cross;
			DirectX::XMStoreFloat3(&cross, Corss);

			float angle{};
			DirectX::XMStoreFloat(&angle, Dot);
			angle = acosf(angle);
			if(cross.y > 0)
			{
				angle = -angle;
			}
			rotate.y = DirectX::XMConvertToDegrees(angle);
		}

		{
			rotate.z = 0;
		}
	}

	//transform->SetRotation(rotate);

	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(ForwardVec, quaternion)));
	//DirectX::XMStoreFloat3(&forward, focus);
	DirectX::XMStoreFloat3(&right, DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(RightVec, quaternion)));
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Normalize(DirectX::XMVector3Rotate(UpVec, quaternion)));

	BaseComponent::Update();
}

#ifdef _DEBUG
void Camera::DrawDebug()
{

	if(ImGui::TreeNode("Camera"))
	{
		if(ImGui::Button("Use Scene Camera"))
		{
			Argent::Graphics::ArGraphics::Instance()->SetCamera(this);
			isSceneCamera = true;
		}


		ImGui::DragFloat("Width", &width, 0.1f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("Height", &height, 0.1f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("Fov", &fov, 1.0f, 
			0, DirectX::XMConvertToDegrees(DirectX::XM_PI));


		ImGui::InputFloat3("Forward", &forward.x);
		ImGui::InputFloat3("Right", &right.x);
		ImGui::InputFloat3("Up", &up.x); 
		ImGui::SliderFloat("Far", &farZ, 1, 10000);

		ImGui::TreePop();
		BaseComponent::DrawDebug();
	}
}
#endif

DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
{
	return GetViewMatrix() * GetProjectionMatrix();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	const Transform* transform = GetOwner()->GetTransform();
	const DirectX::XMVECTOR forwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR focus;

	const DirectX::XMFLOAT3 pos = transform->GetPosition();
	const DirectX::XMFLOAT3 targetPos = pos + forward;
	focus = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPos), DirectX::XMLoadFloat3(&pos));

	const auto eye = DirectX::XMLoadFloat3(&pos);

	DirectX::XMMATRIX Rot = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(transform->GetRotation().x),
				DirectX::XMConvertToRadians(transform->GetRotation().y), 
				DirectX::XMConvertToRadians(transform->GetRotation().z));
	DirectX::XMMATRIX Eye = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX View = Rot * Eye;
	return DirectX::XMMatrixInverse(nullptr, View);

	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, focus, up);//
	return view;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), width / height, nearZ, farZ);
}

CameraController::CameraController():
	BaseComponent("Camera Controller")
,	rotationSpeed(0.05f)
,	vertAndHorMoveSpeed(0.01f)
,	depthMoveSpeed(0.2f)
{}

void CameraController::Update()
{
	//Move
	Transform* transform = GetOwner()->GetTransform();
	Camera* camera = GetOwner()->GetComponent<Camera>();

	if(!camera->GetIsSceneCamera()) return;

	if(Argent::Input::Mouse::Instance().IsButtonPress(Argent::Input::Mouse::Mouses::mMiddleButton))
	{
		DirectX::XMFLOAT3 position = transform->GetPosition();
		if(!camera) return;

		DirectX::XMFLOAT3 Right = camera->GetRight();
		DirectX::XMFLOAT3 Up = camera->GetUp();

		const DirectX::XMFLOAT2 moveVec = Argent::Input::Mouse::Instance().GetMoveVec();
		DirectX::XMStoreFloat3(&Right, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Right), -moveVec.x * vertAndHorMoveSpeed));
		DirectX::XMStoreFloat3(&Up, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Up), moveVec.y * vertAndHorMoveSpeed));


		position.x += Right.x + Up.x;
		position.y += Right.y + Up.y;
		position.z += Right.z + Up.z;

		transform->SetPosition(position); 
	}

	//Rotate
	if(Argent::Input::Mouse::Instance().IsButtonPress(Argent::Input::Mouse::Mouses::mRightButton))
	{
		DirectX::XMFLOAT4 rotation = transform->GetRotation();

		DirectX::XMFLOAT2 moveVec = Argent::Input::Mouse::Instance().GetMoveVec();

		moveVec.x *= rotationSpeed;
		moveVec.y *= rotationSpeed;

		rotation.x += moveVec.y;
		rotation.y += moveVec.x;

		transform->SetRotation(rotation);
	}


	if(Argent::Input::Mouse::Instance().IsMouseWheelRotate())
	{
		float RowWheelRotateValue = Argent::Input::Mouse::Instance().GetRowWheelRotateValue();
		RowWheelRotateValue /= WHEEL_DELTA;
		const float DepthTransitionScale = RowWheelRotateValue * depthMoveSpeed;
		DirectX::XMFLOAT3 position = transform->GetPosition();
		DirectX::XMFLOAT3 Forward = camera->GetForward();
		DirectX::XMStoreFloat3(&Forward, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Forward), 
			DepthTransitionScale));

		position.x += Forward.x;
		position.y += Forward.y;
		position.z += Forward.z;

		transform->SetPosition(position);
	}
}

#ifdef _DEBUG
void CameraController::DrawDebug()
{
	if(ImGui::TreeNode("Camera Controller"))
	{
		ImGui::DragFloat("Vert Hor Speed", &vertAndHorMoveSpeed, 0.001f, 0.001f, 1.0f);
		ImGui::DragFloat("Rotate Speed", &rotationSpeed, 0.001f, 0.001f, 1.0f);
		ImGui::DragFloat("Depth Speed", &depthMoveSpeed, 0.01f, 0.01f, 10.0f);

		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
#endif