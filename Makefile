CC	:= gcc
CPP	:= g++

OBJECTS := support_functions.o visqua_compress.o

INCLUDES := -I include/
LIBS_VISQUA := lib/libev.a lib/libzip.a lib/libz.a lib/libcurl.a lib/libexiv2.a lib/libexpatw.a lib/librt.a
#LIBS_VISQUA := -L shared/ -lev -lz -lzip -lcurl -lexiv2 -lrt -ldl

all: debian/usr/bin/visquacompress binary/visquacompress_i386

debian/usr/bin/visquacompress: $(OBJECTS) client.cpp
	$(CPP) -o debian/usr/bin/visquacompress client.cpp -static-libgcc -static-libstdc++ $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA)
	
binary/visquacompress_i386: $(OBJECTS) client_linux.cpp
	$(CPP) -o binary/visquacompress_i386 client_linux.cpp -static-libgcc -static-libstdc++ $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA)

support_functions.o: support_functions.cpp support_functions.h
	$(CPP) -c  support_functions.cpp $(INCLUDES)

visqua_compress.o: visqua_compress.cpp visqua_compress.h
	$(CPP) -c  visqua_compress.cpp $(INCLUDES)
	
clean:
	rm -rf debian/usr/bin/visquacompress
	rm -rf binary/visquacompress_i386
	
