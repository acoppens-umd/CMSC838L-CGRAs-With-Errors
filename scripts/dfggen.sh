#!/bin/sh

cd $this_eval/dfggen

rustup override set 1.69.0

rm $1*.ll

rm $2*.dot
rm $2*.pdf

rm *.o

rm final.ll
rm final

rm $2_panic_codes.txt

echo ">>>>COMPILING RUST TO LLVM IR<<<<"

rustc $1.rs \
  --crate-type=lib \
  -C panic=abort \
  -C opt-level=0 \
  --emit=llvm-ir \
  -o $1.ll

echo ">>>>OPTIMIZING LLVM IR<<<<"

opt -passes="default<O2>" -disable-loop-unrolling $1.ll -S -o $1_opt.ll

echo ">>>>REWORKING UNSUPPORTED INTRINSICS<<<<"

opt -load-pass-plugin $dfg_generator_dir/build/src/libRemoveIntrinsics.so -passes=lower-umin-umax $1_opt.ll -S -o $1_min_max_opt.ll

echo ">>>>PRODUCING DFG<<<<"

opt -load $dfg_generator_dir/build/src/libdfggenPass.so -fn $2 -nobanks 2 -banksize 8192 -type $dfg_strategy -enable-new-pm=0 -dfggen $1_min_max_opt.ll -S -o $1_opt_instrument.ll

echo ">>>>PRODUCING PDF<<<<"

dot -Tpdf ${2}_PartPredDFG.dot -o ${2}_PartPredDFG.pdf

echo ">>>>PRODUCING instrumentation.o<<<<"

clang++ -c $dfg_generator_dir/src/instrumentation/instrumentation.cpp \
  -O0 \
  -fno-exceptions \
  -fno-rtti \
  -o instrumentation.o

echo ">>>>LOWERING INSTRUMENTED IR TO OBJECT<<<<"

llc $1_opt_instrument.ll \
  -relocation-model=pic \
  -filetype=obj \
  -o $1_opt_instrument.o

echo ">>>>COMPILING INSTRUMENTED EXECUTABLE<<<<"

rustc ../../driver.rs \
  -C panic=abort \
  -C linker=clang++ \
  -C link-arg=$1_opt_instrument.o \
  -C link-arg=instrumentation.o \
  -C link-arg=-lstdc++ \
  -o final

echo ">>>>RUNNING INSTRUMENTED EXECUTABLE<<<<"

mkdir ./memtraces

./final

cp $2_mem_alloc.txt ../mapping/$2_mem_alloc.txt
cp ${2}_*DFG.xml ../mapping/
cp $2_panic_codes.txt ../simulation/$2_panic_codes.txt

cd $top_dir