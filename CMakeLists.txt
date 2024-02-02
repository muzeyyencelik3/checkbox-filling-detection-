cmake_minimum_required(VERSION 3.12)
project(HistogramMatching)

find_package(OpenCV REQUIRED)

# Add birdview.cpp to the library
add_library(birdview birdview.cpp)

# histogram_matching uygulamasını oluştururken birdview kütüphanesini bağlayın
add_executable(histogram_matching "main2.cpp" "birdview.h")

# Add the source files to the histogram_matching target
target_sources(histogram_matching PRIVATE main2.cpp)

# Tesseract ve Leptonica kütüphanelerini ekleme işlemleri devam ediyor...
# Specify the path to Tesseract include and library directories
set(Tesseract_INCLUDE_DIRS "/usr/include/tesseract")
set(Tesseract_LIB_DIRS "/usr/lib")

# Add Tesseract include directories
include_directories(${Tesseract_INCLUDE_DIRS})

# Add Tesseract libraries
find_library(Tesseract_LIB tesseract HINTS ${Tesseract_LIB_DIRS})

# Specify the path to Leptonica include and library directories
set(Leptonica_INCLUDE_DIRS "/usr/include/leptonica")
set(Leptonica_LIB_DIRS "/usr/lib")

# Add Leptonica include directories
include_directories(${Leptonica_INCLUDE_DIRS})

# Add Leptonica libraries
find_library(Leptonica_LIB lept HINTS ${Leptonica_LIB_DIRS})

# Link libraries to the histogram_matching target
target_link_libraries(histogram_matching PRIVATE ${OpenCV_LIBS} birdview ${Tesseract_LIB} ${Leptonica_LIB})
