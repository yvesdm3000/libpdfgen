#include <PdfGen.h>
#include <PngEnc.h>
#include <PbmDec.h>

using namespace pdfgen;
using namespace pngenc;

static void error_handler( HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data )
{
  printf("ERROR: err_no=0x%x, detail_no=%d\n", (int)error_no, (int)detail_no );
  exit(-1);
}

PdfGen::PdfGen()
{
  _doc = HPDF_New( error_handler, nullptr );
  _totalPageCount = 0;
}

PdfGen::~PdfGen()
{
  HPDF_Free(_doc);
}

bool PdfGen::Save( const std::string& outputFile )
{
  HPDF_STATUS status = HPDF_SaveToFile( _doc, outputFile.c_str() );
  if (status == 0x0)
    return true;
  return false;
}


void PdfGen::AddPage()
{
   HPDF_Page page = HPDF_AddPage(_doc);
   HPDF_Font font = HPDF_GetFont (_doc, "Helvetica", NULL);
   HPDF_Page_SetFontAndSize (page, font, 12);
  _totalPageCount++;
}

unsigned int PdfGen::GetPageCount()
{
  HPDF_Pages pages = HPDF_Doc_GetCurrentPages(_doc);
  return 1;
}

void PdfGen::DrawAnnotation( const std::string& str )
{
  HPDF_Rect rect1 = {50, 350, 150, 400};
  HPDF_Page page = HPDF_GetPageByIndex(_doc, _totalPageCount-1);
  HPDF_Annotation annot = HPDF_Page_CreateTextAnnot (page, rect1, str.c_str(), NULL);
  HPDF_TextAnnot_SetOpened (annot, HPDF_TRUE);
}

void PdfGen::DrawText( float x, float y, const std::string& str )
{
  HPDF_Page page = HPDF_GetPageByIndex(_doc, _totalPageCount-1);
  HPDF_REAL pw = HPDF_Page_GetWidth(page);
  HPDF_REAL ph = HPDF_Page_GetHeight(page);

  float fsize = HPDF_Page_GetCurrentFontSize (page);
  HPDF_Font font = HPDF_Page_GetCurrentFont (page);
  HPDF_RGBColor c = HPDF_Page_GetRGBFill (page);

  HPDF_Page_BeginText (page);
  HPDF_Page_SetRGBFill (page, 0, 0, 0);
  HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
  HPDF_Page_SetFontAndSize (page, font, 10);
  HPDF_Page_TextOut (page, pw * x, ph - (ph * y) - 12, str.c_str());
  HPDF_Page_EndText (page);

  HPDF_Page_SetFontAndSize (page, font, fsize);
  HPDF_Page_SetRGBFill (page, c.r, c.g, c.b);
}

void PdfGen::DrawPbmImage( const std::vector<uint8_t>& data )
{
  int width;
  int height;
  int depth;
  std::vector<uint8_t> pixels;
  pbmdec::ColorSpace pbmColorspace;
  ColorSpace colorspace;


  if (!pbmdec::Decode( data.data(), data.size(), width, height, depth, pixels, pbmColorspace))
  {
    printf("DecodePNM failed\n");
    return;
  }

  switch( pbmColorspace )
  {
  case pbmdec::ColorSpace::GRAY:
    colorspace = pdfgen::ColorSpace::GRAY;
    break;
  case pbmdec::ColorSpace::RGB:
    colorspace = pdfgen::ColorSpace::RGB;
    break;
  }
  DrawImage( width, height, colorspace, depth, pixels );
}

bool PdfGen::DrawPngImage( const std::vector<uint8_t>& data )
{
  HPDF_Image image;

  image = HPDF_LoadPngImageFromMem(_doc, data.data(), data.size());
  if (!image)
  {
    printf("Loading image failed\n");
    return false;
  }

  HPDF_Page page = HPDF_GetPageByIndex(_doc, _totalPageCount-1);

  HPDF_Page_DrawImage( page, image, 0, 0, HPDF_Page_GetWidth(page), HPDF_Page_GetHeight(page) );
  return true;
  
}

bool PdfGen::DrawImage( int width, int height, ColorSpace cs, int depth, const std::vector<uint8_t>& pixels )
{
  pngenc::PngEnc enc;
  int channels = 1;
  if (cs == ColorSpace::RGB)
    channels = 3;
  enc.WriteHeader( width, height, cs, depth, 100 );
  for( int y =0; y< height; y++ )
  {
    enc.WriteLine( pixels, y * (width/(8-depth+1)) * channels, width/(8-depth+1) * channels );
  }
  enc.WriteEnd();

  return DrawPngImage( enc.GetData() );

  #if 0
  HPDF_Image image;
  pngenc::PngEnc enc;
  enc.WriteHeader( width, height, cs, depth, 100 );
  for( int y =0; y< height; y++ )
  {
    enc.WriteLine( pixels, y * (width/(8-depth+1)), width/(8-depth+1) );
  }
  enc.WriteEnd();


  image = HPDF_LoadPngImageFromMem(_doc, enc.GetData().data(), enc.GetData().size());
  if (!image)
  {
    printf("Loading image failed\n");
    return false;
  }

  HPDF_Page page = HPDF_GetPageByIndex(_doc, _totalPageCount-1);

  HPDF_Page_DrawImage( page, image, 0, 0, HPDF_Page_GetWidth(page), HPDF_Page_GetHeight (page) );
  return true;
#endif
}

