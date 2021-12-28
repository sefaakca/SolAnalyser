#include <fstream>
#include <list>
#include <iostream>
#include <sstream>
#include <string>

#include <iterator>

#include <boost/algorithm/string.hpp>

#include "ASTAnalyser.hpp"

namespace SolInstru{

ASTAnalyser::ASTAnalyser(std::stringstream& _ast_sstream, nlohmann::json& _jsonast, std::string _mutation_type, int _target_function_id, bool single_file, std::string file_name) {
    std::string new_line;
    while (std::getline(_ast_sstream, new_line)) {
        //Utils::trim(new_line);
        if (!new_line.empty()) {
            ast_lines.emplace_back(new_line);
        }
    }

    jsonast = _jsonast;
    ptr_ast_line = ast_lines.begin();
    num_functions_current_contract = 0;

    if (single_file) {
        while (Utils::substr_by_edge(*ptr_ast_line, "======= ", " =======") != file_name) {
            ++ptr_ast_line;
        }
    }
    target_function_id = _target_function_id;
    mutation_type = _mutation_type;
    current_function_id = 0;
}

std::stringstream ASTAnalyser::analyse() {

    std::stringstream result;
    std::string line;
    std::string compiler_version;
    while (ptr_ast_line != ast_lines.end()) {
        std::string keyword = Utils::retrieve_string_element(*ptr_ast_line, 0, " ");
        Utils::debug_info("Contract-level ast line handling: " + *ptr_ast_line);
        if (keyword == TokenPragmaDirective) {
            // visitor function and class yet implemented
            // using original source
            get_next_token(TokenSource);
            compiler_version = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
            line = "pragma solidity >=0.4.10 <0.6.0;";
            remove_escapes(line);
            remove_escapes(compiler_version);
            result << line << "\n";
        } else if (keyword == TokenImportDirective ) {
            // same as TokenPragmaDirective
            // using original source
            get_next_token(TokenSource);
            //line = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
            compiler_version = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
            remove_escapes(compiler_version);
            line = "pragma solidity >=0.4.10 <0.6.0;";
            remove_escapes(line); 
            result << line << "\n";
        } else if (keyword == TokenContractDefinition ) {
            std::string contract_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
            contract_name = Utils::substr_by_edge(contract_name, "\"", "\"");
            ContractDefinitionNodePtr contract(new ContractDefinitionNode(contract_name));
            contract->vulnerability_type= mutation_type;
            contract->compiler_version = compiler_version;
            contracts.push_back(contract);
            current_contract = contract;
            num_functions_current_contract = 0;
            current_contract_name = contract_name;
            get_next_token(TokenSource);
            line = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
            if (line.find("library") == 0) {
                current_contract->set_as_library();
            }
            if(line.find("interface") == 0) {
                current_contract->set_as_interface();
            }
        } else if (keyword == TokenInheritanceSpecifier ) {
            get_next_token(TokenUserDefinedTypeName);
            line = Utils::substr_by_edge(*ptr_ast_line, TokenUserDefinedTypeName + " \"", "\"");
            current_contract->add_inherit_from(line);
        } else if (keyword == TokenUsingForDirective ) {
            get_next_token(TokenUserDefinedTypeName);
            std::string using_A = Utils::substr_by_edge(*ptr_ast_line, TokenUserDefinedTypeName + " \"", "\""), for_B = "*";
            if (get_next_token() == TokenUserDefinedTypeName) {
                for_B = Utils::substr_by_edge(*ptr_ast_line, TokenUserDefinedTypeName + " \"", "\"");
            } else {
                --ptr_ast_line; // next token is a new node, skipping increasing ast node iterator
            }
            auto using_for = std::static_pointer_cast<ASTNode>(std::make_shared<UsingForDirectiveNode>(using_A, for_B));
            current_contract->append_subnode(using_for);
        } else if (keyword == TokenStructDefinition ) {
            std::string struct_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
            struct_name = Utils::substr_by_edge(struct_name, "\"", "\"");
            StructDefinitionNodePtr struct_node = std::make_shared<StructDefinitionNode>(struct_name);
            int indentation = get_current_indentation();

            while (get_next_token() == TokenVariableDeclaration) {
                if (get_current_indentation() <= indentation) break;
                auto ast_var_decl = std::static_pointer_cast<ASTNode>(handle_variable_declaration());
                struct_node->append_subnode(ast_var_decl);
            } 
            auto ast_struct_node = std::static_pointer_cast<ASTNode>(struct_node);
            current_contract->append_subnode(ast_struct_node);
            continue;

        } else if (keyword == TokenEnumDefinition ) {
            EnumDefinitionNodePtr enum_def = handle_enum_definition();
            current_contract->append_subnode(enum_def);
        } else if (keyword == TokenEnumValue ) {
            // not handled at top level
        } else if (keyword == TokenParameterList ) {
            // not handled at top level
        } else if (keyword == TokenFunctionDefinition ) {
            FunctionDefinitionNodePtr function_node = std::make_shared<FunctionDefinitionNode>();
            std::string function_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
            function_name = Utils::substr_by_edge(function_name, "\"", "\"");
            function_node->set_name(function_name);

            std::string qualifier = get_function_qualifier(current_contract_name, function_name, "visibility");
            std::string stateMutability = get_function_qualifier(current_contract_name, function_name, "stateMutability");

            if (stateMutability != "nonpayable") {
                qualifier = qualifier + " " + stateMutability;
            }
            function_node->set_qualifier(qualifier);

            bool is_constructor = function_is_constructor(current_contract_name, function_name);
            function_node->set_is_constructor(is_constructor);

            get_next_token(TokenParameterList); 
            ParameterListNodePtr params = handle_parameter_list();
            function_node->set_params(params);

            get_next_token(TokenParameterList);
            ParameterListNodePtr returns = handle_parameter_list();
            function_node->set_returns(returns);

            std::string token = get_next_token();
            ModifierInvocationNodePtr modifier_invocation = nullptr;
            while (token == TokenModifierInvocation) {
                modifier_invocation = handle_modifier_invocation();
                function_node->add_modifier_invocation(modifier_invocation);
                token = get_next_token();
            }            
            BlockNodePtr function_body = nullptr;
            if (token == TokenBlock) {
                function_body = handle_block();
                current_function_id++;
                
                if (current_function_id == target_function_id && !current_contract->get_is_Lib() && (stateMutability!="pure" && stateMutability!="view") ) {
                    function_body->set_extra_code(get_extra_code_for_mutation(compiler_version));
                }
            } else {
                --ptr_ast_line;
            }
            function_node->set_function_body(function_body);
            current_contract->append_subnode(function_node);

            num_functions_current_contract++;
        } else if (keyword == TokenVariableDeclaration ) {
            int indentation = get_current_indentation();
            VariableDeclarationNodePtr var_decl = handle_variable_declaration();
            // it might be initialised a value
            std::string token = get_next_token();
            if (indentation < get_current_indentation()) {
                ASTNodePtr value = get_value_equivalent_node(token);
                var_decl->set_initial_value(value);
            } else {
                --ptr_ast_line;
            }
            current_contract->append_subnode(var_decl);
        } else if (keyword == TokenModifierDefinition ) {
            std::string modifier_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
            modifier_name = Utils::substr_by_edge(modifier_name, "\"", "\"");
            get_next_token(TokenParameterList);
            ParameterListNodePtr params = handle_parameter_list();
            if (params->subnodes_size() == 0) params = nullptr;
            std::string token = get_next_token();
            ASTNodePtr body = get_statement_equivalent_node(token);
            ModifierDefinitionNodePtr modifier = std::make_shared<ModifierDefinitionNode>(modifier_name, params, body);
            current_contract->append_subnode(modifier);
        } else if (keyword == TokenModifierInvocation ) {
            // not handled at top level
        } else if (keyword == TokenEventDefinition ) {
            std::string event_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
            event_name = Utils::substr_by_edge(event_name, "\"", "\"");
            EventDefinitionNodePtr event_node = std::make_shared<EventDefinitionNode>(event_name);
            get_next_token(TokenParameterList);
            ParameterListNodePtr para = handle_parameter_list_for_events();
            event_node->append_subnode(para);
            current_contract->append_subnode(event_node);
        } else if (keyword == TokenElementaryTypeName ) {
            // not handled at top level
        } else if (keyword == TokenUserDefinedTypeName ) {
            // not handled at top level
        } else if (keyword == TokenFunctionTypeName ) {
            // not handled at top level
        } else if (keyword == TokenMapping ) {

        } else if (keyword == TokenArrayTypeName ) {
            // not handled at top level
        } else if (keyword == TokenInlineAssembly ) {

        } else if (keyword == TokenBlock ) {
            BlockNodePtr block_node = handle_block();
            current_contract->append_subnode(block_node);
        } else if (keyword == TokenPlaceholderStatement ) {
            // not handled at top level
        } else if (keyword == TokenIfStatement ) {
            // not handled at top level
        } else if (keyword == TokenDoWhileStatement ) {
            // not handled at top level
        } else if (keyword == TokenWhileStatement ) {
            // not handled at top level
        } else if (keyword == TokenForStatement ) {
            // not handled at top level
        } else if (keyword == TokenContinue ) {
            // not handled at top level
        } else if (keyword == TokenBreak ) {
            // not handled at top level
        } else if (keyword == TokenReturn ) {
            // not handled at top level
        } else if (keyword == TokenThrow ) {
            // not handled at top level
        } else if (keyword == TokenEmitStatement ) {
            // not handled at top level
        } else if (keyword == TokenVariableDeclarationStatement ) {
            VariableDeclarationStatementNodePtr var_decl_stmt = handle_variable_declaration_statament();
            current_contract->append_subnode(var_decl_stmt);
        } else if (keyword == TokenExpressionStatement ) {
            // not handled at top level
        } else if (keyword == TokenConditional ) {
            // not handled at top level
        } else if (keyword == TokenAssignment ) {
            // not handled at top level
        } else if (keyword == TokenTupleExpression ) {
            // not handled at top level
        } else if (keyword == TokenUnaryOperation ) {
            // not handled at top level
        } else if (keyword == TokenBinaryOperation ) {
            // not handled at top level
        } else if (keyword == TokenFunctionCall ) {
            // not handled at top level
        } else if (keyword == TokenNewExpression ) {
             // not handled at top level
        } else if (keyword == TokenMemberAccess ) {
            // not handled at top level
        } else if (keyword == TokenIndexAccess ) {
            // not handled at top level
        } else if (keyword == TokenIdentifier ) {
            // not handled at top level
        } else if (keyword == TokenElementaryTypeNameExpression ) {
            // not handled at top level
        } else if (keyword == TokenLiteral ) {
            // not handled at top level
        }
        if (ptr_ast_line == ast_lines.end()) break;
        ++ptr_ast_line;
    }

    Utils::debug_info("File processing finished");

    for (auto it_contract = contracts.begin(); it_contract != contracts.end(); ++it_contract) {
        result << (*it_contract)->source_code();
    }

    Utils::debug_info("New code generated");
    return result;
}

void ASTAnalyser::get_next_token(const std::string& token) {
    if (ptr_ast_line == ast_lines.end()) return;
    ++ptr_ast_line;
    while (ptr_ast_line != ast_lines.end()) {
        std::string line = *ptr_ast_line;
        Utils::trim(line);
        if (Utils::retrieve_string_element(line, 0, " ") == token) {
            return;
        }
        ++ptr_ast_line;
    }
}

std::string ASTAnalyser::get_next_token() {
    if (ptr_ast_line == ast_lines.end()) return "";
    ++ptr_ast_line;
    while (ptr_ast_line != ast_lines.end()) {
        std::string line = *ptr_ast_line;
        Utils::trim(line);
        std::string keyword = Utils::retrieve_string_element(line, 0, " ");
        if (std::find(KeywordsList.begin(), KeywordsList.end(), keyword) != KeywordsList.end()) {
            return keyword;
        }
        ++ptr_ast_line;
    }
    return "";
}

void ASTAnalyser::remove_escapes(std::string& _str){
    std::string double_quote = "\"";
    std::string double_quote_es = "\\\"";
    Utils::str_replace_all(_str, "\\\"", "\"");
    Utils::str_replace_all(_str, "\\\'", "\'");
    Utils::str_replace_all(_str, "\\\\", "\\");
    Utils::str_replace_all(_str, "\\\a", "\"");
    Utils::str_replace_all(_str, "\\\b", "\"");
    Utils::str_replace_all(_str, "\\\f", "\"");
    Utils::str_replace_all(_str, "\\\n", "\"");
    Utils::str_replace_all(_str, "\\\r", "\"");
    Utils::str_replace_all(_str, "\\\t", "\"");
    Utils::str_replace_all(_str, "\\\v", "\"");
    // TO-DO
    /*
    \nnn	arbitrary octal value	byte nnn
    \xnn	arbitrary hexadecimal value	byte nn
    \unnnn (since C++11)	universal character name
    (arbitrary Unicode value);
    may result in several characters	code point U+nnnn
    \Unnnnnnnn (since C++11)	universal character name
    (arbitrary Unicode value);
    may result in several characters	code point U+nnnnnnnn
    */
}



VariableDeclarationNodePtr ASTAnalyser::handle_variable_declaration(){
    // get name from the current token
    std::string variable_name = Utils::substr_by_edge(*ptr_ast_line, TokenVariableDeclaration + " \"", "\"");
    ASTNodePtr type;
    // then try to get type
    std::string next_token = get_next_token();
    type = get_type_name(next_token);
    return std::make_shared<VariableDeclarationNode>(type, variable_name); 
    /*
    if (next_token == TokenElementaryTypeName) {
        ++ptr_ast_line;
        type = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
        return std::make_shared<VariableDeclarationNode>(type, variable_name);
    } else if (next_token == TokenFunctionTypeName) {
        get_next_token(TokenParameterList);
        ParameterListNodePtr parameters = handle_parameter_list();
        get_next_token(TokenParameterList);
        ParameterListNodePtr returns = handle_parameter_list();
        type = "function" + parameters->source_code(); 
        if (returns->subnodes_size()){
            type = type + " returns" + returns->source_code();
        }
        return std::make_shared<VariableDeclarationNode>(type, variable_name);
    } else if (next_token == TokenArrayTypeName) {
        std::string array_base_type_token = get_next_token();
        if (array_base_type_token == TokenElementaryTypeName) {
            ++ptr_ast_line;
            type = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
        } else if (array_base_type_token == TokenUserDefinedTypeName) {
            type = Utils::substr_by_edge(*ptr_ast_line, TokenUserDefinedTypeName + " \"", "\"");
        }
        //try to find out the index of the array
        if (get_next_token() == TokenLiteral) {
            ++ptr_ast_line;
            ++ptr_ast_line;
            type = type + "[" + Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"") + "]";
        } else {
            --ptr_ast_line;
            type = type + "[]";
        }        
        return std::make_shared<VariableDeclarationNode>(type, variable_name);
    }*/


}

ParameterListNodePtr ASTAnalyser::handle_parameter_list() {
    ParameterListNodePtr parameters = std::make_shared<ParameterListNode>();
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    while (token == TokenVariableDeclaration && indentation < get_current_indentation()) {
        VariableDeclarationNodePtr var_decl = handle_variable_declaration();
        
        parameters->append_subnode(var_decl);
        token = get_next_token();
    }
    ptr_ast_line--; // if the token cannot enter while, it is outside the parameter list
    return parameters;
}

ParameterListNodePtr ASTAnalyser::handle_parameter_list_for_events() {
    ParameterListNodePtr parameters = std::make_shared<ParameterListNode>();
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    while (token == TokenVariableDeclaration && indentation < get_current_indentation()) {
        VariableDeclarationNodePtr var_decl = handle_variable_declaration();
        
        parameters->append_subnode(var_decl);
        token = get_next_token();
    }
    ptr_ast_line--; // if the token cannot enter while, it is outside the parameter list
    return parameters;
}

BlockNodePtr ASTAnalyser::handle_block() {
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    BlockNodePtr block = std::make_shared<BlockNode>();
    while (token != "" && ptr_ast_line != ast_lines.end() && indentation < get_current_indentation()) {
        ASTNodePtr block_node_ptr = get_unknown(token);
        block->append_subnode(block_node_ptr);
        /*
        block->append_subnode(get_statement_equivalent_node());
        if (token == TokenExpressionStatement) {
            ExpressionStatementNodePtr expression = handle_expression_statament();
            block->append_subnode(expression);
        } else if (token == TokenEmitStatement) {
            EmitStatementNodePtr emit_stmt = handle_emit_statement();
            block->append_subnode(emit_stmt);
        } else if (token == TokenIfStatement) {
            block->append_subnode(handle_if_statement());
        } else if (token == TokenForStatement) {
            block->append_subnode(handle_for_statement());
        } else if (token == TokenWhileStatement) {
            //block->append_subnode(handle_while_statement());
        } else if (token == TokenDoWhileStatement) {
            //block->append_subnode(handle_do_while_statament());
        } else if (token == TokenVariableDeclarationStatement) {
            VariableDeclarationStatementNodePtr variable_decl_stmt = handle_variable_declaration_statament();
            block->append_subnode(variable_decl_stmt);
        } else if (token == TokenReturn) {
            ReturnNodePtr return_node = handle_return();
            block->append_subnode(return_node);
        }*/
        token = get_next_token();
    }
    ptr_ast_line--;
    return block;
}

ExpressionStatementNodePtr ASTAnalyser::handle_expression_statament() {
    ExpressionStatementNodePtr statement = std::make_shared<ExpressionStatementNode>();
    std::string token = get_next_token();
    statement->append_subnode(get_value_equivalent_node(token));
    return statement;
}

BinaryOperationNodePtr ASTAnalyser::handle_binary_operation() {
    std::string op = Utils::retrieve_string_element(*ptr_ast_line, 3, " ");
    get_next_token(TokenType);
    std::string type_str = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
    ASTNodePtr left_hand, right_hand;
    std::string token = get_next_token();
    left_hand = get_value_equivalent_node(token);

    token = get_next_token();
    right_hand = get_value_equivalent_node(token);

    BinaryOperationNodePtr binary_operation = std::make_shared<BinaryOperationNode>(op, left_hand, right_hand);
    binary_operation->type_str= type_str;
    return binary_operation;
}

UnaryOperationNodePtr ASTAnalyser::handle_unary_operation(){
    std::string operator_loc = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
    bool is_prefix = operator_loc == "(postfix)"? false : true;
    std::string op = Utils::retrieve_string_element(*ptr_ast_line, 2, " ");
    std::string token = get_next_token();
    ASTNodePtr operand = get_value_equivalent_node(token);
    UnaryOperationNodePtr unary_operation = std::make_shared<UnaryOperationNode>(op, operand, is_prefix);
    return unary_operation;
}

LiteralNodePtr ASTAnalyser::handle_literal() {
    get_next_token(TokenSource);
    std::string literal_source = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");

    remove_escapes(literal_source);

    LiteralNodePtr literal = std::make_shared<LiteralNode>(literal_source);
    return literal;
}

TupleExpressionNodePtr ASTAnalyser::handle_tuple_expression() {
    TupleExpressionNodePtr tuple_expression = std::make_shared<TupleExpressionNode>();
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    while (token != "" && indentation < get_current_indentation()) {
        ASTNodePtr node = get_value_equivalent_node(token);
        tuple_expression->append_subnode(node);
        token = get_next_token();
    }
    --ptr_ast_line;
    return tuple_expression;
}

VariableDeclarationStatementNodePtr ASTAnalyser::handle_variable_declaration_statament() {
    int indentation = get_current_indentation();
    get_next_token(TokenVariableDeclaration);
    VariableDeclarationNodePtr decl = handle_variable_declaration();
    std::string token = get_next_token();
    VariableDeclarationStatementNodePtr variable_decl_stmt;
    if (indentation < get_current_indentation()) {
        ASTNodePtr value = get_value_equivalent_node(token);
        variable_decl_stmt = std::make_shared<VariableDeclarationStatementNode>(decl, value);
    } else {
        ASTNodePtr value = nullptr;
        variable_decl_stmt = std::make_shared<VariableDeclarationStatementNode>(decl, value);
    }
    --ptr_ast_line;
    return variable_decl_stmt;
}

IdentifierNodePtr ASTAnalyser::handle_identifier() {
    get_next_token(TokenSource);
    std::string name = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
    return std::make_shared<IdentifierNode>(name);
}

ReturnNodePtr ASTAnalyser::handle_return() {
    int indentation = get_current_indentation();
    std::string token = get_next_token();    
    ReturnNodePtr return_node = std::make_shared<ReturnNode>();
    while (token!= "" && indentation < get_current_indentation()) {
        ASTNodePtr sub_node = get_value_equivalent_node(token);
        return_node->append_subnode(sub_node);
        token = get_next_token();
    }
    --ptr_ast_line;
    return return_node;
}

FunctionCallNodePtr ASTAnalyser::handle_function_call() {
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    ASTNodePtr callee = get_value_equivalent_node(token);
    token = get_next_token();
    FunctionCallNodePtr function_call = std::make_shared<FunctionCallNode>(callee);
    while (token!= "" && indentation < get_current_indentation()) {
        function_call->append_subnode(get_value_equivalent_node(token));
        token = get_next_token();
    }
    --ptr_ast_line;
    return function_call;
}

MemberAccessNodePtr ASTAnalyser::handle_member_access() {
    std::string member = Utils::retrieve_string_element(*ptr_ast_line, 3, " ");
    std::string token = get_next_token();
    ASTNodePtr identifier = get_value_equivalent_node(token);
    return std::make_shared<MemberAccessNode>(identifier, member);
}

EmitStatementNodePtr ASTAnalyser::handle_emit_statement() {
    std::string token = get_next_token();
    ASTNodePtr thing_to_emit = get_value_equivalent_node(token);
    EmitStatementNodePtr emit_stmt = std::make_shared<EmitStatementNode>();
    emit_stmt->append_subnode(thing_to_emit);
    return emit_stmt;
}

ElementaryTypeNameExpressionNodePtr ASTAnalyser::handle_elementary_type_name_expression() {
    get_next_token(TokenSource);
    std::string name = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
    ElementaryTypeNameExpressionNodePtr elementary_type_expr = std::make_shared<ElementaryTypeNameExpressionNode>(name);
    return elementary_type_expr;
}


ASTNodePtr ASTAnalyser::get_value_equivalent_node(std::string& token){
    ASTNodePtr node;
    if (token == TokenBinaryOperation) {
        node = handle_binary_operation();
    } else if (token == TokenLiteral) {
        node = handle_literal();
    } else if (token == TokenTupleExpression) {
        node = handle_tuple_expression();
    } else if (token == TokenUnaryOperation) {
        node = handle_unary_operation();
    } else if (token == TokenIdentifier) {
        node = handle_identifier();
    } else if (token == TokenFunctionCall) {
        node = handle_function_call();
    } else if (token == TokenMemberAccess) {
        node = handle_member_access();
    } else if (token == TokenIndexAccess) {
        node = handle_index_access();
    } else if (token == TokenElementaryTypeNameExpression) {
        node = handle_elementary_type_name_expression();
    } else if (token == TokenConditional) {
        node = handle_conditional();
    } else if (token == TokenAssignment) {
        node = handle_assignment();
    } else if (token == TokenNewExpression) {
        node = handle_new_expression();
    }
    return node;
}

ConditionalNodePtr ASTAnalyser::handle_conditional() {
    std::string token = get_next_token();
    ASTNodePtr condition = get_value_equivalent_node(token);
    
    token = get_next_token();
    ASTNodePtr yes = get_value_equivalent_node(token);
    token = get_next_token();
    ASTNodePtr no = get_value_equivalent_node(token);
    
    ConditionalNodePtr condiitonal = std::make_shared<ConditionalNode>(condition, yes, no);
    return condiitonal;
}

IndexAccessNodePtr ASTAnalyser::handle_index_access(){
    std::string token = get_next_token();
    ASTNodePtr identifier = get_value_equivalent_node(token);
    token = get_next_token();
    ASTNodePtr index = get_value_equivalent_node(token);
    IndexAccessNodePtr index_access = std::make_shared<IndexAccessNode>();
    index_access->append_subnode(identifier);
    index_access->append_subnode(index);
    return index_access;
}

AssignmentNodePtr ASTAnalyser::handle_assignment() {
    std::string op = Utils::retrieve_string_element(*ptr_ast_line, 3, " ");
    std::string token = get_next_token();
    ASTNodePtr left = get_value_equivalent_node(token);
    token = get_next_token();
    ASTNodePtr right = get_value_equivalent_node(token);
    AssignmentNodePtr assignment = std::make_shared<AssignmentNode>(op);
    assignment->append_subnode(left);
    assignment->append_subnode(right);
    return assignment;
}

IfStatementNodePtr ASTAnalyser::handle_if_statement() {
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    ASTNodePtr condition;
    ASTNodePtr if_then = nullptr;
    ASTNodePtr if_else = nullptr;
    condition = get_value_equivalent_node(token);
    token = get_next_token();
    if_then = get_statement_equivalent_node(token);
    token = get_next_token();
    if (indentation < get_current_indentation()) {
        if_else = get_statement_equivalent_node(token);
    } else {
        --ptr_ast_line;
    }
    IfStatementNodePtr if_stmt = std::make_shared<IfStatementNode>(condition, if_then, if_else);
    return if_stmt;
}

ForStatementNodePtr ASTAnalyser::handle_for_statement() {
    int indentation = get_current_indentation();
    ASTNodePtr subnode[4];
    ForStatementNodePtr for_stmt;
    int tokens = 0;
    std::string token_type[4];
    std::string token = get_next_token();
    while (token != "" && indentation < get_current_indentation()) {
        if (std::find(StatementTokenList.begin(), StatementTokenList.end(), token) != StatementTokenList.end()) {
            subnode[tokens] = get_statement_equivalent_node(token);
        } else {
            subnode[tokens] = get_value_equivalent_node(token);
        }
        token_type[tokens] = token;
        tokens++;
        token = get_next_token();
    }
    std::cout << "num of tokens " <<tokens << std::endl;
    --ptr_ast_line;
    if (tokens == 1) {
        for_stmt = std::make_shared<ForStatementNode>(nullptr, nullptr, nullptr, subnode[0]);
    } else if (tokens == 2) {
        if (token_type[0] == TokenExpressionStatement) {
            for_stmt = std::make_shared<ForStatementNode>(nullptr, nullptr, subnode[0], subnode[1]);
        } else if (std::find(StatementTokenList.begin(), StatementTokenList.end(), token_type[0]) != StatementTokenList.end()) {
            for_stmt = std::make_shared<ForStatementNode>(subnode[0], nullptr, nullptr, subnode[1]);
        } else {
            for_stmt = std::make_shared<ForStatementNode>(nullptr, subnode[0], nullptr, subnode[1]);
        }
    } else if (tokens == 3) {
        if (std::find(StatementTokenList.begin(), StatementTokenList.end(), token_type[0]) != StatementTokenList.end() && token_type[1] == TokenExpressionStatement) {
            for_stmt = std::make_shared<ForStatementNode>(subnode[0], nullptr, subnode[1], subnode[2]);
        } else if (std::find(StatementTokenList.begin(), StatementTokenList.end(), token_type[0]) != StatementTokenList.end() && std::find(ExpressionTokenList.begin(), ExpressionTokenList.end(), token_type[1]) != ExpressionTokenList.end()) {
            for_stmt = std::make_shared<ForStatementNode>(subnode[0], subnode[1], nullptr, subnode[2]);
        } else {
            for_stmt = std::make_shared<ForStatementNode>(nullptr, subnode[0], subnode[1], subnode[2]);
        }
    } else {
        for_stmt = std::make_shared<ForStatementNode>(subnode[0], subnode[1], subnode[2], subnode[3]);
    }

    return for_stmt;
}

DoWhileStatementNodePtr ASTAnalyser::handle_do_while_statament() {
    std::string token = get_next_token();
    ASTNodePtr condition = get_unknown(token);
    token = get_next_token();
    ASTNodePtr loop_body = get_unknown(token);
    DoWhileStatementNodePtr do_while = std::make_shared<DoWhileStatementNode>(condition, loop_body);
    return do_while;
}

WhileStatementNodePtr ASTAnalyser::handle_while_statement() {
    std::string token = get_next_token();
    ASTNodePtr condition = get_unknown(token);
    token = get_next_token();
    ASTNodePtr loop_body = get_unknown(token);
    WhileStatementNodePtr while_do = std::make_shared<WhileStatementNode>(condition, loop_body);
    return while_do;
}

ASTNodePtr ASTAnalyser::get_statement_equivalent_node(std::string& token) {
    if (token == TokenBlock) {
        return handle_block();
    } else if (token == TokenPlaceholderStatement) {
        return handle_placeholder();
    } else if (token == TokenIfStatement) {
        return handle_if_statement();
    } else if (token == TokenDoWhileStatement) {
        return handle_do_while_statament();
    } else if (token == TokenWhileStatement) {
        return handle_while_statement();
    } else if (token == TokenForStatement) {
        return handle_for_statement();
    } else if (token == TokenEmitStatement) {
        return handle_emit_statement();
    } else if (token == TokenVariableDeclarationStatement) {
        return handle_variable_declaration_statament();
    } else if (token == TokenExpressionStatement) {
        return handle_expression_statament();
    } else if (token == TokenBreak) {
        return std::make_shared<BreakNode>();
    } else if (token == TokenContinue) {
        return std::make_shared<ContinueNode>();
    } else if (token == TokenReturn){
        return handle_return();
    } else if (token == TokenThrow) {
        return std::make_shared<ThrowNode>();
    } else if (token == TokenInlineAssembly) {
        // In the ast file, inline assembly is stored as the source code
        get_next_token(TokenSource);
        std::string source_line = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
        remove_escapes(source_line);
        InlineAssemblyNodePtr inline_assembly = std::make_shared<InlineAssemblyNode>();
        inline_assembly->set_source(source_line);
        return inline_assembly;
    }
    return nullptr;
}

ASTNodePtr ASTAnalyser::get_unknown(std::string& token){
    if (std::find(StatementTokenList.begin(), StatementTokenList.end(), token) != StatementTokenList.end()) {
        return get_statement_equivalent_node(token);
    } else if (std::find(ExpressionTokenList.begin(), ExpressionTokenList.end(), token) != ExpressionTokenList.end()) {
        return get_value_equivalent_node(token);
    }
    return nullptr;
}

NewExpresionNodePtr ASTAnalyser::handle_new_expression() {
    std::string token = get_next_token();
    ASTNodePtr type_name = get_type_name(token);
    return std::make_shared<NewExpresionNode>(type_name);
}

EnumDefinitionNodePtr ASTAnalyser::handle_enum_definition() {
    std::string name = Utils::substr_by_edge(*ptr_ast_line, TokenEnumDefinition + " \"", "\"");
    EnumDefinitionNodePtr enum_definition = std::make_shared<EnumDefinitionNode>(name);
    int indentation = get_current_indentation();
    std::string token = get_next_token();
    while (token == TokenEnumValue && indentation < get_current_indentation()) {
        EnumValueNodePtr enum_member = handle_enum_value();
        enum_definition->append_subnode(enum_member);
        token = get_next_token();
    }
    --ptr_ast_line;
    return enum_definition;
}

EnumValueNodePtr ASTAnalyser::handle_enum_value() {
    std::string name = Utils::substr_by_edge(*ptr_ast_line, TokenEnumValue + " \"", "\"");
    return std::make_shared<EnumValueNode>(name);
}

PlaceHolderStatementPtr ASTAnalyser::handle_placeholder() {
    get_next_token(TokenSource);
    std::string placeholder = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");
    return std::make_shared<PlaceHolderStatement>(placeholder);
}

ASTNodePtr ASTAnalyser::get_type_name(std::string& token) {
    ASTNodePtr type_name;
    std::string type_str;
    std::string temp_type_str;
    std::vector<std::string> words; //Splitted words

    if (token == TokenElementaryTypeName) {

        //Sefa Added new
        --ptr_ast_line;

        std::string sc = *ptr_ast_line;
         
        boost::split(words, sc, boost::is_any_of("\n\t .,!?\"()"));

        auto match = std::find(begin(words), end(words), "mapping");
        if(match != end(words) ){

            ++ptr_ast_line; 
            ++ptr_ast_line;
            
            type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");

            
        }
        else{
             type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "=");

            if(type_str.size()==0){
                --ptr_ast_line;
                --ptr_ast_line;
                sc = *ptr_ast_line;
                boost::split(words, sc, boost::is_any_of("\n\t .,!?\"()"));
                 auto match = std::find(begin(words), end(words), "mapping");
                 if(match != end(words) ){
                     ++ptr_ast_line;
                     ++ptr_ast_line;
                     ++ptr_ast_line;
                     ++ptr_ast_line;
                     type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");

                 }
                 else{
                    ++ptr_ast_line;
                    ++ptr_ast_line;
                    type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");

                    --ptr_ast_line;
                    --ptr_ast_line;
                    --ptr_ast_line;
                    
                    temp_type_str = Utils::substr_by_edge(*ptr_ast_line, "Type: ", "\0");
                    Utils::str_replace_all(temp_type_str,"uint256","uint");
                    if(boost::starts_with(temp_type_str, type_str)){
                        std::size_t found = temp_type_str.find("memory");
                        if (found!=std::string::npos)
                            type_str = temp_type_str;
                    }

                       

                    ++ptr_ast_line;
                    ++ptr_ast_line;
                    ++ptr_ast_line;
                    
                    ++ptr_ast_line;
                    ++ptr_ast_line;
                    
                   
                 }
            }
            else{

                ++ptr_ast_line;
                ++ptr_ast_line; 
            }

            
        }
        type_name = std::make_shared<ElementaryTypeNameNode>(type_str);
    } else if (token == TokenFunctionTypeName) {
        get_next_token(TokenParameterList);
        ParameterListNodePtr parameters = handle_parameter_list();
        get_next_token(TokenParameterList);
        ParameterListNodePtr returns = handle_parameter_list();
        type_name = std::make_shared<FunctionTypeNameNode>(parameters, returns);
    } else if (token == TokenArrayTypeName) {
        
        std::string array_base_type_token = get_next_token();

        ASTNodePtr base_type_node = get_type_name(array_base_type_token);
        ASTNodePtr index = nullptr;

        //try to find out the index of the array
        int indentation = get_current_indentation();
        std::string token = get_next_token();

        if (token != "" && indentation < get_current_indentation()) {
            index = get_value_equivalent_node(token);
        } else {
            --ptr_ast_line;
        } 
        type_name = std::make_shared<ArrayTypeNameNode>(base_type_node, index);
    } else if (token == TokenUserDefinedTypeName) {
        std::vector<std::string> splittedwords;
        --ptr_ast_line;
        
        type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");//sefa
        if(type_str=="address" ){
            ++ptr_ast_line;
            ++ptr_ast_line;
            type_str = Utils::substr_by_edge(*ptr_ast_line, "Source: \"", "\"");//sefa
            --ptr_ast_line;
            --ptr_ast_line;
        }
        boost::split(splittedwords, type_str, boost::is_any_of("\n\t .,!?\"()"));
        std::string keyword = "new";
        auto match = std::find(begin(splittedwords), end(splittedwords), keyword);
        if(match != end(splittedwords)){
            
             ++ptr_ast_line;
              type_str = Utils::substr_by_edge(*ptr_ast_line, TokenUserDefinedTypeName + " \"", "\"");
              type_name = std::make_shared<UserDefinedTypeNameNode>(type_str);
              
        }
        else
        {
            
            ++ptr_ast_line;
          type_name = std::make_shared<UserDefinedTypeNameNode>(type_str);

        }
          

        type_name = std::make_shared<UserDefinedTypeNameNode>(type_str);
    } else if (token == TokenMapping) {
        std::string token = get_next_token();
        ASTNodePtr key_type = get_type_name(token);
        token = get_next_token();
        ASTNodePtr value_type = get_type_name(token);
        type_name = std::make_shared<MappingNode>(key_type, value_type);
        
    }
    return type_name;
}

