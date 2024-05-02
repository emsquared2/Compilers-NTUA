#ifndef __ARRAYDIM_HPP__
#define __ARRAYDIM_HPP__

#include "const.hpp"
#include <vector>

class ArrayDim : public AST
{
public:
    ArrayDim();
    ~ArrayDim();
    virtual void printOn(std::ostream &out) const override;
    bool isEmpty();
    void setUknownFirstDim();
    void append(Const *num);
    bool getUknown();

private:
    std::vector<Const *> dims;
    bool unknown = false;
};

#endif