/************
 *
 *功能:Main  GUI  Realize
 *
 *
 *
 **/
#include "mainview.h"
#include "ui_mainview.h"
/************
 *
 *功能:主界面
 * 初始实例化所有的子窗口对象，设置字符编码，检测读取版本号，
 *更改或创建wifi管理工具wpa_supplicant的配置文件wpa_supplicant.conf，启动wifi后台管理服务，
 *设置主界面样式和初始化布局，多个子窗口间通信的信号和槽的初始化连接操作。
 *
 *
 **/
mainview::mainview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainview)
{
    /****set Shortcut Keys*****/
    QShortcut  *m_ALT_down_Accel= new QShortcut(QKeySequence(tr("Ctrl+Shift+Alt+Y")), this);

    this->login     = NULL;
    this->netconfig = NULL;
    this->wifi      = NULL;
    ui->setupUi(this);
    logo_x=QApplication::desktop()->width()/2-100;
    logo_y=50;
    open = false;

    //this->setStyleSheet("background-image:url(:/images/terminal-bg.jpg);");
    /*********Set  Unicode *********/
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    spicy_thread_id.clear();

    QFile file_version( "./version.txt" );
    if ( file_version.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_version.readLine();
        QString mg = line;
        file_version.close();
        QStringList md = mg.split("\n");
        version = "ver "+md.at(0);
    }
    else
    {
        version = "ver 3.0.1";
    }

        QFile  file_eth("./wpa_supplicant.conf");
            if (!file_eth.open(QIODevice::ReadOnly)&&
                file_eth.open(QIODevice::WriteOnly|QIODevice::Text)) {
                QTextStream out(&file_eth);
                out<<"ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0\nap_scan=1\n"<<endl;
                out.flush();

                file_eth.close();
            }
            else
                file_eth.close();

    //start wpa_supplicant  service
    QProcess::execute("ifconfig wlan0 up");
    startthread("wpa_supplicant -Dwext -iwlan0 -c ./wpa_supplicant.conf "); 
    //startthread("dhclient wlan0");
    /*********Set Button Style************/
    ui->pushButton->setStyleSheet("background-color:transparent");
    //ui->pushButton->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton->setText("");
    ui->pushButton->setFlat(true);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_1->setStyleSheet("QPushButton{text-align:left;}");
    ui->pushButton_1->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton_1->setStyleSheet("background-color:transparent");
    ui->pushButton_1->setText("");
    ui->pushButton_1->setFlat(true);
    ui->pushButton_1->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_2->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton_2->setStyleSheet("background-color:transparent");
    ui->pushButton_2->setText("");
    ui->pushButton_2->setFlat(true);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_3->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton_3->setStyleSheet("background-color:transparent");
    ui->pushButton_3->setText("");
    ui->pushButton_3->setFlat(true);
    ui->pushButton_3->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_4->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton_4->setStyleSheet("background-color:transparent");
    ui->pushButton_4->setText("");
    ui->pushButton_4->setFlat(true);
    ui->pushButton_4->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_5->setStyleSheet("border: 0px");//消除边框，取消点击效果
    ui->pushButton_5->setStyleSheet("background-color:transparent");
    ui->pushButton_5->setText("");
    ui->pushButton_5->setFlat(true);
    ui->pushButton_5->setFocusPolicy(Qt::NoFocus);
    //ui->pushButton_5->setVisible(false);
#if  1
    this->login = new  Login();
    this->netconfig = new NetworkConfig();
    this->wifi = new  Wifi();
    //this->termigrate = new  Termigrate();

    http = new  SocketService();
    changepasswd= new  ChangePasswd();
    hostlist = new  CreateVmlist();
    
    ui->widgetstack->addWidget(login);
    ui->widgetstack->addWidget(netconfig);
    ui->widgetstack->addWidget(changepasswd);
    ui->widgetstack->addWidget(wifi);
    ui->widgetstack->addWidget(hostlist);
    ui->widgetstack->resize(0,0);


    ui->widgetlist->setMovement(QListView::Static);
    ui->widgetlist->setFocusPolicy(Qt::NoFocus);
    ui->widgetlist->addItem("用户登录");
    ui->widgetlist->addItem("修改配置");
    ui->widgetlist->addItem("修改密码");
    ui->widgetlist->setSpacing(4);
    ui->widgetlist->setMouseTracking(true);
    ui->widgetlist->item(0)->setSizeHint(QSize(30,30));
    ui->widgetlist->item(1)->setSizeHint(QSize(30,30));
    ui->widgetlist->item(2)->setSizeHint(QSize(30,30));

    


    connect(this,SIGNAL(loadoperator(int)),this,SLOT(changeslots(int)));
    connect(ui->widgetlist,SIGNAL(currentRowChanged(int)),this,SLOT(deal_widgets(int)));
    connect(ui->widgetlist,SIGNAL(itemEntered(QListWidgetItem *)),this,SLOT(changeColor(QListWidgetItem *)));
    for(int i=0;i<ui->widgetlist->count();i++)
    {
        ui->widgetlist->item(i)->setBackground(QBrush(QColor(255,255,255)));
        ui->widgetlist->item(i)->setSelected(false);
    }


    /********Set  Function Key As following:reflush, wifi, help, shutdown, reboot**********/
    //get vmhostlist by  click  the  reflush  function
    setMouseTracking(true);
    ui->pushButton->setMouseTracking(true);
    ui->pushButton_1->setMouseTracking(true);
    ui->pushButton_2->setMouseTracking(true);
    ui->pushButton_3->setMouseTracking(true);
    ui->pushButton_4->setMouseTracking(true);
    ui->pushButton_5->setMouseTracking(true);
    connect(ui->pushButton,SIGNAL(clicked()),this->http,SLOT(gethostlist_slots()));
    connect(this->http,SIGNAL(signals_vmlist(QString)),this,SLOT(reflushlisttable(QString)));



    connect(ui->pushButton_1,SIGNAL(clicked()),this,SLOT(moreoperator()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(wifioperator()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(helpoperator()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(closecomputer()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(rebootcomputer()));

    /*** this function is logining  when the login window whose login_signals had been sended by click the OK_button.**/
    connect(this->login,SIGNAL(signals_loginmsg(QString)),this->http,SLOT(login_slots(QString)));//send login information by httpsocket
    //connect(this->login,SIGNAL(signals_loginmsg(QString)),this->hostlist,SLOT(getuserinfo(QString)));//send login information 
    connect(this->http,SIGNAL(signals_loginview(int)),this,SLOT(emit_widgets(int)));
    connect(m_ALT_down_Accel, SIGNAL(activated()), this, SLOT(docreen()));

    /*** this is connect the signals with the networkconfigview and modify the password windows **/
    connect(this->netconfig,SIGNAL(signals_cdms(QString)),this->http,SLOT(registertocdms(QString)));
    connect(this->netconfig,SIGNAL(signals_modifycdms(QString)),this->http,SLOT(modifycdmsSlots(QString)));
    connect(this->changepasswd,SIGNAL(signals_modify(QString)),this->http,SLOT(updateUserinfo(QString)));
    connect(this->changepasswd,SIGNAL(docancel()),this->http,SLOT(gethostlist_slots()));

    /*** when click the OK_button that is the networkconfigview button,the thread will be create to runing shell command**/
    connect(this->netconfig,SIGNAL(signals_startthread(QString)),this,SLOT(startthread(QString)));


    /***it is registertocdms at first when the  programmer  running***/
    connect(this->http,SIGNAL(signals_register()),this->netconfig,SLOT(SendRegisterData()));
    /***if connect error,switch to netconfig view***/
    connect(this->http,SIGNAL(signals_connectError(int)),this,SLOT(emit_widgets(int)));
    /*** it's used to  start  the  spicy  protocol thread at the  SocketService class***/
    connect(this->http,SIGNAL(signals_startspicy(QString)),this,SLOT(startthread(QString)));
    //connect(this->http,SIGNAL(signals_tohostlistcheck(QString)),this->hostlist,SLOT(checkuserinfo(QString)));//check the userinfo is the same as  already logininfo

    /****there is  the  signals which serves for  vmhostlist contained  three vmhost most****/
    connect(this->hostlist,SIGNAL(signals_vm(QString)),this->http,SLOT(getconnectinfo_slots(QString)));
    connect(this->hostlist,SIGNAL(signals_connect(QString)),this->http,SLOT(isableconnectslots(QString)));
    connect(this->hostlist,SIGNAL(signals_start(QString)),this->http,SLOT(startvmslots(QString)));
    connect(this->hostlist,SIGNAL(signals_down(QString)),this->http,SLOT(shutdownvmslots(QString)));
    connect(this->hostlist,SIGNAL(signals_reboot(QString)),this->http,SLOT(rebootvmslots(QString)));

    /*******it's connected the  windows  with  the   choose  table on the  mainview  windows*******/
    connect(this,SIGNAL(signals_widgets(int)),ui->widgetstack,SLOT(setCurrentIndex(int)));
    /****it's used  to  execute  the  shell command int  the  function of  Wifi class****/
    connect(this->wifi,SIGNAL(signals_connect(QString)),this,SLOT(startthread(QString)));
    /****it's used to update  connected  wifiname on  the  mainview  background ********/
    connect(this->wifi,SIGNAL(signals_update()),this,SLOT(repaint()));
    connect(this->wifi,SIGNAL(siglogin(int)),this,SLOT(deal_widgets(int)));

    /******it's used  to  migrate  vitural machine ,that send oldip to tcp server for close it********/
    //connect(this->http,SIGNAL(signals_startmigrate(QString)),this->termigrate,SLOT(WriteIpToShm(QString)));
#endif
}
/************
 *
 *功能:change  the  mouse  style  when  button press
 *
 *
 *
 **/
#if 0
#define   MOUSECHECK(a,b) { \
    QPoint   btnMinPos = b->pos();\
    QPoint   btnMaxPos = b->pos();\
    btnMaxPos.setX(b->pos().x()+b->width());\
    btnMaxPos.setY(b->pos().y()+b->height());\
    if((a.x() >= btnMinPos.x() && a.x() <= btnMaxPos.x()) &&\
            (a.y() >= btnMinPos.y()&& a.y() <= btnMaxPos.y()))\
    {\
        setCursor(Qt::PointingHandCursor);\
    }\
    else\
    {\
        setCursor(Qt::ArrowCursor);\
    }}
#endif
/************
 *
 *功能:Change the  item background color 。
 *
 *
 *
 **/

void    mainview::changeColor(QListWidgetItem *item)
{
    for(int i=0;i<ui->widgetlist->count();i++)
    {
        ui->widgetlist->item(i)->setBackground(QBrush(QColor(255,255,255)));
        ui->widgetlist->item(i)->setSelected(false);
    }
    item->setBackground(QBrush(QColor(173,216,230,70)));
}
/************
 *
 *功能:Change the  Choice  items for  login state。
 *
 *参数：current state,0,sigin in;1,logined
 *
 **/

void    mainview::changeslots(int   currentindex)
{
       switch(currentindex)
       {
           case 0:
            ui->widgetlist->item(0)->setText("用户登录");
           break;
           case 1:
            ui->widgetlist->item(0)->setText("退出登录");
           break;
        }
}
/************
 *
 *功能:检测更改鼠标的样式设置。
 *
 *参数：p, 鼠标所在的位置点 btn,检测更改鼠标样式的按钮
 *
 **/

bool    mainview::enterBtn(QPoint  p,QWidget  *btn)
{
    QPoint   btnMinPos = btn->pos();
    QPoint   btnMaxPos = btn->pos();

    btnMaxPos.setX(btn->pos().x()+btn->width());
    btnMaxPos.setY(btn->pos().y()+btn->height());
    if((p.x() >= btnMinPos.x() && p.x() <= btnMaxPos.x()) &&
            (p.y() >= btnMinPos.y()&& p.y() <= btnMaxPos.y()))
    {

        //setCursor(QCursor(QPixmap("./images/pointhandler.png")));
        setCursor(Qt::PointingHandCursor);
        return  true;

    }
    else
    {
        setCursor(Qt::ArrowCursor);
        return  false;
    }


}
/*void    mainview::keyPressEvent(QKeyEvent  *event)
{
    switch(event->key())
    {
        case    Qt::Key_Escape:
        break;
        //default:
        //    QDialog::keyPressEvent(event);
    }
    keyPressEvent(event);
}*/
void    mainview::mousePressEvent(QMouseEvent  *e)
{
    open = false;
    ui->widgetlist->resize(0,0);
    ui->widgetlist->setCurrentRow(-1);
    for(int i=0;i<ui->widgetlist->count();i++)
    {
        ui->widgetlist->item(i)->setBackground(QBrush(QColor(255,255,255)));
        ui->widgetlist->item(i)->setSelected(false);
    }
}
void    mainview::mouseMoveEvent(QMouseEvent  *e)
{

    if(!enterBtn(e->pos(),ui->pushButton)&& 
            !enterBtn(e->pos(),ui->pushButton_1)&&
            !enterBtn(e->pos(),ui->pushButton_2)&&
            !enterBtn(e->pos(),ui->pushButton_3)&& 
            !enterBtn(e->pos(),ui->pushButton_4)&& 
            !enterBtn(e->pos(),ui->pushButton_5))
        e->accept();
}
void    mainview::deal_widgets(int  id)
{
    switch(id)
    {
        case 0:
                if(http->login_state == true)
                {
                    http->ResetUserInfo();
                    KillSpicyThread();
                }
                emit_widgets(0);
        break;
        case 1:
                emit_widgets(1);
        break;
        case 2:
                emit_widgets(2);
        break;
        case 99:
                if(http->login_state == false)
                {
                    emit_widgets(0);
                }
                else
                {
                    emit_widgets(4);
                }
        break;

    }
    for(int i=0;i<ui->widgetlist->count();i++)
    {
        ui->widgetlist->item(i)->setBackground(QBrush(QColor(255,255,255)));
        ui->widgetlist->item(i)->setSelected(false);
    }
    ui->widgetlist->resize(0,0);
    ui->widgetlist->setCurrentRow(-1);
    open = false;
    //repaint();

}
/************
 *
 *功能:设置窗体大小。
 *
 *id:在窗口栈中存放的窗口的编号。
 *
 **/

void    mainview::emit_widgets(int  id)
{
    //repaint();
    switch(id)
    {
        case 0://login viewsize
            ui->widgetstack->resize(400,300);
            break;
        case 1://network config viewsize
            {netconfig->FillContent();
            ui->widgetstack->resize(0,0);
            break;}
        case 2://changepasswd viewsize
            if(http->login_state == false)
            {
                QMessageBox  box;
                box.setWindowTitle("提示");
                box.setText("Please to Login.");
                box.exec();
                emit_widgets(0);
                return;
            }
            ui->widgetstack->resize(400,300);
            break;
        case 3:
            ui->widgetstack->resize(314,567);
            break;
        case 4:
            {
                //vmlist display  order  in  the  windows
                if(this->hostlist->nums_vm <= 18)
                        ui->widgetstack->resize(this->width()*9/10,510);
                else
                {
                        ui->widgetstack->resize(this->width()*9/10,700);
                }
            break;
            }
        default:
            ui->widgetstack->resize(0,0);
            break;


    }
    emit   signals_widgets(id);
    repaint();
}
/************
 *
 *功能:send the signal to check  cdms
 *
 *
 *
 **/

void    mainview::emit_checkcdms_signal()
{
    qDebug() << "check cdms";
    emit  this->signals_checkcdms(this->cdmpip);
}
/************
 *
 *功能:make the  screen show  normal
 *
 *
 *
 **/
void    mainview::docreen()
{
    this->showNormal();
    repaint();
}



/************
 *
 *功能:check cdms  statu  every    1s 
 *
 *
 *
 **/
void    mainview::CheckNetworkState()
{

    QFile file( "/sys/class/net/"+netconfig->devicename+"/operstate" );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file.readLine();
        QString mg = line;
        file.close();
        qDebug() << mg;
        if(mg.compare("down\n") == 0)
        {
            if(netconfig->devicename.compare("wlan0")==0)
                QProcess::execute("ifconfig wlan0 up");
            else 
                startthread("/etc/init.d/networking  restart");
        }
    }

    QFile file_s( "./server.txt" );
    if ( file_s.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file_s.readLine();
        QString mg = line;
        file_s.close();
        QStringList md = mg.split("\n");
        http->CheckService(md.at(0));    
    }
    else
    {
        emit_widgets(1);
    }
    
    


}
/************
 *
 *功能:创建一个独立线程来执行命令。
 *
 *
 *
 **/
void    mainview::startthread(const QString &msg)
{
    if(msg.length() > 0){
        SpicyThread  *th = new  SpicyThread();
        th->setcmd(msg);
        th->start();
        qDebug()<<"cmd: "<<msg;//<<":id"<<QThread::currentThreadId();
        //int  thid = (int)QThread::currentThreadId();
        //qDebug()<<"cmd: "<<msg<<":id"<<thid;
        if(msg.indexOf("/usr/bin/spicy") >= 0)
        {
            //qDebug()<<"cmd: "<<msg<<":id"<<QString::number(thid);
            //spicy_thread_id = thid;
            int  len = msg.indexOf("-h"),len1=msg.indexOf("-f");
            QString  content = msg.mid(len+3,len1-len);
            qDebug()<<"content:"<<content;
            spicy_thread_id.push_back(content);
        }
        connect(th,SIGNAL(destroyed()),th,SLOT(quit()));

    }

}
/************
 *
 *功能:
 *
 **/
void       mainview::KillSpicyThread(  )
{
        
        QString str ;
        for(int i=0;i < spicy_thread_id.count();i++)
        {
        str= QString("ps aux|grep \"%1\"|grep -v grep|awk '{print $2}'|xargs kill").arg(spicy_thread_id.at(i));
        qDebug() << str;
        system(str.toLocal8Bit());
        str = "";
        }
        spicy_thread_id.clear();
}
/************
 *
 *功能:
 *
 *
 *
 **/
void    mainview::moreoperator()
{
    if(open == false)
    {
        ui->widgetlist->resize(100,120);
        if(http->login_state == false)
        {
            emit  loadoperator(0);
        }
        else
        {
            emit  loadoperator(1);
        }
        open = true;
    }else
    {
        ui->widgetlist->resize(0,0);
        open = false;
    }


}
void    mainview::getcdmsslots(const QString  &mg)
{
    if(mg.length() > 0)
        this->cdmpip = mg;
}
/************
 *
 *功能:刷新虚拟机列表
 *
 *
 *
 **/
void    mainview::reflushlisttable(const QString &vmhlist)
{
    if(vmhlist.length() == 0)
    {
        QMessageBox::critical(NULL, "提示", "no virtual machines.");
    }
    qDebug() <<"=========================";
    qDebug() << vmhlist;
    ui->widgetstack->resize(this->width()*9/10,100);
    if(this->hostlist->vmdata.compare(vmhlist) != 0)
        this->hostlist->CreateVM(vmhlist);
    emit_widgets(4);


}
/************
 *
 *功能:Wifi操作界面
 *
 *
 *
 **/
void    mainview::wifioperator()
{
    //ui->widgetlist->resize(0,0);
    ui->widgetlist->setCurrentRow(-1);
    for(int i=0;i<ui->widgetlist->count();i++)
    {
        ui->widgetlist->item(i)->setBackground(QBrush(QColor(255,255,255)));
        ui->widgetlist->item(i)->setSelected(false);
    }
    wifi->updatelist();
    emit_widgets(3);
}
/************
 *
 *功能:帮助操作界面
 *
 *
 *
 **/
void    mainview::helpoperator()
{
    QDialog  *dialog = new  QDialog();
    dialog->setWindowTitle("帮助");
    dialog->resize(800,600);
    //QPushButton  *button_ok  =  new  QPushButton("OK",dialog);
    QWebView   *view = new  QWebView(dialog);
    //view->setGeometry(0,0,800,550);
    //button_ok->setGeometry(550,750,50,50);
    //view->load(QUrl("http://10.3.3.59/index.php?r=help/index"));
    view->load(QUrl("./help.html"));
    //connect(button_ok,SIGNAL(clicked()),dialog,SLOT(close()));
    dialog->exec();
}
/************
 *
 *功能:关机操作
 *
 *
 *
 **/
void    mainview::closecomputer()
{

    QDialog *tmp = new  QDialog();
    tmp->setWindowIcon(QIcon("./images/logo.png"));
    tmp->setWindowTitle("关机提示");
    QVBoxLayout  *vlay = new  QVBoxLayout();
    QHBoxLayout  *hlay = new  QHBoxLayout();
    QLabel       *afforminfo = new  QLabel("关机?",tmp);
    QPushButton  *button_ok  =  new  QPushButton("OK",tmp);
    QPushButton  *button_cancel  =  new  QPushButton("Quit",tmp);
    hlay->addWidget(button_ok);
    hlay->addWidget(button_cancel);

    vlay->addWidget(afforminfo);
    vlay->addLayout(hlay);
    tmp->setLayout(vlay);

    connect(button_ok,SIGNAL(clicked()),tmp,SLOT(accept()));
    connect(button_cancel,SIGNAL(clicked()),tmp,SLOT(close()));

    if(tmp->exec() == QDialog::Accepted)
    {
        QProcess::execute("shutdown -h now");
    }

}
/************
 *
 *功能:重新启动操作
 *
 *
 *
 **/
void    mainview::rebootcomputer()
{
    QDialog *tmp = new  QDialog();
    tmp->setWindowIcon(QIcon("./images/logo.png"));
    tmp->setWindowTitle("重启提示");
    QVBoxLayout  *vlay = new  QVBoxLayout();
    QHBoxLayout  *hlay = new  QHBoxLayout();
    QLabel       *afforminfo = new  QLabel("重启?",tmp);
    QPushButton  *button_ok  =  new  QPushButton("OK",tmp);
    QPushButton  *button_cancel  =  new  QPushButton("Quit",tmp);
    hlay->addWidget(button_ok);
    hlay->addWidget(button_cancel);

    vlay->addWidget(afforminfo);
    vlay->addLayout(hlay);
    tmp->setLayout(vlay);

    connect(button_ok,SIGNAL(clicked()),tmp,SLOT(accept()));
    connect(button_cancel,SIGNAL(clicked()),tmp,SLOT(close()));

    if(tmp->exec() == QDialog::Accepted)
    {
        QProcess::execute("shutdown -r now");
    }
}
/************
 *
 *功能:绘制背景
 *
 *
 *
 **/
void    mainview::paintEvent(QPaintEvent *)
{

    switch(ui->widgetstack->currentIndex())
    {
        case    0:
        case    2:
        logo_y = this->height()/2 - 200;
        logo_x = this->width()/2 - 100;
        break;
        default:
        logo_y = 50;
        logo_x=this->width()/2-100;
        break;
    }
    switch(ui->widgetstack->currentIndex())
    {
        case  0:
        ui->widgetstack->move(logo_x-80,logo_y+50);
        break;
        case  1://networkconfig
        {
        ui->widgetstack->move(logo_x-150,height()/2-325);
        if(height()/2 > 325)
        ui->widgetstack->resize(461,650);
        break;}
        case  3://wifi
        ui->widgetstack->move(logo_x-50,height()/2-283);
        break;
        case  4://vmlist
        {
                if(this->hostlist->nums_vm <= 12)
                    //vmlist display  order  in  the  windows
                {
                    ui->widgetstack->move(width()/20,this->height()/2-100);
                }
                else if(this->hostlist->nums_vm > 12 && this->hostlist->nums_vm <= 18 )
                {
                    ui->widgetstack->move(width()/20,this->height()/2-200);
                }
                else
                {
                    ui->widgetstack->move(width()/20,this->height()/2-300);
                }
        break;
        }
        default:
        ui->widgetstack->move(logo_x-100,height()/2-80);
        break;

    }
    
    
    
    
    QFont font("Arial",10);
    QPainter painter(this);

    painter.drawPixmap(rect(),QPixmap("./images/terminal-bg.png"));
    painter.drawPixmap(QPoint(logo_x,logo_y),QPixmap("./images/terminal-logo.png"));

    QPixmap  *map = new  QPixmap("./images/information.png");
    painter.drawPixmap(QPoint(this->width()-200,this->height()-50),map->copy(0,map->height()/2,map->width(),
                map->height()/14));//down
    painter.drawPixmap(QPoint(this->width()-100,this->height()-50),map->copy(0,map->height()/2+map->height()/7,//reset
                map->width(),map->height()/14));

    painter.drawPixmap(QPoint(this->width()-70,10),map->copy(0,map->height()/14,map->width(),map->height()/14));//?help
    painter.drawPixmap(QPoint(this->width()-230,10),map->copy(0,map->height()/7+map->height()/14,//reflush
                map->width(),map->height()/14));
    painter.drawPixmap(QPoint(this->width()-270,10),map->copy(0,2*(map->height()/7)+map->height()/14,map->width(),
                                                                  map->height()/14));//more operator
    delete map;

    if(wifi->connectedname.length()<=0)
    {
    map = new  QPixmap("./images/wifi.png");
    painter.drawPixmap(QPoint(this->width()-120,5),map->copy(0,map->height()/2,map->width(),map->height()/2));
    }
    else
    {
    map = new  QPixmap("./images/wifi_img.png"/*wifi->CheckSignals(wifi->choice_signal)*/);
    painter.drawPixmap(QPoint(this->width()-120,5),map->copy(0,10*map->height()/14,map->width(),map->height()/14));
    }

    delete map;

    QFont font1("Arial",10,QFont::Normal,true);
    painter.setFont(font1);//????
    painter.setPen(QColor(Qt::white));
    painter.drawText(155+logo_x,15+logo_y,version);
    painter.setFont(font);//????
    painter.drawText(20,this->height()-30,"Copyright (C)2001-2015 Yovole Corporation, All Rights Reserved");

    font.setPixelSize(13);
    painter.setFont(font);//????
    painter.drawText(this->width()-175,this->height()-30,"关机");
    painter.drawText(this->width()-75,this->height()-30,"重启");
    if(http->login_state == true)
    {
        /*************the  username length is not equal to the drawtext length,there have question.*******/
        //if(hostlist->nums_vm > 0)
        painter.drawText(this->width()-350-(login->GetUserName().length()),30,login->GetUserName());
        //else
        //painter.drawText(this->width()-350,30,"更多操作");
        if(open) 
            emit  loadoperator(1);
    }
    else
    {
        if(open) 
            emit  loadoperator(0);
        painter.drawText(this->width()-350,30,"更多操作");
    }
   
    
    painter.drawText(this->width()-200,30,"刷新列表");
    painter.drawText(this->width()-40,30,"帮助");
    font.setPixelSize(8);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(this->width()-120,35,wifi->connectedname);

    ui->widgetlist->move(this->width()-350,40);
    
    ui->pushButton->move(this->width()-240,10);
    ui->pushButton_1->move(this->width()-350,10);
    ui->pushButton_2->move(this->width()-120,10);
    ui->pushButton_3->move(this->width()-70,10);
    ui->pushButton_4->move(this->width()-200,this->height()-50);
    ui->pushButton_5->move(this->width()-100,this->height()-50);


}
mainview::~mainview()
{
    delete ui;
    delete this->login;
    delete this->netconfig;
    delete this->wifi;
    delete this->http;
    delete this->changepasswd;
    delete this->hostlist;
    //delete this->termigrate;
}
