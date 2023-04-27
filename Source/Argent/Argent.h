#pragma once

#include "Core/ArWindow.h"
#include "Core/Timer.h"
#include "Core/Random.h"
#include "Graphic/Graphics.h"
#include "Core/App.h"


#include "Input/Keyboard.h"
#include "Input/Mouse.h"


#include "Component/Component.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Collider.h"
#include "Component/AudioPlayer.h"

#include "Component/Renderer.h"
#include "Component/SpriteRenderer.h"
#include "Component/MeshRenderer.h"
#include "Component/SkinnedMeshRenderer.h"
#include "Component/EffekseerEmitter.h"

#include "Component/BaseActor.h"


#include "Loader/FbxLoader.h"


#include "GameObject/GameObject.h"

#include "Scene/BaseScene.h"
#include "Scene/SceneManager.h"

#include "Component/Collider.h"
#include "Component/RayCast.h"
#include "Component/ColliderManager.h"

#include "Math/MathHelper.h"

#define AR_DEBUG 1

using KeyCode = Argent::Input::Keyboard::KeyCode;
using HitResult = Argent::Component::Collision::HitResult;
using MouseButton = Argent::Input::Mouse::Button;

namespace Argent
{
	namespace App
	{
		inline void Quit()
		{
			ArApp::Quit();
		}
	}
	namespace Scene
	{
		inline BaseScene* GetCurrentScene()
		{
			return Scene::SceneManager::Instance()->GetCurrentScene();
		}
	}

	namespace Input
	{
		/**
		 * \brief ‰Ÿ‚µ‚Ä‚¢‚éŠÔtrue‚ğ•Ô‚·
		 * \param keyCode 
		 * \return 
		 */
		inline bool GetKey(KeyCode keyCode)
		{
			return Keyboard::Instance().GetKey(keyCode);
		}

		/**
		 * \brief ‰Ÿ‚µ‚½uŠÔtrue‚ğ•Ô‚·
		 * \param keyCode 
		 * \return 
		 */
		inline bool GetKeyDown(KeyCode keyCode)
		{
			return Keyboard::Instance().GetKeyDown(keyCode);
		}

		/**
		 * \brief —£‚µ‚½uŠÔtrue‚ğ•Ô‚·
		 * \param keyCode 
		 * \return 
		 */
		inline bool GetKeyUp(KeyCode keyCode)
		{
			return Keyboard::Instance().GetKeyUp(keyCode);
		}

		inline DirectX::XMFLOAT2 GetMousePosition()
		{
			return Mouse::Instance().GetPosition();
		}

		inline DirectX::XMFLOAT2 GetMouseMoveVector()
		{
			return Mouse::Instance().GetMoveVec();
		}

		inline bool GetButton(Mouse::Button button)
		{
			return Mouse::Instance().GetButton(button);
		}

		inline bool GetButtonDown(Mouse::Button button)
		{
			return Mouse::Instance().GetButtonDown(button);
		}

		inline bool GetButtonUp(Mouse::Button button)
		{
			return Mouse::Instance().GetButtonUp(button);
		}
	}

	namespace Collision
	{
		inline bool RayCollisionDetection(Component::Collision::RayCast* ray, HitResult& hitResult, 
			GameObject::Tag tag = COLLISION_ALL_OBJECT)
		{
			return Argent::Collider::ColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult, static_cast<unsigned>(tag));
		}
	}

	namespace Timer
	{
		inline float GetDeltaTime()
		{
			return ArTimer::Instance().DeltaTime();
		}
	}

	namespace Graphics
	{
		inline float GetWindowWidth()
		{
			return Graphics::Instance()->GetWidth();
		}

		inline float GetWindowHeight()
		{
			return Graphics::Instance()->GetHeight();
		}
	}
}