ModifierInvocationNodePtr ASTAnalyser::handle_modifier_invocation() {
    std::string modifier_name = Utils::retrieve_string_element(*ptr_ast_line, 1, " ");
    modifier_name = Utils::substr_by_edge(modifier_name, "\"", "\"");
    ModifierInvocationNodePtr modifier_invocation = std::make_shared<ModifierInvocationNode>(modifier_name);
    int indentation = get_current_indentation();
    std::string token = get_next_token(); // the token right after modifier invocation is an identifier token which is the name of the modifier
    token = get_next_token();
    while (token != "" && indentation < get_current_indentation()) {
        ASTNodePtr subnode = get_value_equivalent_node(token);
        modifier_invocation->append_subnode(subnode);
        token = get_next_token();
    }
    --ptr_ast_line;

    return modifier_invocation;
}

int ASTAnalyser::get_current_indentation() {
    if (ptr_ast_line == ast_lines.end()) return 0;
    return ptr_ast_line->find_first_not_of(' ');
}
    //std::cout << jsonast.at("nodes").at(2).at("nodes").at(0).at("visibility") << std::endl;

std::string ASTAnalyser::get_function_qualifier(const std::string& _contract_name, const std::string& _function_name, const std::string& _type) {
    unsigned int num_functions = 0;
    auto root_node = jsonast.at("nodes");
    for (size_t i = 0; i < root_node.size(); ++i) {
        auto contract_node = root_node.at(i);
        if (contract_node.find("name") == contract_node.end()) {
            continue;
        }
        std::string this_contract_name = contract_node.at("name");
        if (this_contract_name == _contract_name) {
            for (size_t j = 0; j < contract_node.at("nodes").size(); ++j) {
                auto sub_node = contract_node.at("nodes").at(j);
                if (sub_node.at("nodeType") == TokenFunctionDefinition) {
                    if (sub_node.at("name") == _function_name && num_functions == num_functions_current_contract) {
                        return sub_node.at(_type);
                    }
                    num_functions++;
                }
            }
        }
    }
    return "";
}

