#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

class GameObject;
class Component;

class ComponentManager {
public:
    ComponentManager();
    ~ComponentManager() {
        for (Component* comp : componentsToDelete) {
            delete comp;
        }
    }

    Component* create_component(const std::string& component_name, GameObject* attachedTo);

    std::unordered_map<std::string, std::function<Component* (GameObject*)>> componentMap;
    std::vector<Component*> componentsToDelete;

    template <typename T>
    void register_component(const std::string& name);
};

template<typename T>
inline void ComponentManager::register_component(const std::string& name) {
    componentMap[name] = [](GameObject* attachedTo) { return new T(attachedTo); };
}
