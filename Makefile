.PHONY: all config clean unit test bin
all:
	ninja -C cmake-build-debug
config:
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
clean:
	rm -r cmake-build-debug
test:
	cmake-build-debug/bin/test
bin:
	cmake-build-debug/bin/akela examples/addition.ake