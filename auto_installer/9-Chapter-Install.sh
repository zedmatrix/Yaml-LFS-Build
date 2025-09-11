#!/bin/bash
let keep=0
if [[ ${1} == "keep" ]]; then
    let keep=1
fi
ybuild_scripts=(ybuild-environment.sh ynetwork-files.sh yclock-locale.sh ysystem-config.sh
ybash-startup.sh yskel-files.sh zbuild-environment.sh)

for script in ${ybuild_scripts[@]}; do
    ${script} || { echo "Error in ${script}. Exiting."; break; }
done

package_list=(libtasn1 p11-kit make-ca libunistring libidn2 libpsl wget curl openssh
 dosfstools hwdata pciutils libusb usbutils)

for pkg in ${package_list[@]}; do
    ./yaml-install.sh ${pkg} || { echo "Error in ${pkg}. Exiting."; break; }
done

if [[ -d "/ybuild/tmp" && $keep -eq 0 ]]; then
    echo "Cleaning Up /ybuild/tmp/"
    rm -rf /ybuild/tmp/*
fi
