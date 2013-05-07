CC	:= gcc -Wall
CPP	:= g++ -Wall
OBJECTS := support_functions.o visqua_compress.o

INCLUDES := -I include/
LIBS_VISQUA := lib/libev.a lib/libzip.a lib/libz.a  lib/libexiv2.a lib/libexpatw.a lib/librt.a -lpthread lib/libcurl.a
LINK := -L /home/quang/glibc/ 
all: binary/visquacompress_i386 debian/usr/bin/visquacompress

debian/usr/bin/visquacompress: $(OBJECTS) src/client.cpp
	$(CPP) -o debian/usr/bin/visquacompress src/client.cpp $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA) -static $(LINK)
	
binary/visquacompress_i386: $(OBJECTS) src/client_linux.cpp
	$(CPP) -o binary/visquacompress_i386 src/client_linux.cpp $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA) -static $(LINK)

support_functions.o: src/support_functions.cpp src/support_functions.h
	$(CPP) -c  src/support_functions.cpp  $(INCLUDES)
visqua_compress.o: src/visqua_compress.cpp src/visqua_compress.h
	$(CPP) -c  src/visqua_compress.cpp -static-libgcc  $(INCLUDES)
	
clean:
	rm -rf debian/usr/bin/visquacompress
	rm -rf binary/visquacompress_i386
	rm -rf *.o
	
