//----------------------------------------------------------------------
// FILE: vm.cpp
// DATE: CPSC 326, Spring 2023
// AUTH:
// DESC:
//----------------------------------------------------------------------

#include "vm.h"
#include "mypl_exception.h"
#include "vm_frame.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void VM::error(string msg) const
{
  throw MyPLException::VMError(msg);
}

void VM::error(string msg, const VMFrame &frame) const
{
  int pc = frame.pc - 1;
  VMInstr instr = frame.info.instructions[pc];
  string name = frame.info.function_name;
  msg += " (in " + name + " at " + to_string(pc) + ": " +
         to_string(instr) + ")";
  throw MyPLException::VMError(msg);
}

string to_string(const VM &vm)
{
  string s = "";
  for (const auto &entry : vm.frame_info)
  {
    const string &name = entry.first;
    s += "\nFrame '" + name + "'\n";
    const VMFrameInfo &frame = entry.second;
    for (int i = 0; i < frame.instructions.size(); ++i)
    {
      VMInstr instr = frame.instructions[i];
      s += "  " + to_string(i) + ": " + to_string(instr) + "\n";
    }
  }
  return s;
}

void VM::add(const VMFrameInfo &frame)
{
  frame_info[frame.function_name] = frame;
}

void VM::run(bool DEBUG)
{
  srand(time(NULL));
  // grab the "main" frame if it exists
  if (!frame_info.contains("main"))
    error("No 'main' function");
  shared_ptr<VMFrame> frame = make_shared<VMFrame>();
  frame->info = frame_info["main"];
  call_stack.push(frame);

  // run loop (keep going until we run out of instructions)
  while (!call_stack.empty() and frame->pc < frame->info.instructions.size())
  {

    // get the next instruction
    VMInstr &instr = frame->info.instructions[frame->pc];

    // increment the program counter
    ++frame->pc;

    // for debugging
    if (DEBUG)
    {
      // TODO
      cerr << endl
           << endl;
      cerr << "\t FRAME.........: " << frame->info.function_name << endl;
      cerr << "\t PC............: " << (frame->pc - 1) << endl;
      cerr << "\t INSTR.........: " << to_string(instr) << endl;
      cerr << "\t NEXT OPERAND..: ";
      if (!frame->operand_stack.empty())
        cerr << to_string(frame->operand_stack.top()) << endl;
      else
        cerr << "empty" << endl;
      cerr << "\t NEXT FUNCTION.: ";
      if (!call_stack.empty())
        cerr << call_stack.top()->info.function_name << endl;
      else
        cerr << "empty" << endl;
    }

    //----------------------------------------------------------------------
    // Literals and Variables
    //----------------------------------------------------------------------

    if (instr.opcode() == OpCode::PUSH)
    {
      frame->operand_stack.push(instr.operand().value());
    }

    else if (instr.opcode() == OpCode::POP)
    {
      frame->operand_stack.pop();
    }

    // TODO: Finish LOAD and STORE

    else if (instr.opcode() == OpCode::STORE)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      int pos = get<int>(instr.operand().value());
      if (pos >= frame->variables.size())
      {
        frame->variables.push_back(x);
      }
      else
      {
        frame->variables.at(pos) = x;
      }
    }

    else if (instr.opcode() == OpCode::LOAD)
    {
      VMValue x = frame->variables.at(get<int>(instr.operand().value()));
      frame->operand_stack.push(x);
    }

    //----------------------------------------------------------------------
    // Operations
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::ADD)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(add(y, x));
    }

    else if (instr.opcode() == OpCode::SUB)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(sub(y, x));
    }

    else if (instr.opcode() == OpCode::MUL)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(mul(y, x));
    }
    else if (instr.opcode() == OpCode::DIV)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(div(y, x));
    }

    else if (instr.opcode() == OpCode::AND)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(andd(y, x));
    }

    else if (instr.opcode() == OpCode::OR)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(get<bool>(x) or get<bool>(y));
    }

    else if (instr.opcode() == OpCode::NOT)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      frame->operand_stack.push(!get<bool>(x));
    }

    else if (instr.opcode() == OpCode::CMPEQ)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push(eq(y, x));
    }

    else if (instr.opcode() == OpCode::CMPLT)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(lt(y, x));
    }

    else if (instr.opcode() == OpCode::CMPLE)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(le(y, x));
    }

    else if (instr.opcode() == OpCode::CMPGT)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(gt(y, x));
    }

    else if (instr.opcode() == OpCode::CMPGE)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      frame->operand_stack.push(ge(y, x));
    }

    else if (instr.opcode() == OpCode::CMPNE)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push(neq(y, x));
    }

    else if (instr.opcode() == OpCode::RAND)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push((randoms(get<int>(y), get<int>(x))));
    }

    //----------------------------------------------------------------------
    // Branching
    //----------------------------------------------------------------------

    // TODO: Finish JMP and JMPF

    else if (instr.opcode() == OpCode::JMP)
    {
      int x = get<int>(instr.operand().value());
      frame->pc = x;
    }

    else if (instr.opcode() == OpCode::JMPF)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      int index = get<int>(instr.operand().value());
      if (holds_alternative<bool>(x))
      {
        if (!get<bool>(x))
        {
          frame->pc = index;
        }
      }
    }

    //----------------------------------------------------------------------
    // Functions
    //----------------------------------------------------------------------

    // TODO: Finish CALL, RET
    else if (instr.opcode() == OpCode::CALL)
    {
      string fun_name = get<string>(instr.operand().value());
      shared_ptr<VMFrame> new_frame = make_shared<VMFrame>();
      new_frame->info = frame_info[fun_name];
      call_stack.push(new_frame);
      for (int i = 0; i < frame_info[fun_name].arg_count; i++)
      {
        VMValue x = frame->operand_stack.top();
        new_frame->operand_stack.push(x);
        frame->operand_stack.pop();
      }
      frame = new_frame;
    }

    else if (instr.opcode() == OpCode::RET)
    {
      VMValue v = frame->operand_stack.top();
      frame->operand_stack.pop();
      call_stack.pop();
      if (!call_stack.empty())
      {
        frame = call_stack.top();
        frame->operand_stack.push(v);
      }
    }

    //----------------------------------------------------------------------
    // Built in functions
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::WRITE)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      cout << to_string(x);
    }

    else if (instr.opcode() == OpCode::READ)
    {
      string val = "";
      getline(cin, val);
      frame->operand_stack.push(val);
    }

    else if (instr.opcode() == OpCode::SLEN)
    {
      VMValue x1 = frame->operand_stack.top();
      ensure_not_null(*frame, x1);
      string x = get<string>(frame->operand_stack.top());
      frame->operand_stack.pop();
      int length = x.size();
      frame->operand_stack.push(length);
    }

    else if (instr.opcode() == OpCode::ALEN)
    {
      VMValue x1 = frame->operand_stack.top();
      ensure_not_null(*frame, x1);
      int x = get<int>(frame->operand_stack.top());
      frame->operand_stack.pop();
      vector<VMValue> val = array_heap[x];
      int length = val.size();
      frame->operand_stack.push(length);
    }

    else if (instr.opcode() == OpCode::GETC)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      string word = get<string>(x);
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      int index = get<int>(y);
      if (index >= word.size())
      {
        error("out-of-bounds string index (in main at 2: GETC())");
      }
      else if (index < 0)
      {
        error("out-of-bounds string index (in main at 2: GETC())");
      }
      string ch;
      ch.push_back(word[index]);
      frame->operand_stack.push(ch);
    }

    // TODO: Finish SLEN, ALEN, GETC, TODBL, TOSTR, CONCAT, TOINT

    else if (instr.opcode() == OpCode::TOINT)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      if (holds_alternative<double>(x))
      {
        int y = (int)get<double>(x);
        frame->operand_stack.push(y);
      }
      else if (holds_alternative<string>(x))
      {
        try
        {
          int y = stoi(get<string>(x));
          frame->operand_stack.push(y);
        }
        catch (exception &err)
        {
          error("cannot convert string to int (in main at 1: TOINT())");
        }
      }
    }

    else if (instr.opcode() == OpCode::TODBL)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      if (holds_alternative<int>(x))
      {
        double y = (double)get<int>(x);
        frame->operand_stack.push(y);
      }
      else if (holds_alternative<string>(x))
      {
        string word = get<string>(x);
        try
        {
          double y = stod(get<string>(x));
          frame->operand_stack.push(y);
        }
        catch (exception &err)
        {
          error("cannot convert string to double (in main at 1: TODBL())");
        }
      }
    }

    else if (instr.opcode() == OpCode::TOSTR)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      frame->operand_stack.push(to_string(x));
    }

    else if (instr.opcode() == OpCode::CONCAT)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      string word1 = get<string>(x);
      x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      string word2 = get<string>(x);
      string concat = word2 + word1;
      frame->operand_stack.push(concat);
    }

    //----------------------------------------------------------------------
    // heap
    //----------------------------------------------------------------------

    // TODO: Finish ALLOCS, ALLOCA, ADDF, SETF, GETF, SETI, GETI
    else if (instr.opcode() == OpCode::ALLOCA)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      int size = get<int>(frame->operand_stack.top());
      frame->operand_stack.pop();
      array_heap[next_obj_id] = vector<VMValue>(size, x);
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }

    else if (instr.opcode() == OpCode::ALLOCS)
    {
      struct_heap[next_obj_id] = {};
      frame->operand_stack.push(next_obj_id);
      ++next_obj_id;
    }

    else if (instr.opcode() == OpCode::ADDF)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      int oid = get<int>(x);
      struct_heap[oid][get<string>(instr.operand().value())];
    }

    else if (instr.opcode() == OpCode::SETF)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      int oid = get<int>(y);
      struct_heap[oid][get<string>(instr.operand().value())] = x;
    }

    else if (instr.opcode() == OpCode::GETF)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      int oid = get<int>(x);
      frame->operand_stack.push(struct_heap[oid][get<string>(instr.operand().value())]);
    }

    else if (instr.opcode() == OpCode::SETI)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      VMValue z = frame->operand_stack.top();
      ensure_not_null(*frame, z);
      frame->operand_stack.pop();
      int oid = get<int>(z);
      if (get<int>(y) < array_heap[oid].size())
      {
        if (get<int>(y) < 0)
        {
          error("out-of-bounds array index (in main at 5: SETI())");
        }
        array_heap[oid][get<int>(y)] = x;
      }
      else
      {
        error("out-of-bounds array index (in main at 5: SETI())");
      }
    }

    else if (instr.opcode() == OpCode::GETI)
    {
      VMValue x = frame->operand_stack.top();
      ensure_not_null(*frame, x);
      frame->operand_stack.pop();
      VMValue y = frame->operand_stack.top();
      ensure_not_null(*frame, y);
      frame->operand_stack.pop();
      int oid = get<int>(y);
      if (get<int>(x) < array_heap[oid].size())
      {
        if (get<int>(x) < 0)
        {
          error("out-of-bounds array index (in main at 4: GETI())");
        }
        frame->operand_stack.push(array_heap[oid][get<int>(x)]);
      }
      else
      {
        error("out-of-bounds array index (in main at 4: GETI())");
      }
    }

    //----------------------------------------------------------------------
    // special
    //----------------------------------------------------------------------

    else if (instr.opcode() == OpCode::DUP)
    {
      VMValue x = frame->operand_stack.top();
      frame->operand_stack.pop();
      frame->operand_stack.push(x);
      frame->operand_stack.push(x);
    }

    else if (instr.opcode() == OpCode::NOP)
    {
      // do nothing
    }

    else
    {
      error("unsupported operation " + to_string(instr));
    }
  }
}

