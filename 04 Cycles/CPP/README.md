## Arty's programm to search Collatz Cycles

#### compile
Compile the program as follows:

```console
clang prog.cpp -o ./prog -O3 -m64
```

#### run
Run the program as follows:

```console
./prog
```

#### parameters

* max_x0 means up to which number to scan starting number x0
* max_q means up to which Q to scan
* max_n means that if sequence length reached max_n count then this sequence is dropped and not generated further, so max_n is maximal possible length of a sequence.
