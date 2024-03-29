#include "ast.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& ss, Node* node) {
  if(node->type_is(Node::Type::TypeFunc)) {
    ss << "(";
    for(auto n : node->type_members()) ss << n.first << " : " << n.second << ", ";
    ss << ") => " << node->type_return();
  } else if(node->type_is(Node::Type::TypeStruct)) {
    ss << "{ ";
    for(auto n : node->type_members()) ss << n.first << " : " << n.second << ", ";
    ss << "}";
  } else if(node->type_is(Node::Type::TypeArray)) {
    ss << "[" << node->childs.at(1) << "]" << node->childs.at(0);
  } else if(node->type_is(Node::Type::TypePointer)) {
    ss << "*" << node->childs.at(0);
  } else if(node->type_is(Node::Type::TypePrim)) {
    ss << node->childs.at(0);
  } else if(node->type_is(Node::Type::Program)) {
    for(auto n : node->childs) ss << n << std::endl;
  } else if(node->type_is(Node::Type::FuncDef)) {
    ss << "func " << node->def_name()
       << " : " << node->def_type()
       << " " << node->func_body();
  } else if(node->type_is(Node::Type::GVarDef)) {
    ss << "var " << node->childs.at(0)
       << " : " << node->childs.at(1)
       << ";";
  } else if(node->type_is(Node::Type::TypeDef)) {
    ss << "type " << node->childs.at(0)
       << " : " << node->childs.at(1)
       << ";";
  } else if(node->type_is(Node::Type::Compound)) {
    ss << "{" << std::endl;
    for(auto n : node->childs) ss << "  " << n << std::endl;
    ss << "}";
  } else if(node->type_is(Node::Type::VoidStmt)) {
    ss << ";" << std::endl;
  } else if(node->type_is(Node::Type::ExprStmt)) {
    ss << node->childs.at(0) << ";";
  } else if(node->type_is(Node::Type::LVarDef)) {
    ss << "var " << node->childs.at(0) << " : " << node->childs.at(1) << ";";
  } else if(node->type_is(Node::Type::Assign)) {
    ss << node->childs.at(0) << " = " << node->childs.at(1) << ";";
  } else if(node->type_is(Node::Type::UAssign)) {
    ss << node->childs.at(0);
  } else if(node->type_is(Node::Type::If)) {
    ss << "if " << node->ctrl_cond() << node->ctrl_true();
  } else if(node->type_is(Node::Type::IfElse)) {
    ss << "if " << node->ctrl_cond() << node->ctrl_true() << "else" << node->ctrl_false();
  } else if(node->type_is(Node::Type::Goto)) {
    ss << "goto " << node->childs.at(0) << ";";
  } else if(node->type_is(Node::Type::Label)) {
    ss << node->childs.at(0) << ":";
  } else if(node->type_is(Node::Type::Return)) {
    ss << "return " << node->childs.at(0) << ";";
  } else if(node->type_is(Node::Type::While)) {
    ss << "while " << node->ctrl_cond() << node->ctrl_body();
  } else if(node->type_is(Node::Type::DoWhile)) {
    ss << "do " << node->ctrl_body() << "while" << node->ctrl_cond() << ";";
  } else if(node->type_is(Node::Type::For)) {
    ss << "for " << node->ctrl_init() << node->ctrl_cond() << node->ctrl_iterate() << node->ctrl_body();
  } else if(node->type_is(Node::Type::Continue)) {
    ss << "continue;";
  } else if(node->type_is(Node::Type::Break)) {
    ss << "break;";
  } else if(node->type == Node::Type::Cond) {
    ss << "(" << node->tri_cond() << " ? " << node->tri_true() << " : " << node->tri_false() << ")";
  } else if(node->type == Node::Type::LogicalOr) {
    ss << "(" << node->lhs() << " || " << node->rhs() << ")";
  } else if(node->type == Node::Type::LogicalAnd) {
    ss << "(" << node->lhs() << " && " << node->rhs() << ")";
  } else if(node->type == Node::Type::BitOr)
    ss << "(" << node->lhs() << " | " << node->rhs() << ")";
  else if(node->type == Node::Type::BitXor) {
    ss << "(" << node->lhs() << " ^ " << node->rhs() << ")";
  } else if(node->type == Node::Type::BitAnd) {
    ss << "(" << node->lhs() << " & " << node->rhs() << ")";
  } else if(node->type == Node::Type::EQ) {
    ss << "(" << node->lhs() << " == " << node->rhs() << ")";
  } else if(node->type == Node::Type::NEQ) {
    ss << "(" << node->lhs() << " != " << node->rhs() << ")";
  } else if(node->type == Node::Type::LT) {
    ss << "(" << node->lhs() << " < " << node->rhs() << ")";
  } else if(node->type == Node::Type::LEQ) {
    ss << "(" << node->lhs() << " <= " << node->rhs() << ")";
  } else if(node->type == Node::Type::GT) {
    ss << "(" << node->lhs() << " > " << node->rhs() << ")";
  } else if(node->type == Node::Type::GEQ) {
    ss << "(" << node->lhs() << " >= " << node->rhs() << ")";
  } else if(node->type == Node::Type::RShift) {
    ss << "(" << node->lhs() << " >> " << node->rhs() << ")";
  } else if(node->type == Node::Type::LShift) {
    ss << "(" << node->lhs() << " << " << node->rhs() << ")";
  } else if(node->type == Node::Type::Add) {
    ss << "(" << node->lhs() << " + " << node->rhs() << ")";
  } else if(node->type == Node::Type::Sub) {
    ss << "(" << node->lhs() << " - " << node->rhs() << ")";
  } else if(node->type == Node::Type::Mul) {
    ss << "(" << node->lhs() << " ** " << node->rhs() << ")";
  } else if(node->type == Node::Type::Div) {
    ss << "(" << node->lhs() << " // " << node->rhs() << ")";
  } else if(node->type == Node::Type::Mod) {
    ss << "(" << node->lhs() << " %% " << node->rhs() << ")";
  } else if(node->type == Node::Type::Cast) {
    ss << node->childs.at(0) << ":" << node->childs.at(1);
  } else if(node->type == Node::Type::Ref) {
    ss << node->childs.at(0) << "*";
  } else if(node->type == Node::Type::Addr) {
    ss << node->childs.at(0) << "@";
  } else if(node->type == Node::Type::Array) {
    ss << node->childs.at(0) << "[" << node->childs.at(1) << "]";
  } else if(node->type == Node::Type::Member) {
    ss << node->childs.at(0) << "." << node->childs.at(1);
  } else if(node->type == Node::Type::FuncCall) {
    ss << node->childs.at(0) << "()";
  } else if(node->type == Node::Type::Num) {
    ss << std::to_string(node->val);
  } else if(node->type == Node::Type::Ident) {
    ss << node->str;
  } else {
    ss << "NO PRINT FORMAT";
  }
  return ss;
}
