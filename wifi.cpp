/************
 *
 *功能:
 *
 *
 *
 **/
#include "wifi.h"
#include "ui_wifi.h"
#include "logging.h"
#include <QMessageBox>
#include <stdlib.h>
/************
 *
 *功能:Wifi功能模块的构造函数，检测无线网卡设备工作状态，进行一些UI布局的设置。
 *
 *
 *
 **/
Wifi::Wifi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wifi)
{
    ui->setupUi(this);
    wifi_state="";
    connectedname="";
    wid="";
    choice_signal = 0;
    QPalette palette;
    QPixmap pixmap("./images/modify-bg.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    /*********Set  Unicode *********/
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);

    QFile file( "/sys/class/net/wlan0/operstate" );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file.readLine();
        QString mg = line;
        file.close();
        qDebug() << mg;
        if(mg.compare("down\n") == 0)
        {

            QProcess::execute("ifconfig wlan0  up");
            //emit    this->signals_startthread(mg);
        }
    }
    //emit  signals_connect("dhclient wlan0");

    //timer = new  QTimer();

    palette.setColor(QPalette::Button,QColor(26,112,178,220));
    ui->pushButton->setPalette(palette);
    ui->pushButton->setAutoFillBackground(true);
    ui->pushButton->setFlat(true);

    ui->pushButton->setText("搜索");
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    //palette.setColor(QPalette::ButtonText,Qt::white);
    //ui->pushButton_2->setText("+");


    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setMouseTracking(true);
    connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(LinkWifiSlots()));
    connect(ui->listWidget,SIGNAL(itemEntered(QListWidgetItem *)),this,SLOT(changeColor(QListWidgetItem *)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Updatewifilist()));
    //connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(addWifiSsid()));
        Updatewifilist();
}
#ifdef  __cplusplus
extern "C" {
#endif
char* cmd_system(const char* command)
{
    char* result = ""; 
    FILE *fpRead;
    fpRead = popen(command, "r");
    char buf[CACHE_BUFFER_SIZE];
    int ibuflen = 0;
    memset(buf,'\0',sizeof(buf));
    int tr = 0;
    while((tr = fgetc(fpRead))!=EOF)
    {
        buf[ibuflen++] = (unsigned char)tr;
        if(ibuflen >= CACHE_BUFFER_SIZE -1)
            break;
    }
    if(ibuflen >= CACHE_BUFFER_SIZE -1)
        buf[ibuflen] = '\0';
    else
        buf[ibuflen++] = '\0';
    result = buf;
    if(fpRead!=NULL)
        pclose(fpRead);
    return result;

}
#ifdef  __cplusplus
}
#endif
/************
 *
 *功能:每当鼠标移动到选项列表的某一项时，该项的背景颜色变为浅蓝色，其他背景色为白色。
 *
 *
 *
 **/

void    Wifi::changeColor(QListWidgetItem *item)
{
    for(int i=0;i<ui->listWidget->count();i++)
        ui->listWidget->item(i)->setBackground(QBrush(QColor(255,255,255)));

    item->setBackground(QBrush(QColor(0,245,255,30)));
}
/************
 *
 *功能:对wifi进行断开热点响应操作。
 *
 *
 *
 **/

void    Wifi::disconnectslots()
{
#if 1
    QString   msg = QString(cmd_system("wpa_cli status"));
    if(msg.indexOf("Failed") >= 0)
    {
        emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
    }
    else
    {
        QStringList  statelist = msg.split("\n");
        foreach(QString str,statelist)
        {

            //if(str.indexOf("[CURRENT]") > 0)
            //{
              //  delid = str.split("\t").at(0);
            //}
            if(str.indexOf("id=") >= 0)
            {
                delid = str.split("=").at(1);
            }
        }
    }

#endif
    QString cmd = "wpa_cli -iwlan0 disconnect "+delid;
    msg = "";
    msg = QString(cmd_system(cmd.toLocal8Bit().data()));
    if(QString("OK").compare(msg.split("\n").at(0)) != 0)
    {
        emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
    }
    connectedname = "";
    wifi_state = "";
    updatelist();

}
/************
 *
 *功能:在wifi热点记录表中移除某个热点操作。
 *
 *
 *
 **/

