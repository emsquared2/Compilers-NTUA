#ifndef __RETTYPE_HPP__
#define __RETTYPE_HPP__

#include "customtype.hpp"

class RetType : public CustomType
{
public:
    RetType(DataType dt);
    ~RetType() = default;
    virtual void printOn(std::ostream &out) const override;
    virtual Type ConvertToType() const override;

private:
    DataType data_type;
};

#endif