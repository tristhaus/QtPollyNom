#ifndef BASEX_H
#define BASEX_H

#include "expression.h"

namespace Backend {

class BaseX : Expression
{
    Q_OBJECT

public:
    BaseX();

    virtual std::optional<double> Evaluate(double input);
};

}

#endif // BASEX_H
