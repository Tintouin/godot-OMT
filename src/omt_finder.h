#pragma once
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class OMTFinder : public Node {
    GDCLASS(OMTFinder, Node)

protected:
    static void _bind_methods();

public:
    PackedStringArray get_sources() const;
};