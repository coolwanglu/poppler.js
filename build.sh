#!/bin/bash
set -e

EM_DIR=~/src/emscripten
BASE_DIR=`pwd`
ZLIB_DIR=$EM_DIR/tests/zlib
FREETYPE_DIR=$EM_DIR/tests/freetype
POPPLER_DIR=$BASE_DIR/poppler-0.26.3

do_zlib() {
pushd $ZLIB_DIR
$EM_DIR/emconfigure ./configure \

$EM_DIR/emmake make
popd
}

do_freetype () {
pushd $FREETYPE_DIR
$EM_DIR/emconfigure ./configure \
    --enable-shared=no \

$EM_DIR/emmake make
popd
}

do_poppler () {
pushd $POPPLER_DIR
FONTCONFIG_CFLAGS="-I$BASE_DIR" \
FONTCONFIG_LIBS=' ' \
FREETYPE_CFLAGS="-I$EM_DIR/tests/freetype/include" \
FREETYPE_LIBS=' ' \
$EM_DIR/emconfigure ./configure \
    --enable-shared=no \
    --disable-xpdf-headers \
    --enable-single-precision \
    --disable-libopenjpeg \
    --disable-libtiff \
    --disable-largefile \
    --disable-zlib \
    --disable-libcurl
    --disable-libjpeg \
    --disable-libpng \
    --enable-splash-output \
    --disable-cairo-output \
    --disable-poppler-glib \
    --enable-introspection=no \
    --disable-gtk-doc \
    --disable-gtk-doc-html \
    --disable-poppler-qt4 \
    --disable-poppler-qt5 \
    --disable-poppler-cpp \
    --disable-gtk-test \
    --enable-utils \
    --enable-cms=none \
    --without-x \

$EM_DIR/emmake make
popd
}


do_link () {
pushd web
$EM_DIR/emcc \
    -O2 \
    $POPPLER_DIR/utils/pdftoppm.o \
    $POPPLER_DIR/poppler/.libs/libpoppler.a \
    $POPPLER_DIR/utils/parseargs.o \
    $ZLIB_DIR/libz.a \
    $FREETYPE_DIR/objs/.libs/libfreetype.a \
    -o poppler.js 

popd
}

#do_zlib
#do_freetype
#do_poppler
do_link
