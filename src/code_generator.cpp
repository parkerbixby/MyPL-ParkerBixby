//----------------------------------------------------------------------
// FILE: code_generator.cpp
// DATE: CPSC 326, Spring 2023
// AUTH:
// DESC:
//----------------------------------------------------------------------

#include <iostream> // for debugging
#include "code_generator.h"
#include <random>

using namespace std;

// helper function to replace all occurrences of old string with new
void replace_all(string &s, const string &old_str, const string &new_str)
{
  while (s.find(old_str) != string::npos)
    s.replace(s.find(old_str), old_str.size(), new_str);
}

CodeGenerator::CodeGenerator(VM &vm)
    : vm(vm)
{
}

void CodeGenerator::visit(Program &p)
{
  for (auto &struct_def : p.struct_defs)
    struct_def.accept(*this);
  for (auto &fun_def : p.fun_defs)
    fun_def.accept(*this);
}

void CodeGenerator::visit(FunDef &f)
{
  string name = f.fun_name.lexeme();
  int parameters = f.params.size();
  VMFrameInfo frame = {name, parameters};
  curr_frame = frame;
  var_table.push_environment();
  for (int i = 0; i < f.params.size(); i++)
  {
    curr_frame.instructions.push_back(VMInstr::STORE(i));
    var_table.add(f.params[i].var_name.lexeme());
  }
  for (auto s : f.stmts)
  {
    s->accept(*this);
  }
  if (curr_frame.instructions.empty() || curr_frame.instructions.back().opcode() != OpCode::RET)
  {
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
    curr_frame.instructions.push_back(VMInstr::RET());
  }
  vm.add(curr_frame);
  var_table.pop_environment();
  next_var_index = 0;
}

void CodeGenerator::visit(StructDef &s)
{
  struct_defs[s.struct_name.lexeme()] = s;
}

void CodeGenerator::visit(ReturnStmt &s)
{
  s.expr.accept(*this);
  curr_frame.instructions.push_back(VMInstr::RET());
}

void CodeGenerator::visit(WhileStmt &s)
{
  int starting_index = curr_frame.instructions.size();
  s.condition.accept(*this);
  int jmpf_index = curr_frame.instructions.size();
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  var_table.push_environment();
  for (auto &s : s.stmts)
    s->accept(*this);
  var_table.pop_environment();
  curr_frame.instructions.push_back(VMInstr::JMP(starting_index));
  curr_frame.instructions.push_back(VMInstr::NOP());
  int nop_index = curr_frame.instructions.size() - 1;
  curr_frame.instructions.at(jmpf_index).set_operand(nop_index);
}

void CodeGenerator::visit(ForStmt &s)
{
  var_table.push_environment();
  s.var_decl.accept(*this);
  int starting_index = curr_frame.instructions.size();
  s.condition.accept(*this);
  int jmpf_index = curr_frame.instructions.size();
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  var_table.push_environment();
  for (auto d : s.stmts)
    d->accept(*this);
  var_table.pop_environment();
  s.assign_stmt.accept(*this);
  var_table.pop_environment();
  curr_frame.instructions.push_back(VMInstr::JMP(starting_index));
  curr_frame.instructions.push_back(VMInstr::NOP());
  int nop_index = curr_frame.instructions.size() - 1;
  curr_frame.instructions.at(jmpf_index).set_operand(nop_index);
}

void CodeGenerator::visit(IfStmt &s)
{
  vector<int> jmp;
  vector<int> jmpf;
  s.if_part.condition.accept(*this);
  jmpf.push_back(curr_frame.instructions.size());
  curr_frame.instructions.push_back(VMInstr::JMPF(-1));
  var_table.push_environment();
  for (auto &l : s.if_part.stmts)
    l->accept(*this);
  var_table.pop_environment();
  jmp.push_back(curr_frame.instructions.size());
  curr_frame.instructions.push_back(VMInstr::JMP(-1));
  jmpf.push_back(curr_frame.instructions.size());
  curr_frame.instructions.push_back(VMInstr::NOP());
  curr_frame.instructions.at(jmpf[0]).set_operand(jmpf[1]);

  for (auto e : s.else_ifs)
  {
    int else_counter = 2;
    e.condition.accept(*this);
    jmpf.push_back(curr_frame.instructions.size());
    curr_frame.instructions.push_back(VMInstr::JMPF(-1));
    var_table.push_environment();
    for (auto &l : e.stmts)
      l->accept(*this);
    var_table.pop_environment();
    jmp.push_back(curr_frame.instructions.size());
    curr_frame.instructions.push_back(VMInstr::JMP(-1));
    jmpf.push_back(curr_frame.instructions.size());
    curr_frame.instructions.push_back(VMInstr::NOP());
    curr_frame.instructions.at(jmpf[else_counter]).set_operand(jmpf[else_counter + 1]);
    else_counter += 2;
  }
  for (auto p : s.else_stmts)
  {
    p->accept(*this);
  }
  int end = curr_frame.instructions.size();
  curr_frame.instructions.push_back(VMInstr::NOP());
  for (auto i : jmp)
  {
    curr_frame.instructions.at(i).set_operand(end);
  }
}

