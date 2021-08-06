# Golden automaton optimization

To use the program, make sure to have `GCC` and `Gnu Make` installed on your device, as well as the dependencies listed in the header files.
This program might be unable to work properly on a virtual machine or a docker. Please, make sure that that you are running this software on a plain physical machine.

In order to produce results, the following command must be used : 
`[sh] ./test.sh <limit> <old|new>`

#### Arguments : 
`limit` : All the values above this limit shall not be processed even if they are still stocked in the structures.
`old|new` : Using the `old` argument will ensure that the program uses the catalog-based implementation. Using the `new` argument will ensure that the program uses the boolean vector-base implementation (not recommended).

#### Outputs : 
Each run of the program will create a file with a name following the structure :  `results<limit>.csv` .
The results will be stocked in the `results/old` or `results/new` according to the argument used to run the algorithm.

#### Advices of use : 
It is highly recommended to run the program in a multiplexer such as `Screen` or `Tmux` as the memory track may be invasive and the program may take a long time to end depending on the arguments you use.
Moreover, it is recommended to redirect the memory track in a log file as it may be lost in a multiplexer, or even in a unique terminal.