//----------------------------------------------------------------------
// FILE: ast_parser.cpp
// DATE: CPSC 326, Spring 2023
// AUTH:
// DESC:
//----------------------------------------------------------------------

#include "ast_parser.h"

using namespace std;

ASTParser::ASTParser(const Lexer &a_lexer)
    : lexer{a_lexer}
{
}

void ASTParser::advance()
{
  curr_token = lexer.next_token();
}

void ASTParser::eat(TokenType t, const string &msg)
{
  if (!match(t))
    error(msg);
  advance();
}

bool ASTParser::match(TokenType t)
{
  return curr_token.type() == t;
}

bool ASTParser::match(initializer_list<TokenType> types)
{
  for (auto t : types)
    if (match(t))
      return true;
  return false;
}

void ASTParser::error(const string &msg)
{
  string s = msg + " found '" + curr_token.lexeme() + "' ";
  s += "at line " + to_string(curr_token.line()) + ", ";
  s += "column " + to_string(curr_token.column());
  throw MyPLException::ParserError(s);
}

bool ASTParser::bin_op()
{
  return match({TokenType::PLUS, TokenType::MINUS, TokenType::TIMES,
                TokenType::DIVIDE, TokenType::AND, TokenType::OR, TokenType::EQUAL,
                TokenType::LESS, TokenType::GREATER, TokenType::LESS_EQ,
                TokenType::GREATER_EQ, TokenType::NOT_EQUAL});
}

Program ASTParser::parse()
{
  Program p;
  advance();
  while (!match(TokenType::EOS))
  {
    if (match(TokenType::STRUCT))
      struct_def(p);
    else
      fun_def(p);
  }
  eat(TokenType::EOS, "expecting end-of-file");
  return p;
}

void ASTParser::struct_def(Program &p)
{
  // Create a StructDef
  StructDef s;
  eat(TokenType::STRUCT, "expecting struct");
  // Set the structdef name
  s.struct_name = curr_token;
  eat(TokenType::ID, "expecting ID");
  eat(TokenType::LBRACE, "expecting LBRACE");
  // If the struct isnt closed we called fields with the StructDef passed in
  if (!match(TokenType::RBRACE))
  {
    fields(s);
  }
  eat(TokenType::RBRACE, "expecting RBRACE");
  // push the struct def to the program
  p.struct_defs.push_back(s);
}

void ASTParser::fields(StructDef &s)
{
  // Creating a VarDef
  VarDef v;
  // Then we call datatype and pass in the VarDef's Datatype into it
  data_type(v.data_type);
  // Set the VarDef's name
  v.var_name = curr_token;
  eat(TokenType::ID, "Execting ID");
  // Push the first field onto the Struct
  s.fields.push_back(v);
  // While there are more fields, we creare them and push
  while (match(TokenType::COMMA))
  {
    VarDef v2;
    advance();
    data_type(v2.data_type);
    v2.var_name = curr_token;
    eat(TokenType::ID, "Expecting ID");
    s.fields.push_back(v2);
  }
}

void ASTParser::fun_def(Program &p)
{
  // Create a function object
  FunDef f;
  // If the function isnt a void type then its a data_type
  if (!match(TokenType::VOID_TYPE))
    data_type(f.return_type);
  else
  {
    // set the function's type name
    f.return_type.type_name = curr_token.lexeme();
    advance();
  }
  // Set the function name
  f.fun_name = curr_token;
  eat(TokenType::ID, "Expecting ID");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  params(f);
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  // While the token isnt a right brace we continue to call statement
  while (!match(TokenType::RBRACE))
  {
    stmt(f.stmts);
  }
  eat(TokenType::RBRACE, "Expecting RBRACE");
  // we push the functions to the program
  p.fun_defs.push_back(f);
}

