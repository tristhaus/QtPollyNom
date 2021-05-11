#ifndef BASEX_H
#define BASEX_H

#include "expression.h"

namespace Backend {

    /*!
     * \class BaseX
     * \brief The BaseX class represents the independent (x) variable.
     */
    class BaseX final : public Expression
    {
        Q_OBJECT

    public:
        /*!
         * \brief Initializes a new instance.
         */
        BaseX();
        virtual ~BaseX();
        BaseX(const BaseX&) = delete;
        BaseX(BaseX&&) = delete;
        BaseX& operator=(const BaseX&) = delete;
        BaseX& operator=(BaseX&&) = delete;

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
        virtual std::optional<double> Evaluate(double input) const;

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
         */virtual bool operator!=(const Expression &other) const;
    };

}

#endif // BASEX_H
