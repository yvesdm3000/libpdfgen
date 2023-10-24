#pragma once

#include <string>
#include <vector>
#include <hpdf.h>

namespace pdfgen {

  enum class ColorSpace {
    RGB,
    GRAY
  };

  class PdfGen {
    HPDF_Doc _doc;
    unsigned int _totalPageCount;
  public:
    PdfGen();
    ~PdfGen();
    bool Save( const std::string& outputFile );

    void AddPage();
    unsigned int GetPageCount();

    void DrawPbmImage( const std::vector<uint8_t>& data );
    bool DrawPngImage(const std::vector<uint8_t> &data);
    bool DrawImage( int width, int height, ColorSpace cs, int depth, const std::vector<uint8_t>& pixels );
    void DrawAnnotation(const std::string &str);
    /**
     * Draw text at a location. x and y are relative values ranging [0.0-1.0]
     */
    void DrawText( float x, float y, const std::string& str );
  };

};
