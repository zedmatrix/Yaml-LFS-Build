#!/bin/bash
cat > /etc/ld.so.conf << "EOF"
# Begin /etc/ld.so.conf
/usr/local/lib
/opt/lib
/tools/usr/lib

include /etc/ld.so.conf.d/*.conf

EOF
[ -f "/etc/ld.so.conf" ] && echo "Created: /etc/ld.so.conf"
mkdir -pv /etc/ld.so.conf.d

cat > /etc/nsswitch.conf << "EOF"
# Begin /etc/nsswitch.conf

passwd: files systemd
group: files systemd
shadow: files systemd

hosts: mymachines resolve [!UNAVAIL=return] files myhostname dns
networks: files

protocols: files
services: files
ethers: files
rpc: files

# End /etc/nsswitch.conf
EOF
[ -f "/etc/nsswitch.conf" ] && echo "Created: /etc/nsswitch.conf"
