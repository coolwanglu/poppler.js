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

struct POPPLERJS_Doc {
  POPPLERJS_Doc(const char * filename) 
  {
    GooString s(filename);
    doc = PDFDocFactory().createPDFDoc(s, NULL, NULL);
    splashOut = new SplashOutputDev(splashModeRGB8, 4, gFalse, global.paperColor, gTrue, gTrue, splashThinLineDefault);
    page_count = doc->getNumPages();
    splashOut->startDoc(doc);
  }

  ~POPPLERJS_Doc()
  {
    delete doc;
    delete splashOut;
  }

  PDFDoc * doc;
  SplashOutputDev *splashOut;
  int page_count;
};

struct POPPLERJS_Page {
  POPPLERJS_Page(POPPLERJS_Doc *doc, int page_no)
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

  POPPLERJS_Doc * doc;
  int page_no;
  int width;
  int height;
};

extern "C"
void POPPLERJS_init() {
  global.globalParams = new GlobalParams();
  if (!global.globalParams->setEnableFreeType((char*)"yes")) {
    fprintf(stderr, "Cannot enable freetype\n");
  }
  if (!global.globalParams->setAntialias((char*)"yes")) {
    fprintf(stderr, "Cannot enable font antialiasing\n");
  }
  if (!global.globalParams->setVectorAntialias((char*)"yes")) {
    fprintf(stderr, "Cannot enable vector antialiasing\n");
  }
  // globalParams->setErrQuiet(quiet);
 
  global.paperColor[0] = 255;
  global.paperColor[1] = 255;
  global.paperColor[2] = 255;
}

extern "C"
POPPLERJS_Doc *POPPLERJS_Doc_new(const char * filename) {
  return new POPPLERJS_Doc(filename);
}

extern "C"
void POPPLERJS_Doc_delete(POPPLERJS_Doc *doc) {
  delete doc;
}

extern "C"
int POPPLERJS_Doc_get_page_count(POPPLERJS_Doc *doc) {
  return doc->page_count;
}

extern "C"
POPPLERJS_Page *POPPLERJS_Doc_get_page(POPPLERJS_Doc *doc, int page_no) {
  return new POPPLERJS_Page(doc, page_no);
}

extern "C"
int POPPLERJS_Page_get_width(POPPLERJS_Page *page) {
  return page->width;
}

extern "C"
int POPPLERJS_Page_get_height(POPPLERJS_Page *page) {
  return page->height;
}

extern "C"
SplashBitmap *POPPLERJS_Page_get_bitmap(POPPLERJS_Page *page, int width, int height) {
  page->doc->doc->displayPageSlice(page->doc->splashOut, 
      page->page_no, 72.0, 72.0,
      0,
      gFalse, gFalse, gFalse,
      0, 0, width, height
  );

  return page->doc->splashOut->getBitmap();
}

extern "C"
int POPPLERJS_Bitmap_get_row_size(SplashBitmap *bitmap) {
  return bitmap->getRowSize();
}

extern "C"
const char *POPPLERJS_Bitmap_get_buffer(SplashBitmap *bitmap) {
  return reinterpret_cast<const char*>(bitmap->getDataPtr());
}

extern "C"
void POPPLERJS_Bitmap_destroy(SplashBitmap *bitmap) {
}

extern "C"
void POPPLERJS_Page_destroy(POPPLERJS_Page *page) {
  delete page;
}
