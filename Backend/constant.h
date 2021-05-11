#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"

namespace Backend {

class Constant final : public Expression
{
    Q_OBJECT

private:

    double value;

public:
    Constant(double input);

    virtual int GetLevel() const;
    virtual bool IsMonadic() const;
    virtual std::optional<double> Evaluate(double) const;
    virtual std::optional<std::string> Print() const;
    virtual bool operator==(const Expression &other) const;
    virtual bool operator!=(const Expression &other) const;
};

}

#endif // CONSTANT_H
