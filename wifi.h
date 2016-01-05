/************
*
*功能:Wifi Control Class
*
*
*
**/
#ifndef WIFI_H
#define WIFI_H

#include <QDialog>
#include <QtGui>
namespace Ui {
    class Wifi;
}

class Wifi : public QDialog
{
    Q_OBJECT

public:
    explicit Wifi(QWidget *parent = 0);
    ~Wifi();
    QString    CheckSignals(int  signal_tough);
    QString connectedname;
    void    updatelist()
    {
            Updatewifilist();
    }
    int    choice_signal;
signals:
    void    signals_connect(const QString &);
    //void    closewaitmsg();
    void    signals_update();
    void    siglogin(int);
private slots:
    void    changeColor(QListWidgetItem *item);
    void    Scan();
    void    ConnectDeal();
    void    isKnowConnectDeal();
    void    LinkWifiSlots();
    void    Updatewifilist();
    //void    addWifiSsid();
    void    disconnectslots();
    void    connectslots();
    void    removeslots();

    //void    checkstatus();
private:
    void    Stringsort(); 
    void    GetConnectName(); 
    void    Transcode(QString  &mg);
    void    GetAplist();
    int     num_network;
    int     temp;
    QString  wid;
    //QTimer   *timer;
    QString  newid;
    QString  key;
    QString  delid;
    QString wifi_state;
    QList<QString>  wifi_info;
    Ui::Wifi *ui;
};
#ifdef  __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define   CACHE_BUFFER_SIZE  1024*20
char* cmd_system(const char* command);
#ifdef  __cplusplus
}
#endif

#endif // WIFI_H
