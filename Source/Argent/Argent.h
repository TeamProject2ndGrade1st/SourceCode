#pragma once

#include "Core/ArWindow.h"
#include "Core/Timer.h"
#include "Core/Random.h"
#include "Core/App.h"


#include "Input/Keyboard.h"


#include "Component/Component.h"
#include "Component/Transform.h"
#include "Component/Collider.h"
#include "Component/AudioPlayer.h"

#include "Component/Renderer.h"
#include "Component/SpriteRenderer.h"
#include "Component/MeshRenderer.h"
#include "Component/SkinnedMeshRenderer.h"
#include "Component/EffectRenderer.h"

#include "Component/BaseActor.h"


#include "Loader/FbxLoader.h"


#include "GameObject/GameObject.h"

#include "Scene/BaseScene.h"
#include "Scene/SceneManager.h"

#include "Math/MathHelper.h"

using KeyCode = Argent::Input::Keyboard::KeyCode;
namespace Argent
{
	namespace Scene
	{
		inline BaseScene* GetCurrentScene()
		{
			return Scene::ArSceneManager::Instance()->GetCurrentScene();
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
	}

	namespace Timer
	{
		inline float GetDeltaTime()
		{
			return ArTimer::Instance().DeltaTime();
		}
	}
}