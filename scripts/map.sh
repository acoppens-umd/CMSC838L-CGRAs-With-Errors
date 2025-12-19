#!/bin/sh

cd $this_eval/mapping

rm $2*.bin

rm $2*.csv

rm $2*.log

rm $2*mappingwithlatency.txt

rm ../simulation/*.bin
rm ../simulation/*_mem_alloc.txt

JSON_ARCH=hycube_original.json
JSON_ARCH=$mapper_dir/json_arch/rust/$JSON_ARCH
BANK_SIZE=8192
NUM_BANKS=2

python $mapper_dir/update_mem_alloc.py $JSON_ARCH $2_mem_alloc.txt $BANK_SIZE $NUM_BANKS $JSON_ARCH

export ARCH=""
export PRINT_BIN=""

$mapper_dir/build/src/cgra_xml_mapper -d $2_PartPredDFG.xml \
    -x 4 -y 4 \
    -j $JSON_ARCH \
    -i 0 -t HyCUBE_4REG -m 0

cp *.bin ../simulation
cp *_mem_alloc.txt ../simulation

cd $top_dir