#ifndef __IDLIST_HPP__
#define __IDLIST_HPP__

#include "id.hpp"
#include <vector>

class IdList : public AST
{
public:
    IdList();
    ~IdList();
    const std::vector<Id *> get_idlist();
    void append(Id *id);
    virtual void printOn(std::ostream &out) const override;
    virtual void sem() override;

private:
    std::vector<Id *> idlist;
};

#endif