bool ASTAnalyser::function_is_constructor(const std::string& _contract_name, const std::string& _function_name) {
    unsigned int num_functions = 0;
    auto root_node = jsonast.at("nodes");
    for (size_t i = 0; i < root_node.size(); ++i) {
        auto contract_node = root_node.at(i);
        if (contract_node.find("name") == contract_node.end()) {
            continue;
        }
        std::string this_contract_name = contract_node.at("name");
        if (this_contract_name == _contract_name) {
            for (size_t j = 0; j < contract_node.at("nodes").size(); ++j) {
                auto sub_node = contract_node.at("nodes").at(j);
                if (sub_node.at("nodeType") == TokenFunctionDefinition) {
                    if (sub_node.at("name") == _function_name && num_functions == num_functions_current_contract) {
                        try{
                                //std::cout<<"iscontructor-- "<<sub_node.at("kind")<<std::endl;
                                if(sub_node.at("kind")=="constructor")
                                    return true;
                                else
                                    return false;   
                                //return sub_node.at("kind");
                        }
                        catch (std::exception& e){
                           //std::cout<<"iscontructor exception -- "<<sub_node.at("isConstructor")<<std::endl;
                           return sub_node.at("isConstructor");
                       }
                        
                    }
                    num_functions++;
                }
            }
        }
    }
    return false;
}

