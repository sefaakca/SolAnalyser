#!/bin/bash
for d in *.abi; do

file_name=$(basename "$d" .abi)

java -jar RandomInputGenerator.jar "$file_name".abi $file_name "$file_name"_bytecode.txt "$file_name"_Random_scenario.json

echo "DONE FOR $file_name"
done

