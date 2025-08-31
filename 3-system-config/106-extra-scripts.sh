#!/bin/bash
zmsg() { printf "*** %s\n" "$@"; }
zmsg "Setting Up Extra Functions in /etc/zbuild.d/"

[ ! -d "/etc/zbuild.d/" ] && mkdir -pv /etc/zbuild.d

cat > /etc/zbuild.d/CurlPaste.sh << "EOF"
#!/bin/bash
# Begin of /etc/zbuild.d/CurlPaste.sh
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

# End of /etc/zbuild.d/CurlPaste.sh
EOF
[ -f "/etc/zbuild.d/CurlPaste.sh" ] && zmsg " Created: CurlPaste.sh"

cat > /etc/zbuild.d/MediaFunctions.sh << "EOF"
#!/bin/bash
# Begin of /etc/zbuild.d/MediaFunctions.sh
[ -x /usr/bin/mpv ] || return
Radio() {
    mpv --audio-display=no --loop-playlist --shuffle --playlist="$1"
}

#alias MPV='mpv --geometry=1280x720+1280+60'
alias MPV='mpv --geometry=720x480+50+50'
keep_awake() {
    while pgrep -x mpv >/dev/null; do
        xscreensaver-command -deactivate >/dev/null
        sleep 50
    done
}

Media() {
    keep_awake &
    local pid=$!
    trap "kill $pid 2>/dev/null" EXIT

    MPV --profile=fast --hwdec=auto-safe "${@}"
}
SPlay() {
    Media --shuffle --loop-playlist --playlist="$1"
}
ZPlay() {
    Media --profile=fast --hwdec=auto-safe "$1"
}

export -f keep_awake
export -f Radio
export -f Media
export -f SPlay
export -f ZPlay

PlayList() {
    if [[ $1 == "r" ]]; then
        find . -name "*.mp4" -type f -printf '%T@ %p\n' | sort -r | cut -d' ' -f2-
    elif [[ $1 == "n" ]]; then
        find . -name "*.mp4" -type f -printf '%T@ %p\n' | sort | cut -d' ' -f2-
    else
        echo "Usage: PlayList [r|n] > playlist.m3u"
    fi
}

export -f PlayList

# End of /etc/zbuild.d/MediaFunctions.sh
EOF
[ -f "/etc/zbuild.d/MediaFunctions.sh" ] && zmsg " Created: MediaFunctions.sh"

cat > /etc/zbuild.d/SourceGet.sh << "EOF"
#!/bin/bash
# Begin of /etc/zbuild.d/SourceGet.sh
[ -x /usr/bin/wget ] || return
SourceGet() {
    [ -z "${ZSRC}" ] && { echo "Missing \$ZSRC variable exiting."; exit 1; }
    if [[ -z $1 ]]; then
        printf "\n %s \n" "Usage: $0 [url/file]"
        return 1
    fi

    local url=$1
    wget "$url" --no-clobber -P "${ZSRC}"
}
export -f SourceGet

# End of /etc/zbuild.d/SourceGet.sh
EOF
[ -f "/etc/zbuild.d/SourceGet.sh" ] && zmsg " Created: SourceGet.sh"
