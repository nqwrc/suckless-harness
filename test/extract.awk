# Extract the named C sources from SKILL.md into the current directory.
#
#   awk -f extract.awk ../SKILL.md
#
# Blocks that open with a /* name.c */ line are written under that name.
# arg.h, util.h and lc.c open with the licence line instead, so they are
# identified by content. Every other C block in the document is an isolated
# fragment and is skipped.

/^```(c|makefile)$/ { inblk = 1; n = 0; next }

/^```$/ {
	if (inblk) {
		name = ""
		if (buf[1] ~ /^\/\* [A-Za-z0-9_.]+\.[ch] \*\/$/) {
			name = buf[1]
			sub(/^\/\* /, "", name)
			sub(/ \*\/$/, "", name)
		} else if (buf[1] ~ /^VERSION = /) {
			name = "config.mk"
		} else if (buf[1] ~ /^include config\.mk/) {
			name = "Makefile"
		} else {
			for (i = 1; i <= n; i++) {
				if (buf[i] ~ /#ifndef ARG_H/)  name = "arg.h"
				if (buf[i] ~ /#ifndef UTIL_H/) name = "util.h"
				if (buf[i] ~ /^lc\(FILE \*fp/) name = "lc.c"
			}
		}
		if (name != "") {
			for (i = 1; i <= n; i++)
				print buf[i] > name
			close(name)
			printf "  %-14s %3d lines\n", name, n
		}
	}
	inblk = 0
	next
}

inblk { buf[++n] = $0 }
