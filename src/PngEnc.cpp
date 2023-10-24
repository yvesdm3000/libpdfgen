#include <PngEnc.h>

using namespace pngenc;

static void write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
  png_voidp write_io_ptr = png_get_io_ptr(png_ptr);
  std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(write_io_ptr);
  for (int i=0; i<length; ++i)
    buffer->push_back( data[i] );
}

static void flush_data(png_structp png_ptr)
{
  png_voidp write_io_ptr = png_get_io_ptr(png_ptr);
  std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(write_io_ptr);
  buffer->clear();
}

bool PngEnc::WriteHeader( int width, int height, pdfgen::ColorSpace cs, int depth, int dpi )
{
  const double pixels_per_meter = dpi * 39.3700787401575;
  int color_type;

  _png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!_png_ptr) {
    fprintf(stderr, "png_create_write_struct failed\n");
    return false;
  }

  switch( cs )
  {
    case pdfgen::ColorSpace::RGB:
      color_type = PNG_COLOR_TYPE_RGB;
//      png_set_bgr(_png_ptr);
      break;
    case pdfgen::ColorSpace::GRAY:
      color_type = PNG_COLOR_TYPE_GRAY;
      break;
  }

  _info_ptr = png_create_info_struct(_png_ptr);
  if (!_info_ptr) {
    fprintf(stderr, "png_create_info_struct failed\n");
    return false;
  }

  png_set_write_fn(_png_ptr, &_buffer, write_data, flush_data);

  png_set_IHDR(_png_ptr, _info_ptr, width, height, depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_set_pHYs(_png_ptr, _info_ptr, pixels_per_meter, pixels_per_meter, PNG_RESOLUTION_METER);

  png_write_info(_png_ptr, _info_ptr);
  return true;
}

static int g_lines = 0;

bool PngEnc::WriteLine( const std::vector<uint8_t>& inputData, size_t offset, size_t len )
{
  if (!_png_ptr)
    return false;
  if (!_info_ptr)
    return false;
  int depth = png_get_bit_depth( _png_ptr, _info_ptr );
  if (depth == 1)
  {
    std::vector<uint8_t> line(len);
    for( int i=0; i<len; ++i)
      line[i] = ~inputData[i+offset];
    png_write_row(_png_ptr, line.data() + offset);
    return true;
  }
  png_write_row(_png_ptr, inputData.data() + offset);
  return true;
}

bool PngEnc::WriteEnd()
{
  png_write_end(_png_ptr, _info_ptr);
  png_destroy_write_struct(&_png_ptr, &_info_ptr);
  return true;
}
