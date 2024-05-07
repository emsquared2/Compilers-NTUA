#include "fpartype.hpp"

FParType::FParType(DataType t) : data_type(t), array(nullptr) {}

FParType::FParType(Array *arr) : array(arr)
{
    data_type = (array->getUnknown()) ? TYPE_IARRAY : TYPE_ARRAY;
}

FParType::~FParType()
{
    if (array != nullptr)
        delete array;
}

void FParType::printOn(std::ostream &out) const
{
    out << "FParType(";
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
        case TYPE_ARRAY:
            out << "array of known size --> ";
            array->printOn(out);
            break;
        case TYPE_IARRAY:
            out << "array of unknown size --> ";
            array->printOn(out);
            break;
        default:
            out << "Not valid data_type in Array"; // This should never be reached.
            exit(1);
    }
    out << ")";
}

void FParType::sem() {
    switch (data_type)
    {
        case TYPE_IARRAY:
        case TYPE_ARRAY:
            array->sem();
            break;

    }
}

Type FParType::ConvertToType() const
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
        case TYPE_ARRAY:
            return array->ConvertToType();
            break;
        case TYPE_IARRAY:
            return array->ConvertToType();
            break;
        default:
            std::cout << "Not valid function parameter type" << std::endl; // This should never be reached.
            exit(1);
    }
}