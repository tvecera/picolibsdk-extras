#!/bin/bash

# Go through each directory in the current directory
for dir in */ ; do
    cd "$dir"

    if [ -x c.sh ]; then
        ./c.sh || exit 1
    fi

    # Go back to the parent directory
    cd ..
done