all:
	cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
	ninja -C cmake-build-debug
worm-unit:
	cmake-build-debug/bin/worm-unit
