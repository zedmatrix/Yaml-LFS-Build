#!/bin/bash
echo "Starting - GCC - 15.2.0 - Testsuite"
ZBUILD=/ybuild

mkdir -pv ${ZBUILD}/tmp/gcc-test
pushd ${ZBUILD}/tmp/gcc-test

echo 'int main(){}' | cc -x c - -v -Wl,--verbose &> dummy.log
readelf -l a.out | grep ': /lib'
grep -E -o '/usr/lib.*/S?crt[1in].*succeeded' dummy.log
grep -B4 '^ /usr/include' dummy.log
grep 'SEARCH.*/usr/lib' dummy.log |sed 's|; |\n|g'
grep "/lib.*/libc.so.6 " dummy.log
grep found dummy.log
rm -v a.out dummy.log
mkdir -pv /usr/share/gdb/auto-load/usr/lib
mv -v /usr/lib/*gdb.py /usr/share/gdb/auto-load/usr/lib

popd
echo "Finished - GCC - 15.2.0 - Testsuite"
