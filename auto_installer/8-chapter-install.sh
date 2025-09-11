#!/bin/bash
let keep=0
if [[ ${1} == "keep" ]]; then
    let keep=1
fi

package_list=(man-pages iana-etc glibc tzdata zlib bzip2 xz lz4 zstd file readline m4 bc
 flex tcl expect dejagnu pkgconf binutils gmp mpfr mpc attr acl libcap libxcrypt
 shadow gcc ncurses sed psmisc gettext bison grep bash libtool gdbm gperf expat
 inetutils less perl xml-parser intltool autoconf automake openssl libelf libffi
 python flit-core packaging wheel setuptools ninja meson kmod coreutils diffutils
 gawk findutils groff grub gzip iproute2 kbd libpipeline make patch tar texinfo
 nano markupsafe jinja2)

if [[ $RELEASE == "sysv" ]]; then
    package_list+=( udev udev-lfs man-db-sysv procps util-linux-sysv e2fsprogs sysklogd sysvinit )
else
    package_list+=( systemd dbus man-db procps util-linux e2fsprogs )
fi

for pkg in ${package_list[@]}; do
    ./yaml-install.sh ${pkg} || { echo "Error in ${pkg}. Exiting."; break; }
done

if [[ -d "/ybuild/tmp" && $keep -eq 0 ]]; then
    echo "Cleaning Up /ybuild/tmp/"
    rm -rf /ybuild/tmp/*
fi
