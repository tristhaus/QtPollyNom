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

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <regex>
#include <map>
#include "expression.h"

namespace Backend {

    typedef std::shared_ptr<Expression> (*CreateFunction)(std::shared_ptr<Expression>) ;

    /*!
     * \class Parser
     * \brief The Parser class provides functionality to obtain a \ref Expression from a string.
     */
    class Parser final
    {
    private:
        /*!
         * \brief InputValidationRegex provides cheap validation for the input string.
         * It must be adjusted any time another math function is added to the game.
         */
        static const std::regex InputValidationRegex;

        static const std::string PlusString;
        static const std::string MinusString;
        static const std::string TimesString;
        static const std::string DivideString;
        static const std::string PowerString;

    public:
        /*!
         * \brief Initializes a new instance.
         */
        Parser();

        /*!
         * \brief Parse creates an \ref expression from a string, if possible.
         * \param input The string to parse.
         * \return A pointer to the expression or a nullptr.
         */
        std::shared_ptr<Expression> Parse(const std::string & input) const;

        /*!
         * \brief IsParseable indicates whether the supplied string is parseable.
         * \param input The string to check for parseability.
         * \return true if the argument is parseable, false otherwise.
         */
        bool IsParseable(const std::string & input) const;

        /*!
         * \brief Register registers a function to create an expression representing
         *        a mathematical function under the given human-readable name.
         * \param name The human-readable name of the function, e.g. "sin".
         * \param createFunction Pointer to a function to create an expression
         *        representing the mathematical function.
         * \return A dummy bool such that the function can be used in static initialization.
         */
        static bool Register(std::string name, CreateFunction createFunction);

    private:
        /*!
         * \brief GetRegisteredFunctions Gets the class-static map of registered functions for the parser.
         *
         * By doing it this way, we avoid the static initialization fiasco,
         * because the map returned is actually method-static, initialized on first access.
         *
         * \return The registration map.
         */
        static std::map<std::string, CreateFunction> & GetRegisteredFunctions();
        std::string PrepareInput(const std::string & input) const;
        bool ValidateInput(const std::string & input) const;
        unsigned long long FindMatchingBrace(const std::string & input, unsigned long long pos) const;

        std::shared_ptr<Expression> InternalParse(std::string input) const;
        void Tokenize(const std::string & input, std::vector<std::string> & tokens, std::vector<std::string> & ops) const;
        std::shared_ptr<Expression> ParseToConstant(const std::string & input) const;
        std::shared_ptr<Expression> ParseToSum(std::vector<std::string> & tokens, std::vector<std::string> & ops) const;
        std::shared_ptr<Expression> ParseToProduct(std::vector<std::string> & tokens, std::vector<std::string> & ops) const;
        std::shared_ptr<Expression> ParseToPower(std::vector<std::string> & tokens, std::vector<std::string> & ops) const;
        std::shared_ptr<Expression> ParseToFunction(std::vector<std::string>& tokens) const;
    };

}

#endif // PARSER_H
