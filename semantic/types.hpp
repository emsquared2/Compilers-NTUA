#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "ASTabstracts.hpp"
#include "expressions.hpp"

/* ---------------------------------------------------------------------
   ------------------------- Array Dimensions --------------------------
   --------------------------------------------------------------------- */

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

/* #####################################################################
   #####                        CUSTOM TYPES                       #####
   ##################################################################### */

/* ---------------------------------------------------------------------
   ------------------------------- Array -------------------------------
   --------------------------------------------------------------------- */

class Array : public CustomType
{
public:
    Array(DataType t, ArrayDim *d);
    ~Array();
    virtual void printOn(std::ostream &out) const override;
    virtual Type ConvertToType() const override;
    bool getUnknown() const;

private:
    DataType data_type;
    ArrayDim *dims;
    bool unknown = false;
};

/* ---------------------------------------------------------------------
   ------------------------------ RetType ------------------------------
   --------------------------------------------------------------------- */

class RetType : public CustomType
{
public:
    RetType(DataType dt);
    ~RetType();
    virtual void printOn(std::ostream &out) const override;
    virtual Type ConvertToType() const override;

private:
    DataType data_type;
};

/* ---------------------------------------------------------------------
   ----------------------------- FParType ------------------------------
   --------------------------------------------------------------------- */

class FParType : public CustomType
{
public:
    FParType(DataType t);
    FParType(Array *arr);
    ~FParType();
    virtual void printOn(std::ostream &out) const override;
    virtual Type ConvertToType() const override;

private:
    DataType data_type;
    Array *array;
};

#endif // __TYPES_HPP__
