#include "omt_finder.h"

void OMTFinder::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sources"), &OMTFinder::get_sources);
    ADD_SIGNAL(MethodInfo("sources_changed"));
}

PackedStringArray OMTFinder::get_sources() const {
    PackedStringArray out;
    // TODO: appeler libomt ici
    out.push_back("source de test");
    return out;
}