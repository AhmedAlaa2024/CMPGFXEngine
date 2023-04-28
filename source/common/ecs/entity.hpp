#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>

namespace our {

    class World; // A forward declaration of the World Class

    class Entity{
        World *world; // This defines what world own this entity
        std::list<Component*> components; // A list of components that are owned by this entity

        friend World; // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity
    public:
        std::string name; // The name of the entity. It could be useful to refer to an entity by its name
        Entity* parent;   // The parent of the entity. The transform of the entity is relative to its parent.
                          // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.

        World* getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4 getLocalToWorldMatrix() const; // Computes and returns the transformation from the entities local space to the world space
        void deserialize(const nlohmann::json&); // Deserializes the entity data and components from a json object
        
        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it 
        template<typename T>
        T* addComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            //TODO: (Req 8) Create an component of type T, set its "owner" to be this entity, then push it into the component's list
            // Don't forget to return a pointer to the new component

            // dynamically allocates memory for a new object of type T and returns a pointer to it, where T class is child to component class
            T* component = new T(); 
            // since this new component is added to this entity(container of components), set its "owner" to be this entity
            component->owner = this;
            // push it into the component's list of the entity(container of components)
            this->components.push_back(component);
            // return a pointer to the newly component added to this entity 
            return component;
        }

        // This template method searhes for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(){
            //TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // Return the component you found, or return null of nothing was found.

            // to iterate over the list of components in this entity, you need an iterator
            std::list<Component*>::iterator itr;
            // iterator firstly points to the first element in the list
            itr = components.begin();
            // for loop to increment iterator to point to the next element after each check 
            for(itr; itr != components.end(); itr++){
                // use dynamic allocation to find component of type T* in the list of components using the iterator
                T* t = dynamic_cast<T*>(*itr);
                // dynamic cast return point if an element is found, if not it will return null. so check it
                if(t)
                // return pointer to the found component of type T
                    return t;
            }
            // if no element of type T* is found in the list, return nullptr
            return nullptr;
        }

        // This template method dynami and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end())
                return dynamic_cast<T*>(*it);
            return nullptr;
        }

        // This template method searhes for a component of type T and deletes it
        template<typename T>
        void deleteComponent(){
            //TODO: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // If found, delete the found component and remove it from the components list

            // use iterator to iterate over the list of components in this entity
            std::list<Component*>::iterator itr;
            // initially iterator points to the first element in list
            itr = components.begin();
            // for loop to increment iterator after each check to point to the next element
            for(itr; itr != components.end(); itr++){
                // use dynamic cast to check if component is of type T*, since T is child to component
                T* t = dynamic_cast<T*>(*itr);
                // dynamic cast returns a pointer to the component of type T* if found, else it returns null
                if(t){
                    // if component of type T* is found delete it
                    // delete *itr is used to deallocate the memory associated with the object pointed to by itr
                    delete *itr;
                    // list.erase(itr) is used to remove the pointer from the list.
                    components.erase(itr);
                    // return finally :)
                    return;
                }
            }
        }

        // This template method searhes for a component of type T and deletes it
        void deleteComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end()) {
                delete *it;
                components.erase(it);
            }
        }

        // This template method searhes for the given component and deletes it
        template<typename T>
        void deleteComponent(T const* component){
            //TODO: (Req 8) Go through the components list and find the given component "component".
            // If found, delete the found component and remove it from the components list
            // need an iterator to iterator over the list
            std::list<Component*>::iterator itr;
            // initailly iterator point to the first element in the list
            itr = components.begin();
            // use for loop to iterator over the list
            for(itr; itr != components.end(); itr++){
            // if the passed component equals to the component pointed to by iterator
            if(component == *itr)
                // delete *itr is used to deallocate the memory associated with the object pointed to by itr
                delete *itr;
                // list.erase(itr) is used to remove the pointer from the list.
                components.erase(itr);
            }
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity(){
            //TODO: (Req 8) Delete all the components in "components".

            // Iterate over the list and delete each component
            for (auto itr = components.begin(); itr != components.end(); ++itr) 
            {
                delete *itr;
            }
            // Clear the list to remove all elements
            components.clear();
        }

        // Entities should not be copyable
        Entity(const Entity&) = delete;
        Entity &operator=(Entity const &) = delete;
    };

}