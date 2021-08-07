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

#include <regex>
#include <algorithm>
#include <string>
#include <functional>
#include <set>

#include "parser.h"
#include "basex.h"
#include "constant.h"
#include "sum.h"
#include "product.h"
#include "power.h"
#include "functions.h"

namespace Backend {

    const std::wstring Parser::PlusString = L"+";
    const std::wstring Parser::MinusString = L"-";
    const std::wstring Parser::TimesString = L"*";
    const std::wstring Parser::DivideString = L"/";
    const std::wstring Parser::PowerString = L"^";

    Parser::Parser()
    {
    }

    bool Parser::Register(std::wstring name, CreateFunction createFunction)
    {
        auto & functions = Parser::GetRegisteredFunctions();

        auto it = functions.find(name);
        if (it == functions.end())
        {
            functions.emplace(name, createFunction);
            return true;
        }

        return false;
    }

    std::map<std::wstring, CreateFunction> & Parser::GetRegisteredFunctions()
    {
        static std::map<std::wstring, CreateFunction> theFunctions;
        return theFunctions;
    }

    std::wregex Parser::GetValidationRegex() const
    {
        // This must be adjusted if another math operator is added to the game.
        std::wstring regexString(L"^[-+/*^()0-9.,");

        // allow independent variable
        std::set<wchar_t> allowedLetters {L'x', L'X'};

        // grab from all function names all letters
        auto & registrationMap = Parser::GetRegisteredFunctions();
        auto registrationMapIt = registrationMap.begin();
        auto registrationMapEnd = registrationMap.end();

        for(; registrationMapIt != registrationMapEnd; ++registrationMapIt)
        {
            auto functionName = registrationMapIt->first;
            auto functionNameIt = functionName.begin();
            auto functionNameEnd = functionName.end();

            for(; functionNameIt != functionNameEnd; ++functionNameIt)
            {
                allowedLetters.insert(*functionNameIt);
            }
        }

        // extend regex by the collection of letters
        auto allowedLettersIt = allowedLetters.begin();
        auto allowedLettersEnd = allowedLetters.end();

        for(; allowedLettersIt != allowedLettersEnd; ++allowedLettersIt)
        {
            regexString += *allowedLettersIt;
        }

        regexString.append(L"]+$");

        return std::wregex(regexString.c_str(), std::regex_constants::ECMAScript);
    }

    std::shared_ptr<Expression> Parser::Parse(const std::wstring & input) const
    {
        std::string locale(std::setlocale(LC_ALL, nullptr));

        try
        {
            auto prepared = this->PrepareInput(input);
            if (!this->ValidateInput(prepared))
            {
                return nullptr;
            }

            std::setlocale(LC_ALL, "en_US.UTF-8");
            auto result = this->InternalParse(prepared);
            std::setlocale(LC_ALL, locale.c_str());

            return result;
        }
        catch(std::exception)
        {
            std::setlocale(LC_ALL, locale.c_str());
            return nullptr;
        }
    }

    bool Parser::IsParseable(const std::wstring& input) const
    {
        return this->Parse(input) != nullptr;
    }

    std::wstring Parser::PrepareInput(const std::wstring & input) const
    {
        static std::wregex re(L"[ \t]", std::regex_constants::ECMAScript);

        return std::regex_replace(input, re, L"");
    }

    bool Parser::ValidateInput(const std::wstring & input) const
    {
        // check unsupported characters. The regex should not change after startup
        static auto inputValidationRegex = this->GetValidationRegex();
        if(!std::regex_match(input, inputValidationRegex))
        {
            return false;
        }

        // check for "^-", "^+", which is hard to parse
        if(input.find(L"^-") != std::wstring::npos || input.find(L"^+") != std::wstring::npos)
        {
            return false;
        }

        // check for dangling operators at the end of string
        auto lastChar = input.back();
        if (lastChar == L'+' || lastChar == L'-' || lastChar == L'*' || lastChar == L'\\' || lastChar == L'^' || lastChar == L'(')
        {
            return false;
        }

        // check balanced parentheses

        int count = 0;
        auto inputIt = input.begin();
        auto inputEnd = input.end();

        for (; inputIt != inputEnd; ++inputIt)
        {
            if (*inputIt == L'(')
            {
                count++;
            }
            else if (*inputIt == L')')
            {
                count--;
            }

            if (count < 0)
            {
                return false;
            }
        }

        if (count != 0)
        {
            return false;
        }

        return true;
    }

