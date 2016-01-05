/************
 *
 *功能:
 *
 *
 *
 **/
#include "networkconfig.h"
#include "ui_networkconfig.h"
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostInfo>
#include <QProcess>
#include <QFile>
#include <QMessageBox>

#include "wifi.h"
/************
 *
 *功能:
 *
 *
 *
 **/
NetworkConfig::NetworkConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkConfig)
{
    ui->setupUi(this);


    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);

    QPalette  palette = this->palette();
    palette.setColor(QPalette::Background,Qt::white);
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    palette.setColor(QPalette::ButtonText,Qt::white);


    palette.setColor(QPalette::Button,QColor(26,112,178,220));
    ui->pushButton->setPalette(palette);
    ui->pushButton->setAutoFillBackground(true);
    ui->pushButton->setFlat(true);


    ui->comboBox->addItem("STATIC");
    ui->comboBox->addItem("DHCP");

    username = "";
    password = "";

    QRegExp rx_ip("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
    QValidator *validator = new QRegExpValidator(rx_ip, ui->lineEdit_2);
    ui->lineEdit_2->setValidator(validator);
    validator = new QRegExpValidator(rx_ip, ui->lineEdit_3);
    ui->lineEdit_3->setValidator(validator);
    validator = new QRegExpValidator(rx_ip, ui->lineEdit_4);
    ui->lineEdit_4->setValidator(validator);
    validator = new QRegExpValidator(rx_ip, ui->lineEdit_5);
    ui->lineEdit_5->setValidator(validator);

    QString  resolutions = "1024x768,1280x720,1280x760,1280x768,1280x800,1280x960,1280x1024,1360x768,1366x768,1400x1050,1440x900,1600x900,1600x1200,1680x1050,1920x1080";
    resolus = resolutions.split(",");

    ui->comboBox_2->addItems(resolus);

    ui->pushButton->setFocusPolicy(Qt::NoFocus);


    GetNetworkConfig();
    CheckIpType();

    method_ip = ui->comboBox->currentText();

    this->localnetinform = macaddress+"\n"+localhostname+"\n"+gateway_info+"\n"+mask_info+"\n"+ip_info+"\n"+dns+"\n"+ui->comboBox->currentText();

    //qDebug() << localnetinform;

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(doChangStyleForGetIP()));
    //connect(ui->lineEdit_4,SIGNAL(editingFinished()),this,SLOT(modifygateway()));
    //connect(ui->lineEdit_5,SIGNAL(editingFinished()),this,SLOT(doGetCdms()));

    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyIpandNetMask()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifygateway()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(doGetCdms()));

}
/***
 *when the  pc started ,the funciton will be  called at first.
 *
 *
 **/
void  NetworkConfig::SendRegisterData()
{


    emit  this->signals_cdms(username+password+localnetinform);//connect to cdms
}
/***
 *get cdms address by the socket
 *
 **/
void  NetworkConfig::doGetCdms()
{

    if(localhostname.compare(ui->lineEdit->text()) != 0)
    {
        localhostname = ui->lineEdit->text();
        QProcess::execute("hostname  "+localhostname);
    }
    QFile file( "/sys/class/net/"+devicename+"/operstate" );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file.readLine();
        QString mg = line;
        file.close();
        qDebug() << mg;
        if(mg.compare("down\n") == 0)
        {

            QProcess::execute("ifconfig "+devicename+"  up");
        }
    }
    if((QString(ui->comboBox->currentText()).compare("DHCP")!=0)&&(ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty() ||
                ui->lineEdit_2->text().isEmpty()))
    {
        QMessageBox  box;
        box.setWindowTitle("提示");
        box.setText("network don't have done.");
        box.exec();
        return;
    }
    doChangResolution();
    modifyIpandNetMask();
    if(!ui->lineEdit_5->text().isEmpty())
    {
        QString  msg = ui->lineEdit_5->text();
        if(msg.compare(this->cdms_ip) != 0)
        {
            QFile file( "./server.txt" );
            if ( file.open(QIODevice::WriteOnly | QIODevice::Text) ) {

                QTextStream  out(&file);
                out << msg << endl;
                out.flush();
                file.close();
            }
            this->cdms_ip = msg;

            emit  signals_modifycdms(this->cdms_ip);
        }
        if(QString(ui->comboBox->currentText()).compare("DHCP")==0)
        {
            this->localnetinform = macaddress+"\n"+ui->lineEdit->text()+"\n"+gateway_info+"\n"+mask_info+"\n"+ip_info+"\n"+dns+"\n"+ui->comboBox->currentText();
        }
        else
        {
            this->localnetinform = macaddress+"\n"+ui->lineEdit->text()+"\n"+ui->lineEdit_4->text()+"\n"+ui->lineEdit_3->text()+"\n"+ui->lineEdit_2->text()+"\n"+dns+"\n"+ui->comboBox->currentText();

        }
        emit  this->signals_cdms(username+password+localnetinform);//connect to cdms
    }
}
/************
 *
 *功能:
 *
 *
 *
 **/
