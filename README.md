# Yaml-LFS-Build
<p>
The pre build environment requires building `libuv`, `libarchive`, `cmake`, and `yamp-cpp`<br>
Copying the `yaml-cpp` file into the `/mnt/lfs/tools` folder and adjusting an `ld.so.conf`.<br>
<br>
Having the built-in downloader requires having access to `libcurl`, `libssl` and `libcrypto` inside the chroot environment<br>
Either by copying those libraries from the host to a temporary location or<br>
> then appending the build instructions to include: `openssl`, `libunistring`, `libidn2`, `libpsl`, `curl` as part of the temporary toolchain. Then Clean up the `/tools` as per book instructions.<br>

Cross-Building `curl`, `openssl`, and `yaml-cpp` in chapter 6 after gcc-pass2.<br>
</p><br>

# Linux From Scratch 
>##Chapter 5
<p></p>
<br>
>##Chapter 6
<p></p>
<br>
# yaml Package Definition Builder
<p>This was partially written from scratch, some code was modified from my Zbuild project such as the downloader.
</p>
<br>

# Checkout Other Projects

(My Handy Scripts)[https://github.com/zedmatrix/Bash_Scripts]
