#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include "customtype.hpp"
#include "arraydim.hpp"

class Array : public CustomType
{

public:
    Array(DataType t, ArrayDim *d);
    ~Array();
    virtual void printOn(std::ostream &out) const override;
    virtual Type ConvertToType() const;
    bool getUknown();

private:
    DataType data_type;
    ArrayDim *dims;
    bool uknown = false;
};

#endif