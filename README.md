# MasterMind
The famous mastermind game implemented in C and ARM Assembler to under the concept of Systems Programming.

## Build and Run
You can build the C program, starting from the template in `master-mind-terminal.c`, by typing
> make

and run the Master Mind program in debug mode by typing
> make run

and do unit testing on the matching function
> make unit

## Test
A test script is available to do unit-testing of the matching function. Run it like this from the command line
> sh ./test.sh

To test whether all tests have been successful you can do
> echo $?

which should print `0`.


## Links
- Use the [CPUlator](https://cpulator.01xz.net/?sys=arm-de1soc&d_audio=48000) for running the ARM Assembler code
- You can use any machine with an installation of the `gcc` C compiler for running the C code of the game logic
- C program: [master-mind-terminal.c](master-mind-terminal.c)
- ARM Assembler program: [mm-matches.s](mm-matches.s)

This is an example of doing unit-testing on 2 sequences (C part only; the Assembler version, running on the CPUlator, should show the numbers on the two right-most HEX-displays):
```
> ./cw3 -u 121 313
0 exact matches
1 approximate matches
```

This is an example of doing a full run of the game, using debug mode to show the secret sequence (for testing):
```
> ./cw3 -d
Contents of the sequence (of length 3):  2 1 1
Input seq (len 3): 1 2 3
0 2
Input seq (len 3): 3 2 1
1 1
Input seq (len 3): 2 1 1
3 0
SUCCESS after 3 iterations; secret sequence is  2 1 1
```

The general format for the command line is as follows (see code in master-mind-terminal.c for processing command line options):
```
./mmt [-v] [-d] [-s] <secret sequence> [-u] <sequence1> <sequence2>
```
