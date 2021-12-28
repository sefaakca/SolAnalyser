#!/bin/bash
for d in *.sol ; do
file_name=$(basename "$d" .sol)
abi_file="$file_name".abi
opcode_file="$file_name"_opcode.txt
bytecode_file="$file_name"_bytecode.txt
output_file="$file_name"_scenario.json


rm -f $file_name.json
rm -f $file_name.ast
rm -f $abi_file $opcode_file $bytecode_file

 
export PATH="/usr/bin:$PATH"

solc $file_name.sol --abi >> $abi_file

if [ -s "$abi_file" ]
then
    echo "abi file created :)"
else
    export PATH="/home/s1798183/Documents/tt/solidity_0.4.25/build/solc:$PATH"
    echo "COMPILER 0.4.25 is using for abi"
    solc $file_name.sol --abi >> $abi_file
fi

solc $file_name.sol --bin >> $bytecode_file

if [ -s "$bytecode_file" ]
then
    echo "bytecode file created :)"
else
    export PATH="/home/s1798183/Documents/tt/solidity_0.4.25/build/solc:$PATH"
    echo "COMPILER 0.4.25 is using for bytecode"
    solc $file_name.sol --bin >> $bytecode_file
    
fi

solc $file_name.sol --opcodes >> $opcode_file

if [ -s "$opcode_file" ]
then
    echo "opcode file created :)"
else
    export PATH="/home/s1798183/Documents/tt/solidity_0.4.25/build/solc:$PATH"
    echo "COMPILER 0.4.25 is using for opcode"
    solc $file_name.sol --bin >> $opcode_file
fi


python removeAll.py $bytecode_file $abi_file $opcode_file

echo "DONE FOR $d"
done

