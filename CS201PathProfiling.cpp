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
class TopoSorter {
public:
  enum Color {WHITE, BLACK};
  // Color marks per vertex (BB).
  typedef DenseMap<const BasicBlock *, Color> BBColorMap;
  BBColorMap ColorMap;
  typedef std::vector<std::vector<const BasicBlock *>> BBVector;
  BBVector SortedBBs;

  void runToposort(std::vector<std::vector<const BasicBlock *>> &Loopset,std::vector<std::vector<const BasicBlock *>> &topsortBB) {
	for(unsigned i = 0; i < Loopset.size();++i){
		for (unsigned m = 0, n = Loopset[i].size(); m != n; ++m){
			    errs() << "Loopset of "<<m<<" " << llvm::BlockAddress::get(const_cast<BasicBlock*>(Loopset[i][m])) << ":\n";

    		  ColorMap[Loopset[i][m]] = TopoSorter::WHITE;		
		}
	    errs() << "Topological sort of " << Loopset[i][0] << ":\n";
//change
		DFSTopsort(Loopset[i][1],i,topsortBB,Loopset[i][0]);
		      
	}	
  }
  
  std::vector<const BasicBlock *> Stack;  
	void DFSTopsort(const BasicBlock* BB, int i, std::vector<std::vector<const BasicBlock *>> &topsortBB,const BasicBlock* end){
		ColorMap[BB] = TopoSorter::BLACK;
		errs() << "between\n";
		topsortBB[i].push_back(BB);
		//push bb pred on stack
		errs() << "loop pred entry node\n";
		for (const_pred_iterator PI = pred_begin(BB), E = pred_end(BB); PI != E; ++PI) {
          const BasicBlock *P = *PI;
		  errs() << llvm::BlockAddress::get(const_cast<BasicBlock*>(P))<<"\n";
          Stack.push_back(P);
        }
		errs() << "before while\n";
		while(!Stack.empty()){

	        const BasicBlock *temp = Stack.back();
			if(temp == end){
                Stack.pop_back();
                topsortBB[i].insert(topsortBB[i].begin(),temp);
				break;
            }
			
			bool push = true;
			for(succ_const_iterator SI = succ_begin(temp), F = succ_end(temp); SI !=F;++SI){
				const BasicBlock *s = *SI;
				if(ColorMap[s] == TopoSorter::WHITE){
				//bad
					push = false;
					break;
				}
			}
			if(push){
				ColorMap[temp] = TopoSorter::BLACK;
				topsortBB[i].push_back(temp);
				Stack.pop_back();
				for (const_pred_iterator PI = pred_begin(temp), E = pred_end(temp); PI != E; ++PI) {
          			const BasicBlock *P = *PI;
		            Stack.push_back(P);
					if(P == end){
						Stack.pop_back();
						topsortBB[i].insert(topsortBB[i].begin(),P);
					}
		        }
			}
			else{
				Stack.pop_back();
			}
         }
		
        
      }
		

