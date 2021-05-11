#ifndef BASEX_H
#define BASEX_H

#include "expression.h"

namespace Backend {

class BaseX final : public Expression
{
    Q_OBJECT

public:
    BaseX();

    virtual int GetLevel() const;
    virtual bool IsMonadic() const;
    virtual std::optional<double> Evaluate(double input) const;
    virtual std::optional<std::string> Print() const;
    virtual bool operator==(const Expression &other) const;
    virtual bool operator!=(const Expression &other) const;
};

}

#endif // BASEX_H
