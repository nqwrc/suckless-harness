fail=0
note() { printf '\n== %s ==\n' "$1"; }
bad()  { printf 'FAIL: %s\n' "$1"; fail=1; }
