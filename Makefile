build:
	cmake --build build

proj:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release
	cmake --build release --config Release

install-all: install-ctools install-clang-tools

install-ctools: release
	mkdir -p ./bin
	cp ./release/ctemplate ./bin/ctemplate
	cp ./release/cenum ./bin/cenum

install-clang-tools:
	./scripts/clang-tools.sh
	cp ./tmp/clang-format ./bin/clang-format
	cp ./tmp/clang-tidy ./bin/clang-tidy

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

clean:
	rm -rf build release gen tags bin tmp compile_commands.json

.PHONY: build proj release install-all install-ctools install-clang-tools format clean
.SILENT:
