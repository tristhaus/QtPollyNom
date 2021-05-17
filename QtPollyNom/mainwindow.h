#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <vector>
#include "qcustomplot.h"
#include "../Backend/gamepoc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calcButton_clicked();
    void on_update_finished();

private:
    void InitializePlot();
    void SetupColors();
    void DrawDots();
    void DrawGraphs();
    void SetGameIsBusy(bool isBusy);

private:
    Ui::MainWindow *ui;

    GamePoc gamePoc;
    QFutureWatcher<void> gameUpdateFutureWatcher;

    std::vector<QCPAbstractPlottable*> dotCurves;

    std::vector<QColor> graphColors;
    QColor activeGoodDotColor;
    QColor inactiveGoodDotColor;
    QColor activeBadDotColor;
    QColor inactiveBadDotColor;
};
#endif // MAINWINDOW_H
