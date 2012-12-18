//===- PrintDbgInfo.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file prints out the source code info of loops and the loop_ids
// 
//
//===----------------------------------------------------------------------===//

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/DebugInfo.h"
#include "llvm/Support/type_traits.h"
#include "llvm/ADT/StringRef.h"

using namespace llvm;

StringRef * loopCond= new StringRef ("for.cond");
int loop_count = 0;

namespace {
  
  struct PrintDbgInfo : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    PrintDbgInfo() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
	if (F.isDeclaration())
		     return false;
	for (Function::iterator i = F.begin(), e = F.end(); i != e; ++i)
	{
		
		if(!((i->getName().find(*loopCond,0))))  //the first basic block of for loop
		{
			loop_count++;
			for (BasicBlock::const_iterator I = i->begin(), E = i->end();
		        	  I != E; ++I) {

				//get the line info for the first instruction of the basic block
				if (MDNode *N = I->getMetadata("dbg")) {  // Here I is an LLVM instruction
					DILocation Loc(N);                      // DILocation is in DebugInfo.h
					unsigned Line = Loc.getLineNumber();
					StringRef File = Loc.getFilename();
					//StringRef Dir = Loc.getDirectory();
					errs()<<loop_count<<" "<<Line<<" "<<File<<"\n";
					break;
				}//end of if
			}//end of for
		}//end of if the first basic block of loop
	}
      return false;
    }

    // We don't modify the program, so we preserve all analyses
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
    }
  };
}

char PrintDbgInfo::ID = 0;
static RegisterPass<PrintDbgInfo>
Y("printDbgInfo", "print lines of instructions");
