#!/bin/sh
note() { printf '\n== %s ==\n' "$1"; }
bad()  { printf 'FAIL: %s\n' "$1"; ws_fail=1; }
note "whitespace and CRLF check"

ws_fail=0
root="$(cd "$(dirname "$0")/.." && pwd)"
for f in $(cd "$root" && git ls-files); do
	if grep -q '[ 	]$' "$root/$f"; then
		bad "$f contains trailing whitespace"
	fi
	if grep -q "$(printf '\r')$" "$root/$f"; then
		bad "$f contains CRLF"
	fi
done
if [ "$ws_fail" = 0 ]; then
	printf '  ok\n'
fi

test $ws_fail = 0
