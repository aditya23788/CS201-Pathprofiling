#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <sstream>

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
		GlobalVariable *pathctr = NULL;
		Function *printf_func = NULL;

		//----------------------------------
		void FindFunctionBackEdges(const Function &F, SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &Result);
		void LoopConstruction(SmallVectorImpl<std::pair<const BasicBlock*,const BasicBlock*>> &BackEdgesSet,std::vector<std::vector<const BasicBlock *>> &LoopSet);
		void Innermost_Loop(std::vector<std::vector<const BasicBlock *>> &Loopset);
		void computeEdgeWeights(std::vector<std::vector<const BasicBlock *>> &Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges, std::vector<int> &retpaths);
		void Initcounter(std::vector<std::vector<const BasicBlock *>> &Loopset);
		void InsertBasicBlock(std::vector<std::vector<const BasicBlock*>> &Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges, Function &F);
		void updateArray(BasicBlock* bb,unsigned loopnum, int loopsize);
		void addtoEnd(std::vector<std::vector<const BasicBlock*>> &Loopset,std::vector<int> retpaths);


		bool doInitialization(Module &M) {
			errs() << "\n---------Starting BasicBlockDemo---------\n";
			Context = &M.getContext();
			bbCounter = new GlobalVariable(M, Type::getInt32Ty(*Context), false, GlobalValue::InternalLinkage, ConstantInt::get(Type::getInt32Ty(*Context), 0), "bbCounter");
//			const char *finalPrintString = "BB Count: %d\n";
//			Constant *format_const = ConstantDataArray::getString(*Context, finalPrintString);
//			BasicBlockPrintfFormatStr = new GlobalVariable(M, llvm::ArrayType::get(llvm::IntegerType::get(*Context, 8), strlen(finalPrintString)+1), true, llvm::GlobalValue::PrivateLinkage, format_const, "BasicBlockPrintfFormatStr");
//			printf_func = printf_prototype(*Context, &M);
			 
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
		
//		for(auto &BB: F) 
		    //for(auto &I: BB)
//			errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(&BB))<<"\n"<< BB << "\n";


		std::vector<int> retpaths;

		if(!LoopSet.empty()){
			Innermost_Loop(LoopSet);
		
			std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> edges;
			computeEdgeWeights(LoopSet, edges, retpaths);
			errs() << "num paths " << retpaths[0] << "\n";
			Initcounter(LoopSet);
			InsertBasicBlock(LoopSet, edges, F);

		int totalpaths = 0;
        for(unsigned int i = 0;i< retpaths.size();i++)
            totalpaths+= retpaths[i];

        ArrayType* arrtype = ArrayType::get(Type::getInt32Ty(*Context),
                                   totalpaths);
        pathctr = new GlobalVariable(*(F.getParent()),
						arrtype,
                        false,
                        GlobalValue::ExternalLinkage,
                        Constant::getNullValue(arrtype),
                        "pathctr");
        pathctr->setAlignment(16);
        ConstantAggregateZero* const_arr_2 = ConstantAggregateZero::get(arrtype);
        pathctr->setInitializer(const_arr_2);
		addtoEnd(LoopSet,retpaths);

		for(auto &BB: F) {
		// Add the footer to Main's BB containing the return 0; statement BEFORE calling runOnBasicBlock
		if(isa<ReturnInst>(BB.getTerminator())) { // major hack?
		addFinalPrintf(BB, Context, printf_func,LoopSet,retpaths);
		}
		//runOnBasicBlock(BB);
		}

		}

		for(auto &BB: F)
            //for(auto &I: BB)
            errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(&BB))<< "\n";

