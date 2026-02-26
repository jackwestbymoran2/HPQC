import time

filename = "../data/output_py.txt"

start = time.time()

with open(filename, "w") as f:
    for i in range(10000):
        f.write(f"Line {i}: The quick brown fox jumps over the lazy dog.\n")

end = time.time()
print(f"Time to write file: {end - start:.6f} seconds")
