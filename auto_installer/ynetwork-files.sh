#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }

zmsg "\n\t Setting Up Network Files"

cat > /etc/systemd/network/10-ethernet-dhcp.network << "EOF"
[Match]
Name=enp*

[Network]
DHCP=ipv4

[DHCPv4]
UseDomains=true
EOF
[ -f "/etc/systemd/network/10-ethernet-dhcp.network" ] && zmsg " Created: /etc/systemd/network/10-ethernet-dhcp.network"

echo "Zirconium" > /etc/hostname
[ -f /etc/hostname ] && zmsg "Created: /etc/hostname"

cat > /etc/hosts << "EOF"
# Begin /etc/hosts

# IPv4 and IPv6 localhost aliases
127.0.0.1       localhost
::1       ip6-localhost ip6-loopback
ff02::1   ip6-allnodes
ff02::2   ip6-allrouters

# End /etc/hosts
EOF
[ -f /etc/hosts ] && zmsg "Created: /etc/hosts"

systemctl enable systemd-networkd
#systemctl enable systemd-resolved
cat > /etc/resolv.conf << "EOF"
# Begin /etc/resolv.conf

domain localdomain
nameserver 8.8.8.8
nameserver 8.8.4.4

search localdomain

# End /etc/resolv.conf
EOF
[ -f /etc/resolv.conf ] && zmsg "Created: /etc/resolv.conf"
zmsg "Done"
