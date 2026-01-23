#!/bin/sh

cd $this_eval/mapping

rm ${FUNCTION_NAME}*.bin

rm ${FUNCTION_NAME}*.csv

rm ${FUNCTION_NAME}*.log

rm ${FUNCTION_NAME}*mappingwithlatency.txt

rm ../simulation/*.bin
rm ../simulation/*_mem_alloc.txt

JSON_ARCH=hycube_original.json
if [[ -n "$ARCH" ]]; then
    JSON_ARCH=$ARCH
fi
JSON_ARCH=$mapper_dir/json_arch/$JSON_ARCH
BANK_SIZE=8192
NUM_BANKS=2

python3 $mapper_dir/update_mem_alloc.py $JSON_ARCH ${FUNCTION_NAME}_mem_alloc.txt $BANK_SIZE $NUM_BANKS $JSON_ARCH

export ARCH=""
export PRINT_BIN=""

minII=0

if [[ -n "$MIN_II" ]]; then
    minII=$MIN_II
fi

X=4
if [[ -n "$ARCH_X" ]]; then
    X=$ARCH_X
fi

Y=4
if [[ -n "$ARCH_Y" ]]; then
    Y=$ARCH_Y
fi

$mapper_dir/build/src/cgra_xml_mapper -d ${FUNCTION_NAME}_*DFG.xml \
    -x $X -y $Y \
    -j $JSON_ARCH \
    -i $minII -t HyCUBE_4REG -m 0

cp *.bin ../simulation
cp *_mem_alloc.txt ../simulation

cd $top_dir