void    Wifi::removeslots()
{
    QString cmd = "wpa_cli -iwlan0 remove_network "+wid;
    QString msg = QString(cmd_system(cmd.toLocal8Bit().data()));
    if(QString("OK").compare(msg.split("\n").at(0)) != 0)
    {
        emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
        LOG("warning: wpa_supplicant deamon  restart at remove_network operation.");
    }
    updatelist();


}
/************
 *
 *功能:对wifi进行连接已记录的热点响应操作。
 *
 *
 *
 **/

void    Wifi::connectslots()
{
    QString cmd = "wpa_cli -iwlan0 select_network "+wid;
    QString   msg ;
    msg = QString(cmd_system(cmd.toLocal8Bit().data()));
    qDebug()<<cmd;
    qDebug()<<msg;
    if(QString("OK").compare(msg.split("\n").at(0)) != 0)
    {
        if(QString(cmd_system("wpa_cli -iwlan0 list_network")).indexOf("Failed") >= 0 )
        {
        emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
        LOG("warning: wpa_supplicant deamon  restart at  select_network operation.");
        }
    }
}
Wifi::~Wifi()
{
    delete ui;
}
/************
 *
 *功能:
 *
 *
 *
 **/
#if 0
void    Wifi::addWifiSsid()
{
    QDialog *tmp = new  QDialog(this);
    QPalette palette;
    QPixmap pixmap("./images/modify-bg.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    tmp->setPalette(palette);
    tmp->setAutoFillBackground(true);


    QLineEdit   *namecontent = new  QLineEdit(tmp);
    QLineEdit   *passwdcontent = new  QLineEdit(tmp);
    QLabel  *wifiname = new  QLabel("wifiname:",tmp);
    QLabel  *wifipasswd = new  QLabel("wifi密码:",tmp);
    //QComboBox   *combox = new   QComboBox(tmp);
    QPushButton *ok_button = new QPushButton("OK",tmp);
    QPushButton *cancel_button = new QPushButton("Cancel",tmp);
    passwdcontent->setEchoMode(QLineEdit::Password);


    //combox->addItem("STATIC");
    //combox->addItem("DHCP");

    QHBoxLayout *hlayout= new QHBoxLayout;
    hlayout->addWidget(wifiname);
    hlayout->addWidget(namecontent);
    QHBoxLayout *layout= new QHBoxLayout;
    layout->addWidget(wifipasswd);
    layout->addWidget(passwdcontent);
    QHBoxLayout *layout1= new QHBoxLayout;
    layout1->addWidget(ok_button);
    layout1->addWidget(cancel_button);
    QVBoxLayout  *vlayout = new  QVBoxLayout;

    vlayout->addLayout(hlayout);
    //vlayout->addWidget(combox);
    vlayout->addLayout(layout);
    vlayout->addLayout(layout1);
    tmp->setLayout(vlayout);

    ok_button->setFocusPolicy(Qt::NoFocus);
    cancel_button->setFocusPolicy(Qt::NoFocus);
    connect(ok_button,SIGNAL(clicked()),tmp,SLOT(accept()));
    connect(cancel_button,SIGNAL(clicked()),tmp,SLOT(close()));
    if((tmp->exec() == QDialog::Accepted) && !(namecontent->text().isEmpty()))
    {
        if(passwdcontent->text().length() < 8)
        {
            QMessageBox box;
            box.setText("password length is lower  than 8.");
            box.exec();
            return;
        }    
        QString msg = QString(cmd_system("wpa_cli -iwlan0 add_network"));
        if(msg.length() <= 0)
            return;
        QString id = msg.split("\n").at(0);

        QFile  file("./setssidpsk.sh");
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
            QMessageBox::critical(NULL, "提示", "无法创建文件");

        }
        else
        {
            QString  contents="wpa_cli -iwlan0 set_network "+id+" ssid \'\""+namecontent->text()
                +"\"\'\nwpa_cli -iwlan0 set_network "+id+" psk \'\""+passwdcontent->text()
                +"\"\'\nwpa_cli -iwlan0 enable_network "+id
                +"\nwpa_cli -iwlan0 select_network "+id;

            QTextStream out(&file);
            out<<contents<<endl;
            out.flush();

            file.close();
        }
        QProcess::execute("chmod 744 ./setssidpsk.sh");
        QProcess::execute("./setssidpsk.sh");
        //if(QString("DHCP").compare(combox->currentText())==0)
        //  emit  signals_connect("dhclient wlan0");
        // else
        //{
        // }

        ui->listWidget->addItem(new  QListWidgetItem(QIcon("./images/wifi_4.png"),namecontent->text()));
        wifi_info.push_back(namecontent->text()+"\t"+"any\t"+"on\t"+"wap");
    }

}
#endif
void    Wifi::GetConnectName()
{
 
   QString  msg;
    msg = QString(cmd_system("iwconfig wlan0"));
    if(msg.length() <= 0)
        return;
    QString  state_flg = msg.split("\n").at(0);
    int  fm = state_flg.indexOf("ESSID:");
    if(fm > 0)
    {
        int  len = state_flg.length()-fm-6;
        QStringList tmp = msg.mid(fm+6,len).split("\"");
        if(tmp.size()>1)
            connectedname = tmp.at(1);
        else
            connectedname = "";//unlink state
    qDebug()<<"GetConnectName():"<<connectedname; 
    }

}
#if 0
void    Wifi::checkstatus()
{
    QString  msg;
    msg = QString(cmd_system("wpa_cli status"));
    if(msg.indexOf("Failed") >= 0)
    {
        emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
    }
    QStringList  state = msg.split("\n");
    foreach(QString str,state)
    {
        if(str.indexOf("ssid=") >= 0 && connectedname.compare(str.split("=").at(1)) != 0)
            connectedname = str.split("=").at(1);
        if(str.indexOf("ssid=") < 0)
            connectedname = "";

    }
 
    Transcode(connectedname);
    emit  signals_update();
}
#endif
/************
 *
 *功能:
 *
 *
 *
 **/
