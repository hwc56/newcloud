/********************************************************************************
** Form generated from reading UI file 'wifi.ui'
**
** Created: Wed Dec 16 18:22:15 2015
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIFI_H
#define UI_WIFI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Wifi
{
public:
    QListWidget *listWidget;
    QPushButton *pushButton;

    void setupUi(QDialog *Wifi)
    {
        if (Wifi->objectName().isEmpty())
            Wifi->setObjectName(QString::fromUtf8("Wifi"));
        Wifi->resize(314, 567);
        listWidget = new QListWidget(Wifi);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 291, 511));
        pushButton = new QPushButton(Wifi);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(100, 530, 100, 25));
        QFont font;
        font.setPointSize(10);
        pushButton->setFont(font);

        retranslateUi(Wifi);

        QMetaObject::connectSlotsByName(Wifi);
    } // setupUi

    void retranslateUi(QDialog *Wifi)
    {
        Wifi->setWindowTitle(QApplication::translate("Wifi", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Wifi", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Wifi: public Ui_Wifi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIFI_H
