#!/bin/sh
INSPECT_INSTALL_PREFIX="$HOME"
while getopts 'Vp:' c
do
    case $c in
        V) echo "inspect-installer v0.1.0"; return 0 ;;
        p) INSPECT_INSTALL_PREFIX=$OPTARG ;;
    esac
done
print "All files will be installed into '$INSPECT_INSTALL_PREFIX/.inspect'. To fully delete inspect off of your machine, just remove this directory."
git clone "https://github.com/fuguenot/inspect.git"
cmake -S "./inspect" -B "./inspect/build" -DCMAKE_BUILD_TYPE="Release" -G "Ninja"
cmake --build "./inspect/build" --config "Release"
print "Installing into '$INSPECT_INSTALL_PREFIX/.inspect'..."
cmake --install "./inspect/build" --config "Release" --prefix "$INSPECT_INSTALL_PREFIX/.inspect"
print "Cleaning up..."
rm -rf "./inspect"
