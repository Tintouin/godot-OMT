#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

[ -d "$ROOT/cyomtlib" ] || git clone https://github.com/maybites/cyomtlib "$ROOT/cyomtlib"
INSTALL_DEPS=0 bash "$ROOT/cyomtlib/scripts/build_linux_native.sh"

OUT="$ROOT/cyomtlib/build/linux-native"
mkdir -p "$ROOT/thirdparty/libomt/linux" "$ROOT/project/bin/linux"
cp "$OUT/libomt.h" "$ROOT/thirdparty/libomt/"
cp "$OUT/libomt.so" "$OUT/libvmx.so" "$ROOT/thirdparty/libomt/linux/"
chmod 755 "$ROOT"/thirdparty/libomt/linux/*.so
patchelf --set-rpath '$ORIGIN' "$ROOT/thirdparty/libomt/linux/libomt.so"
cp "$ROOT"/thirdparty/libomt/linux/*.so "$ROOT/project/bin/linux/"