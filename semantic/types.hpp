#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ASTabstracts.hpp"
#include "expressions.hpp"

/* ---------------------------------------------------------------------
   ------------------------- Array Dimensions --------------------------
   --------------------------------------------------------------------- */

class Const;

class ArrayDim : public AST
{
public:
    ArrayDim() : dims() {}
    ~ArrayDim()
    {
        for (Const *num : dims)
        {
            delete num;
        }
    }
    virtual void printOn(std::ostream &out) const override
    {
        if (unknown)
            out << "[ ]";
        for (auto num = dims.rbegin(); num != dims.rend(); ++num)
        {
            out << "[" << **num << "]";
        }
    }
    bool isEmpty()
    {
        return dims.empty();
    }
    void setUknownFirstDim()
    {
        unknown = true;
    }
    void append(Const *num)
    {
        dims.push_back(num);
    }
    bool getUknown() { return unknown; }

private:
    std::vector<Const *> dims;
    bool unknown = false;
};


/* #####################################################################
   #####                        CUSTOM TYPES                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- Array -------------------------------
   --------------------------------------------------------------------- */

class Array : public CustomType
{

public:
    Array(DataType t, ArrayDim *d) : data_type(t), dims(d)
    {
        uknown = dims->getUknown();
    }
    ~Array()
    {
        delete dims;
    }
    virtual void printOn(std::ostream &out) const override
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
                exit(1);
            }
        }
        // out << "( Dimensions --> ";
        dims->printOn(out);

        // out << ") )";
    }
    virtual Type ConvertToType() const override
    {
    }
    bool getUknown() { return uknown; }

private:
    DataType data_type;
    ArrayDim *dims;
    bool uknown = false;
};

/* ---------------------------------------------------------------------
   ------------------------------ RetType ------------------------------
   --------------------------------------------------------------------- */

class RetType : public CustomType
{
public:
    RetType(DataType dt) : data_type(dt) {}
    ~RetType() {}
    virtual void printOn(std::ostream &out) const override
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
                exit(1);
            }
        }
        out << ")";
    }
    virtual Type ConvertToType() const override
    {
        switch (data_type)
        {
            case TYPE_INTEGER:
            {
                return typeInteger;
                break;
            }
            case TYPE_CHAR:
            {
                return typeChar;
                break;
            }
            case TYPE_VOID:
            {
                return typeVoid;
                break;
            }
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
            {
                return typeInteger;
                // exit(1);
            }
        }
    }

private:
    DataType data_type;
};

/* ---------------------------------------------------------------------
   ----------------------------- FParType ------------------------------
   --------------------------------------------------------------------- */

class FParType : public CustomType
{
public:
    FParType(DataType t) : data_type(t) { array = nullptr; }
    FParType(Array *arr) : array(arr)
    {
        data_type = (array->getUknown()) ? TYPE_IARRAY : TYPE_ARRAY;
    }
    ~FParType()
    {
        if (array != nullptr)
            delete array;
    }
    virtual void printOn(std::ostream &out) const override
    {
        out << "FParType(";
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
            case TYPE_ARRAY:
            {
                out << "array of known size --> ";
                array->printOn(out);
                break;
            }
            case TYPE_IARRAY:
            {
                out << "array of uknown size --> ";
                array->printOn(out);
                break;
            }
            default:
            {
                out << "Not valid data_type in Array"; // This should never be reached.
                exit(1);
            }
        }
        out << ")";
    }
    virtual Type ConvertToType() const override
    {
        switch (data_type)
        {
            case TYPE_INTEGER:
            {
                return typeInteger;
                break;
            }
            case TYPE_CHAR:
            {
                return typeChar;
                break;
            }
            case TYPE_VOID:
            {
                return typeVoid;
                break;
            }
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
            {
                return typeInteger;
                // exit(1);
            }
        }
    }

private:
    DataType data_type;
    Array *array;
};

#endif
