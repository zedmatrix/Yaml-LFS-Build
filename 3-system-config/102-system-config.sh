#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }

zmsg "\t Setting Up System Profile Files"

[ ! -d "/etc/zbuild.d" ] && mkdir -pv /etc/zbuild.d

cat > /etc/profile << "EOF"
# Begin /etc/profile

# Source the /etc/zbuild.d/scripts
for script in /etc/zbuild.d/*.sh ; do
    if [ -r $script ] ; then
        . $script
    fi
done

unset script

# Set up some environment variables.
export HISTSIZE=1000
export HISTIGNORE="&:[bf]g:exit"

# Set some defaults for graphical systems
export XDG_DATA_DIRS=${XDG_DATA_DIRS:-/usr/share}
export XDG_CONFIG_DIRS=${XDG_CONFIG_DIRS:-/etc/xdg}
export XDG_RUNTIME_DIR=${XDG_RUNTIME_DIR:-/tmp/xdg-$USER}

# End /etc/profile
EOF
[ -f "/etc/profile" ] && zmsg " Created: /etc/profile"

cat > /etc/inputrc << "EOF"
# Begin /etc/inputrc
# Modified by Chris Lynn <roryo@roryo.dynup.net>

# Allow the command prompt to wrap to the next line
set horizontal-scroll-mode Off

# Enable 8-bit input
set meta-flag On
set input-meta On

# Turns off 8th bit stripping
set convert-meta Off

# Keep the 8th bit for display
set output-meta On

# none, visible or audible
set bell-style none

# All of the following map the escape sequence of the value
# contained in the 1st argument to the readline specific functions
"\eOd": backward-word
"\eOc": forward-word

# for linux console
"\e[1~": beginning-of-line
"\e[4~": end-of-line
"\e[5~": beginning-of-history
"\e[6~": end-of-history
"\e[3~": delete-char
"\e[2~": quoted-insert

# for xterm
"\eOH": beginning-of-line
"\eOF": end-of-line

# for Konsole
"\e[H": beginning-of-line
"\e[F": end-of-line

# End /etc/inputrc
EOF
[ -f "/etc/inputrc" ] && zmsg " Created: /etc/inputrc"

cat > /etc/shells << "EOF"
# Begin /etc/shells

/bin/sh
/bin/bash

# End /etc/shells
EOF
[ -f "/etc/shells" ] && zmsg " Created: /etc/shells"

mkdir -pv /etc/systemd/system/getty@tty1.service.d/
cat > /etc/systemd/system/getty@tty1.service.d/noclear.conf << EOF
[Service]
TTYVTDisallocate=no
EOF

cat > /etc/zbuild.d/readline.sh << "EOF"
# Set up the INPUTRC environment variable.
if [ -z "$INPUTRC" -a ! -f "$HOME/.inputrc" ] ; then
        INPUTRC=/etc/inputrc
fi
export INPUTRC
EOF
[ -f "/etc/zbuild.d/readline.sh" ] && zmsg " Created: readline.sh"

cat > /etc/zbuild.d/umask.sh << "EOF"
# By default, the umask should be set.
if [ "$(id -gn)" = "$(id -un)" -a $EUID -gt 99 ] ; then
  umask 002
else
  umask 022
fi
EOF
[ -f "/etc/zbuild.d/umask.sh" ] && zmsg " Created: umask.sh"

cat > /etc/zbuild.d/i18n.sh << "EOF"
# Begin of /etc/zbuild.d/i18n.sh

for i in $(locale); do
  unset ${i%=*}
done

if [[ "$TERM" = linux ]]; then
    export LANG=C.UTF-8
else
    source /etc/locale.conf

    for i in $(locale); do
        key=${i%=*}
        if [[ -v $key ]]; then
            export $key
        fi
    done
fi

# End of /etc/zbuild.d/i18n.sh
EOF
[ -f "/etc/zbuild.d/i18n.sh" ] && zmsg " Created: i18n.sh"

cat > /etc/bashrc << "EOF"
# Begin /etc/bashrc

alias ls='ls --color=auto'
alias ll='ls -l '
alias grep='grep --color=auto'

mcd() {
    mkdir -v "$1"
    cd "$1"
}

export -f mcd

NORMAL="\[\e[0m\]"
RED="\[\e[1;31m\]"
GREEN="\[\e[1;32m\]"
if [[ $EUID == 0 ]] ; then
  PS1="$RED\u [ $NORMAL\w$RED ]# $NORMAL"
else
  PS1="$GREEN\u [ $NORMAL\w$GREEN ]\$ $NORMAL"
fi

unset RED GREEN NORMAL
# GnuPG wants this or it'll fail with pinentry-curses under some
# circumstances (for example signing a Git commit)
tty -s && export GPG_TTY=$(tty)

# End /etc/bashrc
EOF
[ -f "/etc/bashrc" ] && zmsg " Created: /etc/bashrc"
