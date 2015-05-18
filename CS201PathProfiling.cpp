#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/ADT/SmallSet.h"

#include <map>
#include <algorithm>
//#include "llvm/Analysis/LoopInfo.h"
//#include "llvm/IR/Dominators.h"
//#include "../../IR/AsmWriter.h"
 
using namespace llvm;
 
 
namespace {
	// https://github.com/thomaslee/llvm-demo/blob/master/main.cc
	static Function* printf_prototype(LLVMContext& ctx, Module *mod) {
		std::vector<Type*> printf_arg_types;
		printf_arg_types.push_back(Type::getInt8PtrTy(ctx));
		 
		FunctionType* printf_type = FunctionType::get(Type::getInt32Ty(ctx), printf_arg_types, true);
		Function *func = mod->getFunction("printf");
		if(!func)
		func = Function::Create(printf_type, Function::ExternalLinkage, Twine("printf"), mod);
		func->setCallingConv(CallingConv::C);
		return func;
	}

	struct BasicBlocksDemo : public FunctionPass {
		static char ID;
		LLVMContext *Context;
		BasicBlocksDemo() : FunctionPass(ID) {}
		GlobalVariable *bbCounter = NULL;
		GlobalVariable *BasicBlockPrintfFormatStr = NULL;
		Function *printf_func = NULL;

		//----------------------------------
		void FindFunctionBackEdges(const Function &F, SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &Result);
		void LoopConstruction(SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &BackEdgesSet,std::vector<std::vector<const BasicBlock *>> &LoopSet);
		void Innermost_Loop(std::vector<std::vector<const BasicBlock *>> &Loopset);
		void computeEdgeWeights(std::vector<std::vector<const BasicBlock *>> Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges);
		bool doInitialization(Module &M) {
			errs() << "\n---------Starting BasicBlockDemo---------\n";
			Context = &M.getContext();
			bbCounter = new GlobalVariable(M, Type::getInt32Ty(*Context), false, GlobalValue::InternalLinkage, ConstantInt::get(Type::getInt32Ty(*Context), 0), "bbCounter");
			const char *finalPrintString = "BB Count: %d\n";
			Constant *format_const = ConstantDataArray::getString(*Context, finalPrintString);
			BasicBlockPrintfFormatStr = new GlobalVariable(M, llvm::ArrayType::get(llvm::IntegerType::get(*Context, 8), strlen(finalPrintString)+1), true, llvm::GlobalValue::PrivateLinkage, format_const, "BasicBlockPrintfFormatStr");
			printf_func = printf_prototype(*Context, &M);
			 
			errs() << "Module: " << M.getName() << "\n";
			 
			return true;
		}
		 
		//----------------------------------
		bool doFinalization(Module &M) {
		errs() << "-------Finished BasicBlocksDemo----------\n";
		 
		return false;
		}
		//----------------------------------
		bool runOnFunction(Function &F) override {
		SmallVector<std::pair<const BasicBlock*,const BasicBlock*>,32> BackEdgesSet;

		errs() << "Function: " << F.getName() << '\n';
		FindFunctionBackEdges(F,BackEdgesSet);
		for (unsigned i = 0, e = BackEdgesSet.size(); i != e; ++i){
			errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(BackEdgesSet[i].first)) << " to " << llvm::BlockAddress::get(const_cast<BasicBlock*>(BackEdgesSet[i].second))<<"\n";
		}
		std::vector<std::vector<const BasicBlock*>>LoopSet(BackEdgesSet.size());
		LoopConstruction(BackEdgesSet,LoopSet);	

