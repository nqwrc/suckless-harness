/* See LICENSE file for copyright and license details. */

/*
 * The BROKEN variant, kept here ONLY so run.sh can demonstrate the defect.
 * Do not use this. See ../../SKILL.md § 3.4.
 *
 * ARGEND re-tests argv[0][i_ + 1] after the option argument has been
 * consumed. By then argv[0] is the operand, so for any operand shorter
 * than i_ + 1 that reads past the terminator -- and when the stray byte
 * is zero, the extra argv++/argc-- stacks on the loop's own increment and
 * silently eats a positional argument.
 */

#ifndef ARG_H
#define ARG_H

extern char *argv0;

#define ARGBEGIN	for (argv0 = *argv, argv++, argc--;                    \
				argv[0] && argv[0][0] == '-' && argv[0][1];    \
				argc--, argv++) {                              \
				int i_, argused_;                              \
				if (argv[0][1] == '-' && argv[0][2] == '\0') { \
					argv++;                                \
					argc--;                                \
					break;                                 \
				}                                              \
				for (i_ = 1, argused_ = 0; argv[0][i_]; i_++) {\
					switch (argv[0][i_])

#define ARGEND				if (argused_) {                        \
						if (argv[0][i_ + 1] != '\0')   \
							break;                 \
						argv++;                        \
						argc--;                        \
						break;                         \
					}                                      \
				}                                              \
			}

#define ARGC()		argv[0][i_]

#define EARGF(x)	(argused_ = 1, argv[0][i_ + 1] != '\0' ? &argv[0][i_ + 1] : \
				argv[1] != NULL ? (argc--, argv++, argv[0]) :       \
				((x), (char *)0))

#define ARGF()		(argused_ = 1, argv[0][i_ + 1] != '\0' ? &argv[0][i_ + 1] : \
				argv[1] != NULL ? (argc--, argv++, argv[0]) : (char *)0)

#endif /* ARG_H */
