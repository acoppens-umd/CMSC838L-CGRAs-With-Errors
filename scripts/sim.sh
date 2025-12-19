#!/bin/sh

cd $this_eval/simulation

rm sim_result.txt
rm mismatches*.txt

for trace_file in ../dfggen/memtraces/*.txt; do
    echo "Running on $trace_file"
    $cppsimulator_dir/src/build/hycube_simulator -c $2*.bin -d $trace_file -a $2_mem_alloc.txt -m 16384 -p $2_panic_codes.txt
    base_trace_file=$(basename "$trace_file")
    mv ./mismatches.txt ./mismatches_$base_trace_file
done

cd $top_dir