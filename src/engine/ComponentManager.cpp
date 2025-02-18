#include "ComponentManager.h"

#include "Component.h"

#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/TestMoveComponent.h"
#include "components/LightComponent.h"
#include "components/CollisionComponent.h"

ComponentManager::ComponentManager()
{
	register_component<TransformComponent>("TransformComponent");
	register_component<MeshComponent>("MeshComponent");
	register_component<TestMoveComponent>("TestMoveComponent");
	register_component<LightComponent>("LightComponent");
	register_component<CollisionComponent>("CollisionComponent");
}

Component* ComponentManager::create_component(const std::string& component_name, GameObject* attachedTo)
{
	for (auto& [key, comp] : componentMap) {
		if (component_name == key) {
			Component* newComponent = comp(attachedTo);
			componentsToDelete.push_back(newComponent);
			return newComponent;
		}
	}
	return nullptr;
}