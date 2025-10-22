#!/bin/bash
set +h
umask 022

zzreset="\033[0m"
zzwhite="\033[1;37m"
zzred="\033[1;31m"
zzgreen="\033[1;32m"
zzyellow="\033[1;33m"
zzblue="\033[1;34m"
zzpurple="\033[1;35m"
zzcyan="\033[1;36m"

zprint() { echo -e "${zzwhite} *** $* *** ${zzreset}"; }
zzok() { echo -e "${zzgreen} *** $* *** ${zzreset}"; }
zmsg() { echo -e "${zzred} *** $* *** ${zzreset}"; }
zstars() { echo -e " $(printf '%.0s*' {1..100}) ${zzreset}"; }

print_formatted_duration() {
    duration=$1
    hour=$((duration / 3600))
    mins=$(( (duration % 3600) / 60 ))
    sec=$((duration % 60))
    printf "%02d Hours, %02d Minutes and %02d Seconds \n" ${hour} ${mins} ${sec}
}

# Simple Wait Before Executing Function of 5 seconds or whatever is passed
zbuild_wait() {
    local wait=${1:-5}
    zmsg "Waiting $wait seconds or Press [SPACE] to continue..."
    read -t $wait -n 1 key
    if [[ $key == " " ]]; then
        zmsg "Skipped wait."
    else
        zzok "Continuing..."
    fi
}
# Start of Script
ZBefore=`date +%s`
zmsg "Inside chroot"
YBUILD="/ybuild/ybuild-101"
YREPOS="/ybuild/repos"

# Find package directory
echo -e ${zzcyan}
zstars
package=${1}

[ -z $package ] && { zmsg "Missing Package Argument. Exiting."; exit 127; }
package_dir=$(find ${YREPOS} -type f -name "${package}-[0-9]*.yaml")

if [[ -n $package_dir ]]; then
    zzok "Executing: ${package_dir}"
    zbuild_wait

    ${YBUILD} ${package_dir}
    exit_code=${?}
    if [[ -z ${YLFS} && $exit_code -eq 0 ]]; then
        zprint "Updating ld config"
        /sbin/ldconfig
    fi
else
    zmsg "Package: ${package_dir} Not Found"
    exit 127
fi

if [[ ${exit_code} -ne 0 ]]; then
    echo -e ${zzred} "Error Code: ${exit_code}"
else
    echo -e ${zzgreen}
fi
ZAfter=`date +%s`
let duration=ZAfter-ZBefore
print_formatted_duration $duration
zstars
exit $exit_code
