#!/bin/bash
bad_drive() {
    echo "Invalid Drive Options. Exiting."
    exit 666
}
DRIVE=""
let UEFI=0
let FORCE=0
while (( "$#" )); do
   case $1 in
      [a-z])
        [ -z $DRIVE ] && DRIVE="/dev/sd${1}" || bad_drive
        ;;
      uefi)
        UEFI=1
        ;;
      force)
        FORCE=1
        ;;

      [?])
        echo "Usage: $0 /dev/sd[a or b or c or d] [uefi] [force]" >&2
        exit 1 ;;
    esac
    shift
done

[ ! -z $DRIVE ] && echo "Format Options: DRIVE: $DRIVE " || bad_drive
[ $UEFI -eq 1 ] && echo "Using UEFI Setup" || echo "Using MBR Setup"
[ $FORCE -eq 1 ] && echo "Overwite Enabled" || echo "Overwite Disabled"

printf "\n\t Partition Formatting Layout Creation \n"
ROOTPATH="/usr/sbin"
PATH=${ROOTPATH}:${PATH}

# Check if drive exists
if [[ ! -b "$DRIVE" ]]; then
    echo "Error: $DRIVE is not a block device."
    exit 1
fi

# Check if drive is partitioned
pttype=$(lsblk -n -o PTTYPE $DRIVE | head -1)

if [[ -z "$pttype" ]]; then
    if [[ $UEFI -eq 1 ]]; then
        echo "Creating GPT partition table with EFI, swap, and root..."
        sfdisk "$DRIVE" <<EOF
label: gpt
,512M,U
,2G,S
,,L
EOF
    else
        echo "Creating DOS partition table with swap and root..."
        sfdisk "$DRIVE" <<EOF
label: dos
,2G,S
,,L
EOF
    fi
else
    echo "Partition table already exists: $pttype"
fi

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
#
# Check formatted && Collect partitions
#
PARTS=()
[[ -n "$UEFI" ]] && PARTS+=("$UEFI")
[[ -n "$SWAP" ]] && PARTS+=("$SWAP")
[[ -n "$ROOT" ]] && PARTS+=("$ROOT")

# Check for formatting
for part in "${PARTS[@]}"; do
    if [[ -b "$part" ]]; then
        FSTYPE=$(lsblk -n -o FSTYPE "$part" | head -n 1)
        if [[ -z "$FSTYPE" || $FORCE -eq 1 ]]; then
            echo "$part is unformatted"

            # Format depending on purpose
            case "$part" in
                "$UEFI") mkfs.vfat -v -F32 "$UEFI" ;;
                "$SWAP") mkswap "$SWAP" ;;
                "$ROOT") mkfs.ext4 -v "$ROOT" ;;
            esac
        else
            echo "$part already formatted as $FSTYPE"
        fi
    fi
done

[[ -n "$UEFI" ]] && export UEFI
[[ -n "$SWAP" ]] && export SWAP
[[ -n "$ROOT" ]] && export ROOT