		for (unsigned i = 0, e = LoopSet.size(); i != e; ++i){
		  for (unsigned m = 0, n = LoopSet[i].size(); m != n; ++m){
			errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m])) << "  ";
		  }
			errs() << "\n";
		}

		if(!LoopSet.empty())
			Innermost_Loop(LoopSet);

		for (unsigned i = 0, e = LoopSet.size(); i != e; ++i){
			errs() << "Innermost Loop:";
		  for (unsigned m = 0, n = LoopSet[i].size(); m != n; ++m){
			errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m])) << "  ";
		  }
			errs() << "\n";
		}

		for(auto &BB: F) {
		// Add the footer to Main's BB containing the return 0; statement BEFORE calling runOnBasicBlock
		if(F.getName().equals("main") && isa<ReturnInst>(BB.getTerminator())) { // major hack?
		addFinalPrintf(BB, Context, bbCounter, BasicBlockPrintfFormatStr, printf_func);
		}
		runOnBasicBlock(BB);
		}
		 
		return true; // since runOnBasicBlock has modified the program
		}
		 
		//----------------------------------
		bool runOnBasicBlock(BasicBlock &BB) {
		errs() << "BasicBlock: " << BB.getName()<< llvm::BlockAddress::get(&BB) << '\n';
		//compute dominator info per basic block
		//find all the loops
		//find the innermost loop
		if (BB.hasName()) {              // Print out the label if it exists...
			errs() << "\n";
			errs() << BB.getName();
			errs() << ':';
		  } /*else if (!BB.use_empty()) {      // Don't print block # of no uses...
			errs() << "\n; <label>:";
			int Slot = Machine.getLocalSlot(&BB);
			if (Slot != -1)
			  errs() << Slot;
			else
			  errs() << "<badref>";
		  }*/
		IRBuilder<> IRB(BB.getFirstInsertionPt()); // Will insert the generated instructions BEFORE the first BB instruction
		 
		Value *loadAddr = IRB.CreateLoad(bbCounter);
		Value *addAddr = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context), 1), loadAddr);
		IRB.CreateStore(addAddr, bbCounter);
		 
		for(auto &I: BB)
		errs() << I << "\n";

		//edge_assign(set of loops)
		//path_profile(set of loops)
		 
		return true;
		}
		 
		//----------------------------------
		// Rest of this code is needed to: printf("%d\n", bbCounter); to the end of main, just BEFORE the return statement
		// For this, prepare the SCCGraph, and append to last BB?
		void addFinalPrintf(BasicBlock& BB, LLVMContext *Context, GlobalVariable *bbCounter, GlobalVariable *var, Function *printf_func) {
		IRBuilder<> builder(BB.getTerminator()); // Insert BEFORE the final statement
		std::vector<Constant*> indices;
		Constant *zero = Constant::getNullValue(IntegerType::getInt32Ty(*Context));
		indices.push_back(zero);
		indices.push_back(zero);
		Constant *var_ref = ConstantExpr::getGetElementPtr(var, indices);
		 
		Value *bbc = builder.CreateLoad(bbCounter);
		CallInst *call = builder.CreateCall2(printf_func, var_ref, bbc);
		call->setTailCall(false);
		}
		};
	}

void BasicBlocksDemo::FindFunctionBackEdges(const Function &F,
     SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &Result) {
  const BasicBlock *BB = &F.getEntryBlock();
  if (succ_empty(BB))
    return;

  SmallPtrSet<const BasicBlock*, 8> Visited;
  SmallVector<std::pair<const BasicBlock*, succ_const_iterator>, 8> VisitStack;
  SmallPtrSet<const BasicBlock*, 8> InStack;

  Visited.insert(BB);
  VisitStack.push_back(std::make_pair(BB, succ_begin(BB)));
  InStack.insert(BB);
  do {
    std::pair<const BasicBlock*, succ_const_iterator> &Top = VisitStack.back();
    const BasicBlock *ParentBB = Top.first;
    succ_const_iterator &I = Top.second;

    bool FoundNew = false;
    while (I != succ_end(ParentBB)) {
      BB = *I++;
      if (Visited.insert(BB).second) {
        FoundNew = true;
        break;
      }
      // Successor is in VisitStack, it's a back edge.
      if (InStack.count(BB))
        Result.push_back(std::make_pair(ParentBB, BB));
    }

    if (FoundNew) {
      // Go down one level if there is a unvisited successor.
      InStack.insert(BB);
      VisitStack.push_back(std::make_pair(BB, succ_begin(BB)));
    } else {
      // Go up one level.
      InStack.erase(VisitStack.pop_back_val().first);
    }
  } while (!VisitStack.empty());
}

