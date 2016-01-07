#include "createvm.h"
/************
 *
 *功能:虚拟机列表类
 *实现虚拟机机点的创建排列和布局
 *
 *
 **/

    CreateVmlist::CreateVmlist(QWidget  *parent)
:QWidget(parent)
{
#if  1
    Vmgs.clear();
    vmsendmg.clear();
    ids.clear();
    vmslist.clear();
#endif
    user = "";
    newuser = "";
    vmdata = "";
    idvm = -1;
    lskey = 0;

    this->setWindowIcon(QIcon("./images/logo.png"));

    QPalette palette;
    palette.setColor(QPalette::Background,QColor(0xff,0xff,0xff,0x00));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    btngp = new  QButtonGroup(this);
    list  = new  QListWidget(this);
    list->resize(0,0);
    list->addItem("连接");
    list->addItem("强制关机");
    list->addItem("强制重启");
    list->setSpacing(2);
    list->setFocusPolicy(Qt::NoFocus);
    list->setMouseTracking(true);
                  
    connect(btngp,SIGNAL(buttonClicked(int)),this,SLOT(deal_choicevmslots(int)));
    connect(list,SIGNAL(currentRowChanged(int)),this,SLOT(deal_slots(int)));
    connect(list,SIGNAL(itemEntered(QListWidgetItem *)),this,SLOT(changeColor(QListWidgetItem *)));

}
/************
 *
 *功能:
 *
 *
 *
 **/
void    CreateVmlist::getuserinfo(const QString &userpasswd)
{
    user = userpasswd.split("\n").at(0);
    qDebug()<<"hostlist receive user "<<user;
}
void    CreateVmlist::checkuserinfo(const QString &userpasswd)
{
    newuser = userpasswd.split("&").at(0).split("=").at(1);
    qDebug()<<"hostlist receive newuser "<<newuser;
}
void    CreateVmlist::changeColor(QListWidgetItem *item)
{
    for(int i=0;i<list->count();i++)
        list->item(i)->setBackground(QBrush(QColor(255,255,255)));

    item->setBackground(QBrush(QColor(0,245,255,50)));
}/************
 *
 *功能:
 *
 *
 *
 **/

void    CreateVmlist::deal_slots(int  id)
{
    list->resize(0,0);
    switch(id)
    {
        case    0:
        emit_connect_signals();
        qDebug()<<"连接";
        break;
        case    1:
        emit_shutdown_signals();
        qDebug()<<"强制关机";
        break;
        case    2:
        emit_reboot_signals();
        qDebug()<<"强制重启";
        break;
    }
    list->setCurrentRow(-1);
    for(int i=0;i<list->count();i++)
    {
        list->item(i)->setBackground(QBrush(QColor(255,255,255)));
        list->item(i)->setSelected(false);
    }

}/************
 *
 *功能:set button  background
 *
 **/
void    CreateVmlist::SetStatusVm(QWidget  *btn,const QString  &status)
{
    QPalette palette;
    qDebug()<<"status:"<<status;
    if(QString("0").compare(status) == 0)
    {
        palette.setColor(QPalette::Button,QColor(200,200,183,225));
    }
    else
    {
        palette.setColor(QPalette::Button,QColor(10,250,10,225));
    }
    btn->setPalette(palette);
    btn->setAutoFillBackground(true);


}/************
 *
 *功能:
 *
 *
 *
 **/

