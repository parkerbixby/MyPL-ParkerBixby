//----------------------------------------------------------------------
// FILE: print_visitor.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Parker Bixby
// DESC:
//----------------------------------------------------------------------

#include "print_visitor.h"

using namespace std;

PrintVisitor::PrintVisitor(ostream &output)
    : out(output)
{
}

void PrintVisitor::inc_indent()
{
  indent += INDENT_AMT;
}

void PrintVisitor::dec_indent()
{
  indent -= INDENT_AMT;
}

void PrintVisitor::print_indent()
{
  out << string(indent, ' ');
}

void PrintVisitor::visit(Program &p)
{
  for (auto struct_def : p.struct_defs)
    struct_def.accept(*this);
  for (auto fun_def : p.fun_defs)
    fun_def.accept(*this);
}

// TODO: Finish the visitor functions

// Print the function
void PrintVisitor::visit(FunDef &f)
{
  out << endl;
  indent = 0;
  if (f.return_type.is_array)
  {
    out << "array " << f.return_type.type_name << " " << f.fun_name.lexeme() << "(";
  }
  else
  {
    out << f.return_type.type_name << " " << f.fun_name.lexeme() << "(";
  }

  for (int i = 0; i < f.params.size(); i++)
  {
    out << f.params[i].data_type.type_name << " " << f.params[i].var_name.lexeme();
    if (f.params.size() > 1 && i != f.params.size() - 1)
      out << ", ";
  }

  out << ") {\n";
  inc_indent();
  for (int i = 0; i < f.stmts.size(); i++)
  {
    print_indent();
    f.stmts[i]->accept(*this);
    out << endl;
  }
  dec_indent();
  out << "}\n";
}
// print the struct def
void PrintVisitor::visit(StructDef &s)
{
  out << endl;
  indent = 0;
  out << "struct " << s.struct_name.lexeme() << " {" << endl;
  inc_indent();
  // This prints the fields in the struct
  for (int i = 0; i < s.fields.size(); i++)
  {
    print_indent();
    if (s.fields[i].data_type.is_array)
    {
      out << "array ";
    }
    out << s.fields[i].data_type.type_name << " " << s.fields[i].var_name.lexeme();
    if (!(s.fields.size() == (i + 1)))
      out << ",\n";
  }
  dec_indent();
  out << endl
      << "}" << endl;
}
// Prints the return statement
void PrintVisitor::visit(ReturnStmt &s)
{
  out << "return ";
  s.expr.accept(*this);
}

