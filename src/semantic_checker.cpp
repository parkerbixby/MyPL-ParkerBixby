//----------------------------------------------------------------------
// FILE: semantic_checker.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Parker Bixby
// DESC: Semantic Checker
//----------------------------------------------------------------------

#include <unordered_set>
#include "mypl_exception.h"
#include "semantic_checker.h"
#include <iostream>

using namespace std;

// hash table of names of the base data types and built-in functions
const unordered_set<string> BASE_TYPES{"int", "double", "char", "string", "bool"};
const unordered_set<string> BUILT_INS{"print", "input", "to_string", "to_int",
                                      "to_double", "length", "get", "concat"};

// helper functions

optional<VarDef> SemanticChecker::get_field(const StructDef &struct_def,
                                            const string &field_name)
{
  for (const VarDef &var_def : struct_def.fields)
    if (var_def.var_name.lexeme() == field_name)
      return var_def;
  return nullopt;
}

void SemanticChecker::error(const string &msg, const Token &token)
{
  string s = msg;
  s += " near line " + to_string(token.line()) + ", ";
  s += "column " + to_string(token.column());
  throw MyPLException::StaticError(s);
}

void SemanticChecker::error(const string &msg)
{
  throw MyPLException::StaticError(msg);
}

// visitor functions

void SemanticChecker::visit(Program &p)
{
  // record each struct def
  for (StructDef &d : p.struct_defs)
  {
    string name = d.struct_name.lexeme();
    if (struct_defs.contains(name))
      error("multiple definitions of '" + name + "'", d.struct_name);
    struct_defs[name] = d;
  }
  // record each function def (need a main function)
  bool found_main = false;
  for (FunDef &f : p.fun_defs)
  {
    string name = f.fun_name.lexeme();
    if (BUILT_INS.contains(name))
      error("redefining built-in function '" + name + "'", f.fun_name);
    if (fun_defs.contains(name))
      error("multiple definitions of '" + name + "'", f.fun_name);
    if (name == "main")
    {
      if (f.return_type.type_name != "void")
        error("main function must have void type", f.fun_name);
      if (f.params.size() != 0)
        error("main function cannot have parameters", f.params[0].var_name);
      found_main = true;
    }
    fun_defs[name] = f;
  }
  if (!found_main)
    error("program missing main function");
  // check each struct
  for (StructDef &d : p.struct_defs)
    d.accept(*this);
  // check each function
  for (FunDef &d : p.fun_defs)
    d.accept(*this);
}

void SemanticChecker::visit(SimpleRValue &v)
{
  if (v.value.type() == TokenType::INT_VAL)
    curr_type = DataType{false, "int"};
  else if (v.value.type() == TokenType::DOUBLE_VAL)
    curr_type = DataType{false, "double"};
  else if (v.value.type() == TokenType::CHAR_VAL)
    curr_type = DataType{false, "char"};
  else if (v.value.type() == TokenType::STRING_VAL)
    curr_type = DataType{false, "string"};
  else if (v.value.type() == TokenType::BOOL_VAL)
    curr_type = DataType{false, "bool"};
  else if (v.value.type() == TokenType::NULL_VAL)
    curr_type = DataType{false, "void"};
}

// TODO: Implement the rest of the visitor functions (stubbed out below)

