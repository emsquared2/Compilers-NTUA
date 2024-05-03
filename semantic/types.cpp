#include "types.hpp"

/* ---------------------------------------------------------------------
   ------------------------- Array Dimensions --------------------------
   --------------------------------------------------------------------- */

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
    for (auto num = dims.rbegin(); num != dims.rend(); ++num)
    {
        out << "[" << **num << "]";
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


/* #####################################################################
   #####                        CUSTOM TYPES                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- Array -------------------------------
   --------------------------------------------------------------------- */

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

bool Array::getUnknown() const
{
    return unknown;
}

/* ---------------------------------------------------------------------
   ------------------------------ RetType ------------------------------
   --------------------------------------------------------------------- */

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
        // case TYPE_ARRAY:
        // {
        //     return typeArray;
        //     break;
        // }
        // case TYPE_IARRAY:
        // {
        //     return typeIArray;
        //     break;
        // }
        default:
            return typeInteger;
            // exit(1);
    }   
}

/* ---------------------------------------------------------------------
   ----------------------------- FParType ------------------------------
   --------------------------------------------------------------------- */

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
        return typeInteger;
        // exit(1);
    }
}
