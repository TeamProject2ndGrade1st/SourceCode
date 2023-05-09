#pragma once
#include "Component.h"
#include "../Component/Transform.h"

namespace Argent::Component::Collider
{
	class Collider:
		public BaseComponent
	{
	public:
		enum class ColliderType
		{
			Box,
			Sphere,
			Cylinder,
			Capsule,
			RayCast,
			Max,
		};

		Collider(const char* name, ColliderType type);
		virtual ~Collider() override;


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

		bool DetectionOnly(Collider* other,Transform* transform);


		void DrawDebug() override;

		float GetRadius()const { return radius; }


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


		void DrawDebug() override;


	protected:
		float radius{};
		float height{};
	};
}

