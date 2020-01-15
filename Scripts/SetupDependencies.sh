#!/bin/sh

conan_export() {
    echo "Installing $2"
    git clone "https://github.com/$1/$2.git"
    cd $2
    conan export . $3
    cd ..
    return 1
}

err() {
    echo "Error: $@" >&2 ; return 1
}

# Create a temporary directory
mkdir __tmp
cd __tmp

# Download and configure filament
conan_export luizgabriel conan-filament google/stable

# Download and configure imgui
conan_export luizgabriel conan-imgui ocornut/testing

# Download and configure assimp
conan_export jacmoe conan-assimp ocornut/testing

# Once exported, the the projects are no longer necessary
cd ..
rm -rf __tmp

while true; do
    read -p "Would you like to install SDL2 via VCPKG? [y/n]> " yn
    case $yn in
        [Yys]* ) break;;
        [Nn]* ) echo "Ok! Now you'll just need to install SDL2 manually."; exit;;
        * ) echo "Please answer yes or no.";;
    esac
done

while :
do
    read -r -p "Enter the VCPKG executable path > " vcpkg_exe
    #check already exists
    [[ -e "$vcpkg_exe" ]] || err "$vcpkg_exe does not exists" || continue

    #create a directory, if success - break the cycle
    echo "Installing SDL2"
    $vcpkg_exe install sdl2 && break

    #otherwire repeat the question...
done

echo "Good! All dependencies are configured."