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

}
