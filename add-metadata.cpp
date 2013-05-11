#include <string.h>
#include <iostream>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <cassert>
using namespace std;

string filename;
bool metadata(string &filename){
  //bool status_of_compress=false;
  Exiv2::Image::AutoPtr image;
  image = Exiv2::ImageFactory::open(filename);
  assert(image.get() != 0);
  Exiv2::ExifData exifData;
  exifData["Exif.Image.Software"] = "Compressed by Visqua";
  image->setExifData(exifData);
  image->writeMetadata();
  return 0;
}
int main(int argc, char *argv[]){
  filename = string(argv[1]);
metadata(filename);
}