void    Wifi::Scan()
{
    ui->listWidget->clear();
    GetConnectName(); 
    Transcode(connectedname);
    emit  signals_update();
    qDebug()<<"connectedname:"<<connectedname;
    for(int i= 0;i <wifi_info.size();i++)
    {
        ui->listWidget->addItem(new  QListWidgetItem(QIcon(CheckSignals(wifi_info.at(i).split("\t").at(1).toInt())),wifi_info.at(i).split("\t").at(0)));
    if(connectedname.compare(ui->listWidget->item(i)->text())== 0)
    {
        wifi_state = "\nConnected";
        ui->listWidget->item(i)->setText(ui->listWidget->item(i)->text()+wifi_state);
        ui->listWidget->item(i)->setTextColor(Qt::green);
    }
    }
    //emit   siglogin(0); 
}
/************
 *
 *功能:更新wifi显示列表的操作
 *
 *
 *
 **/

void    Wifi::Updatewifilist()
{
#if 1 
    ui->listWidget->clear();

    QString   msg="";
    GetConnectName(); 
    Transcode(connectedname);
    
    emit  signals_update();
    msg="";
    GetAplist(); 
    bool  is_exit = false;
    for(int i= 0;i <wifi_info.size();i++)
    {
        ui->listWidget->addItem(new  QListWidgetItem(QIcon(CheckSignals(wifi_info.at(i).split("\t").at(1).toInt())),wifi_info.at(i).split("\t").at(0)));
#if 1
    if(connectedname.compare(ui->listWidget->item(i)->text())== 0)
    {
        is_exit = true;
        wifi_state = "\nConnected";
        ui->listWidget->item(i)->setText(ui->listWidget->item(i)->text()+wifi_state);
        ui->listWidget->item(i)->setTextColor(Qt::green);
    }
    }

    if(!is_exit)
    {
        connectedname="";
        emit  signals_update();
    }
#endif
    msg = QString(cmd_system("wpa_cli status"));
    
    if(msg.indexOf("COMPLETED") > 0)
    {
        if(msg.indexOf("ip_address=") < 0 )
        {
            //QProcess::execute("dhclient wlan0");
        }
        else
        {
            //emit   siglogin(0); 
            qDebug()<<"have ip";
        }
        emit   siglogin(99); 
    }
#endif
}
void    Wifi::GetAplist()
{
    wifi_info.clear();
    QString  msg;
    int  m,n;
    msg = QString(cmd_system("iwlist wlan0 scan|egrep 'Cell|SSID|Version|Signal|Encryption'"));
    if(msg.length() <= 0)
        return;
    QString  encrypt_method="";
    QStringList  wifis = msg.split("Cell");
    num_network = wifis.size();
    QString     tough       ;
    QString     entry       ;
    QString     networkname ;

    for(int i=1;i < num_network;i++)
    {
        QStringList     msgs = wifis.at(i).split("\n");
        foreach(QString str,msgs)
        {
            if(str.indexOf(" Version 1") > 0)
            {
                QString  encrypt = str.split(":").at(1);
                int  n = encrypt.indexOf(" Version 1");
                encrypt_method += QString(encrypt.mid(0,n).toLocal8Bit().data())+"$";
            }
            if(str.indexOf("Signal level=") > 0)
            {
                m     = str.indexOf("Signal level=")+13;
                n     = str.length()-m-5;
                tough = str.mid(m,n);
            }
            if(str.indexOf("Encryption") > 0)
            {
                entry       = str.mid(str.indexOf("Encryption"));
            }
            if(str.indexOf("ESSID") > 0)
            {
                networkname = str.split(":").at(1).split("\"").at(1);
            }
        }
        if(networkname.length() <=0 )
            continue;

        Transcode(networkname);

        //qDebug()<<"name:"<<networkname;
        //qDebug()<<"signal tough:"<<tough<<"\nentry:"<<entry;
        //qDebug() <<"encrypt_method:"<<encrypt_method;
        if(networkname.compare(connectedname) == 0)
        {
            wifi_info.insert(0,networkname+"\t"+tough+"\t"+entry.split(":").at(1)+"\t"+encrypt_method);
            choice_signal = tough.toInt();
        }
        else
            wifi_info.push_back(networkname+"\t"+tough+"\t"+entry.split(":").at(1)+"\t"+encrypt_method);
        encrypt_method = "";
        networkname = "";
        tough = "";
        entry = "";
    }

    Stringsort();

}
void    Wifi::Transcode(QString  &name)
{
    if(name.length() <= 0)
        return;
    int  m,n;
#if 1
     m = name.indexOf("\\x");
     n = name.lastIndexOf("\\x");
    if(m >= 0)
    {
        QString code = name.mid(m,n-m+4);
        QStringList  hzlist = code.split("\\x");
        int v[hzlist.size()-1];
        char  buffer[hzlist.size()];
        memset(buffer,0,sizeof(buffer));
        for(int i=0;i<hzlist.size()-1;i++)
        {
            sscanf(hzlist.at(i+1).toLocal8Bit().data(),"%2X",&v[i]);
        } 
        for(int i=0;i<hzlist.size()-1;i++)
            buffer[i] = (char)v[i];
        qDebug()<<"***buffer:"<<tr(buffer);
        name.replace(m,n-m+4,tr(buffer));
    }
#endif

}
/************
 *
 *功能:用选择法对wifi热点列表进行排序，按照信号强度的由大到小来依次排列。
 *
 *
 *
 **/
