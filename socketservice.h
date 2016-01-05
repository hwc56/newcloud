/*****
*
*   The  Class  Function  is  Recv  Data  and  Send  Data  with  HttpService by Http Protocol  
*
*   ,and  Parse  the  JSON   Data  . It's Created int  the  class  MainView   
*
*   construct  function.
*
*
********/
#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QCryptographicHash>
#include "http_curl.h"

#include <usb.h>


class SocketService : public QObject
{
    Q_OBJECT
public:
    explicit SocketService(QObject *parent = 0);
    void    CheckService(const QString &);
    void    ResetUserInfo()
    {
        strloginCheck ="";
        strhostlist = "";
        strConnectInfo = "";
        user = "";
        passwd ="";
        userpass = "username="+user+"&password="+passwd;
        login_state = false;
        vmsinfors.clear();
    }
    bool    login_state;
private:
    std::string    IfJsonData(const std::string &strInfo);
    void    JSParse(const std::string  &strmsg );//js  check  if  have  some  question.
    void    RegisterInfo(const QString  &);
    void    ReadUSB();
    //void    sleep(int  msec);
private:
    CHttpClient *http; 
    std::string strloginCheck;
    std::string strhostlist; 
    std::string strConnectInfo;
    std::string user;
    std::string  passwd;
    std::string  userpass;
    std::string  httphead;
    std::string  cdmsip;
    std::string  httpcache;
    std::string  boxmac;
    std::string  vmstatus;
    std::string  localusbinfo;
    QString  checkmigrate;
    QList<QString>  vmsinfors;
     
signals:
    void    signals_loginview(int);
    void    signals_httperror(const QString &);
    void    signals_gethostlist();
    void    signals_vmlist(const QString  &);
    //void    signals_tohostlistcheck(const QString  &);
    void    signals_connectError(int);
    void    signals_startmigrate(const QString  &);
    void    signals_startspicy(const QString  &);
    void    signals_register();


private slots:
    void    LoginCloud(const QString &);
    void    login_slots(const QString &);
    void    gethostlist_slots();
    void    startvmslots(const QString &);
    void    shutdownvmslots(const QString &);
    void    rebootvmslots(const QString &);
    void    getconnectinfo_slots(const QString &);
    void    displayErrorSlots(const QString &);
    void    registertocdms(const QString &);
    void    updateUserinfo(const QString &);
    void    isableconnectslots(const QString &);
    void    logipslots(const QString &);
    void    modifycdmsSlots(const QString &);
};

#endif // SOCKETSERVICE_H
