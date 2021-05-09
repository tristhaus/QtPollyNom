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
    explicit Expression(QObject *parent = nullptr);

    //virtual int GetLevel() = 0;
    //virtual bool IsMonadic() = 0;
    virtual std::optional<double> Evaluate(double input) = 0;
    //virtual std::optional<std::string> Print() = 0;
};

}

#endif // EXPRESSION_H