// Here we do the same thing as fields but we push it to the function not a struct
void ASTParser::params(FunDef &f)
{
  if (!match(TokenType::RPAREN))
  {
    VarDef s;
    if (match(TokenType::CONST))
    {
      advance();
      s.data_type.is_const = true;
      if (match(TokenType::ARRAY))
      {
        advance();
        s.data_type.is_array = true;
      }
    }
    data_type(s.data_type);
    s.var_name = curr_token;
    eat(TokenType::ID, "Expecting ID");
    f.params.push_back(s);
    while (match(TokenType::COMMA))
    {
      VarDef s;
      advance();
      if (match(TokenType::CONST))
      {
        advance();
        s.data_type.is_const = true;
        if (match(TokenType::ARRAY))
        {
          advance();
          s.data_type.is_array = true;
        }
      }
      data_type(s.data_type);
      s.var_name = curr_token;
      eat(TokenType::ID, "Expecting ID");
      f.params.push_back(s);
    }
  }
}
// We pass in a DataType and set the data_type name and set if the data type is an array
void ASTParser::data_type(DataType &d)
{
  if (match(TokenType::ID))
  {
    d.is_array = false;
    d.type_name = curr_token.lexeme();
    advance();
  }
  else if (match(TokenType::ARRAY))
  {
    d.is_array = true;
    advance();
    if (match(TokenType::ID))
    {
      d.type_name = curr_token.lexeme();
      advance();
    }
    else
    {
      d.type_name = curr_token.lexeme();
      base_type();
    }
  }
  else
  {
    d.is_array = false;
    d.type_name = curr_token.lexeme();
    base_type();
  }
}

// Advance if its a base_type otherwise error
void ASTParser::base_type()
{
  if (match(TokenType::DOUBLE_TYPE) || match(TokenType::INT_TYPE) || match(TokenType::STRING_TYPE) || match(TokenType::CHAR_TYPE) || match(TokenType::BOOL_TYPE))
  {
    advance();
  }
  else
  {
    error("Expecting a base_type");
  }
}

// We pass in a vector of statements
void ASTParser::stmt(std::vector<std::shared_ptr<Stmt>> &s)
{
  if (match(TokenType::DOUBLE_TYPE) || match(TokenType::INT_TYPE) || match(TokenType::STRING_TYPE) || match(TokenType::CHAR_TYPE) || match(TokenType::BOOL_TYPE) || match(TokenType::ARRAY) || match(TokenType::CONST))
  {
    // if the token is a type, we make a vardecl statement and push it to the vector
    VarDeclStmt v;
    if (match(TokenType::CONST))
    {
      advance();
      v.var_def.data_type.is_const = true;
      if (match(TokenType::ARRAY))
      {
        v.var_def.data_type.is_array = true;
      }
      else
      {
        v.var_def.data_type.is_array = false;
      }
    }
    else
    {
      v.var_def.data_type.is_const = false;
    }
    vdecl_stmt(v);
    s.push_back(std::make_shared<VarDeclStmt>(v));
  }
  else if (match(TokenType::ID))
  {
    // If the token is an ID we store the curr token for later
    Token t = curr_token;
    advance();
    if (match(TokenType::LPAREN))
    {
      // If its a left parenthesis you make a CallExpr, set the name of it, and push it to the stmts vector
      CallExpr c;
      c.fun_name = t;
      call_expr(c);
      s.push_back(std::make_shared<CallExpr>(c));
    }
    else if (match(TokenType::ID))
    {
      // if the token is an ID, we make a vardecl statement and push it to the vector
      VarDeclStmt v;
      v.var_def.data_type.type_name = t.lexeme();
      vdecl_stmt(v);
      s.push_back(std::make_shared<VarDeclStmt>(v));
    }
    else
    {
      // Otherwise its a Assign statement, so we create one, then we also need to push the lvalue so we make a
      // VarRef and set the name and push it, then we modify the assignment and push it to the stmt vector
      AssignStmt a;
      VarRef l;
      l.var_name = t;
      a.lvalue.push_back(l);
      assign_stmt(a);
      s.push_back(std::make_shared<AssignStmt>(a));
    }
  }
  else if (match(TokenType::IF))
  {
    // if the token is an If we make an if statement and push it to the stmt vector
    IfStmt i;
    if_stmt(i);
    s.push_back(std::make_shared<IfStmt>(i));
  }
  else if (match(TokenType::WHILE))
  {
    // if the token is a While we make an WhileStmt and push it to the stmt vector
    WhileStmt w;
    while_stmt(w);
    s.push_back(std::make_shared<WhileStmt>(w));
  }
  else if (match(TokenType::FOR))
  {
    // if the token is a For we make an ForStmt and push it to the stmt vector
    ForStmt f;
    for_stmt(f);
    s.push_back(std::make_shared<ForStmt>(f));
  }
  else if (match(TokenType::RETURN))
  {
    // if the token is a Return we make an ReturnStmt and push it to the stmt vector
    ReturnStmt r;
    ret_stmt(r);
    s.push_back(std::make_shared<ReturnStmt>(r));
  }
  else
    error("Expecting stmt");
}

