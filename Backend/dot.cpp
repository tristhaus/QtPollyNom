/*
 * This file is part of QtPollyNom.
 *
 * QtPollyNom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtPollyNom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtPollyNom.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <algorithm>
#include <random>
#include "dot.h"
#include "mathhelper.h"

namespace Backend {

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

    void Dot::ResetIsActive()
    {
        isActive = false;
    }

    void Dot::CheckForHit(const std::shared_ptr<Expression> expression, const std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData)
    {
            bool dotIsHit = false;
            auto xDot = this->GetCoordinates().first;
            auto yDot = this->GetCoordinates().second;
            auto rDot = this->GetRadius();

            auto isInsideDot = [&](double x, double y)
            {
                return SquareDistance(x, y, xDot, yDot) <= (rDot * rDot);
            };

            auto graphDataIt = graphData.begin();
            auto graphDataEnd = graphData.end();

            for(; graphDataIt != graphDataEnd; ++graphDataIt)
            {
                auto xBegin = graphDataIt->first.begin();
                auto xEnd = graphDataIt->first.end();
                auto xIt = std::lower_bound(xBegin, xEnd, xDot - this->GetRadius());

                auto yIt = graphDataIt->second.begin() + (xIt - xBegin);

                auto xIntervalEnd = std::upper_bound(xBegin, xEnd, xDot + this->GetRadius());

                for(; xIt != xIntervalEnd && xIt != xEnd; ++xIt, ++yIt)
                {
                    if(isInsideDot(*xIt, *yIt))
                    {
                        dotIsHit = true;
                        break;
                    }
                }
            }

            if(dotIsHit)
            {
                this->SetIsActive(true);
                return;
            }

            const double localEpsilon = 1e-6;
            const double dampingFactor = 0.8;
            const unsigned int maxIterations = 10000;

            double mid = xDot;
            double increment = rDot;

            unsigned int iterations = 0;

            while (!dotIsHit && increment > localEpsilon && iterations < maxIterations)
            {
                auto midEvaluationResult = expression->Evaluate(mid);
                if (midEvaluationResult.has_value())
                {
                    if (isInsideDot(mid, midEvaluationResult.value()))
                    {
                        this->SetIsActive(true);
                        break;
                    }
                }

                auto rightEvaluationResult = expression->Evaluate(mid + increment);
                if (rightEvaluationResult.has_value())
                {
                    if (isInsideDot(mid + increment, rightEvaluationResult.value()))
                    {
                        this->SetIsActive(true);
                        break;
                    }
                }

                auto leftEvaluationResult = expression->Evaluate(mid - increment);
                if (leftEvaluationResult.has_value())
                {
                    if (isInsideDot(mid - increment, leftEvaluationResult.value()))
                    {
                        this->SetIsActive(true);
                        break;
                    }
                }

                // well, that was easy. Now the hard part.
                if (midEvaluationResult.has_value() && rightEvaluationResult.has_value() && leftEvaluationResult.has_value())
                {
                    auto leftGradient = SquareDistance(xDot, yDot, mid - increment, leftEvaluationResult.has_value());
                    auto rightGradient = SquareDistance(xDot, yDot, mid + increment, rightEvaluationResult.has_value());

                    if (leftGradient < rightGradient)
                    {
                        increment *= leftGradient < 10 ? leftGradient / 20 : 0.75;
                        mid -= increment;
                    }
                    else
                    {
                        increment *= rightGradient < 10 ? rightGradient / 20 : 0.75;
                        mid += increment;
                    }

                    continue;
                }

                // from now on, we are in desperate trouble, and eventually skip out
                iterations++;

                // in the following cases, damp the movement
                if (midEvaluationResult.has_value() && rightEvaluationResult.has_value())
                {
                    mid += increment * dampingFactor;
                    increment *= 0.5;
                    continue;
                }
                else if (midEvaluationResult.has_value() && leftEvaluationResult.has_value())
                {
                    mid -= increment * dampingFactor;
                    increment *= 0.5;
                    continue;
                }
                else if (rightEvaluationResult.has_value())
                {
                    mid += increment * dampingFactor;
                    continue;
                }
                else if (leftEvaluationResult.has_value())
                {
                    mid -= increment * dampingFactor;
                    continue;
                }

                // if everything else fails, pick a new random Mid somewhere in the dot
                double rand = Dot::GetRandom();
                mid = xDot + rDot * 0.75 * (rand - 0.5);
            }

    }

    /* static class member */ double Dot::GetRandom()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

}
