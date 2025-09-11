#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }

mkdir -pv /etc/skel
cat > /etc/skel/.bash_profile << "EOF"
# Begin /etc/skel/.bash_profile

if [ -f "$HOME/.bashrc" ] ; then
    source $HOME/.bashrc
fi

if [ -d "$HOME/bin" ] ; then
    pathprepend $HOME/bin
fi

# End /etc/skel/.bash_profile
EOF

if [[ -f "/etc/skel/.bash_profile" ]]; then
    zmsg " Created: /etc/skel/.bash_profile"
    cp -v /etc/skel/.bash_profile $HOME/.bash_profile
    [ -f "$HOME/.bash_profile" ] && zmsg " Created: $HOME/.bash_profile"
fi

cat > /etc/skel/.profile << "EOF"
# Begin /etc/skel/.profile
# Personal environment variables and startup programs.

if [ -d "$HOME/bin" ] ; then
    pathprepend $HOME/bin
fi

#    Set up user specific i18n variables
#   export LANG=<ll>_<CC>.<charmap><@modifiers>

# End /etc/skel/.profile
EOF
if [[ -f "/etc/skel/.profile" ]]; then
    zmsg " Created: /etc/skel/.profile"
    cp -v /etc/skel/.profile $HOME/.profile
    [ -f "$HOME/.profile" ] && zmsg " Created: $HOME/.profile"
fi

cat > /etc/skel/.bashrc << "EOF"
# Begin /etc/skel/.bashrc

if [ -f "/etc/bashrc" ] ; then
  source /etc/bashrc
fi

#    Set up user specific i18n variables
#   export LANG=<ll>_<CC>.<charmap><@modifiers>

# End /etc/skel/.bashrc
EOF

if [[ -f "/etc/skel/.bashrc" ]]; then
    zmsg " Created: /etc/skel/.bashrc"
    cp -v /etc/skel/.bashrc $HOME/.bashrc
    [ -f "$HOME/.bashrc" ] && zmsg " Created: $HOME/.bashrc"
fi

cat > /etc/skel/.bash_logout << "EOF"
# Begin /etc/skel/.bash_logout

printf "\n\n Good Bye \n\n"

# End /etc/skel/.bash_logout
EOF

if [[ -f "/etc/skel/.bash_logout" ]]; then
    zmsg " Created: /etc/skel/.bash_logout"
    cp -v /etc/skel/.bash_logout $HOME/.bash_logout
    [ -f "$HOME/.bash_logout" ] && zmsg " Created: $HOME/.bash_logout"
fi
