cmake_minimum_required(VERSION 3.16)

project(libpdfgen)

# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++17)

link_directories(/usr/local/lib/)
include_directories(/usr/include/hpdf)

file(GLOB LIBPDFGEN_SOURCES "src/PdfGen.cpp" "src/PngEnc.cpp" "src/PbmDec.cpp")

add_library(libpdfgen STATIC ${LIBPDFGEN_SOURCES})
target_include_directories(libpdfgen PUBLIC include)

file(GLOB PDFGENCMD_SOURCES "src/main.cpp")

add_executable(pdfgen ${PDFGENCMD_SOURCES})

target_link_libraries(pdfgen libpdfgen hpdf png)