//Type::getInt32Ty(*Context), false, GlobalValue::InternalLinkage,
//					ConstantArray::get(ArrayType::get(Type::getInt32Ty(*Context),totalpaths),llvm::ArrayRef<llvm::Constant*>() ));
		
			

		for (unsigned i = 0, e = LoopSet.size(); i != e; ++i){
			errs() << "Innermost Loop:";
		  for (unsigned m = 0, n = LoopSet[i].size(); m != n; ++m){
			errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m])) << "  ";
		  }
			errs() << "\n";
		}

		 
		return true; // since runOnBasicBlock has modified the program
		}
		 
		//----------------------------------
		// Rest of this code is needed to: printf("%d\n", bbCounter); to the end of main, just BEFORE the return statement
		// For this, prepare the SCCGraph, and append to last BB?
		void addFinalPrintf(BasicBlock& BB, LLVMContext *Context, /*GlobalVariable *var,*/ Function *printf_func,std::vector<std::vector<const BasicBlock *>> &LoopSet,std::vector<int> &retpaths) {
		IRBuilder<> IRB(BB.getTerminator()); // Insert BEFORE the final statement
		errs() << "beforeloop\n";
		for(unsigned long i =0;i<LoopSet.size();i++){
			errs() << "print";
			std::stringstream s;
			s << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][0]));
			errs() << "asdf "<< s.str()<<"\n";
			//const char* finalstr = s.str().c_str();
			std::string ss = "Path_";
			ss.append(s.str());
			ss.append("_");

			//2nd for for inner loop
			//loop over retpaths[i]
			for(int j = 0; j<retpaths[i];j++){
				std::string a = ss + std::to_string(j);
//				ss.append(std::to_string(j));
				errs() << a << "\n";
//				finalstr = finalstr + '_'  + j;
			 a =  a + ": %d\n";
			errs() << a <<"\n";
			const char *finalPrintString = a.c_str();
            Constant *format_const = ConstantDataArray::getString(*Context, finalPrintString);
			BasicBlockPrintfFormatStr = new GlobalVariable(*((BB.getParent())->getParent()), llvm::ArrayType::get(llvm::IntegerType::get(*Context, 8), strlen(finalPrintString)+1), true, llvm::GlobalValue::PrivateLinkage, format_const, "BasicBlockPrintfFormatStr");
			printf_func = printf_prototype(*Context, ((BB.getParent())->getParent()));

            

			    Value* idxValue = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context),i * retpaths[i]),ConstantInt::get(Type::getInt32Ty(*Context),j));
	    		std::vector<Value*> gepIndices(2);
			    ConstantInt* initvalue = ConstantInt::get(*Context, APInt(32, StringRef("0"), 10));
			    gepIndices[0] = initvalue;
			    gepIndices[1] = idxValue;
			    GetElementPtrInst* pcpointer = GetElementPtrInst::Create(pathctr,gepIndices,"pcptr",BB.getTerminator());
				errs() << "loaded array1\n";
	    		//load from array
			    LoadInst* oldpc = new LoadInst(pcpointer,"oldpc",BB.getTerminator());

        		std::vector<Constant*> indices;
		        Constant *zero = Constant::getNullValue(IntegerType::getInt32Ty(*Context));
        		indices.push_back(zero);
		        indices.push_back(zero);
        		Constant *var_ref = ConstantExpr::getGetElementPtr(BasicBlockPrintfFormatStr, indices);
				errs() << "loaded array2\n";
		//		Value *bbc = IRB.CreateLoad(bbCounter);
				CallInst *call = IRB.CreateCall2(printf_func, var_ref, oldpc);
				errs() <<"set tail\n";
	    	    call->setTailCall(false);

			}

		}
		 
		
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

//input: vector of loop vectors, and edge map
void BasicBlocksDemo::computeEdgeWeights(std::vector<std::vector<const BasicBlock*>> &Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges,std::vector<int> &retpaths){
		std::map<const BasicBlock*,int> numpaths;
		//iterate in reverse of a loop
		//for (pred_iterator PI = pred_begin(end), e = pred_end(start); PI!=e;++PI){
		for(std::vector<std::vector<const BasicBlock*>>::iterator it = Loopset.begin();it!=Loopset.end();++it){
			int count = 0;
			const BasicBlock *start;
			for (std::vector<const BasicBlock*>::iterator iit = (*it).begin();iit!=(*it).end();++iit){
				const BasicBlock * node = *iit;
				if(count == 0){
					//node is start
					start = *iit;
				}
				else if(count == 1){
					//2nd node which is end
					numpaths[node] = 1;
				}
				else{
					numpaths[node] =0;
					for(succ_const_iterator SI = succ_begin(node), e = succ_end(node);SI!=e;++SI){
						const BasicBlock * suc = *SI;
						std::pair <const BasicBlock*,const BasicBlock*> edge (node,suc);
						edges.insert(edges.begin(),{edge,numpaths[node]});
						//printing
						errs() << "edge: ";
						errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (node));
						errs() << " to ";
						errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc));
						errs() << " wegith ";
						errs() << numpaths[node];
						errs() << "\n";
						//edges.insert(std::make_pair(iit,suc),numpaths[iit]);
						numpaths[node] += numpaths[suc];
					}
				}
				count++;
			}
			numpaths[start] = 0;
			for(succ_const_iterator SI = succ_begin(start), e = succ_end(start);SI!=e;++SI){
				const BasicBlock* suc = *SI;
				std::vector<const BasicBlock*>::iterator it1;
				it1 = find((*it).begin(),(*it).end(),suc);
				if(it1 != (*it).end()){
					std::pair <const BasicBlock*,const BasicBlock*> edge (start,suc);
					edges.insert(edges.begin(),{edge,numpaths[start]});
					//printing
					errs() << "edge: ";
					errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (start));
					errs() << " to ";
					errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc));
					errs() << " wegith ";
					errs() << numpaths[start];
					errs() << "\n";
					numpaths[start] += numpaths[suc];
					
				}
				else{
					//errs() << "an edge outside of loop\n";
				}
				
			}
			
			
			retpaths.push_back(numpaths[start]);
		}	
	}

