//
// Created by bowma on 11/5/2025.
//

#ifndef RISCCOMPILER_ASTNODE_H
#define RISCCOMPILER_ASTNODE_H
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * Enum for AST node types
 */
enum class ASTNodeType {
    Variable,
    Definition,
    Operation,
    Control
};

enum class SubType {
    BinaryExpression,
    Comparison,
    UnaryExpression,
    IfExpression,
    WhileExpression,
    NameStatement,
    TypeStatement,
    AssignmentStatement,
    DataStatement,
    MacroStatement,
    FunctionStatement,
};

using STR = std::string;
template<typename T>
using VEC = std::vector<T>;
using ANT = ASTNodeType;
using ST = SubType;
template<typename K, typename V>
using MAP = std::pmr::unordered_map<K, V>;

inline MAP<ST, ANT> STHierarchy = {{ST::BinaryExpression, ANT::Control}, {ST::Comparison, ANT::Control}, {ST::UnaryExpression, ANT::Operation},
    {ST::IfExpression, ANT::Control}, {ST::WhileExpression, ANT::Control}, {ST::NameStatement, ANT::Variable}, {ST::TypeStatement, ANT::Variable},
    {ST::AssignmentStatement, ANT::Definition}, {ST::FunctionStatement, ANT::Definition}, {ST::MacroStatement, ANT::Definition},
    {ST::DataStatement, ANT::Variable}};

class ASTNode {
public:
    ASTNodeType type;
    SubType subType;


    STR nodeName;
    STR value;
    VEC<ASTNode> children;

    // Line and Column MetaData
    int line;
    int column;

    /**
     * Initialize standard AST node
     *
     * @param type generic type
     * @param subtype sub/specific type information
     * @param line which line read from
     * @param column which item on the line it was built from
     * @param nodeName what is this node
     * @param value what does this node contain
     * @param children does this node have child nodes
     */
    explicit ASTNode(const ASTNodeType type, const SubType subtype, const int line, const int column, STR nodeName = "", STR value = "", VEC<ASTNode> children = {})
        : type(type), subType(subtype), nodeName(std::move(nodeName)), value(std::move(value)), children(std::move(children)),
          line(line), column(column) {
    }

    /**
     * Nicely format AST nodes for printing
     *
     * @return Nicely formatted string representation of an AST node
     */
    [[nodiscard]] STR prettyPrint() const;

    /**
     * add child to list of child nodes
     *
     * @param child child node to add
     */
    void addChild(const ASTNode& child);
};


#endif //RISCCOMPILER_ASTNODE_H