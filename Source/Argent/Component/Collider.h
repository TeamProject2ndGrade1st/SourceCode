#pragma once
#include "ArComponent.h"

namespace Argent::Component::Collider
{
	class Collider:
		public ArComponent
	{
	public:
		enum class Type
		{
			Box,
			Sphere,
			Cylinder,
			Capsule,
		};

		Collider(const char* name):
			ArComponent(name)
		{}
		virtual ~Collider() override = default;


	};

	class SphereCollider:
		public Collider
	{
	public:
		SphereCollider():
			Collider("SphereCollider")
		{}
		~SphereCollider() override = default;


	};

}

