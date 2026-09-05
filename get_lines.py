with open("test/build/lc.c", "r") as f:
    lines = f.readlines()
    for i, line in enumerate(lines):
        print(f"{i+1}: {line.rstrip()}")
