Summary: Kernel driver for Hewlett-Packard fx5 and fx10 graphics accelerators
Name: hpgraphics_kernel
Version: 1.5.13
Release: 1 
Copyright: (c) Hewlett-Packard Company 1996-2000.  All rights reserved.
Distribution: Red Hat Linux 6.2 (2.2.16-3)
Vendor: Hewlett-Packard, Company
URL: http://www.hp.com/workstations/products/linux
Group: System Environment/Libraries
Packager: Hewlett-Packard, Company <http://www.hp.com/workstations/support>
BuildArchitectures: i386
Source: hpgraphics_kernel-%{version}-%{release}.tgz

%description

The hpgraphics_kernel package contains the kernel driver source
which enable the fx5 and fx10 graphics accelerators on linux.

%pre

%post

%preun

%postun

%prep
%setup -c

%build
make

%install
make install

%verifyscript

%files
