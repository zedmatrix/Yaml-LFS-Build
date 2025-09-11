#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }

zmsg "\t Setting Up Linux Console"

cat > /etc/adjtime << "EOF"
0.0 0 0.0
0
LOCAL
EOF
[ -f "/etc/adjtime" ] && zmsg " Created: /etc/adjtime"

cat > /etc/vconsole.conf << "EOF"
KEYMAP=us
FONT=LatArCyrHeb-16
#FONT=LatGrkCyr-8x16
#FONT=Lat2-Terminus16
EOF
[ -f "/etc/vconsole.conf" ] && zmsg " Created: /etc/vconsole.conf"

zmsg " Testing Locale Setup"
zlocale="en_US.utf8"

LC_ALL=$zlocale locale language
LC_ALL=$zlocale locale charmap
LC_ALL=$zlocale locale int_curr_symbol
LC_ALL=$zlocale locale int_prefix

cat > /etc/locale.conf << EOF
LANG=$zlocale
EOF
[ -f "/etc/locale.conf" ] && zmsg " Created: /etc/locale.conf"

zmsg "\n Finished"
