#!/bin/sh
# Extract every C source block from SKILL.md, compile it, and run it.
# Also compares the shipped arg.h against the upstream and broken variants.
#
#   sh test/run.sh
#
# Requires: awk, a C99 compiler (cc, or set CC). AddressSanitizer checks are
# skipped automatically if the compiler does not support -fsanitize=address.
# The section 4.1 Makefile is built, installed and packaged for real, but only
# under GNU make; see the note above that section.

set -e

cd "$(dirname "$0")"
here=$(pwd)

CC=${CC:-cc}
AWK=${AWK:-awk}
MAKE=${MAKE:-make}

for cmd in "$CC" "$AWK"; do
	if ! command -v "$cmd" >/dev/null 2>&1; then
		printf 'FAIL: required command "%s" not found\n' "$cmd" >&2
		exit 1
	fi
done

WARN="-std=c99 -pedantic -Wall -Wextra"
FEAT="-D_POSIX_C_SOURCE=200809L"
# Windows CRT calls fopen/strdup "deprecated"; ignored elsewhere.
FEAT="$FEAT -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS"

rm -rf build
mkdir build

fail=0
note() { printf '\n== %s ==\n' "$1"; }
bad()  { printf 'FAIL: %s\n' "$1"; fail=1; }

note "extracting sources from SKILL.md"
(cd build && "$AWK" -f ../extract.awk ../../SKILL.md)

for f in arg.h util.h util.c lc.c drw.h drw.c config.def.h; do
	[ -f "build/$f" ] || bad "SKILL.md produced no $f"
done

note "worked example (SKILL.md section 11)"
$CC $WARN $FEAT -DVERSION='"test"' -o build/lc build/lc.c build/util.c
out=$(printf 'a\nb\nc\n' | ./build/lc)
[ "$out" = "3 <stdin>" ] || bad "lc stdin: got '$out', want '3 <stdin>'"
printf '  stdin      -> %s\n' "$out"

# die() colon trick: message, then strerror, then exit 1
if err=$(./build/lc no/such/file 2>&1); then
	bad "lc should exit nonzero on a missing file"
else
	printf '  missing    -> %s (exit %d)\n' "$err" "$?"
	case $err in
	"fopen no/such/file: "*) : ;;
	*) bad "die() colon trick: got '$err'" ;;
	esac
fi

note "arg.h option matrix (SKILL.md section 3.4)"
$CC $WARN -I build -o build/t t.c
for a in "-vfX" "-f Y z" "-v" "-- -x" "-f FILE t1 t2" "-vf Y" "a b" "-"; do
	printf '  %-16s -> %s\n' "$a" "$(./build/t $a)"
done
printf '  %-16s -> ' "-f (no operand)"
./build/t -f 2>&1 || true

expect() {
	got=$(./build/t $1)
	[ "$got" = "$2" ] || bad "'$1': got '$got', want '$2'"
}
expect "-vfX"          "v=1 file=X rest=0"
expect "-f Y z"        "v=0 file=Y rest=1 z"
expect "-vf Y"         "v=1 file=Y rest=0"
expect "-- -x"         "v=0 file=(null) rest=1 -x"
expect "-"             "v=0 file=(null) rest=1 -"
expect "a b"           "v=0 file=(null) rest=2 a b"

note "operand loss check: shipped vs upstream vs broken"
# t3.c allocates each argv string with calloc, so the byte after the
# terminator is zero -- the case a normal contiguous stack hides.
for v in shipped upstream broken; do
	case $v in
	shipped)  inc="build" ;;
	upstream) inc="upstream" ;;
	broken)   inc="broken" ;;
	esac
	$CC $WARN -I "$inc" -o "build/t3-$v" t3.c
	got=$(./build/t3-$v | sed -n 's/^actual: *//p')
	printf '  %-9s -> %s\n' "$v" "$got"
	case $v in
	broken)
		[ "$got" = "file=Y rest=1 : z" ] && \
			bad "broken variant was expected to drop the operand" ;;
	*)
		[ "$got" = "file=Y rest=1 : z" ] || \
			bad "$v variant dropped a positional argument" ;;
	esac
done

note "AddressSanitizer: out-of-bounds read on a heap argv"
if printf 'int main(void){return 0;}\n' | \
   $CC -fsanitize=address -x c -o build/asanprobe - 2>/dev/null && \
   ./build/asanprobe >/dev/null 2>&1; then
	for v in shipped upstream broken; do
		case $v in
		shipped)  inc="build" ;;
		upstream) inc="upstream" ;;
		broken)   inc="broken" ;;
		esac
		$CC $WARN -I "$inc" -fsanitize=address -g -o "build/t2-$v" t2.c
		if ./build/t2-$v >/dev/null 2>"build/asan-$v.log"; then
			printf '  %-9s -> clean\n' "$v"
			[ "$v" = shipped ] || \
				printf '             (expected an overflow here)\n'
		else
			printf '  %-9s -> %s\n' "$v" \
				"$(sed -n 's/.*ERROR: AddressSanitizer: \([a-z-]*\).*/\1/p' \
				   "build/asan-$v.log" | head -1)"
			[ "$v" = shipped ] && bad "shipped arg.h read out of bounds"
		fi
	done
else
	printf '  skipped (no working -fsanitize=address)\n'
fi

note "drw.c syntax check (SKILL.md section 3.6)"
$CC $WARN -I x11stub -I build -c build/drw.c -o build/drw.o
printf '  ok (stub X11/Xft headers; not linked, needs a real X server)\n'

