# Automated LFS Yaml Installer

## Chapter 2
Preparing Host System:`2-Prepare-Host-Filesystem`
> Usage: Drive Letter /dev/sd{a, b, c, d} <br>
> uefi - dos/mbr is default <br>
> force - forces formatting/over-write <br>

## Chapter 4
Create Limited Directory Layout: `4-Create-Limited-Directories`<br>
Create Repository: `$LFS/mnt/lfs/var/db/repos` or `$LFS/ybuild/repos`
```
export yrepos=/ybuild/repos && mkdir -pv ${yrepos}
tar xf lfs-stable-yaml-repos.tar.xz -C ${yrepos} --strip-components=1
```
Install Ybuild (static binary)<br>
`tar xf ybuild-static.tar.xz -C $LFS/ybuild`<br>
> includes the Ybuild executable, ca-bundle.crt, yaml-install script <br>
> From this point all packages use: `./yaml-install.sh` package <br>

## Chapter 5 - Cross Compiler
Auto Script: `5-Chapter-Install`
<br>
`binutils-temp1 gcc-temp1 linux-headers glibc-temp1 libstdc++`

## Chapter 6 - Temporary Tools
Auto Script: `6-Chapter-Install`<br>
```
m4-temp ncurses-temp bash-temp coreutils-temp diffutils-temp
file-temp findutils-temp gawk-temp grep-temp gzip-temp make-temp
patch-temp sed-temp tar-temp xz-temp binutils-temp2 gcc-temp2
```
<br>Remove the temporary build files: `rm -rf /mnt/lfs/ybuild/tmp/*`
<br>Chroot Auto Script: `Yaml-lfs-chroot`

## Chapter 7 - Extra Temp Tools
<br>Create Final Layout: `7-5-Creating-Directories-Both`

<br>Create System Files: `7-6-Create-Files_sysv-and-systemd`
> Format system-v `export RELEASE=sysv`

<br>Auto Script: `7-7-Chapter-Install`
<br>
```
gettext-temp bison-temp perl-temp python-temp texinfo-temp util-linux-temp nano-temp
7-z-cleanup.sh
```

## Chapter 8 - Base System - Both
System-V: `export RELEASE="sysv"`
<br>Auto Script: `8-Chapter-install`
```
man-pages iana-etc glibc tzdata zlib bzip2 xz lz4 zstd file readline m4 bc
flex tcl expect dejagnu pkgconf binutils gmp mpfr mpc attr acl libcap libxcrypt
shadow gcc ncurses sed psmisc gettext bison grep bash libtool gdbm gperf expat
inetutils less perl xml-parser intltool autoconf automake openssl libelf libffi
python flit-core packaging wheel setuptools ninja meson kmod coreutils diffutils
gawk findutils groff grub gzip iproute2 kbd libpipeline make patch tar texinfo
nano|vim markupsafe jinja2 
```
<br>systemd: `systemd man-db procps util-linux e2fsprogs`
<br>systemv: `udev udev-lfs man-db-sysv procps util-linux-sysv e2fsprogs sysklogd sysvinit`

## Cleanup
```
rm -rf /ybuild/tmp/*
rm -rf /tmp/{*,.*}
find /usr/lib /usr/libexec -name \*.la -delete
find /usr -depth -name $(uname -m)-ybuild-linux-gnu\* | xargs rm -rf
```
optional: `userdel -r tester`
<br>
## Chapter 9 - System Configure - systemd
Auto Script: `9-Chapter-install`
```
ybuild-environment.sh ynetwork-files.sh yclock-locale.sh ysystem-config.sh 
ybash-startup.sh yskel-files.sh zbuild-environment.sh
```
> system-v users need to verify above scripts before auto building

<br>
```
libtasn1 p11-kit make-ca libunistring libidn2 libpsl wget curl openssh|openssh-sysv
dosfstools hwdata pciutils libusb usbutils
```
### UEFI Boot System-V
```
10-1-sysv-fstab-grub.sh
libaio lvm2-sysv fuse which libpng freetype2 grub-uefi popt efivar efibootmgr
dhcpcd blfs-bootscripts
```
### UEFI Boot System-D
```
10-1-systemd-fstab-grub
libaio lvm2 fuse which libpng freetype2 grub-uefi popt efivar efibootmgr
```

## Chapter 10 - Linux Kernel - Boot
Build Linux Kernel: `linux`

## Chapter 11 - Release Files
`echo r12.4-stable > /etc/lfs-release`

```
cat > /etc/lsb-release << "EOF"
DISTRIB_ID="Linux From Scratch"
DISTRIB_RELEASE="r12.4-stable"
DISTRIB_CODENAME="<your name here>"
DISTRIB_DESCRIPTION="Linux From Scratch"
EOF
```

```
cat > /etc/os-release << "EOF"
NAME="Linux From Scratch"
VERSION="r12.4-stable"
ID=lfs
PRETTY_NAME="Linux From Scratch r12.4-stable"
VERSION_CODENAME="<your name here>"
HOME_URL="https://www.linuxfromscratch.org/lfs/"
RELEASE_TYPE="stable"
EOF
```

# Reboot
