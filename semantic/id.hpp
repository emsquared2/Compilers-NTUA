#ifndef __ID_HPP__
#define __ID_HPP__

#include "lvalue.hpp"
#include <string>

class Id : public LValue
{
public:
    Id(std::string s);
    virtual void printOn(std::ostream &out) const override;
    virtual const char *getName() const override;
    virtual void sem() override;

private:
    std::string name;
};

#endif