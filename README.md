# Yaml-LFS-Build
## yaml Package Definition Builder
<p>This was partially written from scratch, some code was modified from my Zbuild project such as the downloader.</p>

[LFS-r12.4-Stable-Repos](https://ln5.sync.com/dl/9166b41a0#3inpj6jt-8thmtii6-493i2qxx-377jjahy) <br>
> Includes all yaml and source files for complete Systemd or Sys-V 

ybuild-jan-03-2026.tar.xz <br>
> Includes Static and Shared Versions, custom install wrapper and cert file<br>
> ` mkdir -p ybuild && cd ybuild && tar xf ybuild-jan-03-2026.tar.xz ` <br>
> Where ever the *.yaml file is the source file is placed into subdir sources/ <br>
> That folder is accessible using ${YSRC} inside the yaml file.

## Linux From Scratch 
> <ins>Chapter 5</ins><br>
binutils-temp1-2.45, gcc-temp1-15.2.0, linux-6.6.102-headers, glibc-temp1-2.42, libstdc++15.2.0<br>
 
> <ins>Chapter 6</ins><br>
m4-temp-1.4.20, ncurses-temp-6.5-20250809, bash-temp-5.3, coreutils-temp-9.7<br>
diffutils-temp-3.12, file-temp-5.46, findutils-temp-4.10.0<br>
gawk-temp-5.3.2, grep-temp-3.12, gzip-temp-1.14, make-temp-4.4.1<br>
patch-temp-2.8, sed-temp-4.9, tar-temp-1.35, xz-temp-5.8.1<br>
binutils-temp2-2.45, gcc-temp2-15.2.0

> <ins>Chapter 7</ins><br>
gettext-temp-0.26, bison-temp-3.8.2, perl-temp-5.42.0, python-temp-3.13.7, texinfo-temp-7.2, util-linux-temp-2.41.1<br>
zlib-1.3.1, nano-8.6, libunistring-1.3, libidn2-2.3.8, libpsl-0.21.5, openssl-3.5.2, wget-1.25.0 and/or curl-8.15.0
<br>

# Checkout Other Projects
[Zirconium OS](https://github.com/zedmatrix/ZirconiumOS) - a systemd linux from scratch build<br>
[Pluto OS](https://github.com/zedmatrix/PlutoOS) - a sysv linux from scratch build<br>
[Handy Bash Scripts](https://github.com/zedmatrix/Bash_Scripts)<br>
[Linux From Scratch musl with skaarnet's ndevd](https://github.com/zedmatrix/ZBuild-System/tree/main/LFS-gcc15-musl)<br>
