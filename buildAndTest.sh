INPUT=${1}
LLVM_HOME=~/Workspace
if [ $(uname -s) == "Darwin" ]; then
    SHARED_LIB_EXT=dylib;
else
    SHARED_LIB_EXT=so;
fi

clang -emit-llvm support/${INPUT}.c -c -o support/${INPUT}.bc && \
    make clean && \
    make && \
    ${LLVM_HOME}/llvm/Release+Asserts/bin/opt -load ../../../Release+Asserts/lib/CS201PathProfiling.${SHARED_LIB_EXT} -PathProfiling support/${INPUT}.bc -S -o support/${INPUT}.ll && \
    ${LLVM_HOME}/llvm/Release+Asserts/bin/llvm-as support/${INPUT}.ll -o support/${INPUT}.bb.bc && \
    ${LLVM_HOME}/llvm/Release+Asserts/bin/lli support/${INPUT}.bb.bc

