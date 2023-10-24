#include <PbmDec.h>
#include <stdio.h>

bool pbmdec::Decode( const uint8_t* data, const size_t size, int& out_width, int& out_height, int& out_depth, std::vector<uint8_t>& out_pixels, ColorSpace& out_colorSpace)
{
  size_t idx = 3;
  uint8_t strbuf[32] = {0};
  size_t stridx = 0;

  if (size < 16)
    return false;
  while ( (idx < size) && (data[idx] != 0x0a) )
  {
    idx++;
  }
  while ( (idx < size) && (data[idx] != ' ') && (stridx < sizeof(strbuf))) {
    strbuf[stridx] = data[idx];
    strbuf[stridx+1] = 0;
    stridx++;
    idx++;
  }
  if (idx >= size)
    return false;
  if( sscanf((const char*)strbuf, "%d", &out_width) != 1 )
    return false;
  stridx = 0;
  while ( (idx < size) && (data[idx] != 0x0a) && (stridx < sizeof(strbuf))) {
    strbuf[stridx] = data[idx];
    strbuf[stridx+1] = 0;
    stridx++;
    idx++;
  }
  idx++;
  if( sscanf((const char*)strbuf, "%d", &out_height) != 1 )
    return false;

  if (data[1] == '4')
  {
    out_colorSpace = ColorSpace::GRAY;
    out_depth = 1;
  }
  else if (data[1] == '5')
  {
    while ( (idx < size) && (data[idx] != 0x0a) )
      idx++;
    out_colorSpace = ColorSpace::GRAY;
    out_depth = 8;
  }
  else if (data[1] == '6')
  {
    // Skip maxval, generally the value 255
    while ( (idx < size) && (data[idx-1] != 0x0a) )
      idx++;
    idx++;
    while ( (idx < size) && (data[idx-1] != 0x0a) )
      idx++;
    printf("idx=%d", (int)idx);
    out_colorSpace = ColorSpace::RGB;
    out_depth = 8;
  }
  else
  {
    out_colorSpace = ColorSpace::RGB;
    out_depth = 8;
  }

  while( idx < size )
  {
    out_pixels.push_back( data[idx] );
    idx++;
  }

  return true;
}
