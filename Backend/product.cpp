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

#include "product.h"
#include <cfenv>
#include <cmath>

namespace Backend
{
    Product::Product(std::vector<Factor> factors)
        : factors(factors)
    {
    }

    Product::~Product()
    {
        // paranoid: remove possible source for circular references
        factors.clear();
    }

    int Product::GetLevel() const
    {
        return 2;
    }

    bool Product::IsMonadic() const
    {
        return false;
    }

    std::optional<double> Product::Evaluate(double input) const
    {
        double retval(1.0);

        auto expressionIterator = factors.begin();
        auto expressionEnd = factors.end();

        for(;expressionIterator != expressionEnd; ++expressionIterator)
        {
            auto subResult = (*expressionIterator).expression->Evaluate(input);

            if(!subResult.has_value())
            {
                return {};
            }

            auto value = subResult.value();

            switch ((*expressionIterator).exponent)
            {
            case Product::Exponent::Positive:
                retval *= value;
                break;
            case Product::Exponent::Negative:

                if(std::fabs(value) < 1e-9)
                {
                    return {};
                }

                std::feclearexcept(FE_ALL_EXCEPT);
                retval /= value;

                if(std::fetestexcept(FE_DIVBYZERO | FE_OVERFLOW))
                {
                    return {};
                    std::feclearexcept(FE_ALL_EXCEPT);
                }

                break;
            default:
                throw std::exception("programming mistake in Product switch");
            }
        }

        return retval;
    }

    std::optional<std::wstring> Product::Print() const
    {
        std::wstring retval(L"");

        auto expressionIterator = factors.begin();
        auto expressionEnd = factors.end();

        for(;expressionIterator != expressionEnd; ++expressionIterator)
        {
            auto expression = (*expressionIterator).expression;
            auto subResult = expression->Print();

            if(!subResult.has_value())
            {
                return {};
            }

            auto value = subResult.value();
            if(expression->GetLevel() == this->GetLevel() - 1)
            {
                value.insert(0, 1, '(');
                value.append(1, ')');
            }

            switch ((*expressionIterator).exponent)
            {
            case Product::Exponent::Positive:
                if(retval.length() > 0)
                {
                    retval += L"*";
                }
                break;
            case Product::Exponent::Negative:
                if(retval.length() == 0)
                {
                    retval += L"1.0";
                }
                retval += L"/";
                break;
            default:
                throw std::exception("programming mistake in Product switch");
            }

            retval += value;
        }

        return retval;
    }

    bool Product::operator==(const Expression &other) const
    {
        if (const Product * b = dynamic_cast<const Product*>(&other))
        {
            if(b == nullptr)
            {
                return false;
            }

            auto thisTemp = std::vector<Factor>(this->factors);
            auto otherTemp = std::vector<Factor>(b->factors);

            while(!thisTemp.empty()) {

                bool matched = false;

                auto thisFactor = thisTemp.back();

                auto otherIterator = otherTemp.begin();
                auto otherEnd = otherTemp.end();

                while(otherIterator != otherEnd)
                {
                    if(*otherIterator == thisFactor)
                    {
                        matched = true;
                        break;
                    }

                    ++otherIterator;
                }

                if(matched)
                {
                    thisTemp.pop_back();
                    otherTemp.erase(otherIterator);
                }
                else
                {
                    return false;
                }
            }

            return otherTemp.empty();
        }
        else
        {
            return false;
        }
    }

    bool Product::operator!=(const Expression &other) const
    {
        return !(*this == other);
    }

    Product::Factor::Factor(Product::Exponent exponent, std::shared_ptr<Expression> expression)
        : exponent(exponent), expression(expression)
    {
    }

    bool Product::Factor::operator==(const Product::Factor& other) const
    {
        return this->exponent == other.exponent && *(this->expression) == *(other.expression);
    }

    bool Product::Factor::operator!=(const Product::Factor& other) const
    {
        return !(*this == other);
    }
}
