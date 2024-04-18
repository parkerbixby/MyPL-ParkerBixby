//----------------------------------------------------------------------
// FILE: simple_parser.cpp
// DATE: CPSC 326, Spring 2023
// AUTH:
// DESC:
//----------------------------------------------------------------------

#include "simple_parser.h"
#include <iostream>

SimpleParser::SimpleParser(const Lexer &a_lexer)
    : lexer{a_lexer}
{
}

void SimpleParser::advance()
{
  curr_token = lexer.next_token();
}

void SimpleParser::eat(TokenType t, const std::string &msg)
{
  if (!match(t))
    error(msg);
  advance();
}

bool SimpleParser::match(TokenType t)
{
  return curr_token.type() == t;
}

bool SimpleParser::match(std::initializer_list<TokenType> types)
{
  for (auto t : types)
    if (match(t))
      return true;
  return false;
}

void SimpleParser::error(const std::string &msg)
{
  std::string s = msg + " found '" + curr_token.lexeme() + "' ";
  s += "at line " + std::to_string(curr_token.line()) + ", ";
  s += "column " + std::to_string(curr_token.column());
  throw MyPLException::ParserError(s);
}

bool SimpleParser::bin_op()
{
  return match({TokenType::PLUS, TokenType::MINUS, TokenType::TIMES,
                TokenType::DIVIDE, TokenType::AND, TokenType::OR, TokenType::EQUAL,
                TokenType::LESS, TokenType::GREATER, TokenType::LESS_EQ,
                TokenType::GREATER_EQ, TokenType::NOT_EQUAL});
}

void SimpleParser::parse()
{
  advance();
  while (!match(TokenType::EOS))
  {
    if (match(TokenType::STRUCT))
      struct_def();
    else
      fun_def();
  }
  eat(TokenType::EOS, "expecting end-of-file");
}

void SimpleParser::struct_def()
{
  eat(TokenType::STRUCT, "expecting struct");
  eat(TokenType::ID, "expecting ID");
  eat(TokenType::LBRACE, "expecting LBRACE");
  if (!match(TokenType::RBRACE))
  {
    fields();
  }
  eat(TokenType::RBRACE, "expecting RBRACE");
  // TODO: finish this
}

void SimpleParser::fields()
{
  data_type();
  eat(TokenType::ID, "Execting ID");
  while (match(TokenType::COMMA))
  {
    advance();
    data_type();
    eat(TokenType::ID, "Expecting ID");
  }
}

void SimpleParser::fun_def()
{
  if (!match(TokenType::VOID_TYPE))
    data_type();
  else
  {
    eat(TokenType::VOID_TYPE, "Expecting VOID_TYPE");
  }

  eat(TokenType::ID, "Expecting ID");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  params();
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt();
  }
  eat(TokenType::RBRACE, "Expecting RBRACE");
}

void SimpleParser::params()
{
  if (!match(TokenType::RPAREN))
  {
    data_type();
    eat(TokenType::ID, "Expecting ID");
    while (match(TokenType::COMMA))
    {
      advance();
      data_type();
      eat(TokenType::ID, "Expecting ID");
    }
  }
}

void SimpleParser::data_type()
{
  if (match(TokenType::ID))
  {
    advance();
  }
  else if (match(TokenType::ARRAY))
  {
    advance();
    if (match(TokenType::ID))
    {
      advance();
    }
    else
      base_type();
  }
  else
    base_type();
}

void SimpleParser::base_type()
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

void SimpleParser::stmt()
{
  if (match(TokenType::DOUBLE_TYPE) || match(TokenType::INT_TYPE) || match(TokenType::STRING_TYPE) || match(TokenType::CHAR_TYPE) || match(TokenType::BOOL_TYPE) || match(TokenType::ARRAY))
  {
    vdecl_stmt();
  }
  else if (match(TokenType::ID))
  {
    advance();
    if (match(TokenType::LPAREN))
    {
      call_expr();
    }
    else if (match(TokenType::ID))
    {
      vdecl_stmt();
    }
    else
    {
      assign_stmt();
    }
  }
  else if (match(TokenType::IF))
  {
    if_stmt();
  }
  else if (match(TokenType::WHILE))
  {
    while_stmt();
  }
  else if (match(TokenType::FOR))
  {
    for_stmt();
  }
  else if (match(TokenType::RETURN))
  {
    ret_stmt();
  }
  else
    error("Expecting stmt");
}

void SimpleParser::vdecl_stmt()
{
  if (!match(TokenType::ID))
    data_type();
  eat(TokenType::ID, "Expecting ID");
  eat(TokenType::ASSIGN, "Expecting ASSIGN");
  expr();
}

