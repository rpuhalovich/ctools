build:
	cmake --build build

proj:
	cmake -S . -B build

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
	cmake --build release --config Release

install: release
	rm -f /usr/local/bin/ctemplate
	cp ./release/ctemplate /usr/local/bin/ctemplate
	echo "install /usr/local/bin/ctemplate"

remove:
	rm -f /usr/local/bin/ctemplate
	echo "remove /usr/local/bin/ctemplate"

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

clean:
	rm -rf build release gen tags

.PHONY: build proj release install remove format clean
.SILENT:
