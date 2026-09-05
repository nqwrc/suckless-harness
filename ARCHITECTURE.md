# Architecture of suckless-harness

The `suckless-harness` project is not a traditional software application, but rather an AI skill guide (`SKILL.md`) wrapped in a validation harness. The harness tests that the C and Makefile code embedded in the markdown document compiles and executes correctly according to the strict "suckless" philosophy constraints (ISO C99 + POSIX).

## Architecture Overview

The system consists of two primary components:
1. **The Skill Specification:** A single markdown file (`SKILL.md`) that serves as the entire product, instructing an AI on how to write suckless-style C code.
2. **The Test Harness:** A suite of shell and awk scripts located in the `test/` directory. This harness extracts the code snippets from the markdown, compiles them, and runs them to verify their correctness and adherence to constraints (e.g., memory safety without out-of-bounds reads in argument parsing).

## Data Flow Diagram

```text
+-------------------+
|                   |
|    SKILL.md       |  <-- (The core product: Markdown containing C/Make code blocks)
|                   |
+--------+----------+
         |
         | 1. Extraction (test/extract.awk)
         v
+-------------------+
|                   |
| test/build/       |  <-- (Generated C source files, headers, and Makefiles)
|                   |
+--------+----------+
         |
         | 2. Compilation ($CC in test/run.sh)
         v
+-------------------+
|                   |
| Compiled Binaries |  <-- (Test executables: t, t2-*, t3-*, lc)
|                   |
+--------+----------+
         |
         | 3. Execution & Verification (test/run.sh)
         v
+-------------------+
|                   |
| Test Results      |  <-- (Pass/Fail output, memory bounds checks, make validation)
|                   |
+-------------------+
```

## Key Files & Directories

*   **`SKILL.md`**: The primary document and the sole product of this repository. It contains the instructions and embedded code examples defining the suckless style.
*   **`test/run.sh`**: The main execution script for the test harness. It coordinates extraction, compilation, unit testing, memory checking (AddressSanitizer), and build system validation.
*   **`test/extract.awk`**: An awk script responsible for parsing `SKILL.md` and extracting the fenced code blocks into discrete files within the `test/build/` directory based on their annotated filenames.
*   **`test/t*.c`**: Various C files used strictly for testing the extracted logic.
    *   `t.c`: Tests the option parsing matrix of the custom `arg.h`.
    *   `t2.c` and `t3.c`: Memory safety tests (AddressSanitizer checks and operand loss checks) validating the custom `arg.h` behavior against upstream/broken implementations.
    *   `t_util.c`: Unit tests for `util.c` helper functions.
*   **`test/upstream/`** & **`test/broken/`**: Directories containing alternative implementations of `arg.h` used as baselines for the AddressSanitizer and operand loss comparisons.
*   **`test/build/`**: An ephemeral directory created during the test run. All extracted source files and compilation artifacts reside here. It should never be checked into version control.

## Component Interaction & The Test Process

1.  **Extraction phase:** When `test/run.sh` is invoked, it first calls `test/extract.awk` on `SKILL.md`. This parses the markdown and writes out `util.c`, `util.h`, `arg.h`, `drw.c`, `drw.h`, `config.def.h`, `config.mk`, `Makefile`, and `lc.c` to the `test/build/` directory.
2.  **Compilation phase:** `test/run.sh` invokes the system's C99 compiler (`cc` by default). It compiles the worked example (`lc.c`) and builds specialized test binaries incorporating the extracted logic (like `arg.h`).
3.  **Execution phase:** The script runs the compiled binaries with specific arguments to verify behavior. For instance, it provides a matrix of flags to the `arg.h` tester and ensures out-of-bounds reads do not occur. It also runs GNU `make` using the extracted `Makefile` and `config.mk` in a sandbox to ensure standard targets (`all`, `install`, `uninstall`, `dist`) function seamlessly.
4.  **Verification phase:** The return codes and standard outputs of these steps are validated against expected results. If any step fails, `test/run.sh` exits with a non-zero status.

## Design Philosophy

The architecture is deliberately flat and minimalistic, strictly adhering to the "suckless" ethos.
*   **Zero Dependencies:** The test harness relies solely on standard POSIX tools: `sh`, `awk`, and a C compiler.
*   **No Build Tools for the Test Suite:** The testing logic is hand-written in `test/run.sh` rather than utilizing frameworks like CMake or Autotools.
*   **Single Source of Truth:** `SKILL.md` is the absolute source of truth. Changes to the C code are made in the markdown document, not in the standalone C files, ensuring the AI instructions remain perfectly synchronized with the tested code.