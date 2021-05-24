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
#include "evaluator.h"

namespace Backend {

    Evaluator::Evaluator(std::shared_ptr<Expression> expression, std::vector<std::shared_ptr<Dot>> dots, double minX, double maxX, double limit)
        : expression(expression),
          dots(dots),
          minX(minX),
          maxX(maxX),
          limit(limit),
          EvaluateWasCalled(false),
          AddPointToCurrentBranchAtWasCalled(false)
    {
    }

    static double forwardX(double in)
    {
        return in;
    }

    static double backwardX(double in)
    {
        return -in;
    }

    static double SquareDistance(double x1, double y1, double x2, double y2)
    {
       return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    }

    std::vector<std::pair<std::vector<double>, std::vector<double>>> Evaluator::Evaluate()
    {
        if(AddPointToCurrentBranchAtWasCalled)
        {
            throw std::exception("programmer mistake: cannot call Evaluate() after calling AddPointToCurrentBranchAt()");
        }
        EvaluateWasCalled = true;

        this->CreateGraph();

        this->CheckDotsForHit();

        return this->GetGraph();
    }

    void Evaluator::CreateGraph()
    {
        double lastXinPreviousInterval = this->minX;
        double x = lastXinPreviousInterval;

        while (x < this->maxX)
        {
            graphData.emplace_back(std::make_pair(std::vector<double>(),std::vector<double>()));
            auto & branch = graphData.back();

            // look for interval
            double xInCurrentInterval = lastXinPreviousInterval;
            bool foundInterval = false;
            while (!foundInterval && xInCurrentInterval < this->maxX)
            {
                xInCurrentInterval += this->LargeIncrement;
                auto result = this->expression->Evaluate(xInCurrentInterval);
                if (result.has_value())
                {
                    this->AddCompletePointToCurrentBranch(xInCurrentInterval, result.value());
                    foundInterval = true;
                }
            }

            // maybe the function is not defined anywhere in our window
            if (!foundInterval)
            {
                break;
            }

            double lastXinCurrentInterval;

            // work inside interval - backward
            this->WorkAnInterval(backwardX, x, xInCurrentInterval, lastXinCurrentInterval);

            // work inside interval - forward
            this->WorkAnInterval(forwardX, x, xInCurrentInterval, lastXinCurrentInterval);

            // finish up interval
            if (!branch.first.empty())
            {
                lastXinPreviousInterval = lastXinCurrentInterval;
            }
        }
    }