void  NetworkConfig::FillContent()
{
        GetDevicename();
        //GetIpandNetmask();
        ui->lineEdit_2->setText(this->ip_info);
        ui->lineEdit_3->setText(this->mask_info);
        ui->lineEdit_4->setText(this->gateway_info);
        ui->lineEdit->setText(localhostname);

}
void  NetworkConfig::GetGateway()
{
    QProcess p;
    QString msg;
    p.start("ip route");
    p.waitForFinished();

    msg = p.readAllStandardOutput();
    QStringList  way = msg.split(" ");
    if(QString("default").compare(way.at(0)) == 0 &&
            QString("via").compare(way.at(1)) == 0)
    {
        this->gateway_info = way.at(2).toLocal8Bit().data();
        ui->lineEdit_4->setText(this->gateway_info);
        //    qDebug()<<this->gateway_info;
    }
    /***Get gateway ***/
    if(this->gateway_info.length() <= 0)
    {
        if(devicename.compare("eth0")==0)
        {
        QProcess::execute("/etc/init.d/networking restart");//update gateway table 
       // goto  loopgateway;
        }
    }
}
void  NetworkConfig::GetResolution()
{
    QProcess p;
    QString msg;
    p.start("xrandr --screen 0");
    p.waitForFinished();
    msg = p.readAllStandardOutput();

    //    qDebug()<<"resolution :"<<msg;
    QStringList  way = msg.split("\n");
    QString tems = way.at(1).toLocal8Bit().data();
    QStringList  tmps = tems.split(" ");

    //           foreach(QString  sm,way)
    //           {
    //               qDebug() <<"\n "<< sm;
    //           }
    this->VGAarg = tmps.at(0).toLocal8Bit().data();
    tems = tmps.at(2).toLocal8Bit().data();
    tmps = tems.split("+");
    this->currentResolution = tmps.at(0).toLocal8Bit().data();

}
void  NetworkConfig::GetDNS()
{
    QFile file_dns( "/etc/resolv.conf" );
    if ( file_dns.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_dns.readLine();
        file_dns.close();
        QString mg = line;
        QStringList  dnsinfo = mg.split(" ");
        if(dnsinfo.size() > 1)
        {
            this->dns = dnsinfo.at(1).split("\n").at(0);
            qDebug() <<"DNS:"<< dns;
        }
    }
}
void  NetworkConfig::GetIpandNetmask()
{
    QProcess p;
    QString msg;
    p.start("ifconfig "+devicename);
    p.waitForFinished();
    msg = p.readAllStandardOutput();
    QStringList  qsl = msg.split("\n");
    foreach(QString str,qsl)
    {
        if(str.indexOf("inet addr:") > 0)
        {
            int  p = str.indexOf("inet addr:");
            QString ipm =str.mid(p,str.length()-p);
            QStringList  mg = ipm.split(" ");
            foreach(QString msm,mg)
            {
            if(msm.indexOf("addr:") >= 0)
            this->ip_info = msm.split(":").at(1);
            if(msm.indexOf("Mask:") >= 0)
            this->mask_info = msm.split(":").at(1);
            }
        }
        if(str.indexOf("HWaddr")>0)
        {
            int  k = str.indexOf("HWaddr");
            QString mlk = str.mid(k,str.length()-k);
            macaddress = mlk.split(" ").at(1);
            ui->label_25->setText(macaddress);

        }
    }


}
void  NetworkConfig::GetDevicename()
{
    //QString  msg = QString(cmd_system("cat /proc/net/dev | awk '{print $1$2$3}'|grep :"));
    //sleep(1);
    //QString  msg1 = QString(cmd_system("cat /proc/net/dev | awk '{print $1$2$3}'|grep :"));
    //qDebug() <<"===="<<msg<<"\n===="<<msg1;
    //if(msg.compare(msg1) != 0)
    //{
    //    QStringList  qsl = msg.split("\n");
    //    QStringList  qsl1 = msg1.split("\n");
    //    for(int i=0;i < qsl.size();i++)
    //    {
    //        if(qsl.at(i).compare(qsl1.at(i)) != 0)
    //        {
    //            devicename = qsl.at(i).split(":").at(0);
    //            qDebug()<<"sss:"<<devicename;
    //            if(devicename.compare("lo") == 0)
    //                goto  POINT;
    //            break;
    //        }
    //    }
        
    //}
    //else
    {
POINT:
#if 1
        QByteArray  mg;
    QFile   file_config("./config.txt");
      if (!file_config.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QMessageBox::critical(NULL, "提示", "无法创建文件");
                
        }
      else
        mg = file_config.readLine(); 
      QString  ss = mg.split('\n').at(0);
      QStringList  ms = ss.split("$");
      if(QString("device").compare(ms.at(0)) == 0 && ms.size()>1)
          this->devicename = ms.at(1);
        else
            devicename="eth0";
    
    qDebug()<<"devicename:"<<devicename;
      file_config.close();
#endif
    }
}
void  NetworkConfig::GetNetworkConfig()
{
    QFile fileRe( "user.txt" );
    if (!fileRe.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QMessageBox::critical(NULL, "提示", "无法创建文件");
        return;

    }
    QString  filemsg = fileRe.readLine();
    fileRe.close();
    QStringList   qlu = filemsg.split(":");
    username =   qlu.at(0)+"\n";
    password =  qlu.at(1);

    localhostname = QHostInfo::localHostName();
    //    qDebug()<<"localHostName:"<<localhostname;
    ui->lineEdit->setText(localhostname);


    /*************read config  file*************/
    GetDevicename();
    //this will be  contribute to  the wifi damen server stop
#if 0
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list)
    {
        if(interface.name().compare(devicename) == 0)
        {
                        qDebug() << "Device: "<<interface.name();
                        qDebug() << "HardwareAddress: "<<interface.hardwareAddress();
            //macaddress = interface.hardwareAddress();
            //ui->label_25->setText(macaddress);


            QList<QNetworkAddressEntry> entryList =interface.addressEntries();

            foreach(QNetworkAddressEntry entry,entryList)
            {
                    if(entry.ip().toString().length() <= 0)
                    {
                        if(devicename.compare("wlan0") == 0)
                            system("dhclient wlan0");
                    }
                 // qDebug()<<"IP Address: "<<entry.ip().toString();
                 //qDebug()<<"Netmask: "<<entry.netmask().toString();
                 //qDebug()<<"Broadcast: "<<entry.broadcast().toString();
                 //qDebug() << "-------xxxxx-------\n";
            }
        }
        // qDebug() << "-----------------\n";
    }