    unsigned long long Parser::FindMatchingBrace(const std::wstring & input, unsigned long long pos) const
    {
        if (pos > input.length() - 1)
        {
            return -1;
        }

        bool lookForClosing;
        if (input[pos] == L'(')
        {
            lookForClosing = true;
        }
        else if (input[pos] == L')')
        {
            lookForClosing = false;
        }
        else
        {
            return -1;
        }

        int count = 0;
        if (lookForClosing)
        {
            for (unsigned long long index = pos; index < input.length(); index++)
            {
                wchar_t c = input[index];
                if (c == L'(')
                {
                    count++;
                }
                else if (c == L')')
                {
                    count--;
                }

                if (count == 0)
                {
                    return index;
                }
            }
        }
        else
        {
            for (int index = static_cast<int>(pos); index >= 0; index--)
            {
                wchar_t c = input[index];
                if (c == L')')
                {
                    count++;
                }
                else if (c == L'(')
                {
                    count--;
                }

                if (count == 0)
                {
                    return index;
                }
            }
        }

        return -1;
    }

    std::shared_ptr<Expression> Parser::InternalParse(std::wstring input) const
    {
        if (!ValidateInput(input))
        {
            return nullptr;
        }

        // if fully enclosed in braces, we remove them
        if (input[0] == '(' && input.length() - 1 == this->FindMatchingBrace(input, 0))
        {
            return this->InternalParse(input.substr(1, input.length() - 2));
        }

        // deal with a simple case: plain x
        if (input == L"X" || input == L"x")
        {
            return std::make_shared<BaseX>();
        }

        static std::wregex constantRegex(L"^[-+]?[0-9]+[.,]?[0-9]*$", std::regex_constants::ECMAScript);

        // deal with a simple case: a numerical constant
        if (std::regex_search(input, constantRegex))
        {
            return this->ParseToConstant(input);
        }

        // now, tokenize
        std::vector<std::wstring> tokens;
        std::vector<std::wstring> ops;

        this->Tokenize(input, tokens, ops);

        // deal with a signed single token
        static std::wregex singleSignedTokenRegex(L"^[-+]", std::regex_constants::ECMAScript);

        if (tokens.size() == 1 && std::regex_search(input, singleSignedTokenRegex))
        {
            std::wstring subToken = input.substr(1);
            std::shared_ptr<Expression> bracketedExpression = this->InternalParse(subToken);

            if (bracketedExpression == nullptr)
            {
                return nullptr;
            }

            if(input[0] == '-' )
            {
                return std::make_shared<Sum>(std::vector<Sum::Summand>({Sum::Summand(Sum::Sign::Minus, bracketedExpression)}));
            }
            else
            {
                return bracketedExpression;
            }
        }

        // reassemble for recursive parsing
        auto opsBegin = ops.begin();
        auto opsEnd = ops.end();

        // First case: plus and minus
        auto findPlus = std::find(opsBegin, opsEnd, PlusString);
        auto findMinus = std::find(opsBegin, opsEnd, MinusString);

        if (findPlus != opsEnd || findMinus != opsEnd)
        {
            return this->ParseToSum(tokens, ops);
        }

        // Second case: multiply and divide
        auto findTimes = std::find(opsBegin, opsEnd, TimesString);
        auto findDivide = std::find(opsBegin, opsEnd, DivideString);

        if (findTimes != opsEnd || findDivide != opsEnd)
        {
            return this->ParseToProduct(tokens, ops);
        }

        // Third case: power expressions
        auto findPower = std::find(opsBegin, opsEnd, PowerString);
        if (findPower != opsEnd)
        {
            return this->ParseToPower(tokens, ops);
        }

        // Fourth case: functions
        if(tokens.size() == 1)
        {
            return this->ParseToFunction(tokens);
        }

        return nullptr;
    }

    void Parser::Tokenize(const std::wstring & input, std::vector<std::wstring> & tokens, std::vector<std::wstring> & ops) const
    {
        static std::function<bool(wchar_t)> isOperatorChar = [](wchar_t c)
        {
            return c == L'-' || c == L'+' || c == L'*' || c == L'/' || c == L'^';
        };

        std::wstring token;
        for (unsigned long long index = 0; index < input.length(); ++index)
        {
            wchar_t c = input[index];

            if (c == L'(')
            {
                auto endIndex = FindMatchingBrace(input, index);
                token += input.substr(index, endIndex - index + 1);
                index = endIndex;
                continue;
            }

            if (isOperatorChar(c) && token.length() > 0)
            {
                tokens.push_back(token);
                token = L"";
                ops.emplace_back(1, c);
                continue;
            }

            token += c;
        }

        if (token.length() > 0)
        {
            tokens.push_back(token);
        }
    }

    std::shared_ptr<Expression> Parser::ParseToConstant(const std::wstring & input) const
    {
        try
        {
            static std::wregex re(L",", std::regex_constants::ECMAScript);

            auto anglified = std::regex_replace(input, re, L".");

            double parsed = std::stod(anglified);
            return std::make_shared<Constant>(parsed);
        }
        catch (std::exception)
        {
            return nullptr;
        }
    }

