#!/bin/sh

#$1: kernel name (for example csr_spmv for kernels/src/bin/csr_spmv.rs)
#$2: function name (for example csr_spmv for csr_spmv in csr_spmv.rs)
#$3: mode (dfggen, map, sim)

top_dir=$PWD

evaluation_runs_dir=$top_dir/evaluation_runs

mkdir -p $evaluation_runs

dfg_generator_dir=$top_dir/dfg_generator
mapper_dir=$top_dir/mapper
cppsimulator_dir=$top_dir/cppsimulator

this_eval=$evaluation_runs_dir/$1
mkdir -p $this_eval
mkdir -p $this_eval/dfggen
mkdir -p $this_eval/mapping
mkdir -p $this_eval/simulation

dfg_strategy=PartPred

cp ./kernels/src/bin/$1.rs $this_eval/dfggen

if [[ -z "${3:-}" || "$3" = "dfggen" ]]; then
    source ./scripts/dfggen.sh
fi

if [[ -z "${3:-}" || "$3" = "map" ]]; then
    source ./scripts/map.sh
fi

if [[ -z "${3:-}" || "$3" = "sim" ]]; then
    source ./scripts/sim.sh
fi
