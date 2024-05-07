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
    bool isEmpty() const;
    void setUnknownFirstDim();
    void append(Const *num);
    std::vector<Const *> getDims() const;
    bool getUnknown() const;

private:
    std::vector<Const *> dims;
    bool unknown = false;
};

#endif