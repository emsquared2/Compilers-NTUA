#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

#include "localdef.hpp"
#include "header.hpp"
#include "block.hpp"
#include "localdeflist.hpp"
#include "decl.hpp"


class FuncDef : public LocalDef
{
public:
    FuncDef(Header *h, LocalDefList *ldl, Block *b);
    ~FuncDef();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    void ProgramSem();
    virtual llvm::Function *compile() override;

    virtual void setOuterFunction(std::string outer_func_name) override;

    void optimizeFunc(llvm::Function *function);

private:
    Header *header;
    LocalDefList *local_def_list;
    Block *block;
    std::string mangled_name;

    std::vector<std::string> captured_names;
    std::vector<llvmType *> captured_types;
    std::vector<bool> captured_ref;

    llvmType *createFunctionStructType();
    void createFunctionStackFrame(llvmType *stack_frame_type);
    void populateFunctionStackFrame();
};

#endif