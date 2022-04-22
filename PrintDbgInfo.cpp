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
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include <bits/stdc++.h>
// #include "llvm/IR/DebugInfoMetadata.h"
// #include "llvm/Support/type_traits.h"

using namespace llvm;

namespace {
  
  struct PrintDbgInfo : public ModulePass{
    static char ID; // Pass identification, replacement for typeid
    PrintDbgInfo() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
        /* Gather all built-in functions
     --------------------- */
    const TargetLibraryInfo *TLI;
    LibFunc inbuilt_func;
    std::set<StringRef> builtins;
    for (auto &F : M)
        if (TLI->getLibFunc(F, inbuilt_func))
            builtins.insert(F.getFunction().getName());

      Function *t;
      for (auto &F : M){       
      for (BasicBlock &BB : F) {
          /// errs() << "hello world???\n" ;
          for (Instruction &I: BB) {
            if (isa<CallInst>(I)) {
              t = cast<CallInst>(I).getCalledFunction();
              if (t)  {
                StringRef name = t->getName();
                if (builtins.count(name)>0) { // not a func
                  errs() << name.str()  << "\n";
                }
              }
            }
         }
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
Y("PrintDbgInfo", "print lines of instructions");