void VM::ensure_not_null(const VMFrame &f, const VMValue &x) const
{
  if (holds_alternative<nullptr_t>(x))
    error("null reference", f);
}

VMValue VM::add(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x))
    return (get<int>(x) + get<int>(y));
  else
    return get<double>(x) + get<double>(y);
}

VMValue VM::randoms(const VMValue &x, const VMValue &y) const
{
  return rand() % get<int>(y) + get<int>(x);
}

// TODO: Finish the rest of the following arithmetic operators

VMValue VM::sub(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) - get<int>(y);
  else
    return get<double>(x) - get<double>(y);
}

VMValue VM::mul(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) * get<int>(y);
  else
    return get<double>(x) * get<double>(y);
}

VMValue VM::div(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x))
    return get<int>(x) / get<int>(y);
  else
    return get<double>(x) / get<double>(y);
}

VMValue VM::andd(const VMValue &x, const VMValue &y) const
{
  return get<bool>(x) and get<bool>(y);
}

VMValue VM::orr(const VMValue &x, const VMValue &y) const
{
  return get<bool>(x) or get<bool>(y);
}

VMValue VM::eq(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<nullptr_t>(x) and not holds_alternative<nullptr_t>(y))
    return false;
  else if (not holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return false;
  else if (holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return true;
  else if (holds_alternative<int>(x))
    return get<int>(x) == get<int>(y);
  else if (holds_alternative<double>(x))
    return get<double>(x) == get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) == get<string>(y);
  else
    return get<bool>(x) == get<bool>(y);
}

