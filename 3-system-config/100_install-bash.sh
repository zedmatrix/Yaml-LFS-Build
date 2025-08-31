#!/bin/bash
zzreset="\033[0m"
zzwhite="\033[1;37m"
zzred="\033[1;31m"
zzpurple="\033[1;35m"
zprint() { echo -e "${zzwhite} *** $* *** ${zzreset}"; }
zmsg() { echo -e "${zzred} *** $* *** ${zzreset}"; }
stars() { echo -e "${zzpurple} $(printf '%.0s*' {1..100}) ${zzreset}"; }

stars

zprint "Installing Bash Support Files"
basedir=2-zbuild
files=(1-network-files.sh 2-clock-locale.sh 3-system-config.sh 4-extrapaths.sh
 5-bash-startup.sh 6-skel-files.sh 7-extra-functions.sh 8-zbuild-environment.sh)

for file in ${files[@]}; do
    if [[ -x "${basedir}/$file" ]]; then
        zprint "Executing: ${basedir}/${file}"
        ${basedir}/${file}
    else
        zmsg "Failed: ${basedir}/${file}"
    fi
done

stars
