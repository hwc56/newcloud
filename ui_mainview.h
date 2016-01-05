/********************************************************************************
** Form generated from reading UI file 'mainview.ui'
**
** Created: Wed Dec 16 18:22:15 2015
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEW_H
#define UI_MAINVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainview
{
public:
    QListWidget *widgetlist;
    QPushButton *pushButton;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QStackedWidget *widgetstack;

    void setupUi(QWidget *mainview)
    {
        if (mainview->objectName().isEmpty())
            mainview->setObjectName(QString::fromUtf8("mainview"));
        mainview->resize(661, 609);
        widgetlist = new QListWidget(mainview);
        widgetlist->setObjectName(QString::fromUtf8("widgetlist"));
        widgetlist->setGeometry(QRect(300, 40, 0, 0));
        pushButton = new QPushButton(mainview);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(400, 10, 101, 35));
        pushButton_1 = new QPushButton(mainview);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(300, 10, 101, 35));
        pushButton_2 = new QPushButton(mainview);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(510, 10, 41, 35));
        pushButton_3 = new QPushButton(mainview);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(560, 10, 61, 35));
        pushButton_4 = new QPushButton(mainview);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(470, 560, 80, 35));
        pushButton_5 = new QPushButton(mainview);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(560, 560, 80, 35));
        widgetstack = new QStackedWidget(mainview);
        widgetstack->setObjectName(QString::fromUtf8("widgetstack"));
        widgetstack->setGeometry(QRect(360, 360, 0, 0));

        retranslateUi(mainview);

        QMetaObject::connectSlotsByName(mainview);
    } // setupUi

    void retranslateUi(QWidget *mainview)
    {
        mainview->setWindowTitle(QApplication::translate("mainview", "cloudtest", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainview: public Ui_mainview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEW_H
