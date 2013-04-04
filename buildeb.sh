#!/bin/bash
if [ "$1" == "deb" ]; then
	echo "Build deb i386"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo "Downlload from visqua.com"
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	cp debian/etc/init.d/visquacompress amd64/debian/etc/init.d/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/dists/stable/main; dpkg-scanpackages /home/visqua/www/dl/dists/stable/main/binary-i386 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-amd64 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-amd64/Packages.gz'
else
	echo "upload to source code to visqua"
	scp client.cpp client_linux.cpp visqua_compress.h visqua_compress.cpp support_functions.h support_functions.cpp visqua@visqua.com:/home/visqua/workspace
	echo "Make on Visqua"
	ssh visqua@visqua.com 'cd /home/visqua/workspace; make'
	echo "Download and build deb file"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo "Done build"
	echo "Upload and build on linux 64bit"
	scp client.cpp client_linux.cpp visqua_compress.h visqua_compress.cpp support_functions.h support_functions.cpp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/
	ssh quanghn@aaz1.rg.vncomputing.com 'cd /home/quanghn/workspace; make'
	echo "Download and build deb file"
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/visquacompress
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/binary/visquacompress_amd64 binary/visquacompress_amd64
	cp debian/etc/visquacompress/visqua.conf amd64/debian/etc/visquacompress/visqua.conf
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	cp debian/etc/init.d/visquacompress amd64/debian/etc/init.d/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	echo "Upoad to http://visqua.com/dl and update repo"
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-i386 /dev/null | gzip -9c > binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-amd64 /dev/null | gzip -9c > binary-amd64/Packages.gz'
	scp binary/visquacompress_amd64 binary/visquacompress_i386 binary/visqua.conf visqua@visqua.com:/home/visqua/www/dl/
fi

