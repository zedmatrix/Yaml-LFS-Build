#!/bin/bash
echo "Building Python Bindings for Brotli"
sed "/c\/.*\.[ch]'/d;\
     /include_dirs=\[/\
     i libraries=['brotlicommon','brotlidec','brotlienc']," \
    -i setup.py &&
pip3 wheel -w dist --no-build-isolation --no-deps --no-cache-dir $PWD
pip3 install --no-index --find-links dist --no-user Brotli
echo "Done"
