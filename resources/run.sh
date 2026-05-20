#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

cd "$BUILD"
if [ -e linflick ]; then
    ./linflick
else
    echo "No linflick file"
    exit 1
fi
