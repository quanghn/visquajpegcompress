CC	:= gcc 
CPP	:= g++ 
OBJECTS := support_functions.o visqua_compress.o

INCLUDES := -I include/
LIBS_VISQUA := lib/libev.a lib/libzip.a lib/libz.a lib/libcurl.a lib/libexiv2.a lib/libexpatw.a lib/librt.a -lpthread
LINK := -L /home/quang/glibc/
all: binary/visquacompress_i386

#debian/usr/bin/visquacompress: $(OBJECTS) client.cpp
#	$(CPP) -o debian/usr/bin/visquacompress client.cpp -static-libgcc -static-libstdc++ $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA) -static
	
binary/visquacompress_i386: $(OBJECTS) client_linux.cpp
	$(CPP) -o binary/visquacompress_i386 client_linux.cpp $(OBJECTS) $(INCLUDES) $(LIBS_VISQUA) -static $(LINK)

support_functions.o: support_functions.cpp support_functions.h
	$(CPP) -c  support_functions.cpp  $(INCLUDES) -static
visqua_compress.o: visqua_compress.cpp visqua_compress.h
	$(CPP) -c  visqua_compress.cpp -static-libgcc  $(INCLUDES) -static
	
clean:
	rm -rf debian/usr/bin/visquacompress
	rm -rf binary/visquacompress_i386
	rm -rf *.o
	
