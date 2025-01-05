#!/bin/zsh

mkdir -p tmp
cd tmp

if [ ! -f "./LLVM-19.1.6-macOS-ARM64.tar.xz" ]; then
    wget "https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.6/LLVM-19.1.6-macOS-ARM64.tar.xz"
fi

tar -xf "LLVM-19.1.6-macOS-ARM64.tar.xz"
cp "LLVM-19.1.6-macOS-ARM64/bin/clang-format" .
cp "LLVM-19.1.6-macOS-ARM64/bin/clang-tidy" .

cd ..