  // Helper function to recursively run topological sort from a given BB.
  // Returns true if the sort succeeded and false otherwise; topological sort
  // may fail if, for example, the graph is not a DAG (detected a cycle).
  bool recursiveDFSToposort(const BasicBlock *BB, unsigned i) {
//    ColorMap[BB] = TopoSorter::GREY;
    // For demonstration, using the lowest-level APIs here. A BB's successors
    // are determined by looking at its terminator instruction.
    const TerminatorInst *TInst = BB->getTerminator();
	errs() << "before loop\n";
	if(TInst->getNumSuccessors() > 1 && !isa<IndirectBrInst>(TInst))
    for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
		
      BasicBlock *Succ = TInst->getSuccessor(I);
//      Color SuccColor = ColorMap[Succ];
		errs() << "\nblk: "<< llvm::BlockAddress::get(const_cast<BasicBlock*>(Succ))<<"\n";
//      if (SuccColor == TopoSorter::WHITE) {
//        if (!recursiveDFSToposort(Succ,i))
//          return false;
//      } else if (SuccColor == TopoSorter::GREY) {
        // This detects a cycle because grey vertices are all ancestors of the
        // currently explored vertex (in other words, they're "on the stack").
//        outs() << " Detected cycle: edge from " << BB->getName() <<
//                  " to " << Succ->getName() << "\n";
//        return true;
//      }
//    }
//	else
//		return true;
    // This BB is finished (fully explored), so we can add it to the vector.
//    ColorMap[BB] = TopoSorter::BLACK;
    SortedBBs[i].push_back(BB);
	}
    return true;
  }
};


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

	struct BasicBlocksDemo : public FunctionPass ,public TopoSorter{
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
			errs() << "\n---------Starting PathProfiling---------\n";
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
		errs() << "-------Finished PathProfiling----------\n";
		 
		return false;
		}
		//----------------------------------
		bool runOnFunction(Function &F) override {
		SmallVector<std::pair<const BasicBlock*,const BasicBlock*>,32> BackEdgesSet;

		errs() << "Function: " << F.getName() << '\n';
		FindFunctionBackEdges(F,BackEdgesSet);

		for (unsigned i = 0, e = BackEdgesSet.size(); i != e; ++i){
			errs() <<"Backedge: "<< llvm::BlockAddress::get(const_cast<BasicBlock*>(BackEdgesSet[i].first)) << " to " << llvm::BlockAddress::get(const_cast<BasicBlock*>(BackEdgesSet[i].second))<<"\n";
		}


		std::vector<std::vector<const BasicBlock*>>LoopSet(BackEdgesSet.size());
		LoopConstruction(BackEdgesSet,LoopSet);	

		for(auto &BB: F) 
			errs() <<"BasicBlock: "<< llvm::BlockAddress::get(const_cast<BasicBlock*>(&BB))<<"\n"<< BB << "\n";

		errs() << "Innermost Loops:\n";
		for (unsigned i = 0, e = LoopSet.size(); i != e; ++i){
			errs() <<"Loop"<<i<<" {";
			errs() <<llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][0]));
		  for (unsigned m = LoopSet[i].size()-1, n = 1; m >= n; --m){
			errs() << ", "<<llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m]));
		  }
			errs() << "}\n";
		}
