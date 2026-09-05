with open("test/t_util.c", "r") as f:
    code = f.read()
code = code.replace("""	for (i = 0; i < 20; i++) {
		if (q[i] != '\\0') {
			fprintf(stderr, "ecalloc: did not zero memory\\n");
			return 1;
		}
	}""", """	if (q[0] != '\\0' || memcmp(q, q + 1, 19) != 0) {
		fprintf(stderr, "ecalloc: did not zero memory\\n");
		return 1;
	}""")
with open("test/t_util.c", "w") as f:
    f.write(code)
