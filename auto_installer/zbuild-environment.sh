#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }

cat > /etc/ybuild.d/zbuild-environment.sh << "EOF"
#!bin/bash
# Begin of /etc/ybuild.d/zbuild-environment.sh

zzred="${zzred:-\033[1;31m}"
zzgreen="${zzgreen:-\033[1;32m}"
zzyellow="${zzyellow:-\033[1;33m}"
zzwhite="${zzwhite:-\033[1;37m}"
zzreset="${zzreset:-\033[0m}"

#ZBUILD_CONFIG="/etc/zbuild.conf"
ZBUILD="/zbuild"
ZSRC="/sources"

zprint() { printf "${zzwhite} *** %s *** ${zzreset} \n" "${@}"; }
zzok() { printf "${zzgreen} *** %s *** ${zzreset} \n" "${@}"; }

#export ZBUILD_CONFIG
export zzred zzgreen zzyellow zzreset ZSRC ZBUILD
export -f zprint
#export CFLAGS="-Os -pipe"
#export CXXFLAGS="$CFLAGS"
export MAKEFLAGS=-j$(nproc)

# End of /etc/ybuild.d/zbuild-environment.sh
EOF
[ -f "/etc/ybuild.d/zbuild-environment.sh" ] && zmsg " Created: zbuild-environment.sh"
