#include "omt_finder.h"
#include "libomt.h"
#include <godot_cpp/variant/utility_functions.hpp>

void OMTFinder::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sources"), &OMTFinder::get_sources);
    ClassDB::bind_method(D_METHOD("refresh"), &OMTFinder::refresh);
    ADD_SIGNAL(MethodInfo("sources_changed"));
}

void OMTFinder::_ready() {
    UtilityFunctions::print("OMTFinder: _ready has been called");
    set_process(true);
    refresh();
}

void OMTFinder::_process(double delta) {
    elapsed += delta;
    if (elapsed >= 1.0) {
        elapsed = 0.0;
        refresh();
    }
}

void OMTFinder::refresh() {
    int count = 0;
    char **list = omt_discovery_getaddresses(&count);

    PackedStringArray now;
    for (int i = 0; list && i < count; i++) {
        now.push_back(String::utf8(list[i]));
    }

    bool changed = now.size() != sources.size();
    for (int i = 0; !changed && i < now.size(); i++) {
        changed = now[i] != sources[i];
    }
    if (changed) {
        sources = now;
        emit_signal("sources_changed");
    }
}

PackedStringArray OMTFinder::get_sources() const {
    return sources;
}