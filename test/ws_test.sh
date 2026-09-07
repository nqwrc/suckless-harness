#!/bin/sh
. "$(dirname "$0")/common.sh"
note "whitespace and CRLF check"

root="$(cd "$(dirname "$0")/.." && pwd)"
for f in $(cd "$root" && git ls-files); do
	if grep -q '[ 	]$' "$root/$f"; then
		bad "$f contains trailing whitespace"
	fi
	if grep -q "$(printf '\r')$" "$root/$f"; then
		bad "$f contains CRLF"
	fi
done
if [ "$fail" = 0 ]; then
	printf '  ok\n'
fi

test $fail = 0
