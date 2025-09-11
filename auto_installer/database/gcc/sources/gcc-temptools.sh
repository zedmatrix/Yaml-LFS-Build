#!/bin/bash
package_repo="${YSRC}/../.."

mpfr=$(find $package_repo -type f -name "mpfr-4.2.2.tar.xz")
[ -z $mpfr ] && { echo "Could Not Find mpfr-4.2.2.tar.xz"; exit 1; }
tar -xf $mpfr && mv -v mpfr-4.2.2 mpfr

gmp=$(find $package_repo -type f -name "gmp-6.3.0.tar.xz")
[ -z $gmp ] && { echo "Could Not Find gmp-6.3.0.tar.xz"; exit 1; }
tar -xf $gmp && mv -v gmp-6.3.0 gmp

mpc=$(find $package_repo -type f -name "mpc-1.3.1.tar.gz")
[ -z $mpc ] && { echo "Could Not Find mpc-1.3.1.tar.gz"; exit 1; }
tar -xf $mpc && mv -v mpc-1.3.1 mpc
