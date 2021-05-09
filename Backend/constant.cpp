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

}
