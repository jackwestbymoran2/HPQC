# File I/O Benchmarking

## Programs
- `file_write.c` / `file_read.c`
- `file_write.py` / `file_read.py`

## Instructions
1. Compile C programs:
2. Run:

## Observations
- Writing 10,000 lines:
- C: ~0.01 s
- Python: ~0.03 s
- Reading 10,000 lines:
- C: ~0.008 s
- Python: ~0.02 s
- As file size increases, the performance gap widens slightly.
- C executes faster due to compiled execution and lower-level memory control.

## Conclusions
- For I/O-intensive tasks, C is more efficient.
- Python is easier to prototype and read, but slower for larger operations.
- Timing methods (`clock()` in C, `time.time()` in Python, and terminal `time`) provide complementary insights.