void        CreateVmlist::setButtonBackImage(QPushButton *button,QString image,int sizeW, int sizeH)
{
    //163,163为原始分辨率，这里稍做了调整。
    QPixmap pixmap(image);
    QPixmap fitpixmap=pixmap.scaled(163,163).scaled(sizeW, sizeH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    button->setIcon(QIcon(fitpixmap));
    button->setIconSize(QSize(sizeW,sizeH));
    button->setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
    button->setStyleSheet("border: 0px");//消除边框，取消点击效果
}
/************
 *
 *功能:
 *
 *
 *
 **/

void        CreateVmlist::VmFactory()
{      
    vmslist.clear();
    labels.clear();
    QPalette p;
    p.setColor(QPalette::WindowText,Qt::white);
    int  m = 0;
    for(int i=0;i < nums_vm;i++,m++)
    {
        //QPushButton  *btton = new  QPushButton(QIcon("./images/a_detailed.png"),Vmgs[i].at(0)+"\nv"+Vmgs[i].at(1)/*+"\n"+Vmgs[i].at(2)*/,this);
        QPushButton  *btton = new  QPushButton(this);
        QLabel      *label  = new   QLabel(this);
        label->setStyleSheet("font-size:14px");
        btton->setFlat(true);
        btton->setFocusPolicy(Qt::NoFocus);
        setButtonBackImage(btton,"./images/a_detailed.png",60,60);
        if((Vmgs[i].at(0)).length() > 13)
        {
            QString  name = Vmgs[i].at(0);
            QString  name_0 = name.mid(0,13),name_1=name.mid(13,name.length()-13),name_2="";
            //if(name_1.length()>8)
            //    {name_2 = name_1.mid(8,name_1.length()-8);
            //    name_1 = name_1.mid(0,8);
            //    }
            //if(name_2.length() > 0)
            //label->setText(name_0+"\n  "+name_1+"\n    "+name_2+"\n\nVDI_"+Vmgs[i].at(1));
            //else
            label->setText(name_0+"\n "+name_1+"\n\nVDI_"+Vmgs[i].at(1));
        }
        else
        label->setText(Vmgs[i].at(0)+"\n\nVDI_"+Vmgs[i].at(1));
        label->setPalette(p);

        btngp->addButton(btton,i);
        vmslist.push_back(btton);
        labels.push_back(label);
        if(i%6 == 0)
        {
            m = 0;
        }
        /***********more than  6  ************/
        if(nums_vm >= 6)
        {
        btton->setGeometry(x()+130*m+lskey,y()+170*(i/6),60,60);
        label->setGeometry(x()+130*m+lskey,y()+60+170*(i/6),120,60);
        }
        else
        {
        btton->setGeometry(x()+130*i+lskey,y(),60,60);
        label->setGeometry(x()+130*i+lskey,y()+60,120,60);
        }
    }
    repaint();
}
/***********************************/
void       CreateVmlist::Reset( )
{
}
/************
 *
 *功能:
 *
 *
 *
 **/

void       CreateVmlist::CreateVM(const QString &vminfo)
{
    list->resize(0,0);
    nums_vm = 1;
    lskey = 0;

    QStringList  vms;
    vms.clear();
    vms = vminfo.split("\n");
    if(!vms.isEmpty())
        nums_vm = vms.size();
    if(vms.at(nums_vm-1).length() <=0 )
    {
        nums_vm--;
    }
    qDebug()<<"======num:"<<nums_vm<<"=====vminfo:\n"<<vminfo;
#if 1
    QString  val="";
    QStringList qsl;
    qsl.clear();
    Vmgs.clear();
    ids.clear();
    vmsendmg.clear();
    for(int i=0;i < nums_vm;i++)
    {
        val = vms.at(i);
        qsl = val.split("\t");
        vmsendmg.push_back(val);
        Vmgs.push_back(qsl);
        ids.push_back(qsl.at(1)); 
        qsl.clear();
        val="";
    }
    if(nums_vm <=4)
        lskey = width_/2-100*(nums_vm/2);
    else
        lskey = width_/2-300;
    if(lskey <=0 )
        lskey = width_/2;



    if(vmdata.length() != vminfo.length())
    {
        for(int i=0;i < vmslist.size();i++)
        {
            delete labels[i];
            btngp->removeButton(vmslist[i]);
            delete vmslist[i];
        }
        VmFactory();
    }

#endif
    vmdata = vminfo;
}
/************
 *
 *功能:
 *
 *
 **/
void    CreateVmlist::deal_choicevmslots(int id)
{
    if(nums_vm-1 >= id)
        emit  this->signals_vm(vmsendmg[id]);

}
/************
 *
 *功能:
 *
 *
 *
 **/

void    CreateVmlist::deal_operatorslots(int id)
{
    list->move(vmslist[id]->x(),vmslist[id]->y()+120);
    //if(nums_vm > 6)
    //list->resize(80,30);
    //else
    list->resize(80,70);
    list->setCurrentRow(-1);

    idvm = id;
}
#if  1
/************
 *
 *功能:
 *
 *
 **/
void    CreateVmlist::emit_connect_signals()
{
    emit   signals_connect(ids[idvm]);
}
#endif
void    CreateVmlist::emit_start_signals()
{
    emit   signals_start(ids[idvm]);

}
void    CreateVmlist::emit_shutdown_signals()
{
    emit   signals_down(ids[idvm]);
}
void    CreateVmlist::emit_reboot_signals()
{
    emit   signals_reboot(ids[idvm]);
}
#if   1
/*******
 *
 *功能:
 *
 *
 *
 *
 ********/
void    CreateVmlist::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        qDebug()<<"\nsize:"<<btngp->buttons().size();
        for(int i=0;i < vmslist.size();i++)
            if(enterBtn(e,vmslist[i],i))
                return ;

    }
    list->resize(0,0);
    list->setCurrentRow(-1);
    for(int i=0;i<list->count();i++)
    {
        list->item(i)->setBackground(QBrush(QColor(255,255,255)));
        list->item(i)->setSelected(false);
    }

}
/*********
 *
 *功能:
 *
 *
 *
 *
 *
 *******/
bool        CreateVmlist::enterBtn(QMouseEvent  *e,QWidget  *btn,int  n)
{
    if(btn == NULL)
        return  false;
    QPoint   btnMinPos = btn->pos();
    QPoint   btnMaxPos = btn->pos();

    btnMaxPos.setX(btn->pos().x()+btn->width());
    btnMaxPos.setY(btn->pos().y()+btn->height());
    if((e->pos().x() >= btnMinPos.x() && 
                e->pos().x() <= btnMaxPos.x()) &&
            (e->pos().y() >= btnMinPos.y()&& 
             e->pos().y() <= btnMaxPos.y()))
    {
        qDebug()<<"enterBtn  Called!";
        deal_operatorslots(n);
        return  true;
    }
    return   false;

}
#if 0
bool    CreateVmlist::ChangeBtnSty(QPoint  p,QWidget  *btn)
{
    QPoint   btnMinPos = btn->pos();
    QPoint   btnMaxPos = btn->pos();

    btnMaxPos.setX(btn->pos().x()+btn->width());
    btnMaxPos.setY(btn->pos().y()+btn->height());
    if((p.x() >= btnMinPos.x() && p.x() <= btnMaxPos.x()) &&
            (p.y() >= btnMinPos.y()&& p.y() <= btnMaxPos.y()))
    {

        setCursor(QCursor(QPixmap("./images/pointhandler.png")));
        return  true;

    }
    else
    {
        setCursor(Qt::ArrowCursor);
        return  false;
    }


}

#endif
#endif
