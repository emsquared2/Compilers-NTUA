#include "array.hpp"

Array::Array(DataType t, ArrayDim *d) : data_type(t), dims(d) 
{
        uknown = dims->getUknown();
}
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
        {
            out << "int";
            break;
        }
        case TYPE_CHAR: 
        {
            out << "char";
            break;
        }
        default:
        {
            out << "Not valid data_type in Array"; // This should never be reached.
            exit(1) ;
        }
    }
    // out << "( Dimensions --> ";
    dims->printOn(out);

    // out << ") )";
}

bool Array::getUknown()
{
    return uknown;
}

Type Array::ConvertToType() const
{

}

