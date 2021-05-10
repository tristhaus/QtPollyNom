#include "constant.h"

namespace Backend {

Constant::Constant(double input) : value(input)
{
}

int Constant::GetLevel()
{
    return 0;
}

bool Constant::IsMonadic()
{
    return true;
}

std::optional<double> Constant::Evaluate(double)
{
    return this->value;
}

std::optional<std::string> Constant::Print()
{
    return std::to_string(this->value);
}

bool Constant::operator==(const Expression &other) const
{
    if (const Constant * b = dynamic_cast<const Constant*>(&other))
    {
        return b != NULL && this->value == b->value;
    }
    else
    {
        return false;
    }
}

bool Constant::operator!=(const Expression &other) const
{
    return !this->operator==(other);
}

}