#endif
    if(devicename.compare("wlan0") == 0)
            system("ifconfig eth0 down");


    QProcess  p;
    QString   msg;

    //p.start("ifconfig eth0");
    //p.waitForFinished();
    //msg = p.readAllStandardOutput();
    //if(msg.length()> 0 && msg.indexOf("RUNNING") > 0)
    //    devicename = "eth0";
    //p.start("ifconfig wlan0");
    //p.waitForFinished();
    //msg = p.readAllStandardOutput();
    //if(msg.length()> 0 && msg.indexOf("RUNNING") > 0)
    //    devicename = "wlan0";
    GetGateway();
    GetResolution();
    /********  Get  MAC IP NetMask CDMS   As Follow*******/
    GetIpandNetmask();

    QFile file_eth( "/etc/network/interfaces" );
    if ( file_eth.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_eth.readAll();
        file_eth.close();
        QString  mg = line;
        QStringList  info = mg.split("\n");
        int  n = 0; 
        foreach(QString str,info)
        {
            if(str.indexOf("iface ") == 0 && str.indexOf(" "+devicename+" ") > 0)
            {
                if(str.indexOf("dhcp") > 0)
                {
                    ui->comboBox->setCurrentIndex(1);
                    ui->lineEdit_2->setText(ip_info);
                    ui->lineEdit_3->setText(mask_info);
                    iptype = "DHCP";
                }
                else if(str.indexOf("static") > 0)
                {
                    iptype = "STATIC";
                    QStringList mgs = info.at(n+1).split(" ");
                    this->ip_info = mgs.at(1);
                    mgs = info.at(n+2).split(" ");
                    this->mask_info = mgs.at(1);
                    ui->lineEdit_2->setText(ip_info);
                    ui->lineEdit_3->setText(mask_info);
                }
                //CheckIpType();
            }
            n++;

        }
    }
    GetDNS();
