#include "rettype.hpp"

RetType::RetType(DataType dt) : data_type(dt) {}

void RetType::printOn(std::ostream &out) const 
{
    out << "RetType(";
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
        case TYPE_VOID:
        {
            out << "void";
            break;
        }
        default:
        {
            out << "Not valid data_type"; // This should never be reached.
            exit(1) ;
        }
    }
    out << ")";
}

Type RetType::ConvertToType() const
{
    
}