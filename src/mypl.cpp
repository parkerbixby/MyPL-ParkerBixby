//----------------------------------------------------------------------
// FILE: mypl.cpp
// DATE: Spring 2023
// AUTH: Parker Bixby
// DESC:
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "token.h"
#include "lexer.h"
#include "simple_parser.h"
#include "ast_parser.h"
#include "print_visitor.h"
#include "semantic_checker.h"
#include "code_generator.h"

using namespace std;

void usage();
void lex(istream *input);
void parse(istream *input);
void print(istream *input);
void check(istream *input);
void ir(istream *input);
void normalMode(istream *input);
void LexerFunc(istream *input);

char ch;
int newlinecount;

int main(int argc, char *argv[])
{

  istream *input;
  string args[argc];
  // Creating a new array named args that is the string type of the argv's
  for (int i = 0; i < argc; i++)
  {
    args[i] = string(argv[i]);
  }

  // Making sure if the amount of arguments passed in is greater than 3 then the usage function will be called.
  if (argc > 3)
  {
    usage();
  }

  // Setting input to &cin so that it will read the input the user enters
  if (argc == 1)
  {
    input = &cin;
    normalMode(input);
  }

  // Setting the value of input to a file, so that anytime the user enters 3 argc's it will recognize the 3rd as a file.
  if (argc == 3)
  {
    input = new ifstream(argv[2]);
    if (args[1] == "--lex")
    {
      // Checking to see if the file is valid
      if (input->fail())
      {
        cout << "ERROR: Could not read file" << args[2] << endl;
      }
      else
      {
        // If the file is valid I will call the lex function which will then gather the data and print the first letter.
        lex(input);
      }
    }
    // Similarly to --lex I tested to see if the file is valid, if so then I will call the parse function with the input still being a file.
    else if (args[1] == "--parse")
    {
      if (input->fail())
      {
        cout << "ERROR: Could not read file" << args[2] << endl;
      }
      else
      {
        parse(input);
      }
    }
    // We continue to call the functions if the file is valid.
    else if (args[1] == "--print")
    {
      if (input->fail())
      {
        cout << "ERROR: Could not read file" << args[2] << endl;
      }
      else
      {
        print(input);
      }
    }

    else if (args[1] == "--check")
    {
      if (input->fail())
      {
        cout << "ERROR: Could not read file" << args[2] << endl;
      }
      else
      {
        check(input);
      }
    }

    else if (args[1] == "--ir")
    {
      if (input->fail())
      {
        cout << "ERROR: Could not read file" << args[2] << endl;
      }
      else
      {
        ir(input);
      }
    }
  }

  // Here we see if the amount of arguments is 2 then we know it is a user input not a file
  if (argc == 2)
  {
    input = &cin;
    if (args[1] == "--lex")
    {
      lex(input);
    }

    else if (args[1] == "--parse")
    {
      parse(input);
    }

    else if (args[1] == "--print")
    {
      print(input);
    }

    else if (args[1] == "--check")
    {
      check(input);
    }

    else if (args[1] == "--ir")
    {
      ir(input);
    }
    else if (args[1] == "--help")
    {
      usage();
    }
    else
    {
      // Here is my one case where if argc == 2 then input needs to be set to file because we are in normal mode.
      input = new ifstream(argv[1]);
      if (input->fail())
      {
        // Instead of reading file args[2] like the rest, the file name is in args[1]
        cout << "ERROR: Could not read file" << args[1] << endl;
      }
      else
      {
        normalMode(input);
      }
    }
  }
}

// I set up each of the commands into functions, so this one gets the first char and prints it using get().
void lex(istream *input)
{
  cout << "[Lex Mode]" << endl;
  try
  {
    Lexer lexer(*input);
    Token t = lexer.next_token();
    cout << to_string(t) << endl;
    while (t.type() != TokenType::EOS)
    {
      t = lexer.next_token();
      cout << to_string(t) << endl;
    }
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

// For the parse function I set it up in a for loop so I get one character at a time and print it 2 times.
void parse(istream *input)
{
  cout << "[Parse Mode]" << endl;
  try
  {
    Lexer lexer(*input);
    ASTParser parser(lexer);
    parser.parse();
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

// For print, I set up the while loop so I can walk through the file, or &cin and print all of it until the char is a space using isspace().
void print(istream *input)
{
  try
  {
    Lexer lexer(*input);
    ASTParser parser(lexer);
    Program p = parser.parse();
    PrintVisitor v(cout);
    p.accept(v);
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

// Similarly to print, instead of checking for a space, I checked for a '\n' character, and printed until I hit one.
void check(istream *input)
{
  cout << "[Check Mode]" << endl;
  try
  {
    Lexer lexer(*input);
    ASTParser parser(lexer);
    Program p = parser.parse();
    SemanticChecker v;
    p.accept(v);
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

// IR was a little bit more difficult because i had to set up a while loop checking to see if we hit a '\n' character already
// Once I hit the 2nd '\n' character it would print,
// but I instead of using ch and printing every character I added each char to a string and printed the string
void ir(istream *input)
{
  cout << "[IR Mode]" << endl;

  try
  {
    Lexer lexer(*input);
    ASTParser parser(lexer);
    Program p = parser.parse();
    SemanticChecker t;
    p.accept(t);
    VM vm;
    CodeGenerator g(vm);
    p.accept(g);
    cout << to_string(vm) << endl;
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

// Like I was doing the --ir command I added each char to a string until I hit the EOF character and printed the string.
void normalMode(istream *input)
{
  cout << "[Normal Mode]" << endl;
  try
  {
    Lexer lexer(*input);
    ASTParser parser(lexer);
    Program p = parser.parse();
    SemanticChecker t;
    p.accept(t);
    VM vm;
    CodeGenerator g(vm);
    p.accept(g);
    vm.run();
  }
  catch (MyPLException &ex)
  {
    cerr << ex.what() << endl;
  }
}

void usage()
{
  cout << "Usage: ./mpl [option] [script-file]" << endl;
  cout << "Options: " << endl;
  cout << "--help prints this message" << endl;
  cout << "--lex displays token information" << endl;
  cout << "--parse checks for syntax errors" << endl;
  cout << "--print pretty prints program" << endl;
  cout << "--check statically checks program" << endl;
  cout << "--ir print intermediate (code) representation" << endl;
}