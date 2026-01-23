#!/bin/sh

cd $this_eval/simulation

rm sim_result.txt
rm mismatches*.txt

for trace_file in ../dfggen/memtraces/*.txt; do
    echo "Running on $trace_file"
    $cppsimulator_dir/src/build/hycube_simulator -c ${FUNCTION_NAME}*.bin -d $trace_file -a ${FUNCTION_NAME}_mem_alloc.txt -m 16384 -p ${FUNCTION_NAME}_panic_codes.txt \
                                                 -x ${X} -y ${Y}
    base_trace_file=$(basename "$trace_file")
    mv ./mismatches.txt ./mismatches_$base_trace_file
done

cd $top_dir