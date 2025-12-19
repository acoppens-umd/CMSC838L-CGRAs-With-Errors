#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

class LowerUMinUMaxPass : public PassInfoMixin<LowerUMinUMaxPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    bool Changed = false;

    SmallVector<CallInst *, 8> ToErase;

    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        auto *CI = dyn_cast<CallInst>(&I);
        if (!CI)
          continue;

        Function *Callee = CI->getCalledFunction();
        if (!Callee || !Callee->isIntrinsic())
          continue;

        Intrinsic::ID IID = Callee->getIntrinsicID();
        if (IID != Intrinsic::umin && IID != Intrinsic::umax)
          continue;

        Value *A = CI->getArgOperand(0);
        Value *B = CI->getArgOperand(1);

        IRBuilder<> Builder(CI);

        // Create comparison
        Value *Cmp = nullptr;
        if (IID == Intrinsic::umin)
          Cmp = Builder.CreateICmpULT(A, B);
        else
          Cmp = Builder.CreateICmpUGT(A, B);

        // Create select
        Value *Sel = Builder.CreateSelect(Cmp, A, B);

        CI->replaceAllUsesWith(Sel);
        ToErase.push_back(CI);
        Changed = true;
      }
    }

    for (CallInst *CI : ToErase)
      CI->eraseFromParent();

    return Changed ? PreservedAnalyses::none()
                   : PreservedAnalyses::all();
  }
};

} // end anonymous namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
      LLVM_PLUGIN_API_VERSION,
      "lower-umin-umax",
      LLVM_VERSION_STRING,
      [](PassBuilder &PB) {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM,
               ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "lower-umin-umax") {
                FPM.addPass(LowerUMinUMaxPass());
                return true;
              }
              return false;
            });
      }};
}