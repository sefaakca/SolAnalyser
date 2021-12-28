## Input Generator

I built an automated input generation tool for smart contracts that provides inputs for all transactions and functions in a contract.



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

