#!/bin/bash

(
	cmake \
		-DCMAKE_GENERATOR_PLATFORM=x64 \
		-DCMAKE_INSTALL_PREFIX=./build/install \
		-DWITH_CRYPTO=$WITH_CRYPTO \
		-DWITH_SYSTEM_DEPENDENCIES=$WITH_SYSTEM_DEPENDENCIES \
		-DENABLE_TESTING=$ENABLE_TESTING .
	
	cmake --build . --target install
) || exit 1

if [ $ENABLE_TESTING ]; then
	ctest -T memcheck &&
		make test-coverage &&
		bash <(curl -s https://codecov.io/bash) -f test-coverage.info;
fi