void SimpleParser::assign_stmt()
{
  lvalue();
  eat(TokenType::ASSIGN, "Expecting ASSIGN");
  expr();
}

void SimpleParser::lvalue()
{
  while (match(TokenType::DOT) || match(TokenType::LBRACKET))
  {
    if (match(TokenType::DOT))
    {
      advance();
      eat(TokenType::ID, "Expecting ID");
    }
    else
    {
      advance();
      expr();
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
}

void SimpleParser::if_stmt()
{
  eat(TokenType::IF, "Expecting IF");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  expr();
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt();
  }
  advance();
  if_stmt_tail();
}

void SimpleParser::if_stmt_tail()
{
  if (match(TokenType::ELSEIF))
  {
    advance();
    eat(TokenType::LPAREN, "Expecting LPAREN");
    expr();
    eat(TokenType::RPAREN, "Expecting RPAREN");
    eat(TokenType::LBRACE, "Expecting LBRACE");
    while (!match(TokenType::RBRACE))
    {
      stmt();
    }
    advance();
    if_stmt_tail();
  }
  else if (match(TokenType::ELSE))
  {
    advance();
    eat(TokenType::LBRACE, "Expecting LBRACE");
    while (!match(TokenType::RBRACE))
      stmt();
    eat(TokenType::RBRACE, "Expecting RBRACE");
  }
}

void SimpleParser::while_stmt()
{
  eat(TokenType::WHILE, "Expecting WHILE");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  expr();
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt();
  }
  advance();
}

void SimpleParser::for_stmt()
{
  eat(TokenType::FOR, "Expecting FOR");
  eat(TokenType::LPAREN, "Expecting LPAREN");
  vdecl_stmt();
  eat(TokenType::SEMICOLON, "Expecting SEMICOLON");
  expr();
  eat(TokenType::SEMICOLON, "Expecting SEMICOLON");
  eat(TokenType::ID, "Expecting ID");
  assign_stmt();
  eat(TokenType::RPAREN, "Expecting RPAREN");
  eat(TokenType::LBRACE, "Expecting LBRACE");
  while (!match(TokenType::RBRACE))
  {
    stmt();
  }
  advance();
}

void SimpleParser::call_expr()
{
  eat(TokenType::LPAREN, "Expecting LPAREN");
  if (!match(TokenType::RPAREN))
  {
    expr();
    while (match(TokenType::COMMA))
    {
      advance();
      expr();
    }
  }
  eat(TokenType::RPAREN, "Expecting RPAREN");
}

void SimpleParser::ret_stmt()
{
  eat(TokenType::RETURN, "Expecting RETURN");
  expr();
}

void SimpleParser::expr()
{
  if (match(TokenType::NOT))
  {
    advance();
    expr();
  }
  else if (match(TokenType::LPAREN))
  {
    advance();
    expr();
    eat(TokenType::RPAREN, "Expecting RPAREN");
  }
  else
  {
    rvalue();
  }
  if (bin_op())
  {
    advance();
    expr();
  }
}

void SimpleParser::rvalue()
{
  if (match(TokenType::NULL_VAL))
    advance();
  else if (match(TokenType::NEW))
    new_rvalue();
  else if (match(TokenType::ID))
  {
    advance();
    if (match(TokenType::LPAREN))
      call_expr();
    else
      var_rvalue();
  }
  else
    base_rvalue();
}

void SimpleParser::new_rvalue()
{
  eat(TokenType::NEW, "Expecting NEW");
  if (match(TokenType::ID))
  {
    advance();
    if (match(TokenType::LBRACKET))
    {
      advance();
      expr();
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
  else
  {
    base_type();
    eat(TokenType::LBRACKET, "Expecting LBRACKET");
    expr();
    eat(TokenType::RBRACKET, "Expecting RBRACKET");
  }
}

void SimpleParser::base_rvalue()
{
  if (match(TokenType::DOUBLE_VAL) || match(TokenType::INT_VAL) || match(TokenType::STRING_VAL) || match(TokenType::CHAR_VAL) || match(TokenType::BOOL_VAL))
  {
    advance();
  }
  else
  {
    error("Expecting a base_rvalue");
  }
}

void SimpleParser::var_rvalue()
{
  while (match({TokenType::DOT, TokenType::LBRACKET}))
  {
    if (match(TokenType::DOT))
    {
      advance();
      eat(TokenType::ID, "Expecting ID");
    }
    else
    {
      advance();
      expr();
      eat(TokenType::RBRACKET, "Expecting RBRACKET");
    }
  }
}