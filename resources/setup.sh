#!/bin/bash
DIR=$(cd "$(dirname "$0")" && pwd)
APP_PATH="$DIR/../build/linflick"
DESKTOP_DIR="$HOME/.local/share/applications"
ICON_DIR="$HOME/.local/share/icons/hicolor"
ICON_APP_DIR="$ICON_DIR/48x48/apps"

mkdir -p "$DESKTOP_DIR"
cp "$DIR/../data/linflick.desktop" "$DESKTOP_DIR/linflick.desktop"
sed -i "s|Exec=linflick|Exec=$APP_PATH|" "$DESKTOP_DIR/linflick.desktop"

mkdir -p "$ICON_APP_DIR"
cp "$DIR/../data/icons/linflick_icon_48.png" "$ICON_APP_DIR/linflick_icon_48.png"

mkdir -p "$ICON_DIR"
cp "$DIR/../data/theme/index.theme" "$ICON_DIR/index.theme"

gtk-update-icon-cache "$ICON_DIR"
