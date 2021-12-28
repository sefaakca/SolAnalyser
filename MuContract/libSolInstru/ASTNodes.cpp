#include "ASTNodes.hpp"
#include <boost/algorithm/string.hpp>

int counter=0;
namespace SolInstru{

void ASTNode::append_subnode(std::shared_ptr<ASTNode> _node) {
    subnodes.push_back(_node);
}

NodeType ASTNode::get_node_type() {
    return node_type;
}

int ASTNode::subnodes_size() {
    return subnodes.size();
}

ASTNodePtr ASTNode::get_sub_node(const unsigned int& index) {
    if (index < subnodes.size()) return subnodes[index];
    else return nullptr;
}

std::string UsingForDirectiveNode::source_code() const {
    std::string result = "using " + A + " for " + B + ";";
    return result;
}

void VariableDeclarationNode::set_initial_value(ASTNodePtr _initial_value) {
    initial_value = _initial_value;
}

void remove_all_literals(std::string _str){
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

}

std::string& fix_tabs(std::string& s)
{
    size_t start_pos = 0;
    while((start_pos = s.find("\\t", start_pos)) != std::string::npos) {
         s.replace(start_pos, 2, "\t");
         start_pos += 1;
    }
    return s;
}

std::string& fix_empty(std::string& s)
{
    size_t start_pos = 0;
    while((start_pos = s.find("\\r", start_pos)) != std::string::npos) {
         s.replace(start_pos, 2, "\r");
         start_pos += 1;
    }
    return s;
}

std::string VariableDeclarationNode::source_code() const {
    std::string result;
    std::vector<std::string> words; //Splitted words
    if (initial_value != nullptr) {

        //Whole words only
        std::string sc = type->source_code();
       
        fix_tabs(sc);
        
        boost::split(words, sc, boost::is_any_of("\n\t .,!?\"()"));
        
        auto match = std::find(begin(words), end(words), variable_name);

        if(match != end(words) ){ 
            result = type->source_code() + " = " + initial_value->source_code();
        }
        else {
             result = type->source_code() + " " + variable_name + " = " + initial_value->source_code();
        }
    } else {
        std::string sc = type->source_code();

        boost::split(words, sc, boost::is_any_of("\n\t .,!?\"()"));
        auto match = std::find(begin(words), end(words), variable_name);
        if(match != end(words) ){
            result = type->source_code();
        }
        else {
             result = type->source_code() + " " + variable_name;
        }
    }
    

    return result;
}

ASTNodePtr VariableDeclarationStatementNode::getValue() const{

    return value;
}

VariableDeclarationNodePtr VariableDeclarationStatementNode::getDecl() const{
    return decl;
}

void VariableDeclarationStatementNode::setDecl(VariableDeclarationNodePtr _decl){
    decl=_decl;
}

void VariableDeclarationStatementNode::setValue(ASTNodePtr _value){
    value=_value;

}

std::string VariableDeclarationStatementNode::source_code() const {
    if (value) {
        return decl->source_code() + " = " + value->source_code() + ";";
    } else {
        return decl->source_code() + ";";
    }
}

std::string IdentifierNode::source_code() const {
    return name;
}

std::string StructDefinitionNode::source_code() const {
    std::string result = "struct " + name + "{\n";
    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        result = result + (*it)->source_code() + ";\n";
    }
    result = result + "}\n";
    return result;
}

std::string ParameterListNode::source_code() const {
    if (subnodes.size() == 0) return "()";
    std::string result = "(";
    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        result = result + (*it)->source_code() + ", ";
    }
    result = result.substr(0, result.length()-2) + ")";
    return result;
}

std::string EventDefinitionNode::source_code() const {
    std::string result = "event " + name + subnodes[0]->source_code() + ";\n";
    return result;
}

std::string ExpressionStatementNode::source_code() const {
    return subnodes[0] -> source_code() + ";";
}

std::string EmitStatementNode::source_code() const {
    return "emit " + subnodes[0] -> source_code() + ";";
}