std::string ASTAnalyser::get_extra_code_for_mutation(std::string version) {
    std::string result;
    if (mutation_type == "overflow") {
        result = "    //unsigned overflow added\n    uint8 t1=250;\n    uint8 test=255+t1;\n    //********\n";
    } else if (mutation_type == "underflow") {
        result = "    //underflow added\n    uint256 underflow=20;\n    uint256 subtrahend =250;\n    uint256 result = underflow -subtrahend;\n    //********\n";
    } else if (mutation_type == "divbyzero") {
        result = "    //division by zero added\n    uint input=10;\n    uint divisor =input - 10;\n    uint quotient = 100/divisor;\n    //********\n";
    } else if (mutation_type == "timestampusage") {
        result = "    //timestamp usage added\n    uint rand = uint(block.timestamp-1);\n    //********\n";
    } else if (mutation_type == "txorigin") {
        result = "    //TxOrigin usage added\n  bool flag;\n  if(tx.origin==msg.sender)\n      flag =false;\n    //********\n";
    } else if(mutation_type == "uninitializedstorage") {
        std::size_t found = version.find("0.4");
        std::size_t multiplecompiler = version.find(">");
        if (multiplecompiler==std::string::npos){
            if (found!=std::string::npos)
                result = "    //Uninitialized storage variable added\n  uint256[] storage testforuninitalizedstorage;\n //********\n";
        }
        
    }
    else if (mutation_type == "uncheckedsend") {
        result = "    //uncheckedsend function call \n  uncheckedSend(0xCA35b7d915458EF540aDe6068dFe2F44E8fa733c,10)\n    //uncheckedsend function call ended \n";
    } else if (mutation_type == "repetitivefuncall") {
        result = "    //repetitivefuncall function call \n  repFunCall(0xCA35b7d915458EF540aDe6068dFe2F44E8fa733c, 20)\n    //repetitivefuncall function call ended \n";
    }




        

    return result;
}


}