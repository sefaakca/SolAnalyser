#MuContract

MuContract is implemented on top of SIF with abilities to insert vulnerabilities to the original Solidity code.

## How to build

Building the project requires CMake. Instructions of building:
```bash
mkdir build && cd build
cmake ..
make
```
After building the project, the executable is built under the folder build/soli

## How to use
```bash
./soli contract.ast contract.json contract.sol type_of_vulnerability [output_filename.sol]
```
This command requires the text-format as well as the JSON-format abstract syntax tree and the filename of the original Solidity code. It will output the new Solidity code to the output_filename.sol if it is provided or to the standard output.

type_of_vulnerability can be one of the following:

- overflow 
- underflow
- divbyzero
- timestampusage
- txorigin
- unchecked send
- repetitive call

You need the [Solidity compiler](https://github.com/ethereum/solidity/releases/tag/v0.4.25) to generate the AST.

To generate text-format AST:
```bash
solc --ast contract.sol >> contract.ast
```

To generate JSON-format AST:
```bash
solc --ast-compact-json contract.sol >> contract.json
```

