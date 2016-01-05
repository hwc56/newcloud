/************
 *
 *功能:Main  GUI   Class
 *
 *
 *
 **/
#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include <QtGui>
#include <Qt/QtWebKit>
#include "login.h"
#include "networkconfig.h"
#include "wifi.h"
#include "changepassword.h"
#include "createvm.h"
#include "socketservice.h"
//#include "termigrate.h"

namespace Ui {
    class mainview;
}
class SpicyThread:public  QThread
{
    public:
        SpicyThread()
        {

        }
        void    setcmd(const QString  &m){
            this->cmd = m;
        }
        void    run()
        {

            qDebug()<<"this is the thread runing!"<<QThread::currentThreadId();
            QProcess::execute(cmd);
            exec();


        }
    private:
        QString  cmd;
};
class mainview : public QWidget
{
    Q_OBJECT

    public:

        explicit mainview(QWidget *parent = 0);
        ~mainview();
        //void    emit_loadnetconfig();
        void  CheckNetworkState();
        //void    emit_loginsignal();
        void    KillSpicyThread( );
        void    CreateVM(const QString &);
        void    SetStatusVm(QWidget *w,const QString &);
    private slots:
        void    closecomputer();
        void    rebootcomputer();
        void    reflushlisttable(const QString &);
        void    wifioperator();
        void    helpoperator();
        void    moreoperator();
        void    docreen();

        void    changeslots(int);
        void    changeColor(QListWidgetItem *item);

        void    emit_checkcdms_signal();
        void    deal_widgets(int);
        void    emit_widgets(int);
        void    getcdmsslots(const QString  &);
    public slots:
            void    startthread(const QString  &);

    signals:
        //void    signals_hostlist();//for reflushing hostlist
        void    signals_checkcdms(const QString &);
        void    loadoperator(int);
        void    signals_modify(const QString &);
        void    signals_widgets(int);
    private:

        void    mouseMoveEvent(QMouseEvent  *e);
        void    mousePressEvent(QMouseEvent  *e);
        //void    keyPressEvent(QKeyEvent  *e);
        bool    enterBtn(QPoint  p,QWidget  *btn);
        bool    open;

        void   paintEvent(QPaintEvent  *);
    private:
        Ui::mainview    *ui;
        Login           *login;
        NetworkConfig   *netconfig;
        Wifi            *wifi;

        ChangePasswd    *changepasswd;
        CreateVmlist    *hostlist;
        QString         cdmpip;

        SocketService   *http;
        int             logo_x;
        int             logo_y;
        QString         version;
        QList<QString>      spicy_thread_id;
        //int      spicy_thread_id;

       //Termigrate    *termigrate;  
};

#endif // MAINVIEW_H