// Walks through all the statements, sets the return type, and sets the parameter types and arg count
void SemanticChecker::visit(FunDef &f)
{
  DataType return_type = f.return_type;
  if (return_type.type_name != "int" && return_type.type_name != "string" && return_type.type_name != "char" && return_type.type_name != "double" && return_type.type_name != "void" && return_type.type_name != "bool")
  {
    if (!struct_defs.contains(return_type.type_name))
    {
      error("Return type is invalid");
    }
  }
  for (int i = 0; i < f.params.size(); i++)
  {
    if (f.params[i].data_type.is_const && f.params[i].data_type.is_array)
    {
      error("Invalid parameter of type const and array", f.params[i].var_name);
    }
    if (f.params[i].data_type.type_name != "int" && f.params[i].data_type.type_name != "double" && f.params[i].data_type.type_name != "string" && f.params[i].data_type.type_name != "char" && f.params[i].data_type.type_name != "bool")
    {
      if (!symbol_table.name_exists(f.params[i].data_type.type_name) && !struct_defs.contains(f.params[i].data_type.type_name))
      {
        error("Invalid parameter type", f.params[i].var_name);
      }
    }
    for (int j = i + 1; j < f.params.size(); j++)
    {
      if (f.params[i].var_name.lexeme() == f.params[j].var_name.lexeme())
      {
        error("Parameter has duplicate variable name", f.params[i].var_name);
      }
    }
  }
  symbol_table.push_environment();
  symbol_table.add("return", return_type);
  for (int i = 0; i < f.params.size(); i++)
  {
    symbol_table.add(f.params[i].var_name.lexeme(), f.params[i].data_type);
  }
  for (auto s : f.stmts)
  {
    s->accept(*this);
  }
  symbol_table.pop_environment();
}

// Checks to see if the types of the fields in a struct are valid, and if they are already declared
void SemanticChecker::visit(StructDef &s)
{
  for (int i = 0; i < s.fields.size(); i++)
  {
    if (s.fields[i].data_type.type_name != "int" && s.fields[i].data_type.type_name != "double" && s.fields[i].data_type.type_name != "string" && s.fields[i].data_type.type_name != "char" && s.fields[i].data_type.type_name != "bool")
    {
      if (!symbol_table.name_exists(s.fields[i].data_type.type_name) && (!struct_defs.contains(s.fields[i].data_type.type_name)))
      {
        error("Invalid parameter type", s.fields[i].var_name);
      }
    }
    for (int j = i + 1; j < s.fields.size(); j++)
    {
      if (s.fields[i].var_name.lexeme() == s.fields[j].var_name.lexeme())
      {
        error("Parameter has duplicate variable name", s.fields[i].var_name);
      }
    }
  }
  symbol_table.push_environment();

  for (int i = 0; i < s.fields.size(); i++)
  {
    symbol_table.add(s.fields[i].var_name.lexeme(), s.fields[i].data_type);
  }
  symbol_table.pop_environment();
}

// Checks to see if the return type is the same as the function declares it
void SemanticChecker::visit(ReturnStmt &s)
{
  s.expr.accept(*this);
  DataType expected_type = symbol_table.get("return").value();
  if (curr_type.type_name != expected_type.type_name && curr_type.type_name != "void")
    error("Type mismatch", s.expr.first_token());
}

// Checks to see if the condition is a type bool and accepts all the statements
void SemanticChecker::visit(WhileStmt &s)
{
  symbol_table.push_environment();
  s.condition.accept(*this);
  if (curr_type.type_name != "bool" or curr_type.is_array)
  {
    error("Type mismatch for type " + s.condition.first_token().lexeme());
  }
  for (auto t : s.stmts)
  {
    t->accept(*this);
  }
  symbol_table.pop_environment();
}

// Push's environment, then accepts the var decl, then the condition to see if the type is correct
// Then walks through all the statements
void SemanticChecker::visit(ForStmt &s)
{
  symbol_table.push_environment();
  s.var_decl.accept(*this);
  s.condition.accept(*this);
  if (curr_type.type_name != "bool" or curr_type.is_array)
  {
    error("Type mismatch for type " + s.condition.first_token().lexeme());
  }
  s.assign_stmt.accept(*this);

  for (auto t : s.stmts)
  {
    t->accept(*this);
  }
  symbol_table.pop_environment();
}

