#ifndef __FPARTYPE_HPP__
#define __FPARTYPE_HPP__

#include "customtype.hpp"
#include "array.hpp"

class FParType : public CustomType
{
public:
    FParType(DataType t);
    FParType(Array *arr);
    ~FParType();
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;
    virtual Type ConvertToType() const override;

private:
    DataType data_type;
    Array *array;
};

#endif