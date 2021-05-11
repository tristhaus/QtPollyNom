#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"

namespace Backend {

    /*!
     * \class Constant
     * \brief The Constant class represents a constant real number.
     */
    class Constant final : public Expression
    {
        Q_OBJECT

    private:
        double value;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied value.
         * \param input The value to hold as a constant.
         */
        Constant(double input);
        virtual ~Constant();
        Constant(const Constant&) = delete;
        Constant(Constant&&) = delete;
        Constant& operator=(const Constant&) = delete;
        Constant& operator=(Constant&&) = delete;

        /*!
         * \reimp
         */
        virtual int GetLevel() const;

        /*!
         * \reimp
         */
        virtual bool IsMonadic() const;

        /*!
         * \reimp
         */
        virtual std::optional<double> Evaluate(double) const;

        /*!
         * \reimp
         */
        virtual std::optional<std::string> Print() const;

        /*!
         * \reimp
         */
        virtual bool operator==(const Expression &other) const;

        /*!
         * \reimp
         */
        virtual bool operator!=(const Expression &other) const;
    };

}

#endif // CONSTANT_H