VMValue VM::neq(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<nullptr_t>(x) and not holds_alternative<nullptr_t>(y))
    return true;
  else if (not holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return true;
  else if (holds_alternative<nullptr_t>(x) and holds_alternative<nullptr_t>(y))
    return false;
  else if (holds_alternative<int>(x))
    return get<int>(x) != get<int>(y);
  else if (holds_alternative<double>(x))
    return get<double>(x) != get<double>(y);
  else if (holds_alternative<string>(x))
    return get<string>(x) != get<string>(y);
  else
    return get<bool>(x) != get<bool>(y);
}

// TODO: Finish the rest of the comparison operators

VMValue VM::lt(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x) && holds_alternative<int>(y))
    return get<int>(x) < get<int>(y);
  else if (holds_alternative<double>(x) && holds_alternative<double>(y))
    return get<double>(x) < get<double>(y);
  else
    return get<string>(x) < get<string>(y);
}

VMValue VM::le(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x) && holds_alternative<int>(y))
    return get<int>(x) <= get<int>(y);
  else if (holds_alternative<double>(x) && holds_alternative<double>(y))
    return get<double>(x) <= get<double>(y);
  else
    return get<string>(x) <= get<string>(y);
}

VMValue VM::gt(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x) && holds_alternative<int>(y))
    return get<int>(x) > get<int>(y);
  else if (holds_alternative<double>(x) && holds_alternative<double>(y))
    return get<double>(x) > get<double>(y);
  else
    return get<string>(x) > get<string>(y);
}

VMValue VM::ge(const VMValue &x, const VMValue &y) const
{
  if (holds_alternative<int>(x) && holds_alternative<int>(y))
    return get<int>(x) >= get<int>(y);
  else if (holds_alternative<double>(x) && holds_alternative<double>(y))
    return get<double>(x) >= get<double>(y);
  else
    return get<string>(x) >= get<string>(y);
}

VMValue VM::to_int(const VMValue &x) const
{
  if (holds_alternative<double>(x))
    return get<int>(x);
  else if (holds_alternative<string>(x))
    return get<int>(x);
}

VMValue VM::to_dbl(const VMValue &x) const
{
  if (holds_alternative<int>(x))
    return get<double>(x);
  else if (holds_alternative<string>(x))
    return get<double>(x);
}

VMValue VM::to_string_func(const VMValue &x) const
{
  if (holds_alternative<double>(x))
    return get<string>(x);
  else if (holds_alternative<int>(x))
    return get<string>(x);
}
