#include "dot.h"
#include <utility>

Dot::Dot(double x, double y, bool goodDot)
    : x(x), y(y), radius(0.25),
      isActive(false), isGood(goodDot)
{
}

void Dot::SetIsActive(bool active)
{
    isActive = active;
}

std::pair<double, double> Dot::GetCoordinates() const
{
    return std::pair(x, y);
}

double Dot::GetRadius() const
{
    return radius;
}

bool Dot::IsActive() const
{
    return isActive;
}

bool Dot::IsGood() const
{
    return isGood;
}
