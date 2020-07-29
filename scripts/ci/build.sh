#!/bin/bash

if [ "$GENERATOR" == "Visual Studio 15 2017 Win64" ]; then
	echo "[build.sh] - Windows Generator [Visual Studio 15 2017 Win64]"
	(
		cmake \
			-G "$GENERATOR" \
			-DCMAKE_INSTALL_PREFIX=./build/install \
			-DWITH_CRYPTO=$WITH_CRYPTO \
			-DWITH_SYSTEM_DEPENDENCIES=$WITH_SYSTEM_DEPENDENCIES \
			-DENABLE_TESTING=$ENABLE_TESTING .
		
		cmake --build . --target install
	) || exit 1
elif [ "$GENERATOR" == "Visual Studio 16 2019" ]; then
	echo "[build.sh] - Windows Generator [Visual Studio 16 2019]"
	(
		cmake \
			-G "$GENERATOR" \
			-A "x64" \
			-DCMAKE_INSTALL_PREFIX=./build/install \
			-DWITH_CRYPTO=$WITH_CRYPTO \
			-DWITH_SYSTEM_DEPENDENCIES=$WITH_SYSTEM_DEPENDENCIES \
			-DENABLE_TESTING=$ENABLE_TESTING .
		
		cmake --build . --target install
	) || exit 1
else
	echo "[build.sh] - Building for linux"
	(
		cmake \
			-DCMAKE_INSTALL_PREFIX=./build/install \
			-DWITH_CRYPTO=$WITH_CRYPTO \
			-DWITH_SYSTEM_DEPENDENCIES=$WITH_SYSTEM_DEPENDENCIES \
			-DENABLE_TESTING=$ENABLE_TESTING .
		
		cmake --build . --target install
	) || exit 1
fi

if [ $ENABLE_TESTING ]; then
	ctest -T memcheck &&
		make test-coverage &&
		bash <(curl -s https://codecov.io/bash) -f test-coverage.info;
fi
