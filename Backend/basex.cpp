#include "basex.h"

namespace Backend {

BaseX::BaseX()
{
}

int BaseX::GetLevel()
{
    return 0;
}

bool BaseX::IsMonadic()
{
    return true;
}

std::optional<double> BaseX::Evaluate(double input)
{
    return input;
}

std::optional<std::string> BaseX::Print()
{
    return std::string("x");
}

bool BaseX::operator==(const Expression &other) const
{
    if (const BaseX * b = dynamic_cast<const BaseX*>(&other))
    {
        return b != NULL;
    }
    else
    {
        return false;
    }
}

bool BaseX::operator!=(const Expression &other) const
{
    return !this->operator==(other);
}

}
