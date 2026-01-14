#pragma once
#include "../helper/jtypes.h"

extern i32 s_component_counter;

template<class T>
i32 get_id() {
    static i32 s_component_id = s_component_counter++;
    return s_component_id;
}

