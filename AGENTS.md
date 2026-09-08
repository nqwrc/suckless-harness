# AGENTS.md

Suckless-philosophy C programs, shell scripts and the SKILL.md teaching document.

## Setup
- Nothing to install. Needs a C99 compiler (`cc`), `awk`, GNU `make`.

## Verify before opening a PR
- `ASAN_OPTIONS=detect_leaks=0 sh test/run.sh` extracts every C block from SKILL.md, compiles and runs it.
- Keep `detect_leaks=0` in sandboxes without ptrace (Jules VM); LeakSanitizer otherwise aborts the ASan probe binaries.

## Conventions
- suckless style: C99, hand-written Makefile, `die()`/`ecalloc()`, `config.h`, no bloat.
- Man pages, not README paragraphs, for user docs.
- Do not add dependencies or build systems. Do not reformat unrelated code.

## Never touch (auto-merge policy)
PRs are merged by a script. It merges only PRs with at most 15 files and 800 changed lines that do not touch `.github/`, `.env*`, `Dockerfile` or any lockfile. A PR that touches them is not reviewed and is closed after 72h. Do not edit them; if a task needs it, do the rest and say so in the PR description. Never rebase or force-push an existing branch.