    std::shared_ptr<Expression> Parser::ParseToSum(std::vector<std::wstring> & tokens, std::vector<std::wstring> & ops) const
    {
        if(tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        std::vector<Sum::Summand> targetList;

        Sum::Sign sign = Sum::Sign::Plus;

        std::wstring token = tokens[0];
        tokens.erase(tokens.begin());

        auto opsIt = ops.begin();
        auto opsEnd = ops.end();

        for(;opsIt != opsEnd; ++opsIt)
        {
            if (*opsIt == L"+" || *opsIt == L"-")
            {
                auto expression = this->InternalParse(token);
                if (expression == nullptr)
                {
                    return nullptr;
                }

                targetList.push_back(Sum::Summand(sign, expression));
                token = tokens[0];
                tokens.erase(tokens.begin());
                sign = *opsIt == L"+" ? Sum::Sign::Plus : Sum::Sign::Minus;
            }
            else
            {
                token += (*opsIt) + tokens[0];
                tokens.erase(tokens.begin());
            }
        }

        // one last token to take care of
        if (tokens.size() == 1)
        {
            token = tokens[0];
        }

        if (token != L"")
        {
            auto expression = this->InternalParse(token);
            if (expression == nullptr)
            {
                return nullptr;
            }

            targetList.push_back(Sum::Summand(sign, expression));
        }

        return std::make_shared<Sum>(targetList);
    }

    std::shared_ptr<Expression> Parser::ParseToProduct(std::vector<std::wstring> & tokens, std::vector<std::wstring> & ops) const
    {
        if(tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        std::vector<Product::Factor> targetList;

        Product::Exponent sign = Product::Exponent::Positive;

        std::wstring token = tokens[0];
        tokens.erase(tokens.begin());

        auto opsIt = ops.begin();
        auto opsEnd = ops.end();

        for(;opsIt != opsEnd; ++opsIt)
        {
            if (*opsIt == L"*" || *opsIt == L"/")
            {
                auto expression = this->InternalParse(token);
                if (expression == nullptr)
                {
                    return nullptr;
                }

                targetList.push_back(Product::Factor(sign, expression));
                token = tokens[0];
                tokens.erase(tokens.begin());
                sign = *opsIt == L"*" ? Product::Exponent::Positive : Product::Exponent::Negative;
            }
            else
            {
                token += (*opsIt) + tokens[0];
                tokens.erase(tokens.begin());
            }
        }

        // one last token to take care of
        if (tokens.size() == 1)
        {
            token = tokens[0];
        }

        if (token != L"")
        {
            auto expression = this->InternalParse(token);
            if (expression == nullptr)
            {
                return nullptr;
            }

            targetList.push_back(Product::Factor(sign, expression));
        }

        return std::make_shared<Product>(targetList);
    }

    std::shared_ptr<Expression> Parser::ParseToPower(std::vector<std::wstring>& tokens, std::vector<std::wstring>& ops) const
    {
        if(tokens.size() != ops.size() + 1)
        {
            return nullptr;
        }

        auto opsBegin = ops.begin();
        auto opsEnd = ops.end();
        auto findPlus = std::find(opsBegin, opsEnd, PlusString);
        auto findMinus = std::find(opsBegin, opsEnd, MinusString);
        auto findTimes = std::find(opsBegin, opsEnd, TimesString);
        auto findDivide = std::find(opsBegin, opsEnd, DivideString);

        if (findPlus != opsEnd || findMinus != opsEnd || findTimes != opsEnd || findDivide != opsEnd)
        {
            return nullptr;
        }

        auto baseToken = tokens[0];
        tokens.erase(tokens.begin());
        auto baseExpression = this->InternalParse(baseToken);
        if (!baseExpression)
        {
            return nullptr;
        }

        auto exponentToken = tokens[0];
        tokens.erase(tokens.begin());
        for (unsigned long int index = 1; index < ops.size(); index++)
        {
            exponentToken += ops[index] + tokens[0];
            tokens.erase(tokens.begin());
        }

        auto exponentExpression = this->InternalParse(exponentToken);
        if (!exponentExpression)
        {
            return nullptr;
        }

        return std::make_shared<Power>(baseExpression, exponentExpression);
    }

    std::shared_ptr<Expression> Parser::ParseToFunction(std::vector<std::wstring>& tokens) const
    {
        auto functionToken = tokens[0];
        tokens.erase(tokens.begin());
        auto index = functionToken.find('(');
        auto name = functionToken.substr(0, index);

        auto & functions = Parser::GetRegisteredFunctions();

        auto createFunction = functions.find(name);

        if(createFunction == functions.end())
        {
            return nullptr;
        }

        auto argumentToken = functionToken.substr(index);
        auto argument = this->InternalParse(argumentToken);

        if(!argument)
        {
            return nullptr;
        }

        return (*createFunction).second(argument);
    }

}
