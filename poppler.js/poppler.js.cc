#include <stdio.h>
#include <math.h>
#include <poppler-config.h>
#include <goo/GooString.h>
#include <GlobalParams.h>
#include <PDFDoc.h>
#include <PDFDocFactory.h>
#include <splash/SplashBitmap.h>
#include <splash/Splash.h>
#include <SplashOutputDev.h>

struct {
  GlobalParams * globalParams;
  SplashColor paperColor;
} global;

struct PopplerJS_Doc {
  PopplerJS_Doc(const char * filename) 
  {
    GooString s(filename);
    doc = PDFDocFactory().createPDFDoc(s, NULL, NULL);
    splashOut = new SplashOutputDev(splashModeRGB8, 4, gFalse, global.paperColor, gTrue, gTrue, splashThinLineDefault);
    page_count = doc->getNumPages();
    splashOut->startDoc(doc);
  }

  ~PopplerJS_Doc()
  {
    delete doc;
    delete splashOut;
  }

  PDFDoc * doc;
  SplashOutputDev *splashOut;
  int page_count;
};

struct PopplerJS_Page {
  PopplerJS_Page(PopplerJS_Doc *doc, int page_no)
    : doc(doc), page_no(page_no)
  { 
    width = doc->doc->getPageCropWidth(page_no);
    height = doc->doc->getPageCropHeight(page_no);

    int rotate = doc->doc->getPageRotate(page_no);
    if(rotate == 90 || rotate == 270) {
      int tmp = width;
      width = height;
      height = tmp;
    }
  }

  PopplerJS_Doc * doc;
  int page_no;
  int width;
  int height;
};

extern "C"
void PopplerJS_init() {
  global.globalParams = new GlobalParams();
  global.paperColor[0] = 255;
  global.paperColor[1] = 255;
  global.paperColor[2] = 255;
}

extern "C"
PopplerJS_Doc *PopplerJS_Doc_new(const char * filename) {
  return new PopplerJS_Doc(filename);
}

extern "C"
void PopplerJS_Doc_delete(PopplerJS_Doc *doc) {
  delete doc;
}

extern "C"
int PopplerJS_Doc_get_page_count(PopplerJS_Doc *doc) {
  return doc->page_count;
}

extern "C"
PopplerJS_Page *PopplerJS_Doc_get_page(PopplerJS_Doc *doc, int page_no) {
  return new PopplerJS_Page(doc, page_no);
}

extern "C"
int PopplerJS_Page_get_width(PopplerJS_Page *page) {
  return page->width;
}

extern "C"
int PopplerJS_Page_get_height(PopplerJS_Page *page) {
  return page->height;
}

extern "C"
SplashBitmap *PopplerJS_Page_get_bitmap(PopplerJS_Page *page, int width, int height) {
  page->doc->doc->displayPage(page->doc->splashOut, page->page_no, 
      72.0 * width / page->width, 72.0 * height / page->height,
      0,
      gFalse, gTrue, gFalse,
      NULL, NULL, NULL, NULL
  );

  return page->doc->splashOut->getBitmap();
}

extern "C"
int PopplerJS_Bitmap_get_row_size(SplashBitmap *bitmap) {
  return bitmap->getRowSize();
}

extern "C"
const char *PopplerJS_Bitmap_get_buffer(SplashBitmap *bitmap) {
  return reinterpret_cast<const char*>(bitmap->getDataPtr());
}

extern "C"
void PopplerJS_Bitmap_destroy(SplashBitmap *bitmap) {
}

extern "C"
void PopplerJS_Page_destroy(PopplerJS_Page *page) {
  delete page;
}
