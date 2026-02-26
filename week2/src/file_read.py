import time

filename = "../data/output_py.txt"

start = time.time()

with open(filename, "r") as f:
    lines = f.readlines()  # read into memory

end = time.time()
print(f"Time to read file: {end - start:.6f} seconds")
