#include "CollisionManager.h"

#include "components/TransformComponent.h"
#include "../GameObject.h"

#include <iostream>
#include <glm.hpp>

void CollisionManager::compare_collision(CollisionComponent a, CollisionComponent b)
{
	if (a.gameObject == nullptr || b.gameObject == nullptr) {
		std::cout << "Error: CollisionComponent has no GameObject" << std::endl;
		return;
	}

	glm::vec3 aPos = a.gameObject->transform.position;
	glm::vec3 bPos = b.gameObject->transform.position;

    #pragma region Collision-Detection
	// sphere-sphere collision
	if (a.type == CollisionComponent::CollisionType::Sphere && 
		b.type == CollisionComponent::CollisionType::Sphere) {
		if (glm::distance(aPos, bPos) < a.radius + b.radius) {
			a.on_trigger_enter(b.gameObject);
			b.on_trigger_enter(a.gameObject);
		}
	}

    // box-box collision
    else if (a.type == CollisionComponent::CollisionType::Box &&
        b.type == CollisionComponent::CollisionType::Box) {

        glm::vec3 aSize = glm::vec3(a.gameObject->transform.scale);
        glm::vec3 bSize = glm::vec3(b.gameObject->transform.scale);

        if (aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x &&
            aPos.y < bPos.y + bSize.y && aPos.y + aSize.y > bPos.y &&
            aPos.z < bPos.z + bSize.z && aPos.z + aSize.z > bPos.z) {
            a.on_trigger_enter(b.gameObject);
            b.on_trigger_enter(a.gameObject);
        }
    }

    // sphere-box collision
    else if (a.type == CollisionComponent::CollisionType::Sphere &&
        b.type == CollisionComponent::CollisionType::Box) {
        glm::vec3 bSize = glm::vec3(b.gameObject->transform.scale);
        glm::vec3 closestPoint = glm::clamp(aPos, bPos - bSize / 2.0f,
            bPos + bSize / 2.0f);
        float distance = glm::distance(aPos, closestPoint);
        if (distance < a.radius) {
            a.on_trigger_enter(b.gameObject);
            b.on_trigger_enter(a.gameObject);
        }
    }

    // box-sphere collision
    else if (a.type == CollisionComponent::CollisionType::Box &&
        b.type == CollisionComponent::CollisionType::Sphere) {
        compare_collision(b, a);
    }
    #pragma endregion

}
