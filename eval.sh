#!/bin/sh

#$1: kernel name (for example csr_spmv for kernels/src/bin/csr_spmv.rs)
#$2: function name (for example csr_spmv for csr_spmv in csr_spmv.rs)
#$3: mode (dfggen, map, sim)

for i in "$@"; do
  case $i in
    -k=*|--kernel_file=*)
      KERNEL_FILE="${i#*=}"
      shift # past argument=value
      ;;
    -f=*|--function_name=*)
      FUNCTION_NAME="${i#*=}"
      shift # past argument=value
      ;;
    -m=*|--mode=*)
      MODE="${i#*=}"
      shift # past argument=value
      ;;
    -a=*|--arch=*)
      ARCH="${i#*=}"
      shift # past argument=value
      ;;
    -x=*)
      ARCH_X="${i#*=}"
      shift # past argument=value
      ;;
    -y=*)
      ARCH_Y="${i#*=}"
      shift # past argument=value
      ;;
    --minii=*)
      MIN_II="${i#*=}"
      shift # past argument=value
      ;;
    -*|--*)
      echo "Unknown option $i"
      exit 1
      ;;
    *)
      ;;
  esac
done

top_dir=$PWD

evaluation_runs_dir=$top_dir/evaluation_runs

mkdir -p $evaluation_runs

dfg_generator_dir=$top_dir/dfg_generator
mapper_dir=$top_dir/mapper
cppsimulator_dir=$top_dir/cppsimulator

this_eval=$evaluation_runs_dir/$KERNEL_FILE
mkdir -p $this_eval
mkdir -p $this_eval/dfggen
mkdir -p $this_eval/mapping
mkdir -p $this_eval/simulation

dfg_strategy=PartPred

cp ./kernels/src/bin/${KERNEL_FILE}.rs $this_eval/dfggen

if [[ -z "${MODE:-}" || "$MODE" = "dfggen" ]]; then
    source ./scripts/dfggen.sh
fi

if [[ -z "${MODE:-}" || "$MODE" = "map" ]]; then
    source ./scripts/map.sh
fi

if [[ -z "${MODE:-}" || "$MODE" = "sim" ]]; then
    source ./scripts/sim.sh
fi
