all:
	rm -rf build
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -G "Unix Makefiles"
b:
	cmake --build build
r:
	./build/main.exe
test:
	rm -rf tests/build
	cmake -B tests/build -S tests/ -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -DTESTING=ON
testb:
	cmake --build tests/build
testr:
	./tests/build/casm_test.exe