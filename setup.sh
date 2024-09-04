#!/usr/bin/env zsh

echo "Setup script for downloading arm-none-eabi"

# Detect platform
OS=$(uname)

TOOLCHAIN_ZIP=arm-gnu-toolchain.zip
TEMP_DIR=.cache/arm-toolchain
DEST_DIR="$(pwd)/build/arm-none-eabi"

if [ "$OS" = "Linux" ]; then
    TOOLCHAIN_VERSION=arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi
    TOOLS_DIR="$(pwd)/build/_tools/linux"
elif [ "$OS" = "Darwin" ]; then
    TOOLCHAIN_VERSION=arm-gnu-toolchain-13.3.rel1-darwin-x86_64-arm-none-eabi
    TOOLS_DIR="$(pwd)/build/_tools/macos"
else
    echo "Unsupported platform: $OS"
    return 1
fi

TOOLCHAIN_URL="https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/${TOOLCHAIN_VERSION}.tar.xz"

# Create temporary directory if it doesn't exist
if [ ! -d "$TEMP_DIR" ]; then
    mkdir -p "$TEMP_DIR"
fi

echo
if [ -f "$TEMP_DIR/$TOOLCHAIN_ZIP" ]; then
    echo "File \"$TEMP_DIR/$TOOLCHAIN_ZIP\" already exists, skipping download."
else
    echo "Downloading arm-none-eabi toolchain for $OS..."
    curl -L -o "$TEMP_DIR/$TOOLCHAIN_ZIP" "$TOOLCHAIN_URL"
    if [ $? -ne 0 ]; then
        echo "Error downloading arm-none-eabi toolchain."
        return 1
    fi
fi

echo
if [ -d "$DEST_DIR" ]; then
    echo "Toolchain already unpacked at $DEST_DIR, skipping extraction."
else
    echo "Extracting arm-none-eabi toolchain to temporary directory $TEMP_DIR..."
    tar -xf "$TEMP_DIR/$TOOLCHAIN_ZIP" -C "$TEMP_DIR"
    if [ $? -ne 0 ]; then
        echo "Error extracting the downloaded file."
        return 1
    fi

    echo "Copying arm-none-eabi toolchain to $DEST_DIR..."
    mkdir -p "$DEST_DIR"
    cp -r "$TEMP_DIR/$TOOLCHAIN_VERSION"/* "$DEST_DIR"
    if [ $? -ne 0 ]; then
        echo "Error copying files!"
        return 1
    fi
fi

echo
echo "Adding toolchain to PATH for the current user..."
echo "$DEST_DIR/bin"
echo "$TOOLS_DIR"

# Detect which shell is in use and add to the correct shell configuration file
if [ "$SHELL" = "/bin/zsh" ]; then
    SHELL_CONFIG=~/.zshrc
elif [ "$SHELL" = "/bin/bash" ]; then
    SHELL_CONFIG=~/.bashrc
else
    echo "Unsupported shell: $SHELL"
    return 1
fi

echo
if echo "$PATH" | grep -q "$DEST_DIR/bin"; then
    echo "Toolchain already added to PATH, skipping addition."
else
    export PATH="$TOOLS_DIR:$DEST_DIR/bin:$PATH"
    {
        echo
        echo #picolibsdk-extras
        echo "export PATH=\"$TOOLS_DIR:$DEST_DIR/bin:\$PATH\""
        echo
    } >> "$SHELL_CONFIG"
    # Re-source the config to apply changes immediately
fi

which arm-none-eabi-gcc
which make

echo
echo "Testing arm-none-eabi toolchain installation..."
echo
arm-none-eabi-gcc --version
if [ $? -ne 0 ]; then
    echo "Installation of arm-none-eabi toolchain failed!"
    return 1
fi

echo
echo "Testing PicoLibSDK tools installation..."
echo
make --version
if [ $? -ne 0 ]; then
    echo "Adding PicoLibSDK tools to PATH failed!"
    return 1
fi

echo "Project initialization completed..."

echo "Deleting temporary files..."
rm -rf "$TEMP_DIR/$TOOLCHAIN_VERSION"

return 0
