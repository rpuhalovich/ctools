import subprocess as sp
import sys

def main(args: list[str]):
    for string in args:
        if string == "build":
            sp.call(["cmake", "--build", "build"])

main(sys.argv)