void        Wifi::Stringsort()
{
    int i,j,k;
    QString  tmp;
    for(i=0;i < wifi_info.size()-1;i++)
    {
        k = i;
        for(j=i+1;j < wifi_info.size();j++)
        {
            if(wifi_info.at(j).split("\t").at(1).toInt() < wifi_info.at(k).split("\t").at(1).toInt())
                k = j; 
            tmp = wifi_info.at(k);
            wifi_info[k] = wifi_info.at(j);
            wifi_info[j] = tmp;
        }
    }
}
/************
 *
 *功能:每个信号强度范围匹配一个信号图标显示图标。
 *
 *
 *
 **/
QString    Wifi::CheckSignals(int  signalvalue)
{

    int   id = 1;
    //int   signalvalue = signal_tough.toInt(); 
#if 1
    if(signalvalue >= -60)
        id = 1;
    else if(signalvalue >= -70)
        id = 2;
    else if(signalvalue >= -80)
        id = 3;
    else if(signalvalue > -90)
        id = 4;
    else
        id = 5;
#endif
    QString   imagefile="";
    switch(id)
    {
        case    1:imagefile="./images/wifi_1.png";break;
        case    2:imagefile="./images/wifi_2.png";break;
        case    3:imagefile="./images/wifi_3.png";break;
        case    4:imagefile="./images/wifi_4.png";break;
        case    5:imagefile="./images/wifi_5.png";break;
    }
    return  imagefile;
}
/************
 *
 *功能:点击显示列表的某一项时响应操作，该操作中包括连接已经记录的wifi热点和未记录的wifi热点操作。
 *
 *
 *
 **/
