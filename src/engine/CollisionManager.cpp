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

        glm::vec3 aSize = glm::vec3(a.radius);
        glm::vec3 bSize = glm::vec3(b.radius);

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
        glm::vec3 bSize = glm::vec3(b.radius);
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

	// ray-sphere collision
	else if (a.type == CollisionComponent::CollisionType::Ray && 
        b.type == CollisionComponent::CollisionType::Sphere) {
		
		glm::vec3 rayOrigin = aPos;
		glm::vec3 rayDirection = glm::normalize(rayOrigin - (rayOrigin + glm::vec3(0, 0, a.radius)));

		glm::vec3 sphereCenter = bPos;

		for (int i = 0; i < a.radius; i++) {
			glm::vec3 point = rayOrigin + (rayDirection * glm::vec3(i));
			if (glm::distance(point, sphereCenter) < b.radius) {
				a.on_trigger_enter(b.gameObject);
				b.on_trigger_enter(a.gameObject);
				break;
			}
		}
	}

	// sphere-ray collision
	else if (a.type == CollisionComponent::CollisionType::Sphere && 
        b.type == CollisionComponent::CollisionType::Ray) {
		compare_collision(b, a);
	}

	// ray-box collision
    else if (a.type == CollisionComponent::CollisionType::Ray &&
        b.type == CollisionComponent::CollisionType::Box) {

        glm::vec3 rayOrigin = aPos;
        glm::vec3 rayDir = glm::normalize(glm::vec3(0, 0, a.radius));
        glm::vec3 boxMin = bPos - glm::vec3(b.radius);
        glm::vec3 boxMax = bPos + glm::vec3(b.radius);

        glm::vec3 tMin = (boxMin - rayOrigin) / rayDir;
        glm::vec3 tMax = (boxMax - rayOrigin) / rayDir;

        glm::vec3 tEnter = glm::min(tMin, tMax);
        glm::vec3 tExit = glm::max(tMin, tMax);

        float tNear = glm::max(glm::max(tEnter.x, tEnter.y), tEnter.z);
        float tFar = glm::min(glm::min(tExit.x, tExit.y), tExit.z);

        if (tNear < tFar && tFar > 0) {
            a.on_trigger_enter(b.gameObject);
            b.on_trigger_enter(a.gameObject);
        }
    }

	// box-ray collision
	else if (a.type == CollisionComponent::CollisionType::Box &&
		b.type == CollisionComponent::CollisionType::Ray) {
		compare_collision(b, a);
	}

    #pragma endregion

}
