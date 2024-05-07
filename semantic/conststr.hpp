#ifndef __CONSTSTR_HPP__
#define __CONSTSTR_HPP__

#include "lvalue.hpp"
#include <string>

class ConstStr : public LValue
{
public:
    ConstStr(std::string s);
    virtual void printOn(std::ostream &out) const override;
    virtual const char *getName() const override;
    virtual void sem() override;

private:
    std::string str;
};

#endif