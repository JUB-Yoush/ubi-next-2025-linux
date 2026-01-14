#pragma once
#include <bitset>
#include <cassert>
#include <vector>

#include "component_id.h"
#include "component_pool.h"

typedef unsigned long long EntityId;
typedef unsigned int EntityVersion;
typedef unsigned int EntityIndex;
#define INVALID_ENTITY create_entity_id(EntityIndex(-1), 0)

const int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

struct World {
    struct Entity {
        EntityId id;
        ComponentMask bitmask;
    };

    std::vector<EntityIndex> free_entity_indexes;
    std::vector<ComponentPool *> component_pools;
    std::vector<Entity> entities;

    EntityId new_entity() {
        if (!free_entity_indexes.empty()) {
            EntityIndex new_index = free_entity_indexes.back();
            free_entity_indexes.pop_back();
            EntityId new_id = create_entity_id(new_index, get_entity_index(entities[new_index].id));
            entities[new_index].id = new_id;
            return entities[new_index].id;
        }
        // if no free spots, put first version at end
        entities.push_back({create_entity_id(entities.size(), 0), ComponentMask()});
        return entities.back().id;
    }

    // TODO assign multiple components in one line, use a templated tuple or something???
    template<typename T>
    T *assign(EntityId id) {
        assert(entities[get_entity_index(id)].id == id && "invalid entity id");
        int component_id = get_id<T>();

        if (component_pools.size() <= component_id) {
            // not enough pools, make another
            component_pools.resize(component_id + 1, nullptr);
        }
        if (component_pools[component_id] == nullptr) {
            // new null pool, populate with type
            component_pools[component_id] = new ComponentPool(sizeof(T));
        }
        //looks up component in the pool, initalizes it with placement new
        T *component_ptr = new(component_pools[component_id]->get(get_entity_index(id))) T();

        entities[get_entity_index(id)].bitmask.set(component_id);
        return component_ptr;
    }

    template<typename T>
    T *get(EntityId id) {
        assert(entities[get_entity_index(id)].id == id && "invalid entity id");
        i8 component_id = get_id<T>();
        assert(
            entities[get_entity_index(id)].bitmask.test(component_id) &&
            "entity didn't have component you tried to reference");

        T *component_ptr = static_cast<T *>(component_pools[component_id]->get(get_entity_index(id)));
        return component_ptr;
    }

    // template<typename T>
    // bool has(EntityId id) {
    //     assert(entities[get_entity_index(id)].id == id && "invalid entity id");
    //     i8 component_id = get_id<T>();
    //     return !entities[get_entity_index(id)].bitmask.test(component_id);
    // }

    template<typename... ComponentTypes>
    bool has(EntityId id) {
        assert(sizeof...(ComponentTypes) != 0 && "no components passed into has<> check");
        //unpack the template parameters into an initializer list
        i32 component_ids[] = {0, get_id<ComponentTypes>()...};
        for (int i = 1; i < (sizeof...(ComponentTypes) + 1); ++i) {
            if (entities[get_entity_index(id)].bitmask.test(component_ids[i])) {
                return true;
            }
        }
        return false;
    }


    template<typename T>
    void remove(EntityId id) {
        //TODO not completed?
        // ensures you're not accessing an entity that has been deleted
        assert(entities[get_entity_index(id)].id == id && "invalid entity id");
        i32 component_id = get_id<T>();
        entities[get_entity_index(id)].bitmask.reset(component_id);
    }

    void destroy_entity(EntityId id) {
        assert(entities[get_entity_index(id)].id == id && "invalid entity id");
        EntityId new_id = create_entity_id(static_cast<EntityIndex>(-1), get_entity_version(id) + 1);
        entities[get_entity_index(id)].id = new_id;
        entities[get_entity_index(id)].bitmask.reset();
        free_entity_indexes.push_back(get_entity_index(id));
    }

    static inline EntityId create_entity_id(EntityIndex index, EntityVersion version) {
        //shift up index by 32, and put the version in the bottom
        return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version);
    }

    static inline EntityIndex get_entity_index(EntityId id) {
        //shift down 32 to drop version and get index
        return id >> 32;
    }

    static inline EntityVersion get_entity_version(EntityId id) {
        // cast to 32 bit int to get our version number
        return static_cast<EntityVersion>(id);
    }

    static inline bool is_entity_valid(EntityId id) {
        //check if the index is our invalid index
        return (id >> 32) != static_cast<EntityIndex>(-1);
    }
};

