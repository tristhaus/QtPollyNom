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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "expression.h"
#include "parser.h"
#include <memory>
#include <cfenv>
#include <cmath>

/*
 * Documentation for the CREATE_FUNCTION macro below:
 *
 * Create an Expression-inheriting function class from
 *   a class name,
 *   a human-readable function name,
 *   a C++ fragment that
 *       takes a x (of type double) and
 *       gives the correct evaluation (as double).
 *
 * The idea is to only have to modify this file (by adding a CREATE_FUNCTION call)
 * when adding a new function such as sin(x).
 *
 * The two branches of #ifdef ONE_TIME_EXECUTE_FUNCTIONS_H are identical except for
 * the static initializers executed in the first branch. Therefore, only one
 * translation unit may have ONE_TIME_EXECUTE_FUNCTIONS_H defined.
 * Within the first branch, the registration of the newly created function
 * in the parser is executed using static initialization.
 */

#ifdef ONE_TIME_EXECUTE_FUNCTIONS_H

#define CREATE_FUNCTION(classname, functionname, themath)\
namespace Backend\
{\
    class classname : public Expression\
    {\
    private:\
        std::shared_ptr<Expression> expression;\
        static bool IsRegistered;\
    public:\
        classname(std::shared_ptr<Expression> expression) : expression(expression) {}\
        virtual ~classname() { this->expression.reset(); }\
        classname(const classname&) = delete;\
        classname(classname&&) = delete;\
        classname& operator=(const classname&) = delete;\
        classname& operator=(classname&&) = delete;\
        virtual int GetLevel() const { return 3; }\
        virtual bool IsMonadic() const { return true; }\
        virtual std::optional<double> Evaluate(double input) const\
        {\
            auto expressionResult = expression->Evaluate(input);\
            if(!expressionResult.has_value()) { return {}; }\
            auto x = expressionResult.value();\
            std::feclearexcept(FE_ALL_EXCEPT);\
            auto retval = themath;\
            if(!std::isfinite(retval) || std::fetestexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_INVALID))\
            {\
                std::feclearexcept(FE_ALL_EXCEPT);\
                return {};\
            }\
            return retval;\
        }\
        virtual std::optional<std::string> Print() const\
        {\
            auto argumentOptional = expression->Print();\
            if(!argumentOptional.has_value()) { return {}; }\
            return std::string(functionname "(" + argumentOptional.value() + ")");\
        }\
        virtual bool operator==(const Expression &other) const\
        {\
            if (const classname * b = dynamic_cast<const classname*>(&other))\
            {\
                if(b == nullptr) { return false; }\
                return *(this->expression) == *(b->expression);\
            }\
            else { return false; }\
        }\
        virtual bool operator!=(const Expression &other) const { return !(*this == other); }\
        static std::shared_ptr<Expression> Create(std::shared_ptr<Expression> expression) { return std::make_shared<classname>(expression); }\
        static bool SelfRegister()\
        {\
            static bool isRegistered(false);\
            if(!isRegistered)\
            {\
                 Parser::Register(functionname, &classname::Create);\
            }\
            return isRegistered;\
        }\
    };\
    bool classname::IsRegistered = classname::SelfRegister();\
}\

#else // ONE_TIME_EXECUTE_FUNCTIONS_H

#define CREATE_FUNCTION(classname, functionname, themath)\
namespace Backend\
{\
    class classname : public Expression\
    {\
    private:\
        std::shared_ptr<Expression> expression;\
        static bool IsRegistered;\
    public:\
        classname(std::shared_ptr<Expression> expression) : expression(expression) {}\
        virtual ~classname() { this->expression.reset(); }\
        classname(const classname&) = delete;\
        classname(classname&&) = delete;\
        classname& operator=(const classname&) = delete;\
        classname& operator=(classname&&) = delete;\
        virtual int GetLevel() const { return 3; }\
        virtual bool IsMonadic() const { return true; }\
        virtual std::optional<double> Evaluate(double input) const\
        {\
            auto expressionResult = expression->Evaluate(input);\
            if(!expressionResult.has_value()) { return {}; }\
            auto x = expressionResult.value();\
            std::feclearexcept(FE_ALL_EXCEPT);\
            auto retval = themath;\
            if(!std::isfinite(retval) || std::fetestexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_INVALID))\
            {\
                std::feclearexcept(FE_ALL_EXCEPT);\
                return {};\
            }\
            return retval;\
        }\
        virtual std::optional<std::string> Print() const\
        {\
            auto argumentOptional = expression->Print();\
            if(!argumentOptional.has_value()) { return {}; }\
            return std::string(functionname "(" + argumentOptional.value() + ")");\
        }\
        virtual bool operator==(const Expression &other) const\
        {\
            if (const classname * b = dynamic_cast<const classname*>(&other))\
            {\
                if(b == nullptr) { return false; }\
                return *(this->expression) == *(b->expression);\
            }\
            else { return false; }\
        }\
        virtual bool operator!=(const Expression &other) const { return !(*this == other); }\
        static std::shared_ptr<Expression> Create(std::shared_ptr<Expression> expression) { return std::make_shared<classname>(expression); }\
        static bool SelfRegister()\
        {\
            static bool isRegistered(false);\
            if(!isRegistered)\
            {\
                 Parser::Register(functionname, &classname::Create);\
            }\
            return isRegistered;\
        }\
    };\
}\

#endif // ONE_TIME_EXECUTE_FUNCTIONS_H

// the actual function creation

CREATE_FUNCTION(AbsoluteValue, "abs", std::fabs(x));

CREATE_FUNCTION(Sine, "sin", std::sin(x));

CREATE_FUNCTION(Cosine, "cos", std::cos(x));

CREATE_FUNCTION(Tangent, "tan", std::tan(x));

CREATE_FUNCTION(NaturalExponential, "exp", std::exp(x));

CREATE_FUNCTION(NaturalLogarithm, "ln", std::log(x));

#endif // FUNCTIONS_H