void ASTParser::vdecl_stmt(VarDeclStmt &v)
{
  // We pass in the VarDeclStmt and set the data_type and name
  if (!match(TokenType::ID))
  {
    data_type(v.var_def.data_type);
  }
  v.var_def.var_name = curr_token;
  eat(TokenType::ID, "Expecting ID");
  eat(TokenType::ASSIGN, "Expecting ASSIGN");
  expr(v.expr);
}

void ASTParser::assign_stmt(AssignStmt &a)
{
  // Pass in an AssignStmt and then pass that stmt into lvalue and then call expr, with the assignstmt expression
  lvalue(a.lvalue);
  eat(TokenType::ASSIGN, "Expecting ASSIGN");
  expr(a.expr);
}

void ASTParser::lvalue(std::vector<VarRef> &p)
{
  // Pass in the vector and while the token is a dot, or a lbracket we create a VarRef
  while (match(TokenType::DOT) || match(TokenType::LBRACKET))
  {
    VarRef r;
    if (match(TokenType::DOT))
    {
      // If its a dot we advance and set the var name and push it
      advance();
      r.var_name = curr_token;
      eat(TokenType::ID, "Expecting ID");
      p.push_back(r);
    }
    else
    {
      // If it is an Lbracket, we get the value of the end of the p, and we take it off the vector,
      // we advance() and make an expression and and set the array_expr to the expression and push it back onto the vector
      r = p.back();
      p.pop_back();
      advance();
      Expr e;
      expr(e);
      r.array_expr = e;
      p.push_back(r);
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
}

void ASTParser::if_stmt(IfStmt &i)
{
  // Pass in an IfStmt, make the expression, while the if statement isnt over we create statements, and set the if_part equal to them
  eat(TokenType::IF, "Expecting IF");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  BasicIf b;
  expr(b.condition);
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt(b.stmts);
  }
  i.if_part = b;
  advance();
  if_stmt_tail(i);
}

void ASTParser::if_stmt_tail(IfStmt &i)
{
  // Pass in the same IfStmt as IfStmt and do the same thing but with Elseif's and if there are none we do else's if there is one
  if (match(TokenType::ELSEIF))
  {
    BasicIf b;
    advance();
    eat(TokenType::LPAREN, "Expecting LPAREN");
    expr(b.condition);
    eat(TokenType::RPAREN, "Expecting RPAREN");
    eat(TokenType::LBRACE, "Expecting LBRACE");
    while (!match(TokenType::RBRACE))
    {
      stmt(b.stmts);
    }
    i.else_ifs.push_back(b);
    advance();
    if_stmt_tail(i);
  }
  else if (match(TokenType::ELSE))
  {
    advance();
    eat(TokenType::LBRACE, "Expecting LBRACE");
    while (!match(TokenType::RBRACE))
      stmt(i.else_stmts);
    eat(TokenType::RBRACE, "Expecting RBRACE");
  }
}

void ASTParser::while_stmt(WhileStmt &w)
{
  eat(TokenType::WHILE, "Expecting WHILE");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  expr(w.condition);
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt(w.stmts);
  }
  advance();
}

void ASTParser::for_stmt(ForStmt &f)
{
  // Pass in a ForStmt, set the v_decl at the beginning of it, then the expression
  eat(TokenType::FOR, "Expecting FOR");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  vdecl_stmt(f.var_decl);
  eat(TokenType::SEMICOLON, "Expecting SEMICOLON");
  expr(f.condition);
  eat(TokenType::SEMICOLON, "Expecting SEMICOLON");
  // Then we create the VarRef which is the last part of the for stmt and push it
  VarRef v;
  v.var_name = curr_token;
  f.assign_stmt.lvalue.push_back(v);
  eat(TokenType::ID, "Expecting ID");
  assign_stmt(f.assign_stmt);
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  // While there are more statements add them
  while (!match(TokenType::RBRACE))
  {
    stmt(f.stmts);
  }
  advance();
}

void ASTParser::call_expr(CallExpr &c)
{
  // Pass in CallExpr and create an expression, and push the expression, which is the arguments onto the CallExpr args
  eat(TokenType::LPAREN, "Expecting LPAREN");
  if (!match(TokenType::RPAREN))
  {
    Expr e;
    expr(e);
    c.args.push_back(e);
    // Repeat until there are no more args
    while (match(TokenType::COMMA))
    {
      advance();
      Expr e2;
      expr(e2);
      c.args.push_back(e2);
    }
  }
  eat(TokenType::RPAREN, "Expecting RPAREN");
}

void ASTParser::ret_stmt(ReturnStmt &r)
{
  // Pass in ReturnStmt and then set the expression thats being returned
  eat(TokenType::RETURN, "Expecting RETURN");
  expr(r.expr);
}

