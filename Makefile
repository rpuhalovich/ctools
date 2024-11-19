build:
	cmake --build build

ninja:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -G Ninja
	cp build/compile_commands.json compile_commands.json
	ctags -R .

xcode:
	cmake -S . -B xcode -G Xcode

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build release --config Release

preprocess:
	find src -iname "*.ht" -o -iname "*.ct" | xargs -I % ./build/ctemplate ./src/types.ctypes %

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

tidy:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-tidy

clean:
	rm -rf build release xcode tags
	find src -name "*_gen.h" -delete
	find src -name "*_gen.c" -delete

.PHONY: build ninja release format tidy clean preprocess
.SILENT:
