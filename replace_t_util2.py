with open("test/t_util.c", "r") as f:
    code = f.read()
code = code.replace("""	for (i = 0; i < 20; i++) {
		if (q[i] != '\\0') {
			fprintf(stderr, "ecalloc: did not zero memory\\n");
			return 1;
		}
	}""", """	char zeros[20] = {0};
	if (memcmp(q, zeros, 20) != 0) {
		fprintf(stderr, "ecalloc: did not zero memory\\n");
		return 1;
	}""")
with open("test/t_util.c", "w") as f:
    f.write(code)
