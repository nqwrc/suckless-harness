#!/bin/sh
CC=${CC:-cc}
WARN="-std=c99 -pedantic -Wall -Wextra"

echo "t.c"
$CC $WARN -fsanitize=address -g -I test/build -o test/build/t_asan test/t.c
ASAN_OPTIONS=detect_leaks=1 ./test/build/t_asan -vfX || true

echo "t2.c"
$CC $WARN -fsanitize=address -g -I test/build -o test/build/t2_asan test/t2.c
ASAN_OPTIONS=detect_leaks=1 ./test/build/t2_asan || true

echo "t3.c"
$CC $WARN -fsanitize=address -g -I test/build -o test/build/t3_asan test/t3.c
ASAN_OPTIONS=detect_leaks=1 ./test/build/t3_asan || true

echo "t_util.c"
$CC $WARN -fsanitize=address -g -I test/build -o test/build/t_util_asan test/t_util.c test/build/util.c
ASAN_OPTIONS=detect_leaks=1 ./test/build/t_util_asan || true
