# suckless-harness

A skill file that constrains an AI coding agent to write C the way suckless.org
writes C: ISO C99 + POSIX, `config.h` instead of runtime config parsers, a
hand-written Makefile, `die()`/`ecalloc()` for errors, man pages instead of
`--help`, and a hard ceiling on how much code is allowed to exist.

It is one markdown file. `SKILL.md` is the whole product; everything else in this
repository exists to prove that the code inside it compiles and behaves.

## Install

Drop `SKILL.md` into your agent's skills directory, in a folder named
`suckless-harness`:

| Agent | Path |
|---|---|
| Claude Code | `~/.claude/skills/suckless-harness/SKILL.md` |
| Gemini CLI | `~/.gemini/config/skills/suckless-harness/SKILL.md` |

The front matter carries `name` and `description`; activation is driven by the
description text, which lists the trigger phrases (`suckless`, `dwm style`,
`minimal C`, `config.h pattern`, `no bloat`, `Plan 9 style`, and others).

## What is in it

Twelve sections plus a precedence rule that resolves the conflicts between them.
The parts that carry real code:

- **§ 3.3** `die()` / `ecalloc()` / `estrdup()`, including the colon trick and
  the `fmt[0]` guard that keeps `die("")` from reading `fmt[-1]`
- **§ 3.4** a complete `arg.h` — see below
- **§ 3.6** a `drw.h` / `drw.c` X11 plumbing layer with a real double-buffer blit
- **§ 4** `Makefile` + `config.mk`, including the X11 include and link flags
- **§ 5.1** a man page template that works with the `install` target's `sed`
- **§ 11** a complete worked program

## The `arg.h` finding

The header shipped here is **not** upstream suckless `arg.h`, and the difference
is the most substantive thing in this repository.

Upstream (st, dmenu, sbase) walks the option characters with a `brk_` flag and
ends the loop through its condition, `argv[0][i_] && !brk_`. Upstream's `ARGEND`
is two bare closing braces.

Once `EARGF` takes the separate-argument path (`-f FILE`), `argv[0]` *is* the
operand. Any later test that indexes it at the option's old offset reads past the
terminator when the operand is short. Under AddressSanitizer, with a
heap-allocated `argv` so the read is actually visible:

```
ERROR: AddressSanitizer: heap-buffer-overflow
READ of size 1 ... 0 bytes after 2-byte region
```

Three variants, all three built and run by `test/run.sh`:

| Variant | Out-of-bounds read | Parses `prog -f Y z` correctly |
|---|---|---|
| upstream `brk_` | yes | yes — `brk_` ends the loop whatever the stray byte says |
| `argused_` with the test moved into `ARGEND` | yes | **no** — silently drops `z` |
| shipped here | no | yes |

The middle row is the trap. Moving the test into `ARGEND` looks like a tidy-up,
but when the out-of-bounds byte reads zero the extra `argv++; argc--` stacks on
the loop's own increment and a positional argument disappears with no
diagnostic. It passes casual testing because argv strings sit contiguously on a
normal stack, so that byte is almost always nonzero.

The version here branches on `argused_` alone. No index test, no read, and
declarations stay at the top of the block — which upstream's mid-block `int i_;`
does not, and which § 2.5 of the skill requires.

This is a deviation, held deliberately, and § 3.4 says so in the document itself
so that nobody "restores" it later.

## Verify

```bash
sh test/run.sh
```

Needs `awk` and a C99 compiler. It extracts every C block from `SKILL.md` into
`test/build/`, compiles them, runs the worked example, runs `arg.h` through nine
invocation forms, compares the three `arg.h` variants for operand loss and for
out-of-bounds reads, and syntax-checks `drw.c` against stub X11 headers.
AddressSanitizer checks are skipped if the compiler lacks them. If `make` is on
`PATH`, it also extracts § 4.1's `Makefile` and `config.mk`, builds the worked
example through them (`SRC = tool.c util.c`), and dry-runs `install`/`dist`;
skipped with a message if `make` is absent.

`drw.c` is compiled, not linked or run — that needs real Xlib and an X server.

```
test/
├── run.sh            all checks
├── extract.awk       pulls the C sources back out of SKILL.md
├── t.c               option matrix
├── t2.c              heap-allocated argv, for AddressSanitizer
├── t3.c              calloc'd argv, zero slack after the terminator
├── upstream/arg.h    comparison baseline, transcribed from st
├── broken/arg.h      the defective variant, kept to demonstrate the failure
└── x11stub/          minimal X11/Xft headers for the drw.c syntax check
```

`upstream/arg.h` and `broken/arg.h` are there to be tested against. Neither is
what the skill ships.

## Status

Version 1.2. The code blocks are verified as described above; the `Makefile`
and `config.mk` are built and run through `make` when it is present on
`PATH`, and skipped with a message otherwise.

Corrections welcome, particularly on the `arg.h` reasoning.

## License

MIT/X Consortium. See `LICENSE`.
