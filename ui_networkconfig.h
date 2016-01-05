/********************************************************************************
** Form generated from reading UI file 'networkconfig.ui'
**
** Created: Wed Dec 16 18:22:15 2015
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKCONFIG_H
#define UI_NETWORKCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NetworkConfig
{
public:
    QWidget *widget;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QComboBox *comboBox_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QPushButton *pushButton;
    QLabel *label_24;
    QLabel *label_25;
    QFrame *line;

    void setupUi(QDialog *NetworkConfig)
    {
        if (NetworkConfig->objectName().isEmpty())
            NetworkConfig->setObjectName(QString::fromUtf8("NetworkConfig"));
        NetworkConfig->resize(491, 650);
        widget = new QWidget(NetworkConfig);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 491, 651));
        widget->setMinimumSize(QSize(491, 651));
        widget->setMaximumSize(QSize(491, 651));
        QFont font;
        font.setPointSize(11);
        widget->setFont(font);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 150, 61, 20));
        label->setFont(font);
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(170, 140, 251, 41));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 210, 61, 16));
        label_2->setFont(font);
        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(170, 260, 251, 41));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(70, 20, 351, 31));
        QFont font1;
        font1.setPointSize(20);
        label_4->setFont(font1);
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(90, 270, 41, 20));
        label_5->setFont(font);
        comboBox = new QComboBox(widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(170, 200, 101, 41));
        lineEdit_3 = new QLineEdit(widget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(170, 320, 251, 41));
        lineEdit_4 = new QLineEdit(widget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(170, 380, 251, 41));
        lineEdit_5 = new QLineEdit(widget);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(170, 500, 251, 41));
        comboBox_2 = new QComboBox(widget);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(170, 440, 131, 41));
        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(70, 330, 61, 16));
        label_6->setFont(font);
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(90, 390, 31, 20));
        label_7->setFont(font);
        label_8 = new QLabel(widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 450, 81, 20));
        label_8->setFont(font);
        label_9 = new QLabel(widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(50, 510, 81, 20));
        label_9->setFont(font);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(100, 590, 262, 42));
        QFont font2;
        font2.setPointSize(18);
        pushButton->setFont(font2);
        label_24 = new QLabel(widget);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(70, 100, 71, 20));
        label_24->setFont(font);
        label_25 = new QLabel(widget);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(170, 100, 251, 16));
        line = new QFrame(widget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 60, 491, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        retranslateUi(NetworkConfig);

        QMetaObject::connectSlotsByName(NetworkConfig);
    } // setupUi

    void retranslateUi(QDialog *NetworkConfig)
    {
        NetworkConfig->setWindowTitle(QApplication::translate("NetworkConfig", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NetworkConfig", "\347\273\210\347\253\257\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("NetworkConfig", "\351\205\215\347\275\256\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("NetworkConfig", "\344\270\212\346\265\267\346\234\211\345\255\232\347\275\221\347\273\234\344\272\221\346\241\214\351\235\242\351\205\215\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("NetworkConfig", "IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("NetworkConfig", "\345\255\220\347\275\221\346\216\251\347\240\201", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("NetworkConfig", "\347\275\221\345\205\263", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("NetworkConfig", "\345\261\217\345\271\225\345\210\206\350\276\250\347\216\207", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("NetworkConfig", "CDMS\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("NetworkConfig", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("NetworkConfig", "MAC\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        label_25->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class NetworkConfig: public Ui_NetworkConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKCONFIG_H
