#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QObject>
#include <optional>

namespace Backend
{

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

    virtual int GetLevel() const = 0;
    virtual bool IsMonadic() const = 0;
    virtual std::optional<double> Evaluate(double input) const = 0;
    virtual std::optional<std::string> Print() const = 0;
    virtual bool operator==(const Expression &other) const = 0;
    virtual bool operator!=(const Expression &other) const = 0;
};

}

#endif // EXPRESSION_H
