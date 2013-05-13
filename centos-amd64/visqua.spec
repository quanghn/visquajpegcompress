Name: visquajpegcompress          
Version:        1.0
Release:        1%{?dist}
Summary:        Visqua jpeg compress client

Group:          Utilities
License:        GPLv2+
URL:            http://visqua.com/dl
Source0:        visquajpegcompress.tar.gz 

#BuildRequires:  
#Requires:       

%description
%prep
%setup -q
%build
%install
rm -rf %{buildroot}
mkdir %{buildroot}/
#install -m 755 -d %{_builddir}/%name-%version%{_bindir}
#ln -s ../bin/visquacompress_i386 %{_builddir}/%name-%version%{_bindir}
cp -r %{_builddir}/%name-%version/* %{buildroot}/
%clean
#rm -rf $RPM_BUILD_ROOT
%files
%defattr(-,root,root,-)
%doc
#%config %{_sysconfdir}/*
%config %{_sysconfdir}/visquacompress/visqua.conf
%config %{_sysconfdir}/init.d/visquacompress
%{_bindir}/*
%post
chkconfig --add visquacompress
%changelog
* Fri May 10 2013 %{packager} - init 
- Initialize %{name}-%{ver}.%{rel}.
