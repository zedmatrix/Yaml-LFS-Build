#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }
build=/etc/ybuild.d

zmsg "Setting Up Path Functions in $build"
[ ! -d "$build" ] && mkdir -pv $build

cat > $build/0-paths.sh << "EOF"
pathremove() {
    local IFS=':'
    local NEWPATH
    local DIR
    local PATHVARIABLE=${2:-PATH}
    for DIR in ${!PATHVARIABLE} ; do
        if [ "$DIR" != "$1" ] ; then
            NEWPATH=${NEWPATH:+$NEWPATH:}$DIR
        fi
    done
    export $PATHVARIABLE="$NEWPATH"
}
pathprepend() {
    pathremove $1 $2
    local PATHVARIABLE=${2:-PATH}
    export $PATHVARIABLE="$1${!PATHVARIABLE:+:${!PATHVARIABLE}}"
}

pathappend() {
    pathremove $1 $2
    local PATHVARIABLE=${2:-PATH}
    export $PATHVARIABLE="${!PATHVARIABLE:+${!PATHVARIABLE}:}$1"
}
export PATH=/usr/bin
[ $EUID -eq 0 ] && pathappend /usr/sbin

# Optional Local User Binary Paths
[ -d /usr/local/lib/pkgconfig ] && pathappend /usr/local/lib/pkgconfig PKG_CONFIG_PATH
[ -d /usr/local/bin ] && pathprepend /usr/local/bin
[ -d /usr/local/sbin -a $EUID -eq 0 ] && pathprepend /usr/local/sbin

[ -d /usr/share/info ] && pathappend /usr/share/info INFOPATH

# Optional Desktop Add on paths
[ -d /opt/rustc/bin ] && pathappend /opt/rustc/bin
[ -d /opt/qt6/bin ] && pathappend /opt/qt6/bin
[ -d /opt/kf6/bin ] && pathappend /opt/kf6/bin
EOF

cat > $build/CurlPaste.sh << "EOF"
#!/bin/bash
# Begin of CurlPaste.sh
[ -x /usr/bin/curl ] || return
CurlPaste() {
    local file=$1
    if [[ -z $file ]]; then
        printf "Error: Usage: $0 [file] \n"
        return 1
    fi

    if [[ -f $file ]]; then
        printf "\n\t Uploading ${file} to 0x0 \n"
        curl -F'file=@-' https://0x0.st < "${file}"
    else
        printf "\n\t Warning: File does not exist. \n"
        return 1
    fi
}
export -f CurlPaste

# End of CurlPaste.sh
EOF
[ -f "$build/CurlPaste.sh" ] && zmsg " Created: CurlPaste.sh"
