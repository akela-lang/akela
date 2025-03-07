.PHONY: all config clean unit test part unit-part test-part
all:
	ninja -C cmake-build-debug
config:
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
clean:
	rm -r cmake-build-debug
unit:
	cmake-build-debug/bin/zinc-unit
	cmake-build-debug/bin/akela-unit
	cmake-build-debug/bin/dataframe-unit
	cmake-build-debug/bin/cobble-unit
	cmake-build-debug/bin/centipede-unit
	cmake-build-debug/bin/worm-unit
	cmake-build-debug/bin/json-unit
	cmake-build-debug/bin/coverage-unit
	cmake-build-debug/bin/akela-llvm-unit
test:
	cmake-build-debug/bin/akela-parse-test src/akela-parse-test/test-cases
	cmake-build-debug/bin/akela-run-test src/akela-run-test/test-cases
part:
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
	ninja -C cmake-build-debug
unit-part:
	cmake-build-debug/bin/zinc-unit
	cmake-build-debug/bin/akela-unit
	cmake-build-debug/bin/dataframe-unit
	cmake-build-debug/bin/cobble-unit
	cmake-build-debug/bin/centipede-unit
	cmake-build-debug/bin/worm-unit
	cmake-build-debug/bin/json-unit
	cmake-build-debug/bin/coverage-unit
test-part:
	cmake-build-debug/bin/akela-parse-test src/akela-parse-test/test-cases
