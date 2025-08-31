#!/bin/bash

rm -rf /usr/share/{info,man,doc}/*

find /usr/{lib,libexec} -name \*.la -delete

rm -rf /tools/{bin,include,lib{,exec},share,x86_64-ybuild-linux-gnu}

rm -rf /ybuild/tmp/*
