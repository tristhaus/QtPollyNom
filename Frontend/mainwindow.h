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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <vector>
#include "qcustomplot.h"
#include "../Backend/gamepoc.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow
    {
    public:
        QMenuBar *menubar;
        QStatusBar *statusbar;
        QWidget *centralwidget;
        QVBoxLayout *verticalLayout_2;
        QCustomPlot *plot;
        QVBoxLayout *verticalLayout_3;
        QFrame *inputFrame0;
        QHBoxLayout *horizontalLayout;
        QFrame *inputFrame1;
        QVBoxLayout *verticalLayout;

        std::vector<QHBoxLayout*> funcLayout;
        std::vector<QLabel*> funcLabel;
        std::vector<QLineEdit*> funcLineEdit;

        QPushButton *calcButton;

        void setupUi(QMainWindow *MainWindow)
        {
            if (MainWindow->objectName().isEmpty())
                MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
            MainWindow->resize(600, 821);
            centralwidget = new QWidget(MainWindow);
            centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
            verticalLayout_2 = new QVBoxLayout(centralwidget);
            verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
            plot = new QCustomPlot(centralwidget);
            plot->setObjectName(QString::fromUtf8("plot"));
            verticalLayout_3 = new QVBoxLayout(plot);
            verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));

            verticalLayout_2->addWidget(plot);

            inputFrame0 = new QFrame(centralwidget);
            inputFrame0->setObjectName(QString::fromUtf8("inputFrame0"));
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(inputFrame0->sizePolicy().hasHeightForWidth());
            inputFrame0->setSizePolicy(sizePolicy);
            inputFrame0->setFrameShape(QFrame::StyledPanel);
            inputFrame0->setFrameShadow(QFrame::Raised);
            horizontalLayout = new QHBoxLayout(inputFrame0);
            horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
            inputFrame1 = new QFrame(inputFrame0);
            inputFrame1->setObjectName(QString::fromUtf8("inputFrame1"));
            inputFrame1->setFrameShape(QFrame::StyledPanel);
            inputFrame1->setFrameShadow(QFrame::Raised);
            verticalLayout = new QVBoxLayout(inputFrame1);
            verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout0"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel0"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit0"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout1"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel1"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit1"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout2"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel2"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit2"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout3"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel3"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit3"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            funcLayout.emplace_back(new QHBoxLayout());
            funcLayout.back()->setObjectName(QString::fromUtf8("funcLayout4"));

            funcLabel.emplace_back(new QLabel(inputFrame1));
            funcLabel.back()->setObjectName(QString::fromUtf8("funcLabel4"));
            funcLayout.back()->addWidget(funcLabel.back());

            funcLineEdit.emplace_back(new QLineEdit(inputFrame1));
            funcLineEdit.back()->setObjectName(QString::fromUtf8("funcLineEdit4"));
            funcLayout.back()->addWidget(funcLineEdit.back());

            verticalLayout->addLayout(funcLayout.back());


            horizontalLayout->addWidget(inputFrame1);

            calcButton = new QPushButton(inputFrame0);
            calcButton->setObjectName(QString::fromUtf8("calcButton"));

            horizontalLayout->addWidget(calcButton);


            verticalLayout_2->addWidget(inputFrame0);

            MainWindow->setCentralWidget(centralwidget);
            menubar = new QMenuBar(MainWindow);
            menubar->setObjectName(QString::fromUtf8("menubar"));
            menubar->setGeometry(QRect(0, 0, 600, 21));
            MainWindow->setMenuBar(menubar);
            statusbar = new QStatusBar(MainWindow);
            statusbar->setObjectName(QString::fromUtf8("statusbar"));
            MainWindow->setStatusBar(statusbar);

            retranslateUi(MainWindow);

            QMetaObject::connectSlotsByName(MainWindow);
        }

        void retranslateUi(QMainWindow *MainWindow)
        {
            MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QtPollyNom", nullptr));
            funcLabel[0]->setText(QCoreApplication::translate("MainWindow", "Function 1:", nullptr));
            funcLineEdit[0]->setText(QCoreApplication::translate("MainWindow", "1/x", nullptr));
            funcLabel[1]->setText(QCoreApplication::translate("MainWindow", "Function 2:", nullptr));
            funcLineEdit[1]->setText(QCoreApplication::translate("MainWindow", "(x-3.0)*(x+4.0)", nullptr));
            funcLabel[2]->setText(QCoreApplication::translate("MainWindow", "Function 3:", nullptr));
            funcLabel[3]->setText(QCoreApplication::translate("MainWindow", "Function 4:", nullptr));
            funcLabel[4]->setText(QCoreApplication::translate("MainWindow", "Function 5:", nullptr));
            calcButton->setText(QCoreApplication::translate("MainWindow", "Calc!", nullptr));
        }

    };
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    size_t numberOfFunctionInputs;

    Backend::GamePoc gamePoc;
    QFutureWatcher<void> gameUpdateFutureWatcher;
    QTimer waitTimer;

    std::unique_ptr<QMessageBox> waitingMessageBox;

    std::vector<QCPAbstractPlottable*> dotCurves;

    std::vector<QColor> graphColors;
    QColor activeGoodDotColor;
    QColor inactiveGoodDotColor;
    QColor activeBadDotColor;
    QColor inactiveBadDotColor;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OnCalcButtonClicked();
    void OnReturnKeyPressed();
    void OnGameUpdateFinished();
    void OnWaitTimerFinished();
    void OnWaitingMessageBoxButtonClicked();

private:
    void InitializePlot();
    void SetupColors();
    void DrawDots();
    void DrawGraphs();
    void SetGameIsBusy(bool isBusy);
    void UpdateGui();
    void StartCalculation();
};
#endif // MAINWINDOW_H
