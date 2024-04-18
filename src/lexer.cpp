//----------------------------------------------------------------------
// FILE: lexer.cpp
// DATE: CPSC 326, Spring 2023
// NAME: Parker Bixby
// DESC:
//----------------------------------------------------------------------

#include "lexer.h"

using namespace std;

Lexer::Lexer(istream &input_stream)
    : input{input_stream}, column{0}, line{1}
{
}

char Lexer::read()
{
  ++column;
  return input.get();
}

char Lexer::peek()
{
  return input.peek();
}

void Lexer::error(const string &msg, int line, int column) const
{
  throw MyPLException::LexerError(msg + " at line " + to_string(line) +
                                  ", column " + to_string(column));
}

Token Lexer::next_token()
{
  // This variable is to keep track of the starting column when walking through a multi character token.
  int start_column;

  // Reading the first char
  char ch = read();
  // String to check for Reserved words and to print the words later.
  string lexeme = "";

  // First we start with a while loop, every time you move forward a character you make sure your char isnt the end of the character
  while (ch != EOF)
  {
    // Then we check to see if the line is commented out, or if it is a space so we can move forward.
    while (ch == '#' || isspace(ch))
    {
      // Then we see if the next char is a new line we increment line manually, and reset column
      if (ch == '\n')
      {
        line++;
        column = 0;
      }
      // If the char is a # we will just walk through the line until we hit a new line char or a EOF.
      else if (ch == '#')
      {
        while (peek() != '\n' && peek() != EOF)
        {
          ch = read();
        }
        ch = read();
        if (ch == '\n')
        {
          line++;
          column = 0;
        }
        else if (ch == EOF)
        {
          return Token(TokenType::EOS, "end-of-stream", line, column);
        }
      }
      ch = read();
    }
    // This is where we check to see what the char is and return its TokenType.
    if (ch == '=')
    {
      // Here we check if the next character is a = to see if the TokenType is Equal, or Assign.
      if (peek() == '=')
      {
        ch = read();
        return Token(TokenType::EQUAL, "==", line, column - 1);
      }
      return Token(TokenType::ASSIGN, "=", line, column);
    }
    else if (ch == '+')
    {
      return Token(TokenType::PLUS, "+", line, column);
    }
    else if (ch == '-')
    {
      return Token(TokenType::MINUS, "-", line, column);
    }
    else if (ch == '*')
    {
      return Token(TokenType::TIMES, "*", line, column);
    }
    else if (ch == '/')
    {
      return Token(TokenType::DIVIDE, "/", line, column);
    }
    else if (ch == ';')
    {
      return Token(TokenType::SEMICOLON, ";", line, column);
    }
    else if (ch == '<')
    {
      if (peek() == '=')
      {
        ch = read();
        return Token(TokenType::LESS_EQ, "<=", line, column - 1);
      }
      return Token(TokenType::LESS, "<", line, column);
    }
    else if (ch == '>')
    {
      if (peek() == '=')
      {
        ch = read();
        return Token(TokenType::GREATER_EQ, ">=", line, column - 1);
      }
      return Token(TokenType::GREATER, ">", line, column);
    }
    // Here we check to see if the char is a ! then we make sure that the next char has to be a = sign, otherwise we get an error
    else if (ch == '!')
    {
      if (peek() == '=')
      {
        ch = read();
        return Token(TokenType::NOT_EQUAL, "!=", line, column - 1);
      }
      else
      {
        ch = read();
        string error_ch;
        error_ch += ch;
        error("expecting '!=' found '!" + error_ch + '\'', line, column - 1);
      }
    }
    else if (ch == '.')
    {
      return Token(TokenType::DOT, ".", line, column);
    }
    else if (ch == ',')
    {
      return Token(TokenType::COMMA, ",", line, column);
    }
    else if (ch == '(')
    {
      return Token(TokenType::LPAREN, "(", line, column);
    }
    else if (ch == ')')
    {
      return Token(TokenType::RPAREN, ")", line, column);
    }
    else if (ch == ',')
    {
      return Token(TokenType::COMMA, ",", line, column);
    }
    else if (ch == '[')
    {
      return Token(TokenType::LBRACKET, "[", line, column);
    }
    else if (ch == ']')
    {
      return Token(TokenType::RBRACKET, "]", line, column);
    }
    else if (ch == '{')
    {
      return Token(TokenType::LBRACE, "{", line, column);
    }
    else if (ch == '}')
    {
      return Token(TokenType::RBRACE, "}", line, column);
    }
    // Here we are checking for an apostrophe
    else if (ch == '\'')
    {
      // This is checking to see if the next char is an apostrophe because if it is it is an empty char which causes an error
      if (peek() == '\'')
      {
        error("empty character", line, column + 1);
      }
      // Then we check for a new line char because a char can't have \n
      else if (peek() == '\n')
      {
        ch = read();
        error("found end-of-line in character", line, column);
      }
      else
      {
        int start_column = column;
        ch = read();
        lexeme += ch;
        // This is where we check to see if the char is a backslash
        if (ch == '\\')
        {
          ch = read();
          lexeme += ch;
          ch = read();
          return Token(TokenType::CHAR_VAL, lexeme, line, start_column);
        }
        else if (peek() == '\'')
        {
          ch = read();
          return Token(TokenType::CHAR_VAL, lexeme, line, start_column);
        }
        // Check if the char is an EOF, if it is an error is popped up
        else if (peek() == EOF)
        {
          ch = peek();
          string error_ch;
          error_ch += ch;
          error("found end-of-file in character", line, column);
        }
        // Check to see if the char is an alphabetical letter, which would be too many characters
        else if (isalpha(peek()))
        {
          ch = peek();
          string error_ch;
          error_ch += ch;
          error("expecting ' found " + error_ch, line, column + 1);
        }
      }
    }
    // Here we are checking for string_val
    else if (ch == '"')
    {
      start_column = column;
      // We say while the characters are valid for a string_val then we add them
      while (peek() != '"' && peek() != '\n' && peek() != EOF)
      {
        ch = read();
        lexeme += ch;
      }
      if (peek() == '\n')
      {
        error("found end-of-line in string", line, column + 1);
      }
      else if (peek() == EOF)
      {
        error("found end-of-file in string", line, column + 1);
      }
      if (peek() == '"')
      {
        ch = read();
      }
      return Token(TokenType::STRING_VAL, lexeme, line, start_column);
    }
    // Here we check if ch is a digit
    else if (isdigit(ch))
    {
      start_column = column;
      lexeme += ch;
      // Checking to make sure the int doesn't have a leading 0
      if ((ch == '0') && (isdigit(peek())))
        error("leading zero in number", line, column);
      while (isdigit(peek()))
      {
        ch = read();
        lexeme += ch;
      }
      if (peek() == '.')
      {
        ch = read();
        lexeme += ch;
        if (!isdigit(peek()))
          error("missing digit in '" + lexeme + "'", line, (column + 1));
        while (isdigit(peek()))
        {
          ch = read();
          lexeme += ch;
        }
        return Token(TokenType::DOUBLE_VAL, lexeme, line, start_column);
      }
      else
        return Token(TokenType::INT_VAL, lexeme, line, start_column);
    }
    // Here we check to see if the char is a valid character for an ID
    // While we are walking through, everytime we walk through we also have to check if the string is a reserved word.
    // Otherwise we return an ID.
    else if (isalpha(ch))
    {
      start_column = column;
      lexeme += ch;
      while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
      {
        ch = read();
        lexeme += ch;
        if (lexeme == "and")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::AND, lexeme, line, start_column);
          }
        }
        else if (lexeme == "not")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::NOT, lexeme, line, start_column);
          }
        }
        else if (lexeme == "or")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::OR, lexeme, line, start_column);
          }
        }
        else if (lexeme == "if")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::IF, lexeme, line, start_column);
          }
        }
        else if (lexeme == "const")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::CONST, lexeme, line, start_column);
          }
        }
        else if (lexeme == "else")
        {
          if (peek() == 'i')
          {
            ch = read();
            lexeme += ch;
            if (peek() == 'f')
            {
              ch = read();
              lexeme += ch;
              return Token(TokenType::ELSEIF, lexeme, line, start_column);
            }
            else if (peek() != 'f')
            {
              ch = read();
              return Token(TokenType::ID, lexeme, line, start_column);
            }
          }
          return Token(TokenType::ELSE, lexeme, line, start_column);
        }
        else if (lexeme == "while")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::WHILE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "for")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::FOR, lexeme, line, start_column);
          }
        }
        else if (lexeme == "struct")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::STRUCT, lexeme, line, start_column);
          }
        }
        else if (lexeme == "array")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::ARRAY, lexeme, line, start_column);
          }
        }
        else if (lexeme == "return")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::RETURN, lexeme, line, start_column);
          }
        }
        else if (lexeme == "new")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::NEW, lexeme, line, start_column);
          }
        }
        else if (lexeme == "true" || lexeme == "false")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::BOOL_VAL, lexeme, line, start_column);
          }
        }
        else if (lexeme == "int")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::INT_TYPE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "double")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::DOUBLE_TYPE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "char")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::CHAR_TYPE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "string")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::STRING_TYPE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "void")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::VOID_TYPE, lexeme, line, start_column);
          }
        }
        else if (lexeme == "null")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::NULL_VAL, lexeme, line, start_column);
          }
        }
        else if (lexeme == "bool")
        {
          if (isalpha(peek()) || isdigit(peek()) || peek() == '_')
          {
            ch = read();
            lexeme += ch;
            while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
            {
              ch = read();
              lexeme += ch;
            }
          }
          else
          {
            return Token(TokenType::BOOL_TYPE, lexeme, line, start_column);
          }
        }
      }
      return Token(TokenType::ID, lexeme, line, start_column);
    }
    // If there are characters that are invalid we give an error
    else if (!isalpha(peek()) && !isdigit(peek()) && ch != '_' && ch != '\n' && ch != EOF)
    {
      string error_ch;
      error_ch += ch;
      error("unexpected character '" + error_ch + "'", line, column);
    }
    // We also check to see if the ID leads with a _ if it does it gives an error.
    else if (ch == '_')
    {
      string error_ch;
      error_ch += ch;
      error("unexpected character '" + error_ch + "'", line, column);
    }
  }
  return Token(TokenType::EOS, "end-of-stream", line, column);
}
