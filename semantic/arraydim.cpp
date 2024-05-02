#include "arraydim.hpp"

ArrayDim::ArrayDim() : dims() {}
ArrayDim::~ArrayDim()
{
    for (Const *num :dims)
        delete num;
}
void ArrayDim::printOn(std::ostream &out) const
{
    if (unknown)
        out << "[ ]";
    for (auto num = dims.rbegin(); num != dims.rend(); ++num)
    {
        out << "[" << **num << "]";
    }
}

bool ArrayDim::isEmpty()
{
    return dims.empty();
}

void ArrayDim::setUknownFirstDim()
{
    unknown = true;
}

void ArrayDim::append(Const *num)
{
    dims.push_back(num);
}

bool ArrayDim::getUknown() 
{ 
    return unknown; 
}