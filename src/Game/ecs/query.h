#pragma once
#include "world.h"

template<typename... ComponentTypes>
struct Query {
    World *scene_ptr{nullptr};
    ComponentMask component_mask;
    bool all{false};

    Query(World &scene) : scene_ptr(&scene) {
        if (sizeof...(ComponentTypes) == 0) {
            all = true;
        } else {
            //unpack the template parameters into an initializer list
            i32 component_ids[] = {0, get_id<ComponentTypes>()...};
            for (int i = 1; i < (sizeof...(ComponentTypes) + 1); ++i) {
                component_mask.set(component_ids[i]);
            }
        }
    }

    struct Iterator {
        EntityIndex index;
        World *scene_ptr;
        ComponentMask mask;
        bool all{false};

        Iterator(World *scene_ptr, EntityIndex index, ComponentMask mask, bool all) : scene_ptr(scene_ptr),
            index(index), mask(mask), all(all) {
        }

        EntityId operator*() const {
            return scene_ptr->entities[index].id;
        }

        bool operator==(const Iterator &other) const {
            return index == other.index || index == scene_ptr->entities.size();
        }

        bool operator!=(const Iterator &other) const {
            return index != other.index || index != scene_ptr->entities.size();
        }

        bool valid_index() {
            //valid entity id and has the correct component mask, or we're just iterating through everything
            bool valid_entity = World::is_entity_valid(scene_ptr->entities[index].id);
            bool valid_bitmask = mask == (mask & scene_ptr->entities[index].bitmask);
            return valid_entity && (
                       all || valid_bitmask);
        }

        Iterator &operator++() {
            do {
                index++;
            } while (index < scene_ptr->entities.size() && !valid_index());
            return *this;
        }
    };

    const Iterator begin() const {
        int first_index = 0;
        // separate into bools
        while (first_index < scene_ptr->entities.size() && (
                   component_mask != (component_mask & scene_ptr->entities[first_index].bitmask) || !
                   World::is_entity_valid(scene_ptr->entities[first_index].id))) {
            first_index++;
        }
        return Iterator(scene_ptr, first_index, component_mask, all);
    }

    const Iterator end() const {
        return Iterator(scene_ptr, static_cast<EntityIndex>(scene_ptr->entities.size()), component_mask, all);
    }
};