// Checks to see if the If condition is bool, accepts all the statements then does the same
// for if else, then else
void SemanticChecker::visit(IfStmt &s)
{
  symbol_table.push_environment();
  s.if_part.condition.accept(*this);
  if (curr_type.type_name != "bool" || curr_type.is_array)
  {
    error("Type mismatch for type" + s.if_part.condition.first_token().lexeme());
  }
  for (auto t : s.if_part.stmts)
  {
    t->accept(*this);
  }
  symbol_table.pop_environment();
  for (auto e : s.else_ifs)
  {
    e.condition.accept(*this);
    if (curr_type.type_name != "bool")
    {
      error("Type mismatch for type" + s.if_part.condition.first_token().lexeme());
    }
    symbol_table.push_environment();
    for (int i = 0; i < e.stmts.size(); i++)
    {
      e.stmts[i]->accept(*this);
    }
    symbol_table.pop_environment();
  }
  symbol_table.push_environment();
  for (auto e : s.else_stmts)
  {
    e->accept(*this);
  }
  symbol_table.pop_environment();
}

// Checks to see if the Var has already been named in the current environment
// If it isn't then it checks the type if it is valid
void SemanticChecker::visit(VarDeclStmt &s)
{
  if (s.var_def.data_type.type_name != "int" && s.var_def.data_type.type_name != "string" && s.var_def.data_type.type_name != "char" && s.var_def.data_type.type_name != "double" && s.var_def.data_type.type_name != "bool")
  {
    if ((!struct_defs.contains(s.var_def.data_type.type_name)))
    {
      error("Invalid data type type", s.var_def.var_name);
    }
  }
  if (s.var_def.data_type.is_array)
  {
    curr_type = DataType{true, s.var_def.data_type.type_name};
  }
  else{
    curr_type = DataType{false, s.var_def.data_type.type_name};
  }
  if (symbol_table.name_exists_in_curr_env(s.var_def.var_name.lexeme()))
  {
    error("Multiple vars of name " + s.var_def.var_name.lexeme() + " in current environment", s.var_def.var_name);
  }
  symbol_table.add(s.var_def.var_name.lexeme(), s.var_def.data_type);
  DataType lhs = {curr_type.is_array, curr_type.type_name};
  s.expr.accept(*this);
  if (((curr_type.type_name != s.var_def.data_type.type_name) and (curr_type.type_name != "void")))
  {
    if (s.var_def.data_type.is_array)
    {
      curr_type.is_array = true;
    }
    else
    {
      error("Type mismatch", s.var_def.var_name);
    }
  }
}
// Accepts the expression, if the size isn't 1 it will walk through it and check if the type is
// Correct and if not give an error
void SemanticChecker::visit(AssignStmt &s)
{
  s.expr.accept(*this);
  DataType rhs = curr_type;
  if (s.lvalue.size() < 2)
  {
    DataType lhs = *symbol_table.get(s.lvalue[0].var_name.lexeme());
    if (curr_type.type_name != lhs.type_name && curr_type.type_name != "void")
    {
      error("Type mismatch for", s.lvalue[0].var_name);
    }
    if (lhs.is_const)
    {
      error("Cannot re-assign values of a constant type", s.lvalue[0].var_name);
    }
  }
  string var_name = s.lvalue[0].var_name.lexeme();
  if (symbol_table.name_exists(var_name))
  {
    curr_type = DataType{symbol_table.get(var_name)->is_array, symbol_table.get(var_name).value().type_name};
    if (struct_defs.contains(curr_type.type_name))
    {
      for (int i = 1; i < s.lvalue.size(); i++)
      {
        string var_name2 = s.lvalue[i].var_name.lexeme();
        VarDef field = get_field(struct_defs[curr_type.type_name], var_name2).value();
        curr_type = {field.data_type.is_array, field.data_type.type_name};
      }
      if (curr_type.is_const)
      {
        error("Cannot re-assign values of a constant type", s.lvalue.back().var_name);
      }
      if(curr_type.type_name != rhs.type_name){
        error("Type mismatch for", s.lvalue.back().var_name);
      }
    }
  }
  else
  {
    error("Use before definition", s.lvalue[0].var_name);
  }
}

