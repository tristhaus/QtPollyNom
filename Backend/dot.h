#ifndef DOT_H
#define DOT_H

#include <utility>

class Dot
{
private:
    double x;
    double y;
    double radius;
    bool isActive;
    bool isGood;

public:
    Dot(double x, double y, bool goodDot = true);
    ~Dot() = default;
    Dot(const Dot&) = default;
    Dot(Dot&&) = default;
    Dot& operator=(const Dot&) = default;
    Dot& operator=(Dot&&) = default;

    void SetIsActive(bool active);

    std::pair<double, double> GetCoordinates() const;
    double GetRadius() const;
    bool IsActive() const;
    bool IsGood() const;
};

#endif // DOT_H