std::string IndexAccessNode::source_code() const {
    return subnodes[0]->source_code() + "[" + subnodes[1]->source_code() + "]";
}

std::string BinaryOperationNode::source_code() const {
    return left_hand_operand->source_code() + " " + op + " " + right_hand_operand->source_code();
}

std::string BinaryOperationNode::get_op() const {
    return op;
}

ASTNodePtr BinaryOperationNode::get_left() const {
    return left_hand_operand;
}

ASTNodePtr BinaryOperationNode::get_right() const {
    return right_hand_operand;
}

std::string UnaryOperationNode::source_code() const {
    if (is_prefix) {
        if (op == "delete") {
            return op + " " + operand->source_code();
        } else {
            return op + operand->source_code();
        }
    } else {
        return operand->source_code() + op;
    }
}

std::string LiteralNode::source_code() const {
    return literal;
}

std::string TupleExpressionNode::source_code() const {
    std::string result = "(";
    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        result = result + (*it)->source_code() + ", ";
    }
    result = result.substr(0, result.length()-2) + ")";
    return result;
}

std::string BlockNode::source_code() const {
    std::string result = "{\n";
    if (!extra_code.empty()){
        result = result + extra_code;
    }
    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        result = result + (*it)->source_code() + "\n";
    }
    result = result + "}\n";
    return result;
}

void BlockNode::set_extra_code(std::string _extra_code) {
    extra_code = _extra_code;
}

std::string ReturnNode::source_code() const {
    if (subnodes.size()) {
        return "return " + subnodes[0]->source_code() + ";";
    } else {
        return "return;";
    }
}

std::string FunctionDefinitionNode::source_code() const {
    std::string result = "";
    if (name == "") {
        if (is_constructor) {
            result = "constructor" + params->source_code();
        } else {
            result = "function " + params->source_code();
        }
    } else {
        result = "function " + name + params->source_code();
    }
    if (qualifier != "") result = result + " " + qualifier;
    if (modifier_invocation.size() != 0) {
        for (auto it = modifier_invocation.begin(); it != modifier_invocation.end(); ++it) {
            result = result + " " + (*it)->source_code();
        }
    }
    if (returns->subnodes_size() != 0) result = result + " returns" + returns->source_code();
    if (function_body == nullptr) {
        result = result + ";";
    } else {
        result = result + function_body->source_code();
    }
    return result;
}

void FunctionDefinitionNode::set_name(std::string& _name) {
    this->name = _name;
} 

void FunctionDefinitionNode::set_qualifier(std::string& _qualifier) {
    this->qualifier = _qualifier;
}

void FunctionDefinitionNode::set_params(ParameterListNodePtr _params) {
    this->params = _params;
}

void FunctionDefinitionNode::set_returns(ParameterListNodePtr _returns) {
    this->returns = _returns;
}

void FunctionDefinitionNode::set_function_body(BlockNodePtr _function_body) {
    this->function_body = _function_body;
}

void FunctionDefinitionNode::add_modifier_invocation(ModifierInvocationNodePtr _modifier_invocation) {
    modifier_invocation.push_back(_modifier_invocation);
}

void FunctionDefinitionNode::set_is_constructor(bool& _is_constructor) {
    is_constructor = _is_constructor;
}

std::string FunctionCallNode::source_code() const {
    if (subnodes.size()) {
        std::string result = callee->source_code() + "(";
        for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
            result = result + (*it)->source_code() + ", ";
        }
        result = result.substr(0, result.length()-2) + ")";
        return result;
    } else {
        return callee->source_code() + "()";
    }
}

std::string MemberAccessNode::source_code() const {
    return identifier->source_code() + "." + member;
}

std::string ElementaryTypeNameExpressionNode::source_code() const {
    return name;
}

void ContractDefinitionNode::add_inherit_from(std::string& _inherit_from) {
    inherit_from.push_back(_inherit_from);
}



