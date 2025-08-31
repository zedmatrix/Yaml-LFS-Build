#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }
zmsg "Setting Up Path Functions in /etc/zbuild.d/"

[ ! -d "/etc/zbuild.d/" ] && mkdir -pv /etc/zbuild.d

cat > /etc/zbuild.d/01-path-functions.sh << "EOF"
# Begin of /etc/zbuild.d/01-path-functions.sh
pathremove () {
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
pathprepend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        export $PATHVARIABLE="$1${!PATHVARIABLE:+:${!PATHVARIABLE}}"
}

pathappend () {
        pathremove $1 $2
        local PATHVARIABLE=${2:-PATH}
        export $PATHVARIABLE="${!PATHVARIABLE:+${!PATHVARIABLE}:}$1"
}

export -f pathremove pathprepend pathappend
export PATH=/usr/bin

# Attempt to provide backward compatibility with LFS earlier than 11
if [ ! -L /bin ]; then
        pathappend /bin
fi

if [ $EUID -eq 0 ] ; then
        pathappend /usr/sbin
        if [ ! -L /sbin ]; then
                pathappend /sbin
        fi
        unset HISTFILE
fi
# End of /etc/zbuild.d/01-path-functions.sh
EOF
[ -f "/etc/zbuild.d/01-path-functions.sh" ] && zmsg " Created: 01-path-functions.sh"

cat > /etc/zbuild.d/extrapaths.sh << "EOF"
#/bin/bash
# Begin of /etc/zbuild.d/extrapaths.sh
if [ -d /usr/local/lib/pkgconfig ] ; then
    pathappend /usr/local/lib/pkgconfig PKG_CONFIG_PATH
fi

if [ -d /usr/local/bin ]; then
    pathprepend /usr/local/bin
fi

if [ -d /usr/local/sbin -a $EUID -eq 0 ]; then
    pathprepend /usr/local/sbin
fi

# Set some defaults before other applications add to these paths.
pathappend /usr/share/info INFOPATH

# End of /etc/zbuild.d/extrapaths.sh
EOF
[ -f "/etc/zbuild.d/extrapaths.sh" ] && zmsg " Created: extrapaths.sh"
