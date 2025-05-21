.PHONY: all config clean unit test bin
all:
	ninja -C cmake-build-debug
config:
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
clean:
	rm -r cmake-build-debug
unit:
	cmake-build-debug/bin/test
test:
	cmake-build-debug/bin/akela-parse-test src/akela-parse-test/test-cases
	cmake-build-debug/bin/akela-run-test src/akela-run-test/test-cases
bin:
	cmake-build-debug/bin/akela examples/addition.ake