build:
	cmake --build build

proj:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
	cmake --build release --config Release

install-all: install-ctools install-clang-tools
remove-all: remove-ctools remove-clang-tools

install-ctools: release
	cp ./release/ctemplate /usr/local/bin/ctemplate
	cp ./release/cenum /usr/local/bin/cenum
	echo "install ctools"

remove-ctools:
	rm -f /usr/local/bin/ctemplate
	rm -f /usr/local/bin/cenum
	echo "remove ctools"

install-clang-tools:
	./scripts/clang-tools.sh
	cp ./tmp/clang-format /usr/local/bin/clang-format
	cp ./tmp/clang-tidy /usr/local/bin/clang-tidy
	echo "install clang-tools"

remove-clang-tools:
	rm -f /usr/local/bin/clang-format
	rm -f /usr/local/bin/clang-tidy
	echo "remove clang-tools"

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

clean:
	rm -rf build release gen tags tmp compile_commands.json

.PHONY: build proj release install-all remove-all install-ctools remove-ctools install-clang-tools remove-clang-tools format clean
.SILENT:
