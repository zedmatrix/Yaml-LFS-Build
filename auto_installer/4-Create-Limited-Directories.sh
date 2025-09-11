#!/bin/bash
LFS="/mnt/lfs"
DRIVE="/dev/sdb"
ROOTPATH="/usr/sbin"
PATH=${ROOTPATH}:${PATH}

# Check Drive
pttype=$(lsblk -n -o PTTYPE $DRIVE | head -1)
printf "\n\t Drive: %s Partition Type: %s \n" $DRIVE $pttype
[[ $DRIVE =~ 'nvme' ]] && P=p || P=

if [[ $pttype == "dos" ]]; then
    SWAP="${DRIVE}1"
    ROOT="${DRIVE}2"
    printf "\n\t Assuming (SWAP:%s) (ROOT:%s) \n" $SWAP $ROOT
else
    UEFI="${DRIVE}${P}1"
    SWAP="${DRIVE}${P}2"
    ROOT="${DRIVE}${P}3"
    printf "\n\t Assuming (UEFI:%s) (SWAP:%s) (ROOT:%s) \n" $UEFI $SWAP $ROOT
fi

# Mount ROOT partition if not already mounted
if ! mountpoint -q "$LFS"; then
    echo "Mounting $ROOT to $LFS"
    mount -v --mkdir "$ROOT" "$LFS" || {
        echo "Failed to mount $ROOT to $LFS"
        exit 1
    }
    chown -v root:root $LFS
    chmod -v 755 $LFS
else
    echo "$LFS is already mounted."
fi

[[ -n $SWAP ]] && /sbin/swapon $SWAP || { echo "Failed to Activate Swap"; exit 1; }

# Setup yaml-builder paths
mkdir -pv $LFS/ybuild/{tmp,log,repos}
#mkdir -pv $LFS/var/db/repos

# Setup LFS Limited directories
mkdir -pv $LFS/{etc,var,tools} $LFS/usr/{bin,lib,sbin}

for i in bin lib sbin; do
  [ ! -e "$LFS/$i" ] && ln -sv usr/$i $LFS/$i
done

case $(uname -m) in
  x86_64) mkdir -pv $LFS/lib64 ;;
esac

export LFS
