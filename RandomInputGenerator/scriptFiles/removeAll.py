#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
def modify_text(file_name, result_string):
    with open(file_name, "r+") as f:
        f.truncate()   #清空文件
        f.write(result_string)

def removeBinary():
	binary_file = sys.argv[1]
	binary_file_object = open(binary_file)
	binary_file_context = binary_file_object.read()
	
	binary_raw_result = binary_file_context.split("=======")
	
	longest_binary = binary_raw_result[binary_raw_result.index(sorted(binary_raw_result,key=lambda k:len(k),reverse=True)[0])]
	x = longest_binary.strip()
	x = x.lstrip()
	final = x.replace('Binary:', '')
	fianl = x.replace(' ', '')
	result = ''
	for i in final:
		if i.isdigit() or i.isalpha() :
			result = result + i 
	modify_text(binary_file,result) 
	
	
def removeAbi():
	abi_file = sys.argv[2]
	abi_file_object = open(abi_file)
	abi_file_context = abi_file_object.read()
	
	abi_raw_result = abi_file_context.split("=======")
	
	abi_longest = abi_raw_result[abi_raw_result.index(sorted(abi_raw_result,key=lambda k:len(k),reverse=True)[0])]
	y = abi_longest.strip()
	y = y.lstrip()
	abi_final = y.replace('Contract JSON ABI', '')
	abi_result = ''
	abi_result = abi_result + abi_final
	modify_text(abi_file,abi_result)
	
	
def removeOpcode():
	opcode_file_name = sys.argv[3]
	opcode_file_object = open(opcode_file_name)
	opcode_file_context = opcode_file_object.read()
	
	opcode_raw_result = opcode_file_context.split("=======")
	opcode_longest = opcode_raw_result[opcode_raw_result.index(sorted(opcode_raw_result,key=lambda k:len(k),reverse=True)[0])]
	z = opcode_longest.strip()
	z = z.lstrip()
	opcode_final = z.replace('Opcodes:', '')
	opcode_result = ''
	opcode_result = opcode_result + opcode_final
	modify_text(opcode_file_name,opcode_result) 
	
removeBinary()
removeAbi()
removeOpcode()
