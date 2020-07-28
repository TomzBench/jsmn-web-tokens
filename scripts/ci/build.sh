#!/bin/bash

(cmake \
	-DCMAKE_INSTALL_PREFIX=./build/install \
	-DWITH_CRYPTO=$WITH_CRYPTO \
	-DWITH_SYSTEM_DEPENDENCIES=$WITH_SYSTEM_DEPENDENCIES \
	-DENABLE_TESTING=$ENABLE_TESTING .
) || exit 1

(cmake --build . --target install || exit 1)

if [ $ENABLE_TESTING ]; then
	(ctest -T memcheck || exit 1)
	(make test-coverage || exit 1)
	(bash <(curl -s https://codecov.io/bash) -f test-coverage.info || exit 1)
fi
