# AGENTS.md

Suckless-philosophy C programs, shell scripts and the SKILL.md teaching document.

## Setup
- Nothing to install. Needs a C99 compiler (`cc`), `awk`, GNU `make`.

## Verify before opening a PR
- `sh test/run.sh` extracts every C block from SKILL.md, compiles and runs it.

## Conventions
- suckless style: C99, hand-written Makefile, `die()`/`ecalloc()`, `config.h`, no bloat.
- Man pages, not README paragraphs, for user docs.
- Do not add dependencies or build systems. Do not reformat unrelated code.
