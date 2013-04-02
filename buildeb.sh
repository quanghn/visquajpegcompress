#!/bin/bash
if [ "$1" == "deb" ]; then
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	scp visqua@visqua.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/dists/stable/main; dpkg-scanpackages binary-i386 /dev/null | gzip -9c > binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/dists/stable/main; dpkg-scanpackages binary-amd64 /dev/null | gzip -9c > binary-amd64/Packages.gz'
else
	make
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	scp client.cpp client_linux.cpp visqua_compress.h visqua_compress.cpp support_functions.h support_functions.cpp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/
	ssh quanghn@aaz1.rg.vncomputing.com 'cd /home/quanghn/workspace; make'
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/visquacompress
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/binary/visquacompress_amd64 binary/visquacompress_amd64
	cp debian/etc/visquacompress/visqua.conf amd64/debian/etc/visquacompress/visqua.conf
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/dists/stable/main; dpkg-scanpackages binary-i386 /dev/null | gzip -9c > binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/dists/stable/main; dpkg-scanpackages binary-amd64 /dev/null | gzip -9c > binary-amd64/Packages.gz'
	scp binary/visquacompress_amd64 binary/visquacompress_i386 binary/visqua.conf visqua@visqua.com:/home/visqua/www/dl/
fi