void BasicBlocksDemo::LoopConstruction(SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &BackEdgesSet,std::vector<std::vector<const BasicBlock *>> &LoopSet){
    std::vector<const BasicBlock *> Stack;
    for (unsigned i = 0, e = BackEdgesSet.size(); i != e; ++i){
          LoopSet[i].push_back(BackEdgesSet[i].second);
	  //errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i].back()))<<"\n";
	  if(std::find(LoopSet[i].begin(),LoopSet[i].end(),BackEdgesSet[i].first)==LoopSet[i].end()){
	      LoopSet[i].push_back(BackEdgesSet[i].first);
	      Stack.push_back(BackEdgesSet[i].first);
	      //errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(Stack.back()))<<"\n";
	  }
	  while(!Stack.empty()){
	      
	    const BasicBlock *temp = Stack.back();
	    Stack.pop_back();
	    for (const_pred_iterator PI = pred_begin(temp), E = pred_end(temp); PI != E; ++PI) {
	      const BasicBlock *P = *PI;
	      
	      if(std::find(LoopSet[i].begin(),LoopSet[i].end(),P)==LoopSet[i].end()){
		LoopSet[i].push_back(P);
		Stack.push_back(P);
	      }
	    }
	  } 
  }
}


void BasicBlocksDemo::Innermost_Loop(std::vector<std::vector<const BasicBlock *>> &Loopset){
	for(unsigned long i = 0; i < Loopset.size()-1;){
	    const BasicBlock* p = *(Loopset[i].begin());
	    if(std::find(Loopset[i+1].begin(),Loopset[i+1].end(),p)!=Loopset[i+1].end()){
		Loopset.erase(Loopset.begin()+1);
	    }
	    else if(p == *(Loopset[i+1].end()))
		Loopset.erase(Loopset.begin()+1);
	    else
		++i;
	}
}

//input: start and end blocks of a loop
void BasicBlocksDemo::computeEdgeWeights(std::vector<std::vector<const BasicBlock *>> Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges){
		std::map<std::string,int> numpaths;
		//iterate in reverse of a loop
		//for (pred_iterator PI = pred_begin(end), e = pred_end(start); PI!=e;++PI){
		for(std::vector<std::vector<const BasicBlock*>>::iterator it = Loopset.begin();it!=Loopset.end();++it){
			int count = 0;
			const BasicBlock* start;
			for (std::vector<const BasicBlock*>::iterator iit = (*it).begin();iit!=(*it).end();++iit){
				const BasicBlock * node = *iit;
				if(count == 0){
					//node is start
					start = *iit;
				}
				else if(count == 1){
					//2nd node which is end
					numpaths[node->getName()] = 0;
				}
				else{
					numpaths[node->getName()] =0;
					for(const_succ_iterator SI = succ_begin(node), e = succ_end(node);SI!=e;++SI){
						BasicBlock * suc = *SI;
						std::pair <const BasicBlock*,const BasicBlock*> edge (node,suc);
						edges.insert(edges.begin(),{edge,numpaths[node->getName()]});
						//edges.insert(std::make_pair(iit,suc),numpaths[iit]);
						numpaths[node->getName()] += numpaths[suc->getName()];
					}
				}
				count++;
			}
			numpaths[start->getName()] = 0;
			for(const_succ_iterator SI = succ_begin(start), e = succ_end(start);SI!=e;++SI){
				const BasicBlock* suc = *SI;
				std::pair <const BasicBlock*,const BasicBlock*> edge (start,suc);
				edges.insert(edges.begin(),{edge,numpaths[start->getName()]});
				numpaths[start->getName()] += numpaths[suc->getName()];
			}
		}	
	}
 
char BasicBlocksDemo::ID = 0;
static RegisterPass<BasicBlocksDemo> X("bbdemo", "BasicBlocksDemo Pass", false, false);
