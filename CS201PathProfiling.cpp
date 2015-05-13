/*
 * Authors: 	Aditya 
 *		Alex Rogers aroge005@ucr.edu
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

    <set> computeDominator(Node n1, List elements) {
	std::vector<int> v(num_basic_blocks*2);

	if(elements.length < 1)
	   	v = n1; //only node in dom set

        if(elements.ength == 1){
		std:vector<int> v(n1.length + ele[0].length);
		std::vector<int>::iterator it;
	    	it=std:set_union(n1.domset,n1.length,elements[0].domset, elements[0].length,v.begin());
	    	v.resize(it-v.begin());
	}
	//need to intersect
	else{
		std::vector<int> tmp(basic_blocks*2);
		std::sort(ele[0],ele[0].length);
		std::sort(ele[1],ele[1].length);
		
		it = std::set_intersection(ele[0],ele[0].length,ele[1],ele[1].length,v.begin());
		tmp = v;
		for(int i =2;i<elements.length;i++){
			std::sort(ele[i],ele[i].length);
			it = std::set_intersection(ele[i],ele[i].length,tmp,tmp.length,v.begin());
			tmp = v;
		}
		it = std::set_union(n1,n1.length,tmp,tmp.length, v.begin());
		v.resize(it-v.begin());
	}
	return v;
    }
  };
}

char CS201PathProfiling::ID = 0;
static RegisterPass<CS201PathProfiling> X("pathProfiling", "CS201PathProfiling Pass", false, false);

