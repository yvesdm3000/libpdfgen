#pragma once

#include <png.h>
#include <vector>
#include <stdint.h>
#include <PdfGen.h>

namespace pngenc {

  class PngEnc
  {
    png_structp _png_ptr;
    png_infop _info_ptr;
    std::vector<uint8_t> _buffer;
  public:
    PngEnc(){}
    ~PngEnc() = default;

    bool WriteHeader( int width, int height, pdfgen::ColorSpace cs, int depth, int dpi );
    bool WriteLine( const std::vector<uint8_t>& inputData, size_t offset, size_t len );
    bool WriteEnd();
    const std::vector<uint8_t>& GetData() { return _buffer; }
  };
};