void CodeGenerator::visit(VarDeclStmt &s)
{
  s.expr.accept(*this);
  var_table.add(s.var_def.var_name.lexeme());
  curr_frame.instructions.push_back(VMInstr::STORE(var_table.get(s.var_def.var_name.lexeme())));
}

void CodeGenerator::visit(AssignStmt &s)
{
  curr_frame.instructions.push_back(VMInstr::LOAD(var_table.get(s.lvalue[0].var_name.lexeme())));
  for (int i = 0; i < s.lvalue.size(); i++)
  {
    VarRef v = s.lvalue[i];
    string v_name = v.var_name.lexeme();
    if (i != 0)
    {
      curr_frame.instructions.push_back(VMInstr::GETF(v_name));
    }
    if (v.array_expr.has_value())
    {
      v.array_expr->accept(*this);
      curr_frame.instructions.push_back(VMInstr::GETI());
    }
  }
  curr_frame.instructions.pop_back();
  s.expr.accept(*this);
  if (s.lvalue.size() > 1 && s.lvalue.back().array_expr == nullopt)
  {
    curr_frame.instructions.push_back(VMInstr::SETF(s.lvalue.back().var_name.lexeme()));
  }
  else if (s.lvalue.back().array_expr != nullopt)
  {
    curr_frame.instructions.push_back(VMInstr::SETI());
  }
  else
  {
    curr_frame.instructions.push_back(VMInstr::STORE(var_table.get(s.lvalue.back().var_name.lexeme())));
  }
}

void CodeGenerator::visit(CallExpr &e)
{
  string fun_name = e.fun_name.lexeme();
  for (int i = 0; i < e.args.size(); i++)
  {
    e.args[i].accept(*this);
  }
  if (fun_name == "print")
  {
    curr_frame.instructions.push_back(VMInstr::WRITE());
  }
  else if (fun_name == "rand_int")
  {
    curr_frame.instructions.push_back(VMInstr::RAND());
  }
  else if (fun_name == "input")
  {
    curr_frame.instructions.push_back(VMInstr::READ());
  }
  else if (fun_name == "get")
  {
    curr_frame.instructions.push_back(VMInstr::GETC());
  }
  else if (fun_name == "length")
  {
    curr_frame.instructions.push_back(VMInstr::SLEN());
  }
  else if (fun_name == "length_array")
  {
    curr_frame.instructions.push_back(VMInstr::ALEN());
  }
  else if (fun_name == "to_string")
  {
    curr_frame.instructions.push_back(VMInstr::TOSTR());
  }
  else if (fun_name == "to_int")
  {
    curr_frame.instructions.push_back(VMInstr::TOINT());
  }
  else if (fun_name == "to_double")
  {
    curr_frame.instructions.push_back(VMInstr::TODBL());
  }
  else if (fun_name == "concat")
  {
    curr_frame.instructions.push_back(VMInstr::CONCAT());
  }
  else
  {
    curr_frame.instructions.push_back(VMInstr::CALL(fun_name));
  }
}

