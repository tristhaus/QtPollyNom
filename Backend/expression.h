#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QObject>
#include <optional>

namespace Backend
{
    /*!
     * \class Expression
     * \brief The Expression class forms the base for all mathematical expressions.
     *
     * It is abstract and disallows most operations. Its inheritors should basically be immutable.
     */
    class Expression : public QObject
    {
        Q_OBJECT

    public:
        explicit Expression(QObject *parent = nullptr) : QObject(parent)
        {
        }
        virtual ~Expression()
        {
        }
        Expression(const Expression&) = delete;
        Expression(Expression&&) = delete;
        Expression& operator=(const Expression&) = delete;
        Expression& operator=(Expression&&) = delete;

        /*!
         * \brief Gets the precedence level of the underlying operation.
         * \return The precedence level of the underlying operation.
         */
        virtual int GetLevel() const = 0;

        /*!
         * \brief Gets a value indicating whether the expression is monadic.
         * \return A value indicating whether the expression is monadic.
         */
        virtual bool IsMonadic() const = 0;

        /*!
         * \brief Evaluates the expression using the \a input value as x-coordinate.
         * \param input The value to plug in to the expression.
         * \return The evaluated value or nothing if undefined.
         */
        virtual std::optional<double> Evaluate(double input) const = 0;

        /*!
         * \brief Prints the expression as a human-readable and machine-parseable string.
         * \return The string or nothing.
         */
        virtual std::optional<std::string> Print() const = 0;

        /*!
         * \brief Equality operator for the expression, checking type and content.
         * \param other The instance to compare to.
         * \return A value indicating equality.
         */
        virtual bool operator==(const Expression &other) const = 0;

        /*!
         * \brief Inequality operator for the expression, checking type and content.
         * \param other The instance to compare to.
         * \return A value indicating inequality.
         */
        virtual bool operator!=(const Expression &other) const = 0;
    };

}

#endif // EXPRESSION_H
