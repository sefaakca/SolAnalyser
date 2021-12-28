#include <algorithm>
#include <fstream>
#include <list>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <libUtils/Utils.hpp>

namespace SolInstru{

const std::string TokenPragmaDirective = "PragmaDirective";
const std::string TokenImportDirective = "ImportDirective";
const std::string TokenContractDefinition = "ContractDefinition";
const std::string TokenInheritanceSpecifier = "InheritanceSpecifier";
const std::string TokenUsingForDirective = "UsingForDirective";
const std::string TokenStructDefinition = "StructDefinition";
const std::string TokenEnumDefinition = "EnumDefinition";
const std::string TokenEnumValue = "EnumValue";
const std::string TokenParameterList = "ParameterList";
const std::string TokenFunctionDefinition = "FunctionDefinition";
const std::string TokenVariableDeclaration = "VariableDeclaration";
const std::string TokenModifierDefinition = "ModifierDefinition";
const std::string TokenModifierInvocation = "ModifierInvocation";
const std::string TokenEventDefinition = "EventDefinition";
const std::string TokenElementaryTypeName = "ElementaryTypeName";
const std::string TokenUserDefinedTypeName = "UserDefinedTypeName";
const std::string TokenFunctionTypeName = "FunctionTypeName";
const std::string TokenMapping = "Mapping";
const std::string TokenArrayTypeName = "ArrayTypeName";
const std::string TokenInlineAssembly = "InlineAssembly";
const std::string TokenBlock = "Block";
const std::string TokenPlaceholderStatement = "PlaceholderStatement";
const std::string TokenIfStatement = "IfStatement";
const std::string TokenDoWhileStatement = "DoWhileStatement";
const std::string TokenWhileStatement = "WhileStatement";
const std::string TokenForStatement = "ForStatement";
const std::string TokenContinue = "Continue";
const std::string TokenBreak = "Break";
const std::string TokenReturn = "Return";
const std::string TokenThrow = "Throw";
const std::string TokenEmitStatement = "EmitStatement";
const std::string TokenVariableDeclarationStatement = "VariableDeclarationStatement";
const std::string TokenExpressionStatement = "ExpressionStatement";
const std::string TokenConditional = "Conditional";
const std::string TokenAssignment = "Assignment";
const std::string TokenTupleExpression = "TupleExpression";
const std::string TokenUnaryOperation = "UnaryOperation";
const std::string TokenBinaryOperation = "BinaryOperation";
const std::string TokenFunctionCall = "FunctionCall";
const std::string TokenNewExpression = "NewExpression";
const std::string TokenMemberAccess = "MemberAccess";
const std::string TokenIndexAccess = "IndexAccess";
const std::string TokenIdentifier = "Identifier";
const std::string TokenElementaryTypeNameExpression = "ElementaryTypeNameExpression";
const std::string TokenLiteral = "Literal,"; // comma is needed here

const std::list<std::string> KeywordsList{
    TokenPragmaDirective,
    TokenImportDirective,
    TokenContractDefinition,
    TokenInheritanceSpecifier,
    TokenUsingForDirective,
    TokenStructDefinition,
    TokenEnumDefinition,
    TokenEnumValue,
    TokenParameterList,
    TokenFunctionDefinition,
    TokenVariableDeclaration,
    TokenModifierDefinition,
    TokenModifierInvocation,
    TokenEventDefinition,
    TokenElementaryTypeName,
    TokenUserDefinedTypeName,
    TokenFunctionTypeName,
    TokenMapping,
    TokenArrayTypeName,
    TokenInlineAssembly,
    TokenBlock,
    TokenPlaceholderStatement,
    TokenIfStatement,
    TokenDoWhileStatement,
    TokenWhileStatement,
    TokenForStatement,
    TokenContinue,
    TokenBreak,
    TokenReturn,
    TokenThrow,
    TokenEmitStatement,
    TokenVariableDeclarationStatement,
    TokenExpressionStatement,
    TokenConditional,
    TokenAssignment,
    TokenTupleExpression,
    TokenUnaryOperation,
    TokenBinaryOperation,
    TokenFunctionCall,
    TokenNewExpression,
    TokenMemberAccess,
    TokenIndexAccess,
    TokenIdentifier,
    TokenElementaryTypeNameExpression,
    TokenLiteral
};

const std::list<std::string> ExpressionTokenList{
    TokenConditional,
    TokenAssignment,
    TokenTupleExpression,
    TokenUnaryOperation,
    TokenBinaryOperation,
    TokenFunctionCall,
    TokenNewExpression,
    TokenMemberAccess,
    TokenIndexAccess,
    TokenIdentifier,
    TokenElementaryTypeNameExpression,
    TokenLiteral
};

const std::list<std::string> StatementTokenList{
    TokenBlock,
    TokenPlaceholderStatement,
    TokenIfStatement,
    TokenDoWhileStatement,
    TokenWhileStatement,
    TokenForStatement,
    TokenContinue,
    TokenBreak,
    TokenReturn,
    TokenThrow,
    TokenEmitStatement,
    TokenVariableDeclarationStatement,
    TokenExpressionStatement,
    TokenInlineAssembly
};

enum NodeType {
    NodeTypeUsingForDirective,
    NodeTypeVariableDeclaration,
    NodeTypeStructDefinition,
    NodeTypeParameterList,
    NodeTypeEventDefinition,
    NodeTypeBlockNode,
    NodeTypeFunctionDefinition,
    NodeTypeContractDefinition,
    NodeTypeFunctionCall,
    NodeTypeEnumDefinition,
    NodeTypeEnumValue,
    NodeTypeModifierDefinition,
    NodeTypeModifierInvocation,
    NodeTypeMapping,
    NodeTypeInlineAssembly,
    NodeTypePlaceholderStatement,
    NodeTypeIfStatement,
    NodeTypeDoWhileStatement,
    NodeTypeWhileStatement,
    NodeTypeForStatement,
    NodeTypeContinue,
    NodeTypeBreak,
    NodeTypeReturn,
    NodeTypeThrow,
    NodeTypeEmitStatement,
    NodeTypeVariableDeclarationStatement,
    NodeTypeExpressionStatement,
    NodeTypeConditional,
    NodeTypeAssignment,
    NodeTypeTupleExpression,
    NodeTypeUnaryOperation,
    NodeTypeBinaryOperation,
    NodeTypeNewExpression,
    NodeTypeMemberAccess,
    NodeTypeIndexAccess,
    NodeTypeIdentifier,
    NodeTypeElementaryTypeNameExpression,
    NodeTypeLiteral,
    NodeTypeElementaryTypeName,
    NodeTypeUserDefinedTypeName,
    NodeTypeFunctionTypeName,
    NodeTypeArrayTypeName
};

const std::string TokenSource = "Source:";
const std::string TokenType = "Type:";

class ASTNode {
public:
    explicit ASTNode(NodeType _node_type) : node_type(_node_type) {}
    virtual std::string source_code() const = 0;
    virtual void append_subnode(std::shared_ptr<ASTNode> _node);
    NodeType get_node_type();
    int subnodes_size();
    std::shared_ptr<ASTNode> get_sub_node(const unsigned int& index);
    std::string type_str;
    std::string vulnerability_type;
    std::string compiler_version;
private:
    NodeType node_type;
protected:
    std::vector<std::shared_ptr<ASTNode>> subnodes;
};
typedef std::shared_ptr<ASTNode> ASTNodePtr;

class UsingForDirectiveNode : public ASTNode {
public:
    UsingForDirectiveNode(std::string& _A, std::string& _B) : ASTNode(NodeTypeUsingForDirective), A(_A), B(_B) {}
    std::string source_code() const;
private:
    std::string A;
    std::string B;
};
typedef std::shared_ptr<UsingForDirectiveNode> UsingForDirectiveNodePtr;

class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(ASTNodePtr _type, std::string& _variable_name) : ASTNode(NodeTypeVariableDeclaration), type(_type), variable_name(_variable_name), initial_value(nullptr) {}
    std::string source_code() const;
    void set_initial_value(ASTNodePtr _initial_value);
private:
    ASTNodePtr type;
    std::string variable_name;
    ASTNodePtr initial_value;
};
typedef std::shared_ptr<VariableDeclarationNode> VariableDeclarationNodePtr;