std::string ContractDefinitionNode::source_code() const {
    std::string result;
    bool flag=false;
    if (is_library) {
        result = "library " + name;
    } else if(is_interface) {

        result = "interface " + name;
    } 
    else {
        flag=true;
        result = "contract " + name;
    }
    if (inherit_from.size()) {
        auto it = inherit_from.begin();
        result = result + " is " + *it;
        ++it;
        while (it != inherit_from.end()) {
            result = result + ", " + *it;
            ++it;
        }
    }
    result = result + " {\n";

    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        std::string sub_source_code = (*it)->source_code();
        
        if ((*it)->get_node_type() == NodeTypeVariableDeclaration)
        {
             sub_source_code = sub_source_code + ";"; // varibale declared in contract-level needs a ';'
             
        }
        sub_source_code = sub_source_code + "\n";
        result = result + sub_source_code;  
    }

    

    std::size_t found = compiler_version.find("0.4");
    std::size_t multiplecompiler = compiler_version.find(">");
    if(multiplecompiler!=std::string::npos) {
         if(vulnerability_type == "shadowingstatevariable" && flag) {
                    counter++;
                    result = result + " //Shadowing State variable added\n uint n=1;\n function shadowingstatevariable() public returns(uint n){\n \n return n;\n}\n //********\n";
            }
    }
    else{
        if (found!=std::string::npos){
            if(vulnerability_type == "shadowingstatevariable" && flag && counter==0) {
                    counter++;
                    result = result + " //Shadowing State variable added\n uint n=1;\n function shadowingstatevariable() public returns(uint n){\n \n return n;\n}\n //######\n";
            } else if(vulnerability_type == "repetitivefuncall" && flag) {
                    counter++;
                    result = result + " //Repetitive Function call added\n function repFunCall(address to, uint testamount) payable public{\n for(uint i=0;i<10;i++){\n to.send(testamount);\n}\n}\n //######\n";
            } else if(vulnerability_type == "uncheckedsend" && flag) {
                    counter++;
                    result = result + " //Uncheckedsend added\n function uncheckedSend(address to, uint testamount) payable public{\n  to.send(testamount);\n}\n //######\n";
            }
        }
        else{
            if(vulnerability_type == "shadowingstatevariable" && flag && counter==0) {
                    counter++;
                    result = result + " //Shadowing State variable added\n uint n=1;\n function shadowingstatevariable() public returns(uint n){\n \n return n;\n}\n //######\n";
            } else if(vulnerability_type == "repetitivefuncall" && flag) {
                    counter++;
                    result = result + " //Repetitive Function call added\n function repFunCall(address payable to, uint testamount) payable public{\n for(uint i=0;i<10;i++){\n to.send(testamount);\n}\n}\n //######\n";
            } else if(vulnerability_type == "uncheckedsend" && flag) {
                    counter++;
                    result = result + " //Uncheckedsend added\n function uncheckedSend(address payable to, uint testamount) payable public{\n  to.send(testamount);\n}\n //######\n";
            }
        }
    }
        
    
    
    result = result + "}\n";
    fix_tabs(result);
    fix_empty(result);
    return result;
}

void ContractDefinitionNode::set_as_library() {
    is_library = true;
}
void ContractDefinitionNode::set_as_interface() {
    is_interface = true;
}

bool ContractDefinitionNode::get_is_Lib(){
     return is_library;
 }

 bool ContractDefinitionNode::get_is_interface(){
     return is_interface;
 }

std::string ConditionalNode::source_code() const {
    return condition->source_code() + " ? " + yes->source_code() + " : " + no->source_code();
}

std::string AssignmentNode::source_code() const {
    return subnodes[0]->source_code() + " " + op + " " + subnodes[1]->source_code();
}

std::string IfStatementNode::source_code() const {
    std::string result = "if (" + condition->source_code() + ") \n" + if_then->source_code() + "\n";
    if (if_else != nullptr) result = result + " else \n" + if_else->source_code() + "\n";
    return result;    
}

std::string DoWhileStatementNode::source_code() const {
    std::string result = "do \n" + loop_body->source_code() + "while(" + condition->source_code() + ");\n";
    return result;
}

