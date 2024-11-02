# cxx_utils
Collection of CXX utility and reference code.

# Git setup
This project uses 'git config core.autcrlf true' to auto change CRLF to LF when committing and reverse when checking out.

After cloning, you'll need:
>git submodule update --init --recursive
to fetch all the submodules.

To update submodules (which track branches) to the tips of their branches
>git submodule update --recursive --remote
or with a pull
>git pull --recurse-submodules
Note this changes the .gitmodules in this (top level) repo because the submodule commits become updated.

# To add a new submodule (example for spdlog - already added)
> cd ext
> git submodule add https://github.com/gabime/spdlog
> cd spdlog
> git checkout v1.14.1

# CMake'ing
> mkdir build && cd build
> cmake ..
This will create a build project using the default toolchain for your environment (see cmake --help).Note that the default is usual 32 bit.

Some examples for forcing a toolchain
> cmake -G "Visual Studio 16 2019" -A x64 ..

To build
cmake --build .

To build a release
>cmake --build . --config=Release