void    Wifi::LinkWifiSlots()
{
    bool  isknow = false;
    QString msg="";
    QString   id="";
    //QProcess p;
    QString wifname = ui->listWidget->currentItem()->text();
    qDebug()<<"==============="<<wifname;
    if(connectedname.compare(wifname.split("\n").at(0))== 0)
    {
        wifname = wifname.split("\n").at(0);
        //qDebug()<<"*****"<<wifname;
        isknow = true;
        goto loop;
    }
    else
#if 1
    {
        //match the clicked wifiname  with the  content of wpa_supplicant.conf 
        QFile  file("./setssidpsk.sh");
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
            QMessageBox::critical(NULL, "提示", "无法创建文件");

        }
        else
        {

            QString  contents;
            contents="sed -n 's/.*ssid=\\\(\\S\\\)/\\1/p' wpa_supplicant.conf";
            QTextStream out(&file);
            out<<contents<<endl;
            out.flush();

            file.close();
        }
        msg = QString(cmd_system("./setssidpsk.sh"));
        if(msg.length()>0)
        {
            int n=0;
            QStringList   aps = msg.split("\n");
            foreach(QString str,aps)
            {
                qDebug()<<str;
                if(str.length() <= 0)
                    continue;

                wid="";
                if(wifname.compare(str.split("\"").at(1))==0)
                {
                    qDebug()<<"=====****"<<n<<"===++++++"<<str.split("\"").at(1);
                    isknow = true;
                    wid = QString::number(n);
                    break;
                }
                n++;
            }
        }

    }
