GROUP MEMBERS:

ADITYA SWARUP
ALEX ROGERS 

BUILDING THE PASS:

Uses the same buildAndTest.sh supplied for the sample BasicBldemo pass. The script takes the name of C file(without .c extension) to be profiled and prints the output. 
Makefile present in the folder contains all the changes for compiling the path profile code.
[We have not tested the compilation using CMake but I guess have made all the required changes]

IMPORTANT:
For compilation things might differ based on whether LLVM has "Debug+Asserts" or "Release+Asserts" directory for storing .so libraries of the pass. 
On our machine it is "Release+Asserts" so the script buildAndTest.sh will need to be changed if LLVM is configured to store libraries in "Debug+Asserts".

Name of the shared library: CS201PathProfiling.so[/.dylib - MAC]
Command parameter for running path profiling pass when using LLVM opt: -Pathprofiling 

Instructions for running the pass remain same as the BasicBldemo example given to us for understanding LLVM.   


RUNNING THE PASS:

Running the pass and the generated IR:

$ ./buildAndTest.sh sai # to test support/sai.c

# Above script does the following:
$ clang -emit-llvm support/sai.c -c -o support/sai.bc
$ make clean && make && ~/Workspace/llvm/Release+Asserts/bin/opt -load ../../../Release+Asserts/lib/CS201PathProfiling.${SHARED_LIB_EXT} -PathProfiling support/${INPUT}.bc -S -o support/${INPUT}.ll && \
$ ~/Workspace/llvm/Release+Asserts/bin/llvm-as support/sai.ll -o support/sai.bb.bc
$ ~/Workspace/llvm/Release+Asserts/bin/lli support/sai.bb.bc

READING THE OUTPUT:

Prints the NAME OF THE MODULE:

Prints FUNCTION NAME: F() which is being profiled
Prints the Backedges present in F() between the tail and head nodes(Uses Block address as identifiers for Basic Blocks).[Labelled with Backedge]

Prints all the Basic Block in F():
Prints the Basic Block address at the start.[Labelled with Basic Block]
Followed by Label and Basic block information along with instructions.

At the End of F():
Prints all the Loops in F().[Labelled Loops:, identifies all the loops by Loop0, Loop1 etc]

Prints all the innermost Loops in F().[Labelled Innermost Loops:, identifies all innermost loops by Loop0, Loop1 etc]
Prints all the edge weights assigned by Ball and Larus algorithm for each innermost loop in sequence identified by Loop number[Loop0, Loop1 etc.]

If no Loops are present in F() - Prints Loop containing no loop information. Doesn't print the Innermost Loops and Edge weights.

At the end of profiling:

IMPORTANT:
Prints all the unique paths identified in format: Path_"Block address of head node of loop"_"Pathnumber identified by the counter": "Number of times path is taken"
[As documented in the sample output described on the webpage]
Example:
Path_0x25a4880_0: 100 
 
 