std::string WhileStatementNode::source_code() const {
    std::string result = "while(" + condition->source_code() + ")\n" + loop_body->source_code() + "\n";
    return result;
}

std::string ForStatementNode::source_code() const {
    std::string condition_str = "; ";
    if (condition != nullptr) condition_str = condition->source_code() + "; ";

    std::string init_str = "; ";
    if (init != nullptr) init_str = init->source_code() + " ";

    std::string increment_str = "";
    if (increment != nullptr) {
        increment_str = increment->source_code();
        size_t increment_str_len = increment_str.length();
        for (auto i = increment_str.length() - 1; i >= 0; --i) {
            char ch = increment_str[i];
            if (ch == ' ' || ch == '\n') {
                --increment_str_len;
            } else if (ch == ';') {
                --increment_str_len;
                break;
            } else {
                break;
            }
        }
        increment_str = increment_str.substr(0, increment_str_len);
    }

    std::string result = "for (" + init_str + condition_str + increment_str + ")\n" + body->source_code() + "\n";
    return result;
}

std::string BreakNode::source_code() const {
    return "break;";
}

std::string ContinueNode::source_code() const {
    return "continue;";
}

std::string NewExpresionNode::source_code() const {
    return "new " + type_name->source_code();
}

std::string EnumDefinitionNode::source_code() const {
    std::string result = "enum " + name + "{";
    for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
        result = result + (*it)->source_code() + ", ";
    }
    result = result.substr(0, result.length()-2) + "}";
    return result;
}

std::string EnumValueNode::source_code() const {
    return name;
}

std::string ThrowNode::source_code() const {
    return "throw;";
}

std::string PlaceHolderStatement::source_code() const {
    return placeholder+";";
}

std::string ModifierDefinitionNode::source_code() const {
    std::string params_str;
    if (params == nullptr) params_str = "()";
    else params_str = params->source_code();
    std::string result = "modifier  " + name + params_str;
    result = result + body->source_code();
    return result;
}

std::string ModifierInvocationNode::source_code() const {
    if (subnodes.size() == 0) {
        return name + "()";
    } else {
        std::string result = name + "(";
        for (auto it = subnodes.begin(); it != subnodes.end(); ++it) {
            result = result + (*it)->source_code() + ", ";
        }
        result = result.substr(0, result.length()-2) + ")";
        return result;
    }
}

std::string MappingNode::source_code() const{
    return "mapping(" + key_type->source_code() + " => " + value_type->source_code() + ")";
}

std::string ElementaryTypeNameNode::source_code() const {
    return type_name;
}

std::string UserDefinedTypeNameNode::source_code() const {
    return type_name;
}

std::string FunctionTypeNameNode::source_code() const {
    std::string result = "function" + params->source_code();
    if (returns->subnodes_size()) result = result + " returns" + returns->source_code();
    return result;
}

std::string ArrayTypeNameNode::source_code() const {
    std::string result = base_type->source_code();
    if (size != nullptr) {
         std::string intext = "[" + size->source_code() + "]";
         if(result.find(intext)!=std::string::npos){
            result = result;
         }
         else {
             result = result + "[" + size->source_code() + "]";
         }
    }
    else {
        if(result.find("[]")!=std::string::npos)
            result = result;
        else
            result = result + "[]";  
    }
    return result;
}
std::string& fix_newlines(std::string& s)
{
    size_t start_pos = 0;
    while((start_pos = s.find("\\n", start_pos)) != std::string::npos) {
         s.replace(start_pos, 2, "\n");
         start_pos += 1;
    }
    return s;
}

void InlineAssemblyNode::set_source(std::string& _source) {
    source = _source;   
    source = source.substr(0, source.find_last_of('}')+1);
    fix_newlines(source);
    fix_tabs(source);
    fix_empty(source);
}

std::string InlineAssemblyNode::get_source() {
    return source;
}

std::string InlineAssemblyNode::source_code() const {
    return source;
}

}
