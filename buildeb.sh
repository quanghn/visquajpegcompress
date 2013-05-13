#!/bin/bash
if [ "$1" == "deb" ]; then
	echo -e "\033[32m Build deb i386"
	echo -e "\033[0m"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo -e "\033[32m  Download deb files from visqua.com for amd64"
	echo -e "\033[0m"
	scp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/binary/visquacompress amd64/debian/usr/bin/
	cp debian/DEBIAN/changelog amd64/debian/DEBIAN/
	cp debian/etc/init.d/visquacompress amd64/debian/etc/init.d/
	dpkg-deb --build amd64/debian visquacompress_1.0.1_amd64.deb
	scp visquacompress_1.0.1_i386.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-i386
	scp visquacompress_1.0.1_amd64.deb visqua@visqua.com:/home/visqua/www/dl/dists/stable/main/binary-amd64
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-i386 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-i386/Packages.gz'
	ssh visqua@visqua.com 'cd /home/visqua/www/dl/; dpkg-scanpackages dists/stable/main/binary-amd64 /dev/null | gzip -9c > /home/visqua/www/dl/dists/stable/main/binary-amd64/Packages.gz'
elif [ "$1" == "rpm" ]; then
	echo -e "\033[32m Build rpm i386"
	echo -e "\033[0m"
	cp debian/usr/bin/visquacompress centos-x86/visquajpegcompress-1.0/usr/bin/
	scp  -P 235 centos-x86/visqua.spec quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/SPECS/
	scp -P 235 -r centos-x86/visquajpegcompress-1.0 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/
	ssh quanghn@vnc.f.vnpt.lg.vncomputing.com -p 235 'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp -P 235 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/RPMS/i686/visquajpegcompress-1.0-1.el6.i686.rpm /home/quang/workspace/
	echo -e "\033[32m Build rpm amd64"
	echo -e "\033[0m"
	cp amd64/debian/usr/bin/visquacompress centos-amd64/visquajpegcompress-1.0/usr/bin/
	scp  -P 133 centos-amd64/visqua.spec quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/SPECS/
	scp -P 133 -r centos-amd64/visquajpegcompress-1.0 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/
	ssh quanghn@vnc.f.vnpt.lg.vncomputing.com -p 133 'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp -P 133 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/RPMS/x86_64/visquajpegcompress-1.0-1.el6.x86_64.rpm /home/quang/workspace/
elif [ "$1" == "rpmlocal" ]; then
	echo -e "\033[32m Build rpm i386"
	echo -e "\033[0m"
	cp debian/usr/bin/visquacompress centos-x86/visquajpegcompress-1.0/usr/bin/
	echo -e "\033[32m Upload files to server i386"
	echo -e "\033[0m"
	scp  centos-x86/visqua.spec quanghn@10.10.10.166:/home/quanghn/rpmbuild/SPECS/
	scp -r centos-x86/visquajpegcompress-1.0 quanghn@10.10.10.166:/home/quanghn/
	ssh quanghn@10.10.10.166 'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp quanghn@10.10.10.166:/home/quanghn/rpmbuild/RPMS/i686/visquajpegcompress-1.0-1.el6.i686.rpm /home/quang/workspace/
	echo -e "\033[32m Build rpm amd64"
	echo -e "\033[0m"
	cp amd64/debian/usr/bin/visquacompress centos-amd64/visquajpegcompress-1.0/usr/bin/
	scp  centos-amd64/visqua.spec quanghn@10.10.10.133:/home/quanghn/rpmbuild/SPECS/
	scp -r centos-amd64/visquajpegcompress-1.0 quanghn@10.10.10.133:/home/quanghn/
	ssh quanghn@10.10.10.133 'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp quanghn@10.10.10.133:/home/quanghn/rpmbuild/RPMS/x86_64/visquajpegcompress-1.0-1.el6.x86_64.rpm /home/quang/workspace/
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
	echo -e "\033[32m1) upload source code to visqua"
	echo -e "\033[0m"
	scp src/client.cpp src/client_linux.cpp src/visqua_compress.h src/visqua_compress.cpp src/support_functions.h src/support_functions.cpp visqua@visqua.com:/home/visqua/workspace
	echo -e "\033[32m2) Make on Visqua"
	echo -e "\033[0m"
	ssh visqua@visqua.com 'cd /home/visqua/workspace; make'
	echo -e "\033[32m3) Download and build deb file"
	echo -e "\033[0m"
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress debian/usr/bin/
	scp visqua@visqua.com:/home/visqua/workspace/binary/visquacompress_i386 binary/
	dpkg-deb --build debian visquacompress_1.0.1_i386.deb
	echo -e "\033[32m Done build"
	echo -e "\033[32m4) Upload and build on linux 64bit"
	echo -e "\033[0m"
	scp src/client.cpp src/client_linux.cpp src/visqua_compress.h src/visqua_compress.cpp src/support_functions.h src/support_functions.cpp quanghn@aaz1.rg.vncomputing.com:/home/quanghn/workspace/
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
	echo -e "\033[32m Build rpm i386"
	echo -e "\033[0m"
	cp binary/visquacompress_i386 centos-x86/visquajpegcompress-1.0/usr/bin/visquacompress
	scp  -P 235 centos-x86/visqua.spec quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/SPECS/
	scp -P 235 -r centos-x86/visquajpegcompress-1.0 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/
	ssh quanghn@vnc.f.vnpt.lg.vncomputing.com -p 235 'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp -P 235 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/RPMS/i686/visquajpegcompress-1.0-1.el6.i686.rpm /home/quang/workspace/
	echo -e "\033[32m Build rpm amd64"
	echo -e "\033[0m"
	cp binary/visquacompress_amd64 centos-amd64/visquajpegcompress-1.0/usr/bin/visquacompress
	scp  -P 133 centos-amd64/visqua.spec quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/SPECS/
	scp -P 133 -r centos-amd64/visquajpegcompress-1.0 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/
	ssh quanghn@vnc.f.vnpt.lg.vncomputing.com -p 133  'tar -zcvf rpmbuild/SOURCES/visquajpegcompress.tar.gz visquajpegcompress-1.0; rpmbuild -bb rpmbuild/SPECS/visqua.spec'
	scp -P 133 quanghn@vnc.f.vnpt.lg.vncomputing.com:/home/quanghn/rpmbuild/RPMS/x86_64/visquajpegcompress-1.0-1.el6.x86_64.rpm /home/quang/workspace/
	echo -e "\033[32m*) Finished!"
	echo -e "\033[0m"
fi

