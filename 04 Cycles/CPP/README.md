## Arty's programm to search Collatz Cycles

#### compile
Compile the program as follows:

```console
clang-13 collatz_cycles.cpp -o ./collatz_cycles -O3 -m64 -lstdc++
```

#### run
Run the program as follows:

```console
./collatz_cycles > cycles.txt
```

#### parameters

Call the function `find_collatz(max_q, max_x0, max_n)`, for example `find_collatz(4100, 1000, 1000)`, where:

* max_x0 means up to which number to scan starting number x0
* max_q means up to which Q to scan
* max_n means that if sequence length reached max_n count then this sequence is dropped and not generated further, so max_n is maximal possible length of a sequence.