#if  0

    QFile file_eth( "/etc/network/interfaces" );
    if ( file_eth.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_eth.readAll();
        file_eth.close();
        QString  mg = line;
        QStringList  info = mg.split("\n");
        int  n = 0; 
        foreach(QString str,info)
        {
            if(str.indexOf("iface") == 0 && str.indexOf("eth0") > 0)
            {    
                if(str.indexOf("static") > 0)
                {

                    qDebug()<<"iface:"<< str;
                    qDebug()<<info.at(n+1);
                    qDebug()<<info.at(n+2);
                    this->ip_info = info.at(n+1);
                    this->mask_info = info.at(n+2);
                    ui->lineEdit_2->setText(ip_info);
                    i->lineEdit_3->setText(mask_info);
                    break;
                }
            }
            n++;
        }
        ui->lineEdit_2->setText(ip_info);
        ui->lineEdit_3->setText(mask_info);
    }
#endif

    /*********Get  CDMS  IP  Address  from   Local  file As Follow***********/



    QFile file( "./server.txt" );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file.readLine();
        QString mg = line;
        QStringList  tmp = mg.split("\n");
        this->cdms_ip = tmp.at(0);


        ui->lineEdit_5->setText(this->cdms_ip);
        //        qDebug() << mg<<":"<<this->cdms_ip;
        file.close();
    }
    else
        qDebug() << "open file failed!\n";

    /****************Get  All  Resolution  Existed As Follow****************/
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    QString  resolu = QString("%1x%2").arg(screenRect.width()).arg(screenRect.height());
    this->currentResolution = resolu;



    int  n=0;
    foreach(QString  tm,resolus)
    {
        if(tm.compare(this->currentResolution) == 0)
        {
            ui->comboBox_2->setCurrentIndex(n);
            break;
        }
        n++;
    }
}
/**
  this   function  is to  modify the  resolution
 */
/****/
void    NetworkConfig::doChangResolution()
{

    QString  msg = ui->comboBox_2->currentText();
    //if( msg.compare(this->currentResolution) != 0)
    {

        QStringList  list = msg.split("x");
        QString wid = list.at(0).toLocal8Bit().data();
        QString hei = list.at(1).toLocal8Bit().data();
        //she zhi fenbianlv
        QProcess  sp;

#if 1
        const QString  newresolution = QString("cvt %1 %2 %3").arg(wid).arg(hei).arg("60");
        sp.start(newresolution);
        sp.waitForFinished();
        QString  val = sp.readAllStandardOutput();
        sp.close();

        int  index = val.indexOf("Modeline");
        QString  need = val.mid(index+8,val.length()-index-8);
        QString  stdresolu = need.split(" ").at(1).toLocal8Bit().data();


        //qDebug()<<val<<"\nffffffffffffffff:"<<stdresolu;

        //创建新的mode
        const QString  command = QString("xrandr --newmode %1").arg(need);

        qDebug()<<command;
        sp.start(command);
        sp.waitForFinished();
        sp.close();
        //xrandr显示出来的列表中，要更改目标输出的设备名称
        const  QString  command1 = QString("xrandr --addmode  %1  %2").arg(this->VGAarg).arg(stdresolu);
        qDebug()<<command1;
        sp.start(command1);
        sp.waitForFinished();
        sp.close();
#endif
        const  QString  comm = QString("xrandr -s %1").arg(msg);
        qDebug()<<comm;
        sp.start(comm);
        sp.waitForFinished();
        sp.close();


    }
}

