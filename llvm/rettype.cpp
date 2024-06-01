#include "rettype.hpp"

RetType::RetType(DataType dt) : data_type(dt) {}

RetType::~RetType() {}

void RetType::printOn(std::ostream &out) const
{
    out << "RetType(";
    switch (data_type)
    {
        case TYPE_INTEGER:
            out << "int";
            break;
        case TYPE_CHAR:
            out << "char";
            break;
        case TYPE_VOID:
            out << "void";
            break;
        default:
            out << "Not valid data_type";
            exit(1);
    }
    out << ")";
}

Type RetType::ConvertToType() const
{
    switch (data_type)
    {
        case TYPE_INTEGER:
            return typeInteger;
            break;
        case TYPE_CHAR:
            return typeChar;
            break;
        case TYPE_VOID:
            return typeVoid;
            break;
        default:
            std::cout << "Not valid return type" << std::endl;
            exit(1);
    }
}