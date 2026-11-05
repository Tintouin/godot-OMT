#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

using namespace godot;

class OMTFinder : public Node {
    GDCLASS(OMTFinder, Node)

    PackedStringArray sources;
    double elapsed = 0.0;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _process(double delta) override;
    void refresh();
    PackedStringArray get_sources() const;
};