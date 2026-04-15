import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import re

sizes = []
times = []

with open("results.txt", "r") as f:
    for line in f:
        match = re.search(r"size = (\d+)", line)
        match2 = re.search(r"avg = ([0-9eE\.\-]+)", line)

        if match and match2:
            size = int(match.group(1))
            time = float(match2.group(1))

            sizes.append(size * 4)  
            times.append(time)

plt.plot(sizes, times, marker='o')
plt.xlabel("message size (bytes)")
plt.ylabel("average time per ping-pong (s)")
plt.title("mpi bandwidth test")
plt.xscale("log")
plt.yscale("log")
plt.grid(True)

plt.savefig("plot.png")
print("plot saved as plot.png")
