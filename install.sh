#!/bin/sh
INSPECT_DIR="$HOME/.inspect"
while getopts 'Vp:' c; do
    case $c in
    V)
        echo "inspect-installer v0.1.0"
        exit 0
        ;;
    p) INSPECT_DIR=$OPTARG/.inspect ;;
    esac
done
echo "All files will be installed into '$INSPECT_DIR'. To fully delete inspect off of your machine, just remove this directory."
git clone "https://github.com/fuguenot/inspect.git"
cmake -S "./inspect" -B "./inspect/build" -DCMAKE_BUILD_TYPE="Release" -G "Ninja"
cmake --build "./inspect/build" --config "Release"
echo "Installing into '$INSPECT_DIR'..."
cmake --install "./inspect/build" --config "Release" --prefix "$INSPECT_DIR"
echo "Cleaning up..."
rm -rf "./inspect"
echo "Adding inspect to \$PATH..."
RC_FILE=""
case $(basename "$SHELL") in
zsh) RC_FILE=".zshrc" ;;
bash) RC_FILE=".bashrc" ;;
esac
echo "\n# inspect\nexport INSPECT_DIR=\"$INSPECT_DIR\"\nexport PATH=\"\$INSPECT_DIR/bin:\$PATH\"" >>$HOME/$RC_FILE
