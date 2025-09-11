#!/bin/bash

package_list=(gettext-temp bison-temp perl-temp python-temp texinfo-temp util-linux-temp nano-temp)

for pkg in ${package_list[@]}; do
    ./yaml-install.sh ${pkg} || { echo "Error in ${pkg}. Exiting."; break; }
done

if [[ -d "/ybuild/tmp" ]]; then
    echo "Cleaning Up /ybuild/tmp/"
    rm -rf /ybuild/tmp/*
fi
