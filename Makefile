build:
	cmake --build build

proj:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
	cmake --build release --config Release

install: release
	cp ./release/ctemplate /usr/local/bin/ctemplate
	cp ./release/cenum /usr/local/bin/cenum
	echo "install ctools"

remove:
	rm -f /usr/local/bin/ctemplate
	rm -f /usr/local/bin/cenum
	echo "remove ctools"

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

clean:
	rm -rf build release gen tags

.PHONY: build proj release install remove format clean
.SILENT:
