#!/bin/zsh

rm -rf tmp
mkdir -p tmp
cd tmp

wget "https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.6/LLVM-19.1.6-macOS-ARM64.tar.xz"
tar -xf "LLVM-19.1.6-macOS-ARM64.tar.xz"
cp "LLVM-19.1.6-macOS-ARM64/bin/clang-format" .
cp "LLVM-19.1.6-macOS-ARM64/bin/clang-tidy" .

cd ..
