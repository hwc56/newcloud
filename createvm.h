#ifndef     __CREATEVM_H__
#define     __CREATEVM_H__

#include <QtGui>
#include <QWidget>
#include <QVector>
class   CreateVmlist:public     QWidget
{
    Q_OBJECT
public:
        CreateVmlist(QWidget  *parent = 0);
        void    CreateVM(const QString  &);
        void    Reset();
        QString  vmdata;//check if the same data every reflush.
        int     nums_vm;//the number of vmachine
        int     width_;
private:
        void    SetStatusVm(QWidget   *btn,const QString &);
signals:
        void    signals_vm(const QString &);
        void    signals_connect(const QString &);
        void    signals_start(const QString &);
        void    signals_down(const QString &);
        void    signals_reboot(const QString &);

private slots:
        void    changeColor(QListWidgetItem *item);
        void    emit_connect_signals();
        void    emit_start_signals();
        void    emit_shutdown_signals();
        void    emit_reboot_signals();
        void    deal_operatorslots(int);
        void    deal_choicevmslots(int);
        void    deal_slots(int);
        void    setButtonBackImage(QPushButton *,QString , int ,int);
        void    getuserinfo(const QString &);
        void    checkuserinfo(const QString &);
private:
        void    VmFactory();
        bool    enterBtn(QMouseEvent *e,QWidget  *btn,int  m);
        void    mousePressEvent(QMouseEvent *e);
        
private:
    QList <QPushButton *>  vmslist;
    QList <QLabel *>  labels;
    QButtonGroup  *btngp;
    QVector <QStringList>  Vmgs;//vmname
    QVector <QString>  vmsendmg;
    QVector <QString>  ids;

    int     idvm;//restart or shutdown by the idvm of button
    int     lskey;
    QListWidget      *list;
    QString  user,newuser;
};

#endif
