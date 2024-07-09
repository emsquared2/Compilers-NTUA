#include "arraydim.hpp"

ArrayDim::ArrayDim() : dims(), unknown(false) {}

ArrayDim::~ArrayDim()
{
    for (Const *num : dims)
    {
        delete num;
    }
}

void ArrayDim::printOn(std::ostream &out) const
{
    if (unknown)
        out << "[ ]";
    
    for (auto num : getReversed(dims))
    {
        out << "[" << *num << "]";
    }
}

bool ArrayDim::isEmpty() const
{
    return dims.empty();
}

void ArrayDim::setUnknownFirstDim()
{
    unknown = true;
}

void ArrayDim::append(Const *num)
{
    dims.push_back(num);
}

std::vector<Const *> ArrayDim::getDims() const
{ 
    return dims;
}

bool ArrayDim::getUnknown() const
{
    return unknown;
}