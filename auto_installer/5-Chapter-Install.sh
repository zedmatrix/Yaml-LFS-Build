#!/bin/bash

package_list=(binutils-temp1 gcc-temp1 linux-headers glibc-temp1 libstdc++)

for pkg in ${package_list[@]}; do
    ./yaml-install.sh ${pkg} || { echo "Error in ${pkg}. Exiting."; break; }
done

echo "Cleaning Up ybuild/tmp/"
if [[ -d "$LFS/ybuild/tmp" ]]; then
    rm -rf ${LFS}/ybuild/tmp/* && echo "...Cleaned"
fi
