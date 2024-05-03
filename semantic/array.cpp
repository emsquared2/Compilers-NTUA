#include "array.hpp"

Array::Array(DataType t, ArrayDim *d) : data_type(t), dims(d), unknown(d->getUnknown()) {}

Array::~Array()
{
    delete dims;
}

void Array::printOn(std::ostream &out) const
{
    out << "Array_";
    switch (data_type)
    {
    case TYPE_INTEGER:
        out << "int";
        break;
    case TYPE_CHAR:
        out << "char";
        break;
    default:
        out << "Not valid data_type in Array";
        exit(1);
    }
    dims->printOn(out);
}

Type Array::ConvertToType() const
{
    Type baseType = (data_type == TYPE_INTEGER) ? typeInteger : typeChar;
    for (Const *dim : dims->getDims())
        baseType = typeArray(dim->getVal(), baseType);
    if (unknown)
        baseType = typeIArray(baseType);
    return baseType;
}

bool Array::getUnknown() 
{
    return unknown;
}