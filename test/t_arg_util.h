static void
usage(void)
{
	fprintf(stderr, "usage\n");
	exit(1);
}

static void
run(int argc, char *argv[], const char *suffix)
{
	char *file = NULL;
	int i;

	ARGBEGIN {
	case 'f':
		file = EARGF(usage());
		break;
	default:
		usage();
	} ARGEND;

	printf("file=%s rest=%d%s", file ? file : "(null)", argc, suffix);
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
}
