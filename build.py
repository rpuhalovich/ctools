import os
import shutil
import subprocess
import sys
import tarfile
import urllib.request

def exe(cmd: str) -> None:
    subprocess.call(cmd.split());

def mkdir(path: str) -> None:
    if not os.path.exists(path): os.makedirs(path)

def rmdir(path: str) -> None:
    if os.path.exists(path): shutil.rmtree(path)

def cp(source: str, destination: str) -> None:
    shutil.copy(source, destination)

def exeExists(exe: str) -> bool:
    return shutil.which(exe) != None

def main(args: list[str]) -> None:
    if args[0] == "all":
        main(["install-ctools"])
        main(["install-clang-tools"])

    if args[0] == "build":
        exe("cmake --build build")

    if args[0] == "proj":
        cmd = "cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug"
        if exeExists("ninja"): cmd + " -G Ninja"
        exe(cmd)

    if args[0] == "release":
        exe("cmake -S . -B release -DCMAKE_BUILD_TYPE=Release")
        exe("cmake --build release --config Release")

    if args[0] == "install-ctools":
        main(["release"])
        mkdir("bin")

        if sys.platform == "win32":
            cp("./release/Release/ctemplate.exe", "./bin/ctemplate.exe")
            cp("./release/Release/cenum.exe", "./bin/cenum.exe")
            return

        cp("./release/ctemplate", "./bin/ctemplate")
        cp("./release/cenum", "./bin/cenum")

    if args[0] == "install-clang-tools":
        if sys.platform != "darwin": return

        mkdir("bin")
        mkdir("tmp")

        if not os.path.exists("./tmp/LLVM-19.1.6-macOS-ARM64.tar.xz"):
            urllib.request.urlretrieve(
                "https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.6/LLVM-19.1.6-macOS-ARM64.tar.xz",
                "LLVM-19.1.6-macOS-ARM64.tar.xz")

        tar = tarfile.open("tmp/LLVM-19.1.6-macOS-ARM64.tar.xz")
        tar.extractall(filter="data")
        tar.close()

        cp("LLVM-19.1.6-macOS-ARM64/bin/clang-format", "./bin/clang-format")
        cp("LLVM-19.1.6-macOS-ARM64/bin/clang-tidy", "./bin/clang-tidy")
        rmdir("LLVM-19.1.6-macOS-ARM64")

    if args[0] == "clean":
        rmdir("bin")
        rmdir("build")
        rmdir("release")

main(sys.argv[1:])
