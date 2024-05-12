#!/bin/bash

set -x

folder_path="./build"
folder_exists=false

if [ -d "$folder_path" ]; then
    echo "Folder exists"
    folder_exists=true
else
    echo "Folder does not exist"
    folder_exists=false

    mkdir build
fi

cd build

# Now you can use the 'folder_exists' variable in your script
if [ "$folder_exists" = true ]; then
    echo "Performing actions for existing folder..."
    # Add your logic here for the case when the folder exists
else
    echo "Performing actions for non-existing folder..."
    cmake ..
fi

cmake --build .

set +x