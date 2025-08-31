#!/bin/bash
zprint() { echo -e "\033[1;32m *** $1 *** \033[0m"; }
stars() { printf '%.0s*' {1..100}; printf '\n'; }

LFS=${LFS:-/mnt/lfs}
LFS_TGT=${LFS_TGT:-$(uname -m)-lfs-linux-gnu}
LC_ALL=POSIX
LANG=C.UTF-8
CFLAGS=${CFLAGS:-"-Os -pipe"}
CXXFLAGS=${CXXFLAGS:-"$CFLAGS"}

chroot_pre() {
    stars
    zprint " === Mounting Virtual Kernel Filesystems === "
    mkdir -pv $LFS/{dev,proc,sys,run}
    mount -v --bind /dev $LFS/dev
    mount -vt devpts devpts -o gid=5,mode=0620 $LFS/dev/pts
    mount -vt proc proc $LFS/proc
    mount -vt sysfs sysfs $LFS/sys
    mount -vt tmpfs tmpfs $LFS/run
    if [ -h $LFS/dev/shm ]; then
        install -v -d -m 1777 $LFS$(realpath /dev/shm)
    else
        mount -vt tmpfs -o nosuid,nodev tmpfs $LFS/dev/shm
    fi
    if [ ! -f $LFS/etc/resolv.conf ]; then
        printf "nameserver 1.1.1.1\nnameserver 8.8.8.8\n" > $LFS/etc/resolv.conf
    fi
    stars
}

chroot_exec() {
    stars
    zprint " === Entering Chroot $LFS === "
    /usr/sbin/chroot "$LFS" \
    /usr/bin/env -i HOME=/root TERM="$TERM" \
    PS1='(lfs chroot) \u:\w\$ ' \
    PATH=/usr/bin:/usr/sbin \
    CFLAGS="-Os -pipe" CXXFLAGS="$CFLAGS" \
    MAKEFLAGS="-j$(nproc)" \
    TESTSUITEFLAGS="-j$(nproc)" \
    ZSRC=/sources \
    ZBUILD=/zbuild \
    /bin/bash --login
    zprint " === Welcome Back === "
    stars
}
check_unmount() { mountpoint -q "$1" && umount -v "$1"; }

chroot_post() {
    stars
    zprint " === Un-Mounting Virtual Kernel Filesystems === "
    check_unmount $LFS/sys/firmware/efi/efivars
    check_unmount $LFS/dev/pts
    check_unmount $LFS/dev/shm
    check_unmount $LFS/dev
    check_unmount $LFS/run
    check_unmount $LFS/proc
    check_unmount $LFS/sys
    stars
}
stars
# checks if directory exists
[ ! -d $LFS ] && { zprint "Error $LFS is not a mountpoint"; exit 1; }

# mounts virtual kernel filesystems
chroot_pre

# enters the new root environment
chroot_exec

# cleans up the virtual kernel filesystems
chroot_post

stars