    void Evaluator::CheckDotsForHit()
    {
        for(auto dotIterator = this->dots.begin(); dotIterator != this->dots.end(); ++dotIterator)
        {
            bool dotIsHit = false;
            auto searchPredicate = [&](std::pair<std::vector<double>, std::vector<double>> branch){ auto x = (*dotIterator)->GetCoordinates().first; return branch.first.front() <= x && branch.first.back() >= x; };
            auto relevantBranch = std::find_if(graphData.begin(), graphData.end(), searchPredicate);

            auto xDot = (*dotIterator)->GetCoordinates().first;
            auto yDot = (*dotIterator)->GetCoordinates().second;
            auto rDot = (*dotIterator)->GetRadius();

            auto isInsideDot = [&](double x, double y)
            {
                return SquareDistance(x, y, xDot, yDot) <= (rDot * rDot);
            };

            if(relevantBranch != graphData.end())
            {
                auto xBegin = relevantBranch->first.begin();
                auto xEnd = relevantBranch->first.end();
                auto xIt = std::lower_bound(xBegin, xEnd, xDot - (*dotIterator)->GetRadius());

                auto yIt = relevantBranch->second.begin() + (xIt - xBegin);

                auto xIntervalEnd = std::upper_bound(xBegin, xEnd, xDot + (*dotIterator)->GetRadius());

                for(; xIt != xIntervalEnd; ++xIt, ++yIt)
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
                (*dotIterator)->SetIsActive(true);
                continue;
            }

            const double localEpsilon = 1e-6;
            const double dampingFactor = 0.8;
            const uint maxIterations = 10000;

            double mid = xDot;
            double increment = rDot;

            uint iterations = 0;

            while (!dotIsHit && increment > localEpsilon && iterations < maxIterations)
            {
                auto midEvaluationResult = expression->Evaluate(mid);
                if (midEvaluationResult.has_value())
                {
                    if (isInsideDot(mid, midEvaluationResult.value()))
                    {
                        (*dotIterator)->SetIsActive(true);
                        break;
                    }
                }

                auto rightEvaluationResult = expression->Evaluate(mid + increment);
                if (rightEvaluationResult.has_value())
                {
                    if (isInsideDot(mid + increment, rightEvaluationResult.value()))
                    {
                        (*dotIterator)->SetIsActive(true);
                        break;
                    }
                }

                auto leftEvaluationResult = expression->Evaluate(mid - increment);
                if (leftEvaluationResult.has_value())
                {
                    if (isInsideDot(mid - increment, leftEvaluationResult.value()))
                    {
                        (*dotIterator)->SetIsActive(true);
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
                double rand = Evaluator::GetRandom();
                mid = xDot + rDot * 0.75 * (rand - 0.5);
            }
        }
    }

    bool Evaluator::AddPointToCurrentBranchAt(double x)
    {
        if(EvaluateWasCalled)
        {
            throw std::exception("programmer mistake: cannot call AddPointToCurrentBranchAt() after calling Evaluate()");
        }
        AddPointToCurrentBranchAtWasCalled = true;

        EnsureAtLeastOneBranch();

        auto evaluationResult = this->expression->Evaluate(x);
        if(evaluationResult.has_value())
        {
            this->AddCompletePointToCurrentBranch(x, evaluationResult.value());
            return true;
        }
        else
        {
            return false;
        }
    }

    void Evaluator::AddCompletePointToCurrentBranch(double x, double y)
    {
        auto & branch = graphData.back();

        // keep list sorted by x value
        if(branch.first.empty() || x > branch.first.back())
        {
            branch.first.emplace_back(x);
            branch.second.emplace_back(y);
            return;
        }

        if(x < branch.first.front())
        {
            branch.first.insert(branch.first.begin(), x);
            branch.second.insert(branch.second.begin(), y);
            return;
        }

        // we need to locate the correct position
        auto xBegin = branch.first.begin();
        auto xEnd = branch.first.end();
        auto xIt = std::lower_bound(xBegin, xEnd, x);

        auto yIt = branch.second.begin() + (xIt - xBegin);

        branch.first.insert(xIt, x);
        branch.second.insert(yIt, y);
    }

    std::vector<std::pair<std::vector<double>, std::vector<double>>> Evaluator::GetGraph()
    {
        return this->graphData;
    }

    /* static class member */ double Evaluator::GetRandom()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    void Evaluator::EnsureAtLeastOneBranch()
    {
        if(graphData.empty()){
            graphData.emplace_back(std::make_pair(std::vector<double>(),std::vector<double>()));
        }
    }

    void Evaluator::WorkAnInterval(double (*direction)(double), double & x, double xInCurrentInterval, double & xOld)
    {
        x = xInCurrentInterval + direction(Epsilon);
        auto yOptional = this->expression->Evaluate(x);
        xOld = xInCurrentInterval;

        double y = yOptional.value_or(0.0);
        double yOld = yOptional.value_or(this->expression->Evaluate(xInCurrentInterval).value());

        double incr = this->InitialIncrement;

        // scan the interval until it is interrupted
        bool interrupt = false;
        while (!interrupt)
        {
            interrupt = true;
            yOptional = this->expression->Evaluate(x);

            if (yOptional.has_value())
            {
                y = yOptional.value();
                interrupt = !(this->minX <= x && x <= this->maxX && -this->limit <= y && y <= this->limit);
            }

            if (!interrupt)
            {
                double squareDist = SquareDistance(x, y, xOld, yOld);
                if (squareDist < this->TargetDistance || incr < this->Epsilon)
                {
                    if (squareDist < this->Epsilon)
                    {
                        incr *= 2.0;
                    }

                    this->AddCompletePointToCurrentBranch(x, y);
                    xOld = x;
                    yOld = y;
                    x = x + direction(incr);
                }
                else
                {
                    incr *= 0.5;
                    x = x - direction(incr);
                }
            }
        }
    }

}
