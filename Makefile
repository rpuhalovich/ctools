build:
	cmake --build build

proj:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
	cp build/compile_commands.json compile_commands.json
	ctags -R .

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
	cmake --build release --config Release

install: release
	rm -f /usr/local/bin/ctemplate
	cp ./release/ctemplate /usr/local/bin/ctemplate

uninstall:
	rm -f /usr/local/bin/ctemplate

ctemplate:
	mkdir -p ./gen
	./build/ctemplate ./src/array.ctypes ./src/array.ht ./gen/array.h
	./build/ctemplate ./src/array.ctypes ./src/array.ct ./gen/array.c

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

clean:
	rm -rf build release gen tags

.PHONY: build ninja release format tidy clean ctemplate install uninstall
.SILENT:
