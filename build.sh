#!/bin/bash
EM_DIR=~/src/emscripten

FONTCONFIG_CFLAGS=' ' \
FONTCONFIG_LIBS=' ' \
$EM_DIR/emconfigure ./configure \
    --enable-shared=no \
    --disable-libopenjpeg \
    --disable-libtiff \
    --disable-largefile \
    --disable-libjpeg \
    --disable-libpng \
    --disable-cairo-output \
    --disable-poppler-glib \
    --disable-gtk-doc \
    --disable-gtk-doc-html \
    --disable-poppler-qt4 \
    --disable-poppler-qt5 \
    --disable-poppler-cpp \
    --disable-gtk-test \
    --enable-cms=none \
    --without-x \


$EM_DIR/emmake make

mkdir web || true
pushd web
$EM_DIR/emcc -O2 ../utils/pdftoppm.o ../poppler/.libs/libpoppler.a ../utils/parseargs.o -o pdftoppm.js
popd

