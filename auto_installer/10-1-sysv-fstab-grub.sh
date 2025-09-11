#!/bin/bash

DRIVE="/dev/sdb"
pttype=$(blkid -s PTTYPE -o value $DRIVE)
codename="yaml-sysv"
kernelversion="6.6.102"
[[ $DRIVE =~ 'nvme' ]] && P=p || P=

if [[ $pttype == "dos" ]]; then
    SWAP=$(blkid ${DRIVE}1 | awk '{print $2}' | sed 's/"//g')
    ROOT=$(blkid ${DRIVE}2 | awk '{print $2}' | sed 's/"//g')
    ROOTPART=$(blkid ${DRIVE}2 | awk '{print $4}' | sed 's/"//g')
else
    UEFI=$(blkid ${DRIVE}${P}1 | awk '{print $2}' | sed 's/"//g')
    SWAP=$(blkid ${DRIVE}${P}2 | awk '{print $2}' | sed 's/"//g')
    ROOT=$(blkid ${DRIVE}${P}3 | awk '{print $2}' | sed 's/"//g')
    ROOTPART=$(blkid ${DRIVE}${P}3 | awk '{print $5}' | sed 's/"//g')
    UEFI_MNT="$UEFI /boot/efi   vfat    noauto,codepage=437,iocharset=iso8859-1  0 1"
fi

cat > /etc/fstab <<EOF
# Begin /etc/fstab

# file system  mount-point    type     options             dump  fsck
#                                                                order

$ROOT      /              ext4     defaults            1     1
$SWAP      swap           swap     pri=1               0     0
$UEFI_MNT
proc           /proc          proc     nosuid,noexec,nodev 0     0
sysfs          /sys           sysfs    nosuid,noexec,nodev 0     0
devpts         /dev/pts       devpts   gid=5,mode=620      0     0
tmpfs          /run           tmpfs    defaults            0     0
devtmpfs       /dev           devtmpfs mode=0755,nosuid    0     0
tmpfs          /dev/shm       tmpfs    nosuid,nodev        0     0
cgroup2        /sys/fs/cgroup cgroup2  nosuid,noexec,nodev 0     0

# End /etc/fstab
EOF
[ -f /etc/fstab ] && printf "\n Created: /etc/fstab \n"

install -v -m755 -d /etc/modprobe.d
cat > /etc/modprobe.d/usb.conf << "EOF"
# Begin /etc/modprobe.d/usb.conf

install ohci_hcd /sbin/modprobe ehci_hcd ; /sbin/modprobe -i ohci_hcd ; true
install uhci_hcd /sbin/modprobe ehci_hcd ; /sbin/modprobe -i uhci_hcd ; true

# End /etc/modprobe.d/usb.conf
EOF
[ -f /etc/modprobe.d/usb.conf ] && printf "\n Created: /etc/modprobe.d/usb.conf \n"

mkdir -pv /boot/grub
cat > /boot/grub/grub.cfg <<EOF
# Begin /boot/grub/grub.cfg
set default=0
set timeout=10

insmod part_gpt
insmod ext2

search --no-floppy --fs-uuid --set=root $ROOT
set gfxpayload=1280x1024x32

menuentry "GNU/Linux, Linux-${kernelversion}-${codename}" {
    linux   /boot/vmlinuz-${kernelversion}-${codename} root=$ROOTPART ro

}
EOF
[ -f /boot/grub/grub.cfg ] && printf "\n Created: /boot/grub/grub.cfg \n"

if [[ $pttype == "dos" ]]; then
    grub-install --target=i386-pc $DRIVE
else
    echo "* mounting efi partition ${DRIVE}${P}1 *"
    mount --mkdir -v -t vfat ${DRIVE}${P}1 -o codepage=437,iocharset=iso8859-1 /boot/efi

    echo " Updating UEFI Boot"
    grub-install --target=x86_64-efi --removable
    mountpoint /sys/firmware/efi/efivars || mount -v -t efivarfs efivarfs /sys/firmware/efi/efivars
    grub-install --bootloader-id=LFS --recheck
    efibootmgr | cut -f 1
fi