/*
		errs() << "unsort\n";
		for(unsigned i = 0, e = LoopSet.size(); i != e; ++i){
			for (unsigned m = 0, n = LoopSet[i].size(); m != n; ++m){
            errs() << ", "<<llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m]));
          }
            errs() << "}\n";
        }
*/

		std::vector<int> retpaths;

		if(!LoopSet.empty()){
			Innermost_Loop(LoopSet);
			std::vector<std::vector<const BasicBlock *>> toposortloopset(LoopSet.size());
		errs() << "runtoposort\n";
		errs() << "size " << toposortloopset.size()<<"\n";	

			runToposort(LoopSet,toposortloopset);

		errs() << "sort?\n";		
		errs() << "size " << toposortloopset.size();	
        for(unsigned i = 0; i< toposortloopset.size(); ++i){
			errs() << "firstloop \n";
            for (unsigned m = 0, n = toposortloopset[i].size(); m != n; ++m){
	            errs() << " , "<<llvm::BlockAddress::get(const_cast<BasicBlock*>(toposortloopset[i][m]));
          } errs() << "\n";
        }



			std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> edges;
			computeEdgeWeights(toposortloopset, edges, retpaths);
			//errs() << "num paths " << retpaths[0] << "\n";
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

for(auto &BB: F)
            errs() <<"BasicBlock: "<< llvm::BlockAddress::get(const_cast<BasicBlock*>(&BB))<<"\n"<< BB << "\n";



    errs() << "after code:\n";
        for (unsigned i = 0, e = LoopSet.size(); i != e; ++i){
            errs() <<"Loop"<<i<<" {";
            errs() <<llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][0]));
          for (unsigned m = LoopSet[i].size()-1, n = 1; m >= n; --m){
            errs() << ", "<<llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][m]));
          }
            errs() << "}\n";
        }


		}
		//runOnBasicBlock(BB);
		}

		}


		 
		return true; // since runOnBasicBlock has modified the program
		}
		 
		//----------------------------------
		// Rest of this code is needed to: printf("%d\n", bbCounter); to the end of main, just BEFORE the return statement
		// For this, prepare the SCCGraph, and append to last BB?
		void addFinalPrintf(BasicBlock& BB, LLVMContext *Context, /*GlobalVariable *var,*/ Function *printf_func,std::vector<std::vector<const BasicBlock *>> &LoopSet,std::vector<int> &retpaths) {
		IRBuilder<> IRB(BB.getTerminator()); // Insert BEFORE the final statement
		for(unsigned long i =0;i<LoopSet.size();i++){
			std::stringstream s;
			s << llvm::BlockAddress::get(const_cast<BasicBlock*>(LoopSet[i][0]));
			//const char* finalstr = s.str().c_str();
			std::string ss = "Path_";
			ss.append(s.str());
			ss.append("_");

			//2nd for for inner loop
			//loop over retpaths[i]
			for(int j = 0; j<retpaths[i];j++){
				std::string a = ss + std::to_string(j);
			 a =  a + ": %d\n";
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
	    		//load from array
			    LoadInst* oldpc = new LoadInst(pcpointer,"oldpc",BB.getTerminator());

        		std::vector<Constant*> indices;
		        Constant *zero = Constant::getNullValue(IntegerType::getInt32Ty(*Context));
        		indices.push_back(zero);
		        indices.push_back(zero);
        		Constant *var_ref = ConstantExpr::getGetElementPtr(BasicBlockPrintfFormatStr, indices);
				CallInst *call = IRB.CreateCall2(printf_func, var_ref, oldpc);
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
	  if(std::find(LoopSet[i].begin(),LoopSet[i].end(),BackEdgesSet[i].first)==LoopSet[i].end()){
	      LoopSet[i].push_back(BackEdgesSet[i].first);
	      Stack.push_back(BackEdgesSet[i].first);
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
			errs() << "Edge values: {";
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
						/*
						errs() << "edge: ";
						errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (node));
						errs() << " to ";
						errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc));
						errs() << " wegith ";
						errs() << numpaths[node];
						errs() << "\n";
						*/
						errs() << "(" << llvm::BlockAddress::get(const_cast<BasicBlock*> (node)) << "," << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc)) 
							<< ", "<<numpaths[node]<<"),";
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
					/*
					errs() << "edge: ";
					errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (start));
					errs() << " to ";
					errs() << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc));
					errs() << " wegith ";
					errs() << numpaths[start];
					errs() << "\n";
					*/
					errs() << "("<< llvm::BlockAddress::get(const_cast<BasicBlock*> (start)) << "," << llvm::BlockAddress::get(const_cast<BasicBlock*> (suc))
                    << ", "<< numpaths[start]<<"),";
					numpaths[start] += numpaths[suc];
					
				}
				else{
					//errs() << "an edge outside of loop\n";
				}
				
			}
			errs() << "}\n";

			
			
			retpaths.push_back(numpaths[start]);
		}	
	}

void BasicBlocksDemo::Initcounter(std::vector<std::vector<const BasicBlock*>> &Loopset){
    for (unsigned i = 0, e = Loopset.size(); i != e; ++i){
	IRBuilder<> IRB(const_cast<BasicBlock *>(Loopset[i][0])->getFirstInsertionPt());
	Value *addAddr = IRB.CreateAdd(ConstantInt::get(Type::getInt32Ty(*Context), 0), ConstantInt::get(Type::getInt32Ty(*Context), 0));
	IRB.CreateStore(addAddr, bbCounter);
	//errs()<< *(Loopset[i][0]) << "\n";
    }
	
}
	
void BasicBlocksDemo::InsertBasicBlock(std::vector<std::vector<const BasicBlock*>> &Loopset, std::map<std::pair<const BasicBlock*,const BasicBlock*>,int> &edges, Function &F){
    
    for(auto &I : edges){
	std::pair<const BasicBlock*,const BasicBlock*> BBpair = I.first;
	int edge = I.second;
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
	for (unsigned i = 0, e = Loopset.size(); i != e; ++i){
		updateArray(const_cast<BasicBlock*>(Loopset[i][1]),i,retpaths[i]);
    }
}

 
char BasicBlocksDemo::ID = 0;
static RegisterPass<BasicBlocksDemo> X("pathProfiling", "BasicBlocksDemo Pass", false, false);
