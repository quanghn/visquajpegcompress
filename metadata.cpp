#include <string.h>
#include <iostream>
#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <cassert>
using namespace std;

string filename;
bool metadata(string &filename){
  bool status_of_compress=false;
  Exiv2::Image::AutoPtr image;
  image = Exiv2::ImageFactory::open(filename);
  assert(image.get() != 0);
  image->readMetadata();
  Exiv2::IptcData &iptcData = image->iptcData();
  Exiv2::IptcData::const_iterator end_iptcData = iptcData.end();
  for (Exiv2::IptcData::const_iterator md = iptcData.begin(); md != end_iptcData; ++md) {
    string compressed_by_visqua = md->value().toString().c_str();
    if (compressed_by_visqua == "Compressed by Visqua")
    {            
        status_of_compress = true;
        break;
    }
  }
      if (status_of_compress)
      {
        cout << filename << " was compressed by Visqua" << "'\n";
        return true;
      }
      else{
        cout << "has not compressed by visqua before" << "'\n";
        return false;
      }
}
int main(int argc, char *argv[]){
  filename = string(argv[1]);
metadata(filename);
}