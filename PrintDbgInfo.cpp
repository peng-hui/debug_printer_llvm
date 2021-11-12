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
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/DebugInfo.h"
// #include "llvm/IR/DebugInfoMetadata.h"
// #include "llvm/Support/type_traits.h"

using namespace llvm;

namespace {
  
  struct PrintDbgInfo : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    PrintDbgInfo() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
	if (F.isDeclaration())
		     return false;
        for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            const DILocation *DIL = I->getDebugLoc();
            if (!DIL)
                continue;
            errs() << DIL->getFilename()<< " ==  " << DIL->getLine() <<"\n";
        }

        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                const DILocation *DIL = I.getDebugLoc();
                if (!DIL)
                    continue;
                errs() << DIL->getFilename()<< " --  " << DIL->getLine() <<"\n";
            }
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
