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
#include "mathhelper.h"

namespace Backend {

    Evaluator::Evaluator(std::shared_ptr<Expression> expression, double minX, double maxX, double limit)
        : expression(expression),
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

    std::vector<std::pair<std::vector<double>, std::vector<double>>> Evaluator::Evaluate()
    {
        if(AddPointToCurrentBranchAtWasCalled)
        {
            throw std::exception("programmer mistake: cannot call Evaluate() after calling AddPointToCurrentBranchAt()");
        }
        EvaluateWasCalled = true;

        this->CreateGraph();

        return this->GetGraph();
    }

    void Evaluator::CreateGraph()
    {
        double lastXinPreviousInterval = this->minX;
        double x = lastXinPreviousInterval;

        while (x < this->maxX)
        {
            if(graphData.size() == 0 || !graphData.back().first.empty())
            {
                graphData.emplace_back(std::make_pair(std::vector<double>(),std::vector<double>()));
            }

            // look for interval
            double xInCurrentInterval = lastXinPreviousInterval;
            bool foundInterval = false;
            while (!foundInterval && xInCurrentInterval < this->maxX)
            {
                xInCurrentInterval += this->LargeIncrement;
                auto result = this->expression->Evaluate(xInCurrentInterval);
                foundInterval = result.has_value();
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
            lastXinPreviousInterval = lastXinCurrentInterval;
        }

        while(graphData.size() > 1)
        {
            auto is_empty = [](std::pair<std::vector<double>, std::vector<double>> branch){ return branch.first.empty(); };
            auto begin = graphData.begin();
            auto end = graphData.end();
            auto findIt = std::find_if(begin, end, is_empty);

            if(findIt == end)
            {
                break;
            }

            graphData.erase(findIt);
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

    void Evaluator::EnsureAtLeastOneBranch()
    {
        if(graphData.empty()){
            graphData.emplace_back(std::make_pair(std::vector<double>(), std::vector<double>()));
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
