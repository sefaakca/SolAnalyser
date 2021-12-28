## Input Generator

We automatically generate inputs for instrumented smart contracts. First, we use Solidity compiler to generate Application binary interface (ABI) and bytecode files. Then, using these two files, we generate inputs for smart contracts. Our input generator, implemented in Java,  supports all Solidity types such as signed/unsigned integers types with widths ranging from 8 to 256. The generated inputs call each of the functions in the smart contract at least once.


## How to use

java -jar ABIAnalyser.jar $abi_file.abi $name_of_the_contract $bytecode_file.txt $output_file.json



You need the [Solidity compiler](https://github.com/ethereum/solidity/releases/) to generate the abi and bytecode file.

To generate Bytecode file:
```bash
solc --bin contract.sol >> bytecode_file.txt

```

To generate ABI file:
```bash
solc --abi contract.sol >> abi_file.abi
``` 

