#!/bin/sh

cd $this_eval/dfggen

rustup override set 1.69.0

rm ${KERNEL_FILE}*.ll

rm ${FUNCTION_NAME}*.dot
rm ${FUNCTION_NAME}*.pdf

rm *.o

rm final.ll
rm final

rm ${FUNCTION_NAME}_panic_codes.txt

echo ">>>>COMPILING RUST TO LLVM IR<<<<"

rustc ${KERNEL_FILE}.rs \
  --crate-type=lib \
  -C panic=abort \
  -C opt-level=0 \
  --emit=llvm-ir \
  -o ${KERNEL_FILE}.ll

echo ">>>>OPTIMIZING LLVM IR<<<<"

opt -passes="default<O2>" -disable-loop-unrolling ${KERNEL_FILE}.ll -S -o ${KERNEL_FILE}_opt.ll

echo ">>>>REWORKING UNSUPPORTED INTRINSICS<<<<"

opt -load-pass-plugin $dfg_generator_dir/build/src/libRemoveIntrinsics.so -passes=lower-umin-umax ${KERNEL_FILE}_opt.ll -S -o ${KERNEL_FILE}_min_max_opt.ll

echo ">>>>PRODUCING DFG<<<<"

opt -load $dfg_generator_dir/build/src/libdfggenPass.so -fn $FUNCTION_NAME -nobanks 2 -banksize 8192 -type $dfg_strategy -enable-new-pm=0 \
    -dfggen ${KERNEL_FILE}_min_max_opt.ll -S -o ${KERNEL_FILE}_opt_instrument.ll

echo ">>>>PRODUCING PDF<<<<"

dot -Tpdf ${FUNCTION_NAME}_PartPredDFG.dot -o ${FUNCTION_NAME}_PartPredDFG.pdf

echo ">>>>PRODUCING instrumentation.o<<<<"

clang++ -c $dfg_generator_dir/src/instrumentation/instrumentation.cpp \
  -O0 \
  -fno-exceptions \
  -fno-rtti \
  -o instrumentation.o

echo ">>>>LOWERING INSTRUMENTED IR TO OBJECT<<<<"

llc ${KERNEL_FILE}_opt_instrument.ll \
  -relocation-model=pic \
  -filetype=obj \
  -o ${KERNEL_FILE}_opt_instrument.o

echo ">>>>COMPILING INSTRUMENTED EXECUTABLE<<<<"

rustc $dfg_generator_dir/src/rust/driver.rs \
  -C panic=abort \
  -C linker=clang++ \
  -C link-arg=${KERNEL_FILE}_opt_instrument.o \
  -C link-arg=instrumentation.o \
  -C link-arg=-lstdc++ \
  -o final

echo ">>>>RUNNING INSTRUMENTED EXECUTABLE<<<<"

mkdir ./memtraces

./final

cp ${FUNCTION_NAME}_mem_alloc.txt ../mapping/${FUNCTION_NAME}_mem_alloc.txt
cp ${FUNCTION_NAME}_*DFG.xml ../mapping/
cp ${FUNCTION_NAME}_panic_codes.txt ../simulation/${FUNCTION_NAME}_panic_codes.txt

cd $top_dir