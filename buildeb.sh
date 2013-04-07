#!/bin/bash
if [ "$1" == "deb" ]; then
	echo -e "\033[32m Build deb i386"
	echo -e "\033[0m"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo -e "\033[32m  Downlload deb files from visqua.com"
	echo -e "\033[0m"
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	cp debian/etc/init.d/visquacompress amd64/debian/etc/init.d/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-i386 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-amd64 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-amd64/Packages.gz'
elif [ "$1" == "maketest" ]; then
	echo -e "\033[32m Make test"
	echo -e "\033[0m"
	make clean
	make
elif [ "$1" == "debtest" ]; then
	echo -e "\033[32m Build deb test"
	echo -e "\033[0m"
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	sudo dpkg -i visquacompress_1.0.1_i386.deb
else
	echo -e "\033[32m1) upload to source code to visqua"
	echo -e "\033[0m"
	scp client.cpp client_linux.cpp visqua_compress.h visqua_compress.cpp support_functions.h support_functions.cpp visqua@visqua.com:/home/visqua/workspace
	echo -e "\033[32m2) Make on Visqua"
	echo -e "\033[0m"
	ssh visqua@visqua.com 'cd /home/visqua/workspace; make'
	echo -e "\033[32m3) Download and build deb file"
echo -e "\033[0m"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo -e "\033[32m Done build"
	echo -e "\033[32m4) Upload and build on linux 64bit"
	echo -e "\033[0m"
	scp client.cpp client_linux.cpp visqua_compress.h visqua_compress.cpp support_functions.h support_functions.cpp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/
	ssh quanghn@aaz1.rg.vncomputing.com 'cd /home/quanghn/workspace; make'
	echo -e "\033[32m5) Download and build deb file"
	echo -e "\033[0m"
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/visquacompress amd64/debian/usr/bin/visquacompress
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/binary/visquacompress_amd64 binary/visquacompress_amd64
	cp debian/etc/visquacompress/visqua.conf amd64/debian/etc/visquacompress/visqua.conf
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	cp debian/etc/init.d/visquacompress amd64/debian/etc/init.d/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	echo -e "\033[32m6) Upoad to http://visqua.com/dl and update repo"
	echo -e "\033[0m"
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-i386 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-amd64 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-amd64/Packages.gz'
	echo -e "\033[32m7) Upoad to binary files to http://visqua.com/dl"
	echo -e "\033[0m"
	scp binary/visquacompress_amd64 binary/visquacompress_i386 binary/visqua.conf visqua@visqua.com:/home/visqua/www/dl/
	echo -e "\033[32m*) Finished!"
	echo -e "\033[0m"
fi