/************
 *
 *功能:check  getip  style
 *
 *
 *
 **/
void    NetworkConfig::CheckIpType()
{
    if(QString(ui->comboBox->currentText()).compare("DHCP")==0)
    {
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        ui->lineEdit_4->setText("");
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_4->setDisabled(true);
        QPalette  palette = ui->lineEdit_2->palette();
        palette.setColor(QPalette::Background,QColor(170,170,170,50));
        ui->lineEdit_2->setPalette(palette);
        ui->lineEdit_3->setPalette(palette);
        ui->lineEdit_4->setPalette(palette);

    }
    else
    {
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);
        ui->lineEdit_4->setDisabled(false);
        ui->lineEdit_2->setText(this->ip_info);
        ui->lineEdit_3->setText(this->mask_info);
        ui->lineEdit_4->setText(this->gateway_info);
    }

}
void    NetworkConfig::doChangStyleForGetIP()
{
    //QMessageBox msgBox;
    //msgBox.setInformativeText("doChangStyleForGetIP!");
    //msgBox.setStandardButtons(QMessageBox::Save |QMessageBox::Cancel);
    //msgBox.setDefaultButton(QMessageBox::Save);

    CheckIpType(); 
}
/************
 *
 *功能:
 *
 *
 *
 **/
void    NetworkConfig::modifyIpandNetMask()
{
    QString  msip,msmask,contents="";
    msip = ui->lineEdit_2->text();
    msmask = ui->lineEdit_3->text();
    QString  type = ui->comboBox->currentText();    

    QFile file_eth( "/etc/network/interfaces" );
    if ( file_eth.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_eth.readAll();
        file_eth.close();
        QString  mg = line;
        QStringList  info = mg.split("\n");
        int  n = 0; 
        foreach(QString str,info)
        {
            if(!str.contains("#",Qt::CaseInsensitive))
            {
                if(str.contains("iface "+devicename+" inet ",Qt::CaseInsensitive))
                {

                    if(type.compare("DHCP") != 0 && (!msip.isEmpty() && !msmask.isEmpty()))
                    {
                        contents +=  "iface "+devicename+" inet static\n";
                        QString  address = QString("address ")+msip+"\n";
                        QString  netmask = QString("netmask ")+msmask+"\n";
                        QString  gateway = QString("gateway ")+ui->lineEdit_4->text()+"\n";
                        contents += address +netmask+gateway;    
                    }
                    else
                    {
                        contents += "iface "+devicename+" inet dhcp\n";
                    }
                }
                else
                {
                    if(str.length() > 0)
                    contents += str+"\n";
                }
                qDebug()<<"====t:"<<str;
            }
            n++;
        }
    }


    qDebug()<<"========content:"<<contents;

    //if(iptype.compare(ui->comboBox->currentText()) != 0)
    //{
#if 0  
    QString  content="";
    QFile file_eth( "/etc/network/interfaces" );
    if ( file_eth.open(QIODevice::ReadOnly | QIODevice::Text) ) 
    {
        content = file_eth.readAll();  
        file_eth.close();
    }
    if(content.length() > 0)
    {
        QString  type = ui->comboBox->currentText();    
        if(content.contains("iface eth0 inet *",Qt::CaseInsensitive))
        {
            if(type.compare("DHCP") != 0 && (!msip.isEmpty() && !msmask.isEmpty()))
            {

                QString  address = QString("address ")+msip+"\n";
                QString  netmask = QString("netmask ")+msmask+"\n";
                QString  gateway = QString("gateway ")+ui->lineEdit_4->text()+"\n";
                //content.replace(QRegExp("iface eth0 inet *"),QString("iface eth0 inet ")+type+"\n"+address+netmask+gateway);
                content.replace(content.indexOf("iface eth0 inet ")+16,6,type+"\n");
                content.replace(content.indexOf("address "),19,address);
                content.replace(content.indexOf("netmask "),19,netmask);
                content.replace(content.indexOf("gateway "),19,gateway);
            }else 
                content.replace(content.indexOf("iface eth0 inet ")+16,6,type+"\n");
        }


        qDebug()<<"========content:"<<content;


#endif
        if(devicename.compare("eth0") == 0)
        {
        if (!file_eth.open(QIODevice::WriteOnly|QIODevice::Text)) {
            QMessageBox::critical(NULL, "提示", "无法创建文件");

        }
        else
        {

            QTextStream out(&file_eth);
            out<<contents<<endl;
            out.flush();

            file_eth.close();
        }
        }
        
        if(type.compare(iptype) != 0)
        {
        if(type.compare("DHCP") != 0 )
        {
            //iptype = "STATIC";
            QString  str="";
            str = QString("ifconfig "+devicename+" %1 netmask %2").arg(msip).arg(msmask);
            qDebug()<<str;
            QProcess::execute(str);
            emit    this->signals_startthread(str);

        }else
        {
            //iptype = "DHCP";
            if(devicename.compare("wlan0") == 0)
            {
                QProcess p;
                p.start("wpa_cli wlan0");
                p.waitForFinished();
                QString msg = p.readAllStandardOutput();
                if(msg.length() <= 0)
                    return;
                QStringList  states = msg.split("\n");
                foreach(QString sm,states)
                {
                    if(sm.indexOf("wpa_state=") >= 0 && 
                        QString("COMPLETED").compare(sm.split("=").at(1))==0)
                    {
                    this->signals_startthread("dhclient wlan0");
                    QTimer::singleShot(1000, this, SLOT(GetGateway()));
                    }
                }
            }
            else
                emit    this->signals_startthread("/etc/init.d/networking  restart");
        }
        }

    }
    /************
     *
     *功能:
     *
     *
     *
     **/
    void    NetworkConfig::modifygateway()
    {
        if(QString("DHCP").compare(ui->comboBox->currentText()) == 0)
            return;
        QString  gatewaymg = ui->lineEdit_4->text();
        if(!gatewaymg.isEmpty()&& gatewaymg.compare(this->gateway_info) == 0)
        {
            /*QFile   file_eth("gateway.txt");
              if (!file_eth.open(QIODevice::WriteOnly|QIODevice::Text)) {
              QMessageBox::critical(NULL, "提示", "无法创建文件");

              }

              QTextStream out(&file_eth);
              out<<gatewaymg<<endl;
              out.flush();

              file_eth.close();
             */


            QString  str;
            str = QString("route add default gw  %1").arg(ui->lineEdit_4->text());
            qDebug()<<str;
            QProcess::execute(str);
        }
    }
    /************
     *
     *功能:
     *
     *
     *
     **/
NetworkConfig::~NetworkConfig()
{
        delete ui;
}
/**********
Remote  Control The  Box   of  function  to  realize
*********/
#if 0
void    NetworkConfig::remote_doChangehostnameSlots(   const QString &newhostname)
{
    if(newhostname.length() > 0)
    {
        QProcess::execute("hostname "+newhostname);
    }
}
void    NetworkConfig::remote_doChangenetworkSlots(    const QString &net)
{
    //"ip:*.*.*.*\nnetmask:*.*.*.*\ngateway:*.*.*.*\n"
    QStringList   qnet = net.split("\n");
    QString   cmdline="ifconfig  ";
    QString   cmdline1="route  add  default  gw  ";
    foreach(QString str,qnet)
    {
        if(str.indexOf("ip") >= 0)
        {
            ip_info = str.split(":").at(1);
            cmdline +=devicename+"  "+str.split(":").at(1)+"  ";
        }
        else if(str.indexOf("netmask") >= 0)
        {
            mask_info = str.split(":").at(1);
            cmdline +="netmask "+str.split(":").at(1)+"  ";
        }
        else if(str.indexOf("gateway") >=0 )
        {
            gateway_info = str.split(":").at(1);
            cmdline1 += str.split(":").at(1);
        }
    }

    qDebug()<<"netconfig cmd:"<<cmdline;
    qDebug()<<"set gateway cmd:"<<cmdline1;
}
void    NetworkConfig::remote_doChangeresolutionSlots( const QString &resolution)
{
    qDebug()<<resolution; 
}
#endif