loop:
    QDialog *temp_dialog = new QDialog(this);   //get()函数中的临时dialog，get函数结束时释放内存
    if(isknow)
    {
#if 1
        //QPushButton *connect_button = new QPushButton("连接",temp_dialog);
        QPushButton *disconnect_button = new QPushButton("断开",temp_dialog);
        //QPushButton *dissave = new QPushButton("删除",temp_dialog);
        QHBoxLayout *layout= new QHBoxLayout(temp_dialog);
        //layout->addWidget(connect_button);
        //layout->addWidget(disconnect_button);
        //layout->addWidget(dissave);

        //connect_button->setFocusPolicy(Qt::NoFocus);
        disconnect_button->setFocusPolicy(Qt::NoFocus);
        //dissave->setFocusPolicy(Qt::NoFocus);

        //connect(connect_button,SIGNAL(clicked()),this,SLOT(connectslots()));
        connect(disconnect_button,SIGNAL(clicked()),this,SLOT(disconnectslots()));
        //connect(dissave,SIGNAL(clicked()),this,SLOT(removeslots()));
        //connect(connect_button,SIGNAL(clicked()),temp_dialog,SLOT(close()));
        connect(disconnect_button,SIGNAL(clicked()),temp_dialog,SLOT(close()));
        //connect(dissave,SIGNAL(clicked()),temp_dialog,SLOT(close()));
        if(wifname.compare(connectedname)== 0)
        {
            layout->addWidget(disconnect_button);
            temp_dialog->exec();
            //connect_button->setEnabled(false);
        }
        else
        {
            connectslots();
            //QMessageBox  box;
            //box.setText("Waiting....");
            //connect(this,SIGNAL(closewaitmsg()),&box,SLOT(close));
            //box.show();
            QTimer::singleShot(10, this, SLOT(isKnowConnectDeal()));
            //layout->addWidget(connect_button);
            //disconnect_button->setEnabled(false);
        }
        //dissave->setEnabled(false);

        //temp_dialog->exec();
#endif
        if(wifi_info.size() <=0 )
            Updatewifilist();
        else
            Scan();

    }
    else
    {
        //QDialog *temp_dialog = new QDialog(this);   //get()函数中的临时dialog，get函数结束时释放内存
        QPalette palette;
        bool    is_encryption = false;

        foreach(QString str,wifi_info)
        {
            if(str.split("\t").at(0).compare(wifname) == 0)
            {
                //qDebug()<<str;
                if(str.split("\t").at(2).compare("on") == 0)
                    is_encryption = true;
                break;
            }
            qDebug()<<str;
        }
        if(!is_encryption)
        {
            QProcess::execute("iw dev  wlan0  connect "+wifname);
            if(wifi_info.size() <=0 )
            Updatewifilist();
            else
            Scan();
            return ;
        }



        QPixmap pixmap("./images/modify-bg.jpg");
        palette.setBrush(QPalette::Window, QBrush(pixmap));
        temp_dialog->setPalette(palette);
        temp_dialog->setAutoFillBackground(true);

        temp_dialog->setWindowTitle("WIFI名称:"+wifname);

        //下面是定义临时dialog中的控件，自己添加控件并排版

        QLineEdit *context = new QLineEdit(temp_dialog);
        //QComboBox   *combox = new   QComboBox(temp_dialog);
        QLabel  *wifiname = new  QLabel(temp_dialog);
        QLabel  *wifipasswd = new  QLabel("密码:",temp_dialog);
        QPushButton *ok_button = new QPushButton("OK",temp_dialog);
        QPushButton *cancel_button = new QPushButton("Cancel",temp_dialog);



        //combox->addItem("STATIC");
        //combox->addItem("DHCP");
        wifiname->setText("密码至少包含8个字符");

        ok_button->setFocusPolicy(Qt::NoFocus);
        cancel_button->setFocusPolicy(Qt::NoFocus);


        context->setEchoMode(QLineEdit::Password);
        ///排版
        QHBoxLayout *layout= new QHBoxLayout;
        layout->addWidget(wifipasswd);
        layout->addWidget(context);
        QHBoxLayout *layout1= new QHBoxLayout;
        layout1->addWidget(ok_button);
        layout1->addWidget(cancel_button);
        QVBoxLayout  *vlayout = new  QVBoxLayout;
        vlayout->addWidget(wifiname,Qt::AlignHCenter);
        //vlayout->addWidget(combox);
        vlayout->addLayout(layout);
        vlayout->addLayout(layout1);
        temp_dialog->setLayout(vlayout);

        connect(ok_button,SIGNAL(clicked()),temp_dialog,SLOT(accept()));
        connect(cancel_button,SIGNAL(clicked()),temp_dialog,SLOT(close()));
        key="";
LMP:
        if(is_encryption&&temp_dialog->exec() == QDialog::Accepted)
        {
            int  len = context->text().length();
            if( len < 8&& len >0 )
            {
                QMessageBox::critical(NULL, "提示", "密码至少包含8个字符!");
                goto LMP;
            }

            key = context->text();

            msg = QString(cmd_system("wpa_cli -iwlan0 add_network"));
            if(msg.length() <= 0)
                return;
            id = msg.split("\n").at(0);
            newid = id;
            QFile  file("./setssidpsk.sh");
            if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
                QMessageBox::critical(NULL, "提示", "无法创建文件");

            }
            else
            {

                QString  contents;
                if(is_encryption)
                {
                    contents="wpa_cli -iwlan0 set_network "+id+" ssid \'\""+wifname
                        +"\"\'\nwpa_cli -iwlan0 set_network "+id+" psk \'\""+context->text()
                        +"\"\'\nwpa_cli -iwlan0 enable_network "+id
                        +"\nwpa_cli -iwlan0 select_network "+id;
                }

                QTextStream out(&file);
                out<<contents<<endl;
                out.flush();

                file.close();
            }
            QProcess::execute("chmod 744 ./setssidpsk.sh");
            QProcess::execute("./setssidpsk.sh");
            QTimer::singleShot(1500, this, SLOT(ConnectDeal()));
        }


    if(wifi_info.size() <=0 )
            Updatewifilist();
    else
            Scan();
    }
