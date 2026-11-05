// omt_runtime.cpp
#include "omt_runtime.h"
#include "libomt.h"
#include <godot_cpp/variant/utility_functions.hpp>

#if defined(__linux__)
#include <dlfcn.h>
#include <string>
#endif

namespace {
#if defined(__linux__)
std::string module_dir() {
    Dl_info info;
    if (dladdr(reinterpret_cast<void *>(&module_dir), &info) && info.dli_fname) {
        std::string path(info.dli_fname);
        return path.substr(0, path.find_last_of('/'));
    }
    return std::string();
}
#endif
} // namespace

namespace omt_runtime {

void initialize() {
#if defined(__linux__)
    // libomt charge libvmx.so à l'exécution : on la précharge par chemin absolu.
    std::string dir = module_dir();
    if (!dir.empty()) {
        std::string vmx = dir + "/libvmx.so";
        if (!dlopen(vmx.c_str(), RTLD_NOW | RTLD_GLOBAL)) {
            godot::UtilityFunctions::push_warning(
                godot::String("OMT: impossible de précharger ") + godot::String::utf8(vmx.c_str()) +
                " : " + godot::String::utf8(dlerror()));
        }
    }
#endif
    // TODO: Windows (GetModuleHandleEx) et macOS (dladdr)
}

void shutdown() {
    omt_shutdown();
}

} // namespace omt_runtime