// Checks to see if the function called is a reserved function, then sets the type, and see's if the arguments passed
// In is correct, with the correct types
void SemanticChecker::visit(CallExpr &e)
{
  string fun_name = e.fun_name.lexeme();
  if (fun_name == "print")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters!", e.first_token());
    }
    e.args[0].accept(*this);
    if (struct_defs.contains(curr_type.type_name))
    {
      error("Cannot print type struct", e.first_token());
    }
    if (curr_type.is_array)
    {
      error("Invalid parameter for argument cannot have an array", e.first_token());
    }
    curr_type = {false, "void"};
  }
  else if (fun_name == "input")
  {
    if (e.args.size() != 0)
    {
      error("Invalid number of parameters", e.first_token());
    }
    curr_type = {false, "string"};
  }
  else if (fun_name == "to_string")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name == "bool" || curr_type.type_name == "void" || curr_type.type_name == "string")
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    if (curr_type.is_array)
    {
      error("Invalid parameter for argument cannot have an array", e.first_token());
    }
    curr_type = {false, "string"};
  }
  else if (fun_name == "to_int")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if ((curr_type.type_name == "int") || (curr_type.type_name == "void") || (curr_type.type_name == "bool"))
    {
      error("Invalid parameter for argument", e.first_token());
    }
    if (curr_type.is_array)
    {
      error("Invalid parameter for argument cannot have an array", e.first_token());
    }
    curr_type = {false, "int"};
  }
  else if (fun_name == "to_double")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters!", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name == "int" || curr_type.type_name == "string")
    {
      curr_type = {false, "double"};
    }
    else if (curr_type.is_array == true)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    else
    {
      error("Invalid parameter for argument ", e.first_token());
    }
  }
  else if (fun_name == "get")
  {
    if (e.args.size() != 2)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name != "int" || curr_type.is_array)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    e.args[1].accept(*this);
    if (curr_type.is_array || (curr_type.type_name != "string"))
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    curr_type = {false, "char"};
  }
  else if (fun_name == "concat")
  {
    if (e.args.size() != 2)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name != "string" || curr_type.is_array != false)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    e.args[1].accept(*this);
    if (curr_type.type_name != "string" || curr_type.is_array != false)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    curr_type = {false, "string"};
  }
  else if (fun_name == "length")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name != "string")
    {
      if (curr_type.is_array == false)
      {
        error("Invalid parameter for argument ", e.first_token());
      }
    }
    curr_type = {false, "int"};
  }
  else if (fun_name == "length_array")
  {
    if (e.args.size() != 1)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.is_array == false)
    {
      error("Cannot call length_array on a non-array type", e.first_token());
    }
    curr_type = {false, "int"};
  }
  else if (fun_name == "rand_int")
  {
    if (e.args.size() != 2)
    {
      error("Invalid number of parameters", e.first_token());
    }
    e.args[0].accept(*this);
    if (curr_type.type_name != "int" || curr_type.is_array != false)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    e.args[1].accept(*this);
    if (curr_type.type_name != "int" || curr_type.is_array != false)
    {
      error("Invalid parameter for argument ", e.first_token());
    }
    curr_type = {false, "int"};
  }
  else
  {
    if (fun_defs.contains(fun_name))
    {
      FunDef f = fun_defs[fun_name];
      if (e.args.size() != f.params.size())
      {
        error("Invalid number of parameters passed in", e.first_token());
      }
      for (int i = 0; i < e.args.size(); i++)
      {
        DataType params = f.params[i].data_type;
        e.args[i].accept(*this);
        if (curr_type.type_name != params.type_name || curr_type.is_array != params.is_array)
        {
          if (curr_type.type_name != "void")
          {
            error("Mismatch type for parameters", e.first_token());
          }
        }
      }
      curr_type = DataType{f.return_type.is_array, f.return_type.type_name};
    }
    else
    {
      error("Function used before defined", e.first_token());
    }
  }
}

