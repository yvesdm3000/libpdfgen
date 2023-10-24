#pragma once
#include <vector>
#include <stdint.h>
#include <stddef.h>


namespace pbmdec {
  enum class ColorSpace {
    RGB,
    GRAY
  };
  bool Decode( const uint8_t* data, const size_t size, int& out_width, int& out_height, int& out_depth, std::vector<uint8_t>& out_pixels, ColorSpace& out_colorSpace);
};
