#pragma once

#include "atl/include/ofstream.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

namespace ACC {

namespace X64 {

class Operand {
public:
  virtual ~Operand() {}

  virtual atl::string opType() const = 0;
  virtual atl::string toString() const = 0;
};

class Register : public Operand {
public:
  const int bits;
  const atl::string name;

  Register(int bits, const atl::string &name);
  bool operator==(const Register &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class GlobalVariable : public Operand {
public:
  const atl::string name;
  const int size;

  GlobalVariable(const atl::string &name, int size);
  bool operator==(const GlobalVariable &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class IntValue : public Operand {
public:
  const atl::string val;

  IntValue(const atl::string &val);
  bool operator==(const IntValue &rhs);
  atl::string opType() const override;
  atl::string toString() const override;
};

class None : public Operand {
public:
  atl::string opType() const override;
  atl::string toString() const override;
};

class Writer final {
public:
  Writer(const atl::string &filename);
  Writer(const Writer &) = delete;

  void add(const atl::shared_ptr<X64::Operand> &op1,
           const atl::shared_ptr<X64::Operand> &op2,
           const atl::string &comment = "");
  void block(atl::string blockName, const atl::string &comment = "");
  void call(const atl::string &ident, const atl::string &comment = "");
  void cmp(const atl::shared_ptr<X64::Operand> &reg, const int value,
           const atl::string &comment = "");
  void comment(const atl::string &comment);
  void imul(const atl::shared_ptr<X64::Operand> &dst,
            const atl::shared_ptr<X64::Operand> &src,
            const atl::string &comment = "");
  void jeq(const atl::string &label, const atl::string &comment = "");
  void jmp(const atl::string &label, const atl::string &comment = "");
  void mov(const atl::shared_ptr<X64::Operand> &dst,
           const atl::shared_ptr<X64::Operand> &src,
           const atl::string &comment = "");
  void pop(const atl::shared_ptr<X64::Operand> &reg,
           const atl::string &comment = "");
  void push(const atl::shared_ptr<X64::Operand> &op,
            const atl::string &comment = "");
  void ret(const atl::string &comment = "");
  void sub(const atl::shared_ptr<X64::Operand> &reg, const int value,
           const atl::string &comment = "");
  void write(const atl::string &str);

private:
  atl::ofstream x86Output;
};

static atl::shared_ptr<Register> rax(new Register(32, "rax"));
static atl::shared_ptr<Register> rbx(new Register(32, "rbx"));
static atl::shared_ptr<Register> rcx(new Register(32, "rcx"));
static atl::shared_ptr<Register> rdx(new Register(32, "rdx"));
static atl::shared_ptr<Register> rsi(new Register(32, "rsi"));
static atl::shared_ptr<Register> rdi(new Register(32, "rdi"));
static atl::shared_ptr<Register> rsp(new Register(32, "rsp"));
static atl::shared_ptr<Register> rbp(new Register(32, "rbp"));

} // namespace X64

} // namespace ACC
