import shutil
import os
import subprocess
import sys

def exe(cmd: str) -> None:
    subprocess.call(cmd.split());

def mkdir(path: str) -> None:
    if not os.path.exists(path):
        os.makedirs(path)

def cp(source: str, destination: str) -> None:
    shutil.copy(source, destination)

def main(args: list[str]) -> None:
    if args[0] == "build":
        exe("cmake --build build")

    if args[0] == "proj":
        exe("cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug")

    if args[0] == "release":
        exe("cmake -S . -B release -DCMAKE_BUILD_TYPE=Release")
        exe("cmake --build release --config Release")

    if args[0] == "install-ctools":
        main(["release"])
        mkdir("bin")
        if os.name == "nt":
            cp("./release/Release/ctemplate.exe", "./bin/ctemplate.exe")
            cp("./release/Release/cenum.exe", "./bin/cenum.exe")
        else:
            cp("./release/ctemplate", "./bin/ctemplate")
            cp("./release/cenum", "./bin/cenum")

    if args[0] == "clean":
        shutil.rmtree("bin")
        shutil.rmtree("build")
        shutil.rmtree("release")

main(sys.argv[1:])
