#include "ComponentManager.h"

#include "components/Component.h"

#include "components/TransformComponent.h"
#include "components/MeshComponent.h"
#include "components/TestMoveComponent.h"

ComponentManager::ComponentManager()
{
	register_component<TransformComponent>("TransformComponent");
	register_component<MeshComponent>("MeshComponent");
	register_component<TestMoveComponent>("TestMoveComponent");
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