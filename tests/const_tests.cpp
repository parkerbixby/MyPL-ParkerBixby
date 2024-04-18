//----------------------------------------------------------------------
// FILE: const_tests.cpp
// DATE: CPSC 326, Spring 2023
// AUTH: Parker Bixby
// DESC: Basic code generator tests
//----------------------------------------------------------------------



#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "mypl_exception.h"
#include "lexer.h"
#include "ast_parser.h"
#include "vm.h"
#include "code_generator.h"
#include "semantic_checker.h"

using namespace std;


streambuf* stream_buffer;


void change_cout(stringstream& out)
{
  stream_buffer = cout.rdbuf();
  cout.rdbuf(out.rdbuf());
}

void restore_cout()
{
  cout.rdbuf(stream_buffer);
}

string build_string(initializer_list<string> strs)
{
  string result = "";
  for (string s : strs)
    result += s + "\n";
  return result;
}


//----------------------------------------------------------------------
// Simple getting started tests
//----------------------------------------------------------------------


TEST(BasicConstVarTest, SimplePrintConstVar) {
  stringstream in(build_string({
        "void main() {",
        "  const int x = 3",
        "   print(x)",
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("3", out.str());
  restore_cout();
}

TEST(BasicConstVarTest,
 SimplePrintConstArray) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {1,2,3,4,5}",
        "   print(xs[0])",
        "   print(xs[1])",
        "   print(xs[2])",
        "   print(xs[3])",
        "   print(xs[4])",
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("12345", out.str());
  restore_cout();
}

TEST(BasicConstVarTest, ReassigningConstArrayValues) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {1,2,3,4,5}",
        "   xs[3] = 9",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, TypesDontMatchInVarDecl) {
  stringstream in(build_string({
        "void main() {",
        "  const array string xs = new int {1,2,3,4,5}",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, TypesDontMatchInArrayExpr) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {'a','b','c','d','e'}",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, TypesBeforeExpr) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new char {1,2,3,4,5}",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, NoNewRValue) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = int {1,2,3,4}",
        "   xs[3] = 9",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Parser Error:"));
  }
}

TEST(BasicConstVarTest, ParsingErrorWithoutArray) {
  stringstream in(build_string({
        "void main() {",
        "  const int xs = int {1,2,3,4}",
        "   xs[3] = 9",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Parser Error:"));
  }
}

TEST(BasicConstVarTest, AddOneToConstValue) {
  stringstream in(build_string({
        "void main() {",
        "  const int xs = 7",
        "   add_1(xs)",
        "   print(xs)",
        "}",
        "int add_1(int z) {",
        "   z = z + 1",
        "   return z",
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("7", out.str());
  restore_cout();
}

TEST(BasicConstVarTest, ChangeConstValueInArray) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {7,4,2}",
        "   change_index_0(xs[0])",
        "   print(xs[0])",
        "}",
        "int change_index_0(int z) {",
        "     z = 9",
        "     return z",
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("7", out.str());
  restore_cout();
}

TEST(BasicConstVarTest, StringChangeInConstArray) {
  stringstream in(build_string({
    "void main(){"
        "const array string words = new string {\"apples\", \"bananas\", \"oranges\"}"
        "apples_to_grapes(words[0])",
        "print(words[0])",
    "}",
    "string apples_to_grapes(string word){",
      "word = \"grapes\"",
      "return word",
    "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("apples", out.str());
  restore_cout();
}



TEST(BasicConstVarTest, ChangingAConstParamInfunc) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {5,4,3,2,1}",
        "  add_one(xs[3])",
        "}",
        "int add_one(const int zs){",
        "    zs = 4"
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, PassingConstArrayInFunc) {
  stringstream in(build_string({
        "void main() {",
        "  const array int xs = new int {5,4,3,2,1}",
        "  add_one(xs)",
        "}",
        "int add_one(const array int zs){",
          "for(int i = 0; i < length_array(zs); i = i + 1){",
              "zs[i] = zs[i] + 1",
          "}"
          "return zs"
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, ReassigningConstStringValues) {
  stringstream in(build_string({
        "void main() {",
        "  const string word = \"helloworld\"",
        "   word = \"byeworld\"",
        "}"
      }));
  SemanticChecker checker;
  try {
    ASTParser(Lexer(in)).parse().accept(checker);
    FAIL();
  } catch (MyPLException& ex) {
    string msg = ex.what();
    ASSERT_TRUE(msg.starts_with("Static Error:"));
  }
}

TEST(BasicConstVarTest, SimpleMultWithConst) {
  stringstream in(build_string({
        "void main() {",
        "  const int x = 4 * 3",
        "  double y = 4.5 * 3.25", 
        "  int z = 4 * x",
        "  print(x)",
        "  print(' ')",
        "  print(y)", 
        "  print(' ')",
        "  print(z)", 
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("12 14.625000 48", out.str());
  restore_cout();
}     

TEST(BasicConstVarTest, ConcatWithConstStrings) {
  stringstream in(build_string({
        "void main() {",
        "  const string word1 = \"hello\"",
        "  const string word2 = \"world\"",
        "  const string word3 = concat(word1,word2)",
        "  print(word3)"
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("helloworld", out.str());
  restore_cout();
} 

TEST(BasicConstVarTest, ForInnerShadowing) {
  stringstream in(build_string({
        "void main() {",
        "  for (int i = 0; i < 3; i = i + 1) {", 
        "    const int i = 42",
        "    print(i)",
        "    print(' ')",
        "  }",
        "}"
      }));
  VM vm;
  CodeGenerator generator(vm);
  ASTParser(Lexer(in)).parse().accept(generator);
  stringstream out;
  change_cout(out);
  vm.run();
  EXPECT_EQ("42 42 42 ", out.str());
  restore_cout();
}





//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}