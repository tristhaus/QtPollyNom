#include "basex.h"

namespace Backend {

BaseX::BaseX()
{
}

BaseX::~BaseX()
{
}

int BaseX::GetLevel() const
{
    return 0;
}

bool BaseX::IsMonadic() const
{
    return true;
}

std::optional<double> BaseX::Evaluate(double input) const
{
    return input;
}

std::optional<std::string> BaseX::Print() const
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