void SemanticChecker::visit(Expr &e)
{
  e.first->accept(*this);
  DataType lhs = curr_type;
  if (e.op.has_value())
  {
    e.rest->accept(*this);
    DataType rhs = curr_type;
    if (e.op.value().lexeme() == "+" or e.op.value().lexeme() == "-" or e.op.value().lexeme() == "*" or e.op.value().lexeme() == "/")
    {
      if ((lhs.type_name != rhs.type_name) and (lhs.type_name != "double" && lhs.type_name != "int"))
      {
        error("Type mismatch, cannot use type" + lhs.type_name + "with " + e.op.value().lexeme() + " operator", e.first_token());
      }
    }
    else if (e.op.value().lexeme() == "==" or e.op.value().lexeme() == "!=")
    {
      if (lhs.type_name != rhs.type_name)
      {
        if (lhs.type_name == "void" or rhs.type_name == "void")
        {
          curr_type = {false, "bool"};
        }
        else
        {
          error("Type mismatch, cannot use type" + lhs.type_name + "with " + e.op.value().lexeme() + " operator", e.first_token());
        }
      }
      else
      {
        curr_type = {false, "bool"};
      }
    }
    else if (e.op.value().lexeme() == "<" or e.op.value().lexeme() == "<=" or e.op.value().lexeme() == ">" or e.op.value().lexeme() == ">=")
    {
      if (lhs.type_name != rhs.type_name)
      {
        error("Type Mismatch, must have same type while using" + e.op.value().lexeme() + "operator", e.op.value());
      }
      if (lhs.type_name == "int" or lhs.type_name == "double" or lhs.type_name == "char" or lhs.type_name == "string")
      {
        curr_type.type_name = "bool";
      }
      else if (lhs.type_name == "bool" || rhs.type_name == "bool")
      {
        error("Type mismatch, cannot use type" + lhs.type_name + "with " + e.op.value().lexeme() + " operator", e.first_token());
      }
    }
    else if (e.op.value().lexeme() == "and" or e.op.value().lexeme() == "or" or e.op.value().lexeme() == "not")
    {
      if (lhs.type_name != "bool" || rhs.type_name != "bool")
      {
        error("Type mismatch, cannot use type" + lhs.type_name + "with " + e.op.value().lexeme() + " operator", e.first_token());
      }
      curr_type.type_name = "bool";
    }
  }
}

void SemanticChecker::visit(SimpleTerm &t)
{
  t.rvalue->accept(*this);
}

void SemanticChecker::visit(ComplexTerm &t)
{
  t.expr.accept(*this);
}

void SemanticChecker::visit(NewRValue &v)
{
  if (v.type.lexeme() != "int" && v.type.lexeme() != "string" && v.type.lexeme() != "char" && v.type.lexeme() != "double" && v.type.lexeme() != "bool")
  {
    if (!symbol_table.name_exists(v.type.lexeme()) && (!struct_defs.contains(v.type.lexeme())))
    {
      error("Invalid data type type", v.type);
    }
  }
  DataType lhs = curr_type;
  if (v.array_expr.has_value())
  {
    curr_type = {true, v.type.lexeme()};
  }
  else if (v.const_array.size() >= 1)
  {
    curr_type = {true, v.type.lexeme()};
    if(lhs.type_name != curr_type.type_name){
      error("Type mismatch", v.type);
    }
    for (int i = 0; i < v.const_array.size(); i++)
    {
      v.const_array[i].accept(*this);
      if (v.type.lexeme() != curr_type.type_name )
      {
          error("Type mismatch inside array expr");
      }
    }
  }
  else{
    curr_type = {false, v.type.lexeme()};
  }
}

void SemanticChecker::visit(VarRValue &v)
{
  string var_name = v.path[0].var_name.lexeme();
  if (symbol_table.name_exists(var_name))
  {
    curr_type = DataType{symbol_table.get(var_name)->is_array, symbol_table.get(var_name).value().type_name};
    if (struct_defs.contains(curr_type.type_name))
    {
      for (int i = 1; i < v.path.size(); i++)
      {
        string var_name2 = v.path[i].var_name.lexeme();
        VarDef field = get_field(struct_defs[curr_type.type_name], var_name2).value();
        curr_type = {field.data_type.is_array, field.data_type.type_name};
      }
    }
    if (v.path.back().array_expr.has_value())
    {
      curr_type.is_array = false;
    }
  }
  else
  {
    error("Use before definition", v.path[0].var_name);
  }
}