void ASTParser::expr(Expr &e)
{
  // If the token is a NOT then it is negated, then we follow the expression that we passed in
  if (match(TokenType::NOT))
  {
    e.negated = true;
    advance();
    expr(e);
  }
  // Only case of complex term is if there is a LParen so here we make one, and push the expressions first
  else if (match(TokenType::LPAREN))
  {
    ComplexTerm c;
    advance();
    expr(c.expr);
    eat(TokenType::RPAREN, "Expecting RPAREN");
    e.first = std::make_shared<ComplexTerm>(c);
  }
  // Otherwise we make a simple term and push it
  else
  {
    SimpleTerm s;
    rvalue(s.rvalue);
    e.first = std::make_shared<SimpleTerm>(s);
  }
  // if there is a bin_op then we set it, and then make an expression and push the e.rest
  if (bin_op())
  {
    e.op = curr_token;
    advance();
    Expr r;
    expr(r);
    e.rest = std::make_shared<Expr>(r);
    ;
  }
}
// Pass in a shared pointer of RValue
void ASTParser::rvalue(std::shared_ptr<RValue> &r)
{
  // If the token is NULL its a simpleRvalue so we make one set the value and push it
  if (match(TokenType::NULL_VAL))
  {
    SimpleRValue s;
    s.value = curr_token;
    r = std::make_shared<SimpleRValue>(s);
    advance();
  }
  // If its new we make a NewRValue and push it
  else if (match(TokenType::NEW))
  {
    NewRValue n;
    new_rvalue(n);
    r = std::make_shared<NewRValue>(n);
  }
  // Same as lvalue we check what we need to make, and edit the expression, and push it
  else if (match(TokenType::ID))
  {
    Token t = curr_token;
    advance();
    if (match(TokenType::LPAREN))
    {
      CallExpr c;
      c.fun_name = t;
      call_expr(c);
      r = std::make_shared<CallExpr>(c);
    }
    else
    {
      VarRValue a;
      VarRef v;
      v.var_name = t;
      a.path.push_back(v);
      var_rvalue(a.path);
      r = std::make_shared<VarRValue>(a);
    }
  }
  else
  {
    SimpleRValue s;
    base_rvalue(s);
    r = std::make_shared<SimpleRValue>(s);
  }
}

// We set the type to the NewRValue we pass in, edit the expression we make if its an array and set it to the array_expr
void ASTParser::new_rvalue(NewRValue &n)
{
  eat(TokenType::NEW, "Expecting NEW");
  if (match(TokenType::ID))
  {
    n.type = curr_token;
    advance();
    if (match(TokenType::LBRACKET))
    {
      advance();
      Expr e;
      expr(e);
      n.array_expr = e;
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
  else
  {
    n.type = curr_token;
    base_type();
    if (match(TokenType::LBRACKET))
    {
      eat(TokenType::LBRACKET, "Expecting LBRACKET");
      Expr e;
      expr(e);
      n.array_expr = e;
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
    else if (match(TokenType::LBRACE))
    {
      eat(TokenType::LBRACE, "Expecting LBRACE");
      SimpleRValue s;
      base_rvalue(s);
      n.const_array.push_back(s);
      while (match(TokenType::COMMA))
      {
        advance();
        SimpleRValue s2;
        base_rvalue(s2);
        n.const_array.push_back(s2);
      }
      eat(TokenType::RBRACE, "Expecting RBRACE");
    }
  }
}

// Cehck to see if a SimpleRValue we pass in is a Val and set the value else error
void ASTParser::base_rvalue(SimpleRValue &s)
{
  if (match(TokenType::DOUBLE_VAL) || match(TokenType::INT_VAL) || match(TokenType::STRING_VAL) || match(TokenType::CHAR_VAL) || match(TokenType::BOOL_VAL))
  {
    s.value = curr_token;
    advance();
  }
  else
  {
    error("Expecting a base_rvalue");
  }
}

// Same thing as lvalue
void ASTParser::var_rvalue(std::vector<VarRef> &p)
{
  while (match({TokenType::DOT, TokenType::LBRACKET}))
  {
    VarRef v;
    if (match(TokenType::DOT))
    {
      advance();
      v.var_name = curr_token;
      eat(TokenType::ID, "Expecting ID");
      p.push_back(v);
    }
    else
    {
      v = p.back();
      p.pop_back();
      advance();
      Expr e;
      expr(e);
      v.array_expr = e;
      p.push_back(v);
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
}