#pragma once
#include "../helper/jtypes.h"

constexpr u32 MAX_ENTITIES = 256;

// consider making type safe w templates instead of generic data pointers
struct ComponentPool {
    i8 *first_ptr{nullptr};
    size_t element_size{0};

    explicit ComponentPool(size_t new_element_size) {
        //alloc enough memory to hold MAX_ENTITIES
        element_size = new_element_size;
        first_ptr = new i8[element_size * MAX_ENTITIES];
    }

    ~ComponentPool() {
        delete [] first_ptr;
    }

    inline void *get(size_t index) {
        // fetch the component at the desired index
        return first_ptr + index * element_size;
    }
};