#endif
}
void    Wifi::ConnectDeal()
{
    QString  msg;
  LOOP:         
            msg = QString(cmd_system("wpa_cli status"));
            if(msg.indexOf("Failed") >= 0)
            {
                emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
                QMessageBox box;
                box.setText("wifiserver start!");
                box.exec();
                goto LOOP;
            }
            QStringList  state = msg.split("\n");
            
            qDebug()<<"++++++++++++++"<<msg;
            foreach(QString str,state)
            {
            if(str.indexOf("wpa_state=") < 0 )
                continue;
#if 1
            if(QString("4WAY_HANDSHAKE").compare(str.split("=").at(1))==0)
            {
                QProcess::execute("wpa_cli -iwlan0 remove_network "+newid);
                QMessageBox::critical(NULL, "提示", "wifi密码错误!\n请重新输入..");       
                Updatewifilist();
                return;
            }
#endif

            if(QString("COMPLETED").compare(str.split("=").at(1))==0)
            {
                if(wifi_info.size() <=0 )
                    Updatewifilist();
                else
                    Scan();
                emit  signals_update();
#if 1 
                QFile  file_eth("./wpa_supplicant.conf");
                if (!file_eth.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)) {
                    QMessageBox::critical(NULL, "提示", "无法创建文件");

                }
                else
                {

                    QString  contents;
                        contents="\nnetwork={\nssid=\""+connectedname+"\"\npsk=\""+key+"\"\npriority=2\n}";
                    //else
                    //contents="ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0\nap_scan=1\nnetwork={\nssid=\""+wifname+"\"\nkey_mgmt=NONE\n}";

                    QTextStream out(&file_eth);
                    out<<contents<<endl;
                    out.flush();

                    file_eth.close();
                }
                    emit   siglogin(99); 
                    //QProcess::execute("dhclient wlan0");

#endif
            }
            else if(QString("DISCONNECTED").compare(str.split("=").at(1))==0)
            {
                    QMessageBox::critical(NULL, "提示", "无法连接");
                    goto LOOP;
            }
            else
                goto LOOP;
            }
 }
void    Wifi::isKnowConnectDeal()
{
    QString  msg;
      LOP:         
            msg = QString(cmd_system("wpa_cli status"));
            if(msg.indexOf("Failed") >= 0)
            {
                emit  signals_connect("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
                QMessageBox box;
                box.setText("wifiserver start!");
                box.exec();
                goto LOP;
            }
            QStringList  state = msg.split("\n");
            
            qDebug()<<"++++++++++++++"<<msg;
            foreach(QString str,state)
            {
            if(str.indexOf("wpa_state=") < 0 )
                continue;

            if(QString("COMPLETED").compare(str.split("=").at(1))==0)
            {
                //emit   closewaitmsg();
                if(wifi_info.size() <=0 )
                    Updatewifilist();
                else
                    Scan();
                emit  signals_update();
                emit   siglogin(99); 

            }
            else
                goto LOP;
            }
 } 