void BasicBlocksDemo::Initcounter(std::vector<std::vector<const BasicBlock*>> &Loopset){
    for (unsigned i = 0, e = Loopset.size(); i != e; ++i){
	IRBuilder<> IRB(const_cast<BasicBlock *>(Loopset[i][0])->getFirstInsertionPt());
	Value *addAddr = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context), 0), ConstantInt::get(Type::getInt32Ty(*Context), 0));
	IRB.CreateStore(addAddr, bbCounter);
	errs()<< *(Loopset[i][0]) << "\n";
    }
	
}
	
void BasicBlocksDemo::InsertBasicBlock(std::vector<std::vector<const BasicBlock*>> &Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges, Function &F){
    
    for(auto &I : edges){
	std::pair<const BasicBlock*,const BasicBlock*> BBpair = I.first;
	errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (BBpair.first))<< " to " << llvm::BlockAddress::get(const_cast<BasicBlock*> (BBpair.second))<<"\n";
	int edge = I.second;
	errs() << "Before temp"<< "\n";
	//const BasicBlock* temp = llvm::SplitEdge(const_cast<BasicBlock*>(BBpair.first), const_cast<BasicBlock*>(BBpair.second), this);
	BasicBlock* temp = llvm::BasicBlock::Create(*Context,"",const_cast<BasicBlock *>(BBpair.second)->getParent() ,const_cast<BasicBlock *>(BBpair.second));
	
	
	TerminatorInst *PredTerm = const_cast<TerminatorInst *>(BBpair.first->getTerminator());
	for (unsigned i = 0, e = PredTerm->getNumSuccessors(); i != e; ++i)
	    if (PredTerm->getSuccessor(i) == BBpair.second) {
		const_cast<BasicBlock *>(BBpair.second)->removePredecessor(const_cast<BasicBlock *>(BBpair.first), true);
		const_cast<TerminatorInst *>(PredTerm)->setSuccessor(i,const_cast<BasicBlock *>(temp));
	    }
	    
	//BasicBlock* assign = const_cast<BasicBlock*>(BBpair.second);
	IRBuilder<> IRB(temp); // Will insert the generated instructions BEFORE the first BB instruction
		 
	Value *loadAddr = IRB.CreateLoad(bbCounter);
	Value *addAddr = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context), edge), loadAddr);
	IRB.CreateStore(addAddr, bbCounter);
	IRB.CreateBr(const_cast<BasicBlock *>(BBpair.second));
	
	errs() << *temp << "\n";
	/*for(auto &BB: F) 
	    errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(&BB))<<"\n"<< BB << "\n";*/
	
	
	/*BasicBlock* assign = const_cast<BasicBlock*>(BBpair.second);
	IRBuilder<> IRB(assign->getFirstInsertionPt()); // Will insert the generated instructions BEFORE the first BB instruction
		 
	Value *loadAddr = IRB.CreateLoad(bbCounter);
	Value *addAddr = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context), edge), loadAddr);
	IRB.CreateStore(addAddr, bbCounter);*/
    
    }
    
}

void BasicBlocksDemo::updateArray(BasicBlock *bb, unsigned loopnum,int loopsize)
{
	IRBuilder<> IRB(bb->getTerminator());
	Value *inc = IRB.CreateLoad(bbCounter);
	Value* idxValue = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context),loopnum * loopsize),inc);
	std::vector<Value*> gepIndices(2);
	ConstantInt* initvalue = ConstantInt::get(*Context, APInt(32, StringRef("0"), 10));
    gepIndices[0] = initvalue;
    gepIndices[1] = idxValue;
	GetElementPtrInst* pcpointer = GetElementPtrInst::Create(pathctr,gepIndices,"pcptr",bb->getTerminator());
	LoadInst* oldpc = new LoadInst(pcpointer,"oldpc",bb->getTerminator());
	Value* newpc = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context),1),oldpc);
	new StoreInst(newpc,pcpointer,bb->getTerminator());
	}
void BasicBlocksDemo::addtoEnd(std::vector<std::vector<const BasicBlock*>> &Loopset,std::vector<int> retpaths){
	errs() <<"addtoend\n";
	for (unsigned i = 0, e = Loopset.size(); i != e; ++i){
		updateArray(const_cast<BasicBlock*>(Loopset[i][1]),i,retpaths[i]);
    }
}
 
char BasicBlocksDemo::ID = 0;
static RegisterPass<BasicBlocksDemo> X("pathProfiling", "BasicBlocksDemo Pass", false, false);