// prints the while statement
void PrintVisitor::visit(WhileStmt &s)
{
  out << "while (";
  inc_indent();
  s.condition.accept(*this);
  out << ") {" << endl;
  // prints the statements in the while loop
  for (int i = 0; i < s.stmts.size(); i++)
  {
    print_indent();
    s.stmts[i]->accept(*this);
    out << endl;
  }
  dec_indent();
  print_indent();
  out << "}";
}
// Prints the For stmt
void PrintVisitor::visit(ForStmt &s)
{
  out << "for (";
  s.var_decl.accept(*this);
  out << "; ";
  s.condition.accept(*this);
  out << "; ";
  s.assign_stmt.accept(*this);
  out << ") {" << endl;
  inc_indent();
  // Prints the stmts inside the for loop
  for (int i = 0; i < s.stmts.size(); i++)
  {
    print_indent();
    s.stmts[i]->accept(*this);
  }
  out << endl;
  dec_indent();
  print_indent();
  out << "}";
}
// prints the if stmt
void PrintVisitor::visit(IfStmt &s)
{
  out << "if (";
  s.if_part.condition.accept(*this);
  out << ") {" << endl;
  inc_indent();
  // prints the statments inside the if statement
  for (int i = 0; i < s.if_part.stmts.size(); i++)
  {
    print_indent();
    s.if_part.stmts[i]->accept(*this);
    out << endl;
  }
  dec_indent();
  print_indent();
  out << "}";
  if (s.else_ifs.size() != 0)
  {
    // prints the statements in the else if statement
    out << endl;
    for (int i = 0; i < s.else_ifs.size(); i++)
    {
      print_indent();
      out << "elseif (";
      s.else_ifs[i].condition.accept(*this);
      out << ") {" << endl;
      // prints the statemtnts in else if
      for (int j = 0; j < s.else_ifs[i].stmts.size(); j++)
      {
        inc_indent();
        print_indent();
        s.else_ifs[i].stmts[j]->accept(*this);
        out << endl;
        dec_indent();
      }
      print_indent();
      out << "}" << endl;
    }
  }
  // prints the else statement
  if (s.else_stmts.size() != 0)
  {
    print_indent();
    out << "else {" << endl;
    inc_indent();
    for (int i = 0; i < s.else_stmts.size(); i++)
    {
      print_indent();
      s.else_stmts[i]->accept(*this);
      out << endl;
    }
    dec_indent();
    print_indent();
    out << "}";
  }
}
// Printe the VarDeclStatements
void PrintVisitor::visit(VarDeclStmt &s)
{
  if (s.var_def.data_type.is_const)
  {
    out << "const ";
    if (s.var_def.data_type.is_array)
    {
      out << " array ";
    }
  }
  out << s.var_def.data_type.type_name << " " << s.var_def.var_name.lexeme() << " = ";
  s.expr.accept(*this);
}
// Prints the AssignStmt
void PrintVisitor::visit(AssignStmt &s)
{
  for (int i = 0; i < s.lvalue.size(); i++)
  {
    // prints the lvalue name
    out << s.lvalue[i].var_name.lexeme();
    // if its an array you add the brackets and then the lvalue
    if (s.lvalue[i].array_expr.has_value())
    {
      out << "[";
      s.lvalue[i].array_expr->accept(*this);
      out << "]";
    }
    // if its a path we print the dots
    if (!(s.lvalue.size() == (i + 1)))
    {
      out << ".";
    }
  }
  // print = then the right side
  out << " = ";
  s.expr.accept(*this);
}
// Prints the call expr
void PrintVisitor::visit(CallExpr &e)
{
  out << e.fun_name.lexeme() << "(";
  for (int i = 0; i < e.args.size(); i++)
  {
    e.args[i].accept(*this);
    if (e.args.size() != (i + 1))
    {
      out << ", ";
    }
  }
  out << ")";
}
// prints the expression
void PrintVisitor::visit(Expr &e)
{
  // if its negated we print not (
  if (e.negated)
  {
    out << "not (";
  }
  e.first->accept(*this);
  // if it has an operator we print it
  if (e.op.has_value())
  {
    out << " " << e.op.value().lexeme() << " ";
    e.rest->accept(*this);
  }
  if (e.negated)
  {
    out << ")";
  }
}
// prints the simple term
void PrintVisitor::visit(SimpleTerm &t)
{
  t.rvalue->accept(*this);
}
// prints the complex term
void PrintVisitor::visit(ComplexTerm &t)
{
  out << "(";
  t.expr.accept(*this);
  out << ")";
}
// prints the simple value, if its a string or char we add "" or ''
void PrintVisitor::visit(SimpleRValue &v)
{
  if (v.first_token().type() == TokenType::STRING_VAL)
  {
    out << "\"" << v.first_token().lexeme() << "\"";
  }
  else if (v.first_token().type() == TokenType::CHAR_VAL)
  {
    out << "\'" << v.first_token().lexeme() << "\'";
  }
  else
  {
    out << v.value.lexeme();
  }
}
// prints the NewRValue
void PrintVisitor::visit(NewRValue &v)
{
  out << "new " << v.type.lexeme();
  if (v.array_expr.has_value())
  {
    out << "[";
    v.array_expr->accept(*this);
    out << "]";
  }
}
// Prints the VarRValue
void PrintVisitor::visit(VarRValue &v)
{
  for (int i = 0; i < v.path.size(); i++)
  {
    out << v.path[i].var_name.lexeme();
    // if its an array we add the []
    if (v.path[i].array_expr.has_value())
    {
      out << "[";
      v.path[i].array_expr->accept(*this);
      out << "]";
    }
    if (v.path.size() != (i + 1))
    {
      out << ".";
    }
  }
}