note "Makefile + config.mk (SKILL.md section 4.1)"
# Only GNU make is exercised. `include config.mk` is also BSD make syntax, but
# no bmake is available here to prove it, so a non-GNU make is skipped rather
# than guessed at. Every target is run for real: all, install, uninstall, dist
# (which runs clean first). A dry run would prove nothing -- make -n never
# reads a recipe body closely enough to fail on it.
ok=0
if ! command -v "$MAKE" >/dev/null 2>&1; then
	printf '  skipped (no make on PATH)\n'
elif ! "$MAKE" --version 2>/dev/null | grep -q GNU; then
	printf '  skipped (make is not GNU make; BSD make is untested here)\n'
else
	mkdir -p build/maketest
	(cd build/maketest && "$AWK" '
		/^```makefile$/ { inblk = 1; n = 0; next }
		/^```$/ {
			if (inblk) {
				name = ""
				if (buf[1] ~ /^VERSION = /)         name = "config.mk"
				if (buf[1] ~ /^include config\.mk/) name = "Makefile"
				if (name != "") {
					for (i = 1; i <= n; i++)
						print buf[i] > name
					close(name)
				}
			}
			inblk = 0
			next
		}
		inblk { buf[++n] = $0 }
	' ../../../SKILL.md)
	ok=1
	# a missing marker must FAIL cleanly, not abort the script under set -e
	for f in config.mk Makefile; do
		[ -f "build/maketest/$f" ] || { bad "SKILL.md produced no $f"; ok=0; }
	done
	for f in util.c util.h arg.h config.def.h lc.c; do
		[ -f "build/$f" ] || { bad "make section needs build/$f"; ok=0; }
	done
fi
if [ "$ok" = 1 ]; then
	# tool.c/SRC are the Makefile's generic placeholder name; the worked
	# example (lc.c + util.c + arg.h) is real code that fits the same slot.
	cp build/util.c build/util.h build/arg.h build/config.def.h build/maketest/
	cp build/lc.c build/maketest/tool.c
	# Section 4.1 is a template, not a project: dist packages LICENSE,
	# README and tool.1, which no code block defines. The harness supplies
	# them so the recipe has real files to copy, nothing more.
	cp ../LICENSE build/maketest/LICENSE
	printf 'scaffold, so the dist target has a README to package\n' \
		> build/maketest/README
	printf '.TH TOOL 1 tool-VERSION\n.SH NAME\ntool \\- scaffold\n' \
		> build/maketest/tool.1
	mk() { (cd build/maketest && "$MAKE" CC="$CC" "$@"); }
	ver=$(sed -n 's/^VERSION = //p' build/maketest/config.mk)
	dest=$here/build/destdir
	man=$dest/usr/local/share/man/man1/tool.1

	if mk >build/make.log 2>&1 && [ -x build/maketest/tool ]; then
		out=$(printf 'a\nb\nc\n' | ./build/maketest/tool)
		[ "$out" = "3 <stdin>" ] || bad "make-built tool: got '$out', want '3 <stdin>'"
		printf '  make           -> built tool (SRC = tool.c util.c), ran it: %s\n' "$out"
	else
		bad "make failed to build tool from the shipped Makefile/config.mk"
		sed 's/^/  /' build/make.log
		ok=0
	fi

	if [ "$ok" = 1 ] && mk install DESTDIR="$dest" >build/make-install.log 2>&1; then
		# print the action, then assert -- so a FAIL below is never
		# contradicted by a success line printed after it
		printf '  make install   -> ran with DESTDIR=test/build/destdir\n'
		[ -f "$dest/usr/local/bin/tool" ] || bad "make install: no bin/tool under DESTDIR"
		[ -f "$man" ] || bad "make install: no man1/tool.1 under DESTDIR"
		# the install recipe seds VERSION into the page as it copies it
		grep -q "tool-$ver" "$man" 2>/dev/null || \
			bad "make install: VERSION not substituted in tool.1"
		if mk uninstall DESTDIR="$dest" >>build/make-install.log 2>&1; then
			printf '  make uninstall -> ran\n'
			[ -f "$dest/usr/local/bin/tool" ] && bad "make uninstall left bin/tool"
			[ -f "$man" ] && bad "make uninstall left man1/tool.1"
		else
			bad "make uninstall failed"
			sed 's/^/  /' build/make-install.log
		fi
	elif [ "$ok" = 1 ]; then
		bad "make install failed"
		sed 's/^/  /' build/make-install.log
	fi

	# dist runs clean first, so it goes last; the tool binary is gone after.
	if [ "$ok" = 1 ] && mk dist >build/make-dist.log 2>&1 && \
	   [ -f "build/maketest/tool-$ver.tar.gz" ]; then
		tar -tzf "build/maketest/tool-$ver.tar.gz" >build/dist.list 2>&1 || \
			bad "make dist: tool-$ver.tar.gz is not readable"
		printf '  make dist      -> tool-%s.tar.gz, %s members\n' \
			"$ver" "$(wc -l <build/dist.list | tr -d ' ')"
		# section 4.1 warns that a tarball missing a header fails only on
		# the downloader's machine, so check HDR really landed in it.
		for f in LICENSE Makefile README config.def.h config.mk tool.1 \
			 tool.c util.c arg.h util.h; do
			grep -qx "tool-$ver/$f" build/dist.list || \
				bad "make dist: tool-$ver.tar.gz lacks $f"
		done
	elif [ "$ok" = 1 ]; then
		bad "make dist failed to produce tool-$ver.tar.gz"
		sed 's/^/  /' build/make-dist.log
	fi
fi

echo
if [ "$fail" = 0 ]; then
	echo "all checks passed"
else
	echo "SOME CHECKS FAILED"
	exit 1
fi
