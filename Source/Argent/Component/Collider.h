#pragma once
#include "Component.h"

namespace Argent::Component::Collider
{
	class Collider:
		public ArComponent
	{
	public:
		enum class ColliderType
		{
			Box,
			Sphere,
			Cylinder,
			Capsule,
			Max,
		};

		Collider(const char* name, ColliderType type);
		virtual ~Collider() override = default;


		void OnCollision(const Collider* collider);
		virtual bool CollisionDetection(Collider* other) = 0;

		ColliderType GetType() const { return type; }

	protected:
		ColliderType type;
	};

	class SphereCollider:
		public Collider
	{
	public:
		SphereCollider(float radius = 1.0f):
			Collider("SphereCollider", ColliderType::Sphere)
		,	radius(radius)
		{}
		~SphereCollider() override = default;

		bool CollisionDetection(Collider* other) override;

#ifdef _DEBUG
		void DrawDebug() override;
#endif

	protected:
		float radius{};

	};

	class Capsule:
		public Collider
	{
	public:
		Capsule(float radius = 1.0f, float height = 1.0f) :
			Collider("Capsule", ColliderType::Capsule)
		,	radius(radius)
		,	height(height)
		{}
		~Capsule() override = default;

		bool CollisionDetection(Collider* other) override;

#ifdef _DEBUG
		void DrawDebug() override;
#endif

	protected:
		float radius{};
		float height{};
	};
}

