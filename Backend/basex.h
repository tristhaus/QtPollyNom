#ifndef BASEX_H
#define BASEX_H

#include "expression.h"

namespace Backend {

class BaseX final : public Expression
{
    Q_OBJECT

public:
    BaseX();

    virtual int GetLevel();
    virtual bool IsMonadic();
    virtual std::optional<double> Evaluate(double input);
    virtual std::optional<std::string> Print();
    virtual bool operator==(const Expression &other) const;
    virtual bool operator!=(const Expression &other) const;
};

}

#endif // BASEX_H