void CodeGenerator::visit(Expr &e)
{
  e.first->accept(*this);
  if (e.op.has_value())
  {
    e.rest->accept(*this);
    if (e.op.value().lexeme() == "+")
    {
      curr_frame.instructions.push_back(VMInstr::ADD());
    }
    else if (e.op.value().lexeme() == "-")
    {
      curr_frame.instructions.push_back(VMInstr::SUB());
    }
    else if (e.op.value().lexeme() == "*")
    {
      curr_frame.instructions.push_back(VMInstr::MUL());
    }
    else if (e.op.value().lexeme() == "/")
    {
      curr_frame.instructions.push_back(VMInstr::DIV());
    }
    else if (e.op.value().lexeme() == "and")
    {
      curr_frame.instructions.push_back(VMInstr::AND());
    }
    else if (e.op.value().lexeme() == "or")
    {
      curr_frame.instructions.push_back(VMInstr::OR());
    }
    else if (e.op.value().lexeme() == "<")
    {
      curr_frame.instructions.push_back(VMInstr::CMPLT());
    }
    else if (e.op.value().lexeme() == "<=")
    {
      curr_frame.instructions.push_back(VMInstr::CMPLE());
    }
    else if (e.op.value().lexeme() == ">")
    {
      curr_frame.instructions.push_back(VMInstr::CMPGT());
    }
    else if (e.op.value().lexeme() == ">=")
    {
      curr_frame.instructions.push_back(VMInstr::CMPGE());
    }
    else if (e.op.value().lexeme() == "!=")
    {
      curr_frame.instructions.push_back(VMInstr::CMPNE());
    }
    else if (e.op.value().lexeme() == "==")
    {
      curr_frame.instructions.push_back(VMInstr::CMPEQ());
    }
  }
  if (e.negated)
  {
    curr_frame.instructions.push_back(VMInstr::NOT());
  }
}

void CodeGenerator::visit(SimpleTerm &t)
{
  t.rvalue->accept(*this);
}

void CodeGenerator::visit(ComplexTerm &t)
{
  t.expr.accept(*this);
}

void CodeGenerator::visit(SimpleRValue &v)
{
  if (v.value.type() == TokenType::INT_VAL)
  {
    int val = stoi(v.value.lexeme());
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::DOUBLE_VAL)
  {
    double val = stod(v.value.lexeme());
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::STRING_VAL)
  {
    string val = v.value.lexeme();
    replace_all(val, "\\n", "\n");
    replace_all(val, "\\t", "\t");
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::CHAR_VAL)
  {
    string val = v.value.lexeme();
    replace_all(val, "\\n", "\n");
    replace_all(val, "\\t", "\t");
    curr_frame.instructions.push_back(VMInstr::PUSH(val));
  }
  else if (v.value.type() == TokenType::BOOL_VAL)
  {
    if (v.value.lexeme() == "true")
      curr_frame.instructions.push_back(VMInstr::PUSH(true));
    else if (v.value.lexeme() == "false")
      curr_frame.instructions.push_back(VMInstr::PUSH(false));
  }
  else if (v.value.type() == TokenType::NULL_VAL)
  {
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
  }
}

// You would probably just do the ALLOCA, then go through each item of the initializer, evaluate the corresponding expression, push the index, then call SETF.
// I don't think you need a new instruction, and instead, can just reuse existing instructions.
void CodeGenerator::visit(NewRValue &v)
{
  if (v.array_expr.has_value())
  {
    v.array_expr->accept(*this);
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
    curr_frame.instructions.push_back(VMInstr::ALLOCA());
  }
  else if (v.const_array.size() >= 1)
  {
    int size = v.const_array.size();
    curr_frame.instructions.push_back(VMInstr::PUSH(size));
    curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
    curr_frame.instructions.push_back(VMInstr::ALLOCA());
    for (int i = 0; i < v.const_array.size(); i++)
    {
      curr_frame.instructions.push_back(VMInstr::DUP());
      curr_frame.instructions.push_back(VMInstr::PUSH(i));
      curr_frame.instructions.push_back(VMInstr::PUSH(v.const_array[i].value.lexeme()));
      curr_frame.instructions.push_back(VMInstr::SETI());
    }
  }
  else
  {
    curr_frame.instructions.push_back(VMInstr::ALLOCS());
    for (auto &field : struct_defs[v.type.lexeme()].fields)
    {
      curr_frame.instructions.push_back(VMInstr::DUP());
      curr_frame.instructions.push_back(VMInstr::ADDF(field.var_name.lexeme()));
      curr_frame.instructions.push_back(VMInstr::DUP());
      curr_frame.instructions.push_back(VMInstr::PUSH(nullptr));
      curr_frame.instructions.push_back(VMInstr::SETF(field.var_name.lexeme()));
    }
  }
}

void CodeGenerator::visit(VarRValue &v)
{
  curr_frame.instructions.push_back(VMInstr::LOAD(var_table.get(v.path[0].var_name.lexeme())));
  for (int i = 0; i < v.path.size(); i++)
  {
    VarRef vr = v.path[i];
    string v_name = vr.var_name.lexeme();
    if (i != 0)
    {
      curr_frame.instructions.push_back(VMInstr::GETF(v_name));
    }
    if (vr.array_expr.has_value())
    {
      vr.array_expr->accept(*this);
      curr_frame.instructions.push_back(VMInstr::GETI());
    }
  }
}