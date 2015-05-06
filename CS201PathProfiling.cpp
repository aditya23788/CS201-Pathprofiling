/*
 * Authors: Name(s) <email(s)>
 * 
 */

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"

using namespace llvm;

namespace {
  struct CS201PathProfiling : public FunctionPass {
  static char ID;
  CS201PathProfiling() : FunctionPass(ID) {}

    //----------------------------------
    bool doInitialization(Module &M) {
      return false;
    }

    //----------------------------------
    bool doFinalization(Module &M) {
      return false;
    }
    
    //----------------------------------
    bool runOnFunction(Function &F) override {
      return true;
    }

  };
}

char CS201PathProfiling::ID = 0;
static RegisterPass<CS201PathProfiling> X("pathProfiling", "CS201PathProfiling Pass", false, false);

