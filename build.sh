#!/bin/bash
set -e

EM_DIR=~/src/emscripten
BASE_DIR=`pwd`
ZLIB_DIR=$EM_DIR/tests/zlib
LIBPNG_DIR=$BASE_DIR/libpng-1.6.7
FREETYPE_DIR=$EM_DIR/tests/freetype
POPPLER_DIR=$BASE_DIR/poppler-0.24.4

do_zlib() {
pushd $ZLIB_DIR
$EM_DIR/emconfigure ./configure \

$EM_DIR/emmake make
popd
}

do_libpng () {
pushd $LIBPNG_DIR
CPPFLAGS=-I$ZLIB_DIR \
$EM_DIR/emconfigure ./configure \
    --enable-shared=no \

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
LIBPNG_CFLAGS="-I$BASE_DIR/libpng-1.6.7" \
LIBPNG_LIBS=' ' \
FONTCONFIG_CFLAGS="-I$BASE_DIR" \
FONTCONFIG_LIBS=' ' \
FREETYPE_CFLAGS="-I$EM_DIR/tests/freetype/include" \
FREETYPE_LIBS=' ' \
$EM_DIR/emconfigure ./configure \
    --enable-shared=no \
    --disable-libopenjpeg \
    --disable-libtiff \
    --disable-largefile \
    --disable-libjpeg \
    --enable-libpng \
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
popd
}


do_link () {
pushd web
$EM_DIR/emcc \
    -O2 \
    $POPPLER_DIR/utils/pdftoppm.o \
    $POPPLER_DIR/poppler/.libs/libpoppler.a \
    $POPPLER_DIR/utils/parseargs.o \
    $LIBPNG_DIR/.libs/libpng16.a \
    $ZLIB_DIR/libz.a \
    $FREETYPE_DIR/objs/.libs/libfreetype.a \
    -o poppler.js 

popd
}

#do_zlib
#do_libpng
#do_freetype
#do_poppler
do_link