class VariableDeclarationStatementNode : public ASTNode {
public:
    VariableDeclarationStatementNode(VariableDeclarationNodePtr _decl, ASTNodePtr _value) : ASTNode(NodeTypeVariableDeclarationStatement), decl(_decl), value(_value) {}
    std::string source_code() const;
    //get and set functions here
    VariableDeclarationNodePtr getDecl() const;
    ASTNodePtr getValue() const;
    void setDecl(VariableDeclarationNodePtr _decl);
    void setValue(ASTNodePtr _value);

private:
    VariableDeclarationNodePtr decl;
    ASTNodePtr value;
};
typedef std::shared_ptr<VariableDeclarationStatementNode> VariableDeclarationStatementNodePtr;

class IdentifierNode : public ASTNode {
public:
    IdentifierNode(std::string _name) : ASTNode(NodeTypeIdentifier), name(_name) {}
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<IdentifierNode> IdentifierNodePtr;

class StructDefinitionNode : public ASTNode {
public:
    StructDefinitionNode(std::string _name) : ASTNode(NodeTypeStructDefinition), name(_name) {};
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<StructDefinitionNode> StructDefinitionNodePtr;

class ParameterListNode : public ASTNode {
public:
    ParameterListNode() : ASTNode(NodeTypeParameterList) {};
    std::string source_code() const;
private:
};
typedef std::shared_ptr<ParameterListNode> ParameterListNodePtr;

class EventDefinitionNode : public ASTNode {
public:
    EventDefinitionNode(std::string _name) : ASTNode(NodeTypeEventDefinition), name(_name) {}
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<EventDefinitionNode> EventDefinitionNodePtr;

class ExpressionStatementNode : public ASTNode {
public:
    ExpressionStatementNode() : ASTNode(NodeTypeExpressionStatement) {}
    std::string source_code() const;
};
typedef std::shared_ptr<ExpressionStatementNode> ExpressionStatementNodePtr;

class EmitStatementNode : public ASTNode {
public:
    EmitStatementNode() : ASTNode(NodeTypeEmitStatement) {}
    std::string source_code() const;
    //thing to emit is stored in sub nodes;
};
typedef std::shared_ptr<EmitStatementNode> EmitStatementNodePtr;

class IndexAccessNode : public ASTNode {
public:
    IndexAccessNode() : ASTNode(NodeTypeIndexAccess) {}
    std::string source_code() const;
};
typedef std::shared_ptr<IndexAccessNode> IndexAccessNodePtr;

class BinaryOperationNode : public ASTNode {
public:
    BinaryOperationNode(std::string& _op, ASTNodePtr _left, ASTNodePtr _right) : ASTNode(NodeTypeBinaryOperation), op(_op), left_hand_operand(_left), right_hand_operand(_right) {}
    std::string source_code() const;
    std::string get_op() const;
    ASTNodePtr get_left() const;
    ASTNodePtr get_right() const;
    
private:
    std::string op; // operator
    ASTNodePtr left_hand_operand;
    ASTNodePtr right_hand_operand;
};
typedef std::shared_ptr<BinaryOperationNode> BinaryOperationNodePtr;

class UnaryOperationNode : public ASTNode {
public:
    UnaryOperationNode(std::string& _op, ASTNodePtr _operand, bool _is_prefix) : ASTNode(NodeTypeUnaryOperation), op(_op), operand(_operand), is_prefix(_is_prefix) {}
    std::string source_code() const;
private:
    std::string op;
    ASTNodePtr operand;
    bool is_prefix;
};
typedef std::shared_ptr<UnaryOperationNode> UnaryOperationNodePtr;

class LiteralNode : public ASTNode {
public:
    LiteralNode(std::string _literal) : ASTNode(NodeTypeLiteral), literal(_literal) {}
    std::string source_code() const;
private:
    std::string literal;
};
typedef std::shared_ptr<LiteralNode> LiteralNodePtr;

class TupleExpressionNode : public ASTNode {
public:
    TupleExpressionNode() : ASTNode(NodeTypeTupleExpression) {}
    std::string source_code() const;
};
typedef std::shared_ptr<TupleExpressionNode> TupleExpressionNodePtr;

class BlockNode : public ASTNode {
public:
    BlockNode() : ASTNode(NodeTypeBlockNode), extra_code("") {}
    std::string source_code() const;
    void set_extra_code(std::string _extra_code);
private:
    std::string extra_code;
};
typedef std::shared_ptr<BlockNode> BlockNodePtr;

class ReturnNode : public ASTNode {
public:
    ReturnNode() : ASTNode(NodeTypeReturn) {}
    std::string source_code() const;
};
typedef std::shared_ptr<ReturnNode> ReturnNodePtr;

class ModifierDefinitionNode : public ASTNode {
public:
    ModifierDefinitionNode(std::string& _name, ParameterListNodePtr _params, ASTNodePtr _body) : ASTNode(NodeTypeModifierDefinition), name(_name), params(_params), body(_body) {}
    std::string source_code() const;
private:
    std::string name;
    ParameterListNodePtr params;
    ASTNodePtr body;
};
typedef std::shared_ptr<ModifierDefinitionNode> ModifierDefinitionNodePtr;

class ModifierInvocationNode : public ASTNode {
public:
    ModifierInvocationNode(std::string _name) : ASTNode(NodeTypeModifierInvocation), name(_name) {}
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<ModifierInvocationNode> ModifierInvocationNodePtr;

class FunctionDefinitionNode : public ASTNode {
public:
    FunctionDefinitionNode() : ASTNode(NodeTypeFunctionDefinition) {};
    FunctionDefinitionNode(std::string& _name, std::string& _qualifier, ParameterListNodePtr _params, ParameterListNodePtr _returns, BlockNodePtr _function_body) : ASTNode(NodeTypeFunctionDefinition), name(_name), qualifier(_qualifier), params(_params), returns(_returns), function_body(_function_body) {}
    std::string source_code() const;
    void add_modifier_invocation(ModifierInvocationNodePtr _modifier_invocation);
    void set_name(std::string& _name);
    void set_qualifier(std::string& _qualifier);
    void set_params(ParameterListNodePtr _params);
    void set_returns(ParameterListNodePtr _returns);
    void set_function_body(BlockNodePtr _function_body);
    void set_is_constructor(bool& _is_constructor);
private:
    std::string name;
    std::string qualifier;
    ParameterListNodePtr params;
    ParameterListNodePtr returns;
    std::vector<ModifierInvocationNodePtr> modifier_invocation;
    BlockNodePtr function_body;
    bool is_constructor;
};
typedef std::shared_ptr<FunctionDefinitionNode> FunctionDefinitionNodePtr;

class FunctionCallNode : public ASTNode {
public:
    FunctionCallNode(ASTNodePtr _callee) : ASTNode(NodeTypeFunctionCall), callee(_callee) {}
    std::string source_code() const;
private:
    ASTNodePtr callee;
    // arguments is stored in subnodes
};
typedef std::shared_ptr<FunctionCallNode> FunctionCallNodePtr;

class MemberAccessNode : public ASTNode {
public:
    MemberAccessNode(ASTNodePtr _identifier, std::string& _member) : ASTNode(NodeTypeMemberAccess), identifier(_identifier), member(_member) {}
    std::string source_code() const;
private:
    ASTNodePtr identifier;
    std::string member;
};
typedef std::shared_ptr<MemberAccessNode> MemberAccessNodePtr;

class ElementaryTypeNameExpressionNode : public ASTNode{
public:
    ElementaryTypeNameExpressionNode(std::string& _name) : ASTNode(NodeTypeElementaryTypeNameExpression), name(_name) {}
    std::string source_code() const;
    
private:
    std::string name;
};
typedef std::shared_ptr<ElementaryTypeNameExpressionNode> ElementaryTypeNameExpressionNodePtr;

class ContractDefinitionNode : public ASTNode {
public:
    ContractDefinitionNode(std::string& _name) : ASTNode(NodeTypeContractDefinition), name(_name), inherit_from{}, is_library(false), is_interface(false) {}
    std::string source_code() const;
    void add_inherit_from(std::string& _inherit_from);
    void set_as_library();
    void set_extra_function();
    void set_mutation_type();
    bool get_is_Lib();
    void set_as_interface();
    bool get_is_interface();
private:
    std::string name;
    std::vector<std::string> inherit_from;
    bool is_library;
    bool is_interface;
};
typedef std::shared_ptr<ContractDefinitionNode> ContractDefinitionNodePtr;

class IfStatementNode : public ASTNode {
public:
    IfStatementNode(ASTNodePtr _condition, ASTNodePtr _if_then, ASTNodePtr _if_else) : ASTNode(NodeTypeIfStatement), condition(_condition), if_then(_if_then), if_else(_if_else) {}
    std::string source_code() const;
private:
    ASTNodePtr condition;
    ASTNodePtr if_then;
    ASTNodePtr if_else;
};
typedef std::shared_ptr<IfStatementNode> IfStatementNodePtr;

class DoWhileStatementNode : public ASTNode {
public:
    DoWhileStatementNode(ASTNodePtr _condition, ASTNodePtr _loop_body) : ASTNode(NodeTypeDoWhileStatement), condition(_condition), loop_body(_loop_body) {}
    std::string source_code() const;
private:
    ASTNodePtr condition;
    ASTNodePtr loop_body;
};
typedef std::shared_ptr<DoWhileStatementNode> DoWhileStatementNodePtr;


class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ASTNodePtr _condition, ASTNodePtr _loop_body) : ASTNode(NodeTypeWhileStatement), condition(_condition), loop_body(_loop_body) {}
    std::string source_code() const;
private:
    ASTNodePtr condition;
    ASTNodePtr loop_body;
};
typedef std::shared_ptr<WhileStatementNode> WhileStatementNodePtr;


class ForStatementNode : public ASTNode {
public:
    ForStatementNode(ASTNodePtr _init, ASTNodePtr _condition, ASTNodePtr _increment, ASTNodePtr _body) : ASTNode(NodeTypeForStatement), init(_init), condition(_condition), increment(_increment), body(_body) {}
    std::string source_code() const;
private:
    ASTNodePtr init;
    ASTNodePtr condition;
    ASTNodePtr increment;
    ASTNodePtr body;
};
typedef std::shared_ptr<ForStatementNode> ForStatementNodePtr;

class ConditionalNode : public ASTNode {
public:
    ConditionalNode(ASTNodePtr _condition, ASTNodePtr _yes, ASTNodePtr _no) : ASTNode(NodeTypeConditional),  condition(_condition), yes(_yes), no(_no) {}
    std::string source_code() const;
private:
    ASTNodePtr condition;
    ASTNodePtr yes;
    ASTNodePtr no;
};
typedef std::shared_ptr<ConditionalNode> ConditionalNodePtr;

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(std::string _op) : ASTNode(NodeTypeAssignment), op(_op) {}
    std::string source_code() const;
private:
    std::string op;
};
typedef std::shared_ptr<AssignmentNode> AssignmentNodePtr;

class BreakNode : public ASTNode {
public:
    BreakNode() : ASTNode(NodeTypeBreak) {};
    std::string source_code() const;
};
typedef std::shared_ptr<BreakNode> BreakNodePtr;

class ContinueNode : public ASTNode {
public:
    ContinueNode() : ASTNode(NodeTypeContinue) {};
    std::string source_code() const;
};
typedef std::shared_ptr<ContinueNode> ContinueNodePtr;

class NewExpresionNode : public ASTNode {
public:
    NewExpresionNode(ASTNodePtr _type_name) : ASTNode(NodeTypeNewExpression), type_name(_type_name) {}
    std::string source_code() const;
private:
    ASTNodePtr type_name;
};
typedef std::shared_ptr<NewExpresionNode> NewExpresionNodePtr;

class EnumDefinitionNode : public ASTNode {
public:
    EnumDefinitionNode(std::string _name) : ASTNode(NodeTypeEnumDefinition), name(_name) {}
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<EnumDefinitionNode> EnumDefinitionNodePtr;

class EnumValueNode : public ASTNode {
public:
    EnumValueNode(std::string _name) : ASTNode(NodeTypeEnumValue), name(_name) {}
    std::string source_code() const;
private:
    std::string name;
};
typedef std::shared_ptr<EnumValueNode> EnumValueNodePtr;

class ThrowNode : public ASTNode {
public:
    ThrowNode() : ASTNode(NodeTypeThrow) {};
    std::string source_code() const;
};
typedef std::shared_ptr<ThrowNode> ThrowNodePtr;

class PlaceHolderStatement : public ASTNode {
public:
    PlaceHolderStatement(std::string _place_holder) : ASTNode(NodeTypePlaceholderStatement), placeholder(_place_holder) {};
    std::string source_code() const;
private:
    std::string placeholder;
};
typedef std::shared_ptr<PlaceHolderStatement> PlaceHolderStatementPtr;

class MappingNode : public ASTNode {
public:
    MappingNode(ASTNodePtr _key_type, ASTNodePtr _value_type) : ASTNode(NodeTypeMapping), key_type(_key_type), value_type(_value_type) {}
    std::string source_code() const;
private:
    ASTNodePtr key_type;
    ASTNodePtr value_type;
};
typedef std::shared_ptr<MappingNode> MappingNodePtr;

class ElementaryTypeNameNode : public ASTNode {
public:
    ElementaryTypeNameNode(std::string _type_name) : ASTNode(NodeTypeElementaryTypeName), type_name(_type_name) {}
    std::string source_code() const;
private:
    std::string type_name;
};
typedef std::shared_ptr<ElementaryTypeNameNode> ElementaryTypeNameNodePtr;

class UserDefinedTypeNameNode : public ASTNode {
public:
    UserDefinedTypeNameNode(std::string _type_name) : ASTNode(NodeTypeUserDefinedTypeName), type_name(_type_name) {}
    std::string source_code() const;
private:
    std::string type_name;
};
typedef std::shared_ptr<UserDefinedTypeNameNode> UserDefinedTypeNameNodePtr;

class FunctionTypeNameNode : public ASTNode {
public:
    FunctionTypeNameNode(ASTNodePtr _params, ASTNodePtr _returns) : ASTNode(NodeTypeFunctionTypeName), params(_params), returns(_returns) {}
    std::string source_code() const;
private:
    ASTNodePtr params;
    ASTNodePtr returns;
};
typedef std::shared_ptr<FunctionTypeNameNode> FunctionTypeNameNodePtr;

class ArrayTypeNameNode : public ASTNode {
public:
    ArrayTypeNameNode(ASTNodePtr _base_type, ASTNodePtr _size) : ASTNode(NodeTypeArrayTypeName), base_type(_base_type), size(_size) {}
    std::string source_code() const;
private:
    ASTNodePtr base_type;
    ASTNodePtr size;
};
typedef std::shared_ptr<ArrayTypeNameNode> ArrayTypeNameNodePtr;

class InlineAssemblyNode : public ASTNode {
public:
    InlineAssemblyNode() : ASTNode(NodeTypeInlineAssembly) {};
    void set_source(std::string& _source);
    std::string get_source();
    std::string source_code() const;
private:
    std::string source;
};
typedef std::shared_ptr<InlineAssemblyNode> InlineAssemblyNodePtr;

}