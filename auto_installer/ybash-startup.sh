#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }
build=/etc/ybuild.d

zmsg "Setting Up Path Functions in $build"

[ ! -d "$build" ] && mkdir -pv $build

cat > $build/bash_completion.sh << "EOF"
# Begin bash_completion.sh
# Import bash completion scripts

# If the bash-completion package is installed, use its configuration instead
if [ -f /usr/share/bash-completion/bash_completion ]; then

  # Check for interactive bash and that we haven't already been sourced.
  if [ -n "${BASH_VERSION-}" -a -n "${PS1-}" -a -z "${BASH_COMPLETION_VERSINFO-}" ]; then

    # Check for recent enough version of bash.
    if [ ${BASH_VERSINFO[0]} -gt 4 ] || \
       [ ${BASH_VERSINFO[0]} -eq 4 -a ${BASH_VERSINFO[1]} -ge 1 ]; then
       [ -r "${XDG_CONFIG_HOME:-$HOME/.config}/bash_completion" ] && \
            . "${XDG_CONFIG_HOME:-$HOME/.config}/bash_completion"
       if shopt -q progcomp && [ -r /usr/share/bash-completion/bash_completion ]; then
          # Source completion code.
          . /usr/share/bash-completion/bash_completion
       fi
    fi
  fi

else

  # bash-completions are not installed, use only bash completion directory
  if shopt -q progcomp; then
    for script in /etc/bash_completion.d/* ; do
      if [ -r $script ] ; then
        . $script
      fi
    done
  fi
fi

# End bash_completion.sh
EOF
[ -f "$build/bash_completion.sh" ] && zmsg " Created: bash_completion.sh"

cat > $build/dircolors.sh << "EOF"
# Setup for /bin/ls and /bin/grep to support color, the alias is in /etc/bashrc.
if [ -f "/etc/dircolors" ] ; then
        eval $(dircolors -b /etc/dircolors)
fi

if [ -f "$HOME/.dircolors" ] ; then
        eval $(dircolors -b $HOME/.dircolors)
fi
EOF
[ -f "$build/dircolors.sh" ] && zmsg " Created: dircolors.sh"
