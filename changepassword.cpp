#include "changepassword.h"

ChangePasswd::ChangePasswd(QWidget  *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    //setWindowIcon(QIcon("./images/logo.png"));
    //setWindowTitle("修改密码");
    QPalette  palette;
    palette.setColor(QPalette::Background,QColor(0xff,0xff,0xff,0x90));
    //QPixmap pixmap("./images/modify-bg.jpg");
    //palette.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(palette);
    setAutoFillBackground(true);

    //QVBoxLayout  *vlay  = new  QVBoxLayout();
    //QHBoxLayout  *hlay  = new  QHBoxLayout();
    //QHBoxLayout  *hlay1 = new  QHBoxLayout();
    //QHBoxLayout  *hlay2 = new  QHBoxLayout();
    //QHBoxLayout  *hlay3 = new  QHBoxLayout();
    QLabel       *oldpasswd  = new  QLabel("  原密码",this);
    QLabel       *newpasswd  = new  QLabel("  新密码",this);
    QLabel       *newpasswd1 = new  QLabel("重复密码",this);
    insoldpd   = new  QLineEdit(this);
    insnewpd   = new  QLineEdit(this);
    insnewpd1  = new  QLineEdit(this);

    QPushButton  *button_ok        =  new  QPushButton("完成",this);
    QPushButton  *button_cancel    =  new  QPushButton("取消",this);

    oldpasswd->setStyleSheet("font-size:15px");
    newpasswd->setStyleSheet("font-size:15px");
    newpasswd1->setStyleSheet("font-size:15px");
    button_ok->setStyleSheet("font-size:16px");
    button_cancel->setStyleSheet("font-size:16px");
    insoldpd->setStyleSheet("font-size:16px");
    insnewpd->setStyleSheet("font-size:16px");
    insnewpd1->setStyleSheet("font-size:16px");


    button_ok->setFocusPolicy(Qt::NoFocus);
    button_cancel->setFocusPolicy(Qt::NoFocus);
    
    insoldpd->setEchoMode(QLineEdit::Password);
    insnewpd->setEchoMode(QLineEdit::Password);
    insnewpd1->setEchoMode(QLineEdit::Password);

    palette.setColor(QPalette::Button,QColor(26,112,178,220));
    palette.setColor(QPalette::ButtonText,QColor(255,255,255));
    button_ok->setPalette(palette);
    button_ok->setAutoFillBackground(true);
    button_ok->setFlat(true);

    button_cancel->setPalette(palette);
    button_cancel->setAutoFillBackground(true);
    button_cancel->setFlat(true);

    oldpasswd->setGeometry(x()+10,y()+20,60,40);
    newpasswd->setGeometry(x()+10,y()+90,60,40);
    newpasswd1->setGeometry(x()+10,y()+160,60,40);
    insoldpd->setGeometry(x()+100,y()+20,250,40);
    insnewpd->setGeometry(x()+100,y()+90,250,40);
    insnewpd1->setGeometry(x()+100,y()+160,250,40);
    button_ok->setGeometry(x()+50,y()+240,100,40);
    button_cancel->setGeometry(x()+250,y()+240,100,40);
    
    //hlay->addWidget(oldpasswd);
    //hlay->addWidget(insoldpd);
    //hlay->setSpacing(50);
    //hlay->setMargin(20);

    //hlay1->addWidget(newpasswd);
    //hlay1->addWidget(insnewpd);
    //hlay1->setSpacing(50);
    //hlay1->setMargin(20);

    //hlay2->addWidget(newpasswd1);
    //hlay2->addWidget(insnewpd1);
    //hlay2->setSpacing(50);
    //hlay2->setMargin(20);

    //hlay3->addWidget(button_ok);
    //hlay3->addWidget(button_cancel);
    //hlay3->setSpacing(100);
    //hlay3->setMargin(20);


    //vlay->addLayout(hlay);
    //vlay->addLayout(hlay1);
    //vlay->addLayout(hlay2);
    //vlay->addLayout(hlay3);

    //setLayout(vlay);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(doUpdateInfo()));
    connect(button_cancel,SIGNAL(clicked()),this,SIGNAL(docancel()));

    QFile fileRe( "user.txt" );
    if (!fileRe.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QMessageBox::critical(NULL, "提示", "无法创建文件");
        return;

    }
    QString  filemsg = fileRe.readLine();
    fileRe.close();
    QStringList info = filemsg.split(":");
    opasswd="";
    username="";
    if(info.size() > 0)
    {
        username = info.at(0).toLocal8Bit().data();
        if(info.size() > 1)
        {
            opasswd = info.at(1).toLocal8Bit().data();
            QStringList infop = opasswd.split("\n");
            opasswd = infop.at(0);
            qDebug() << username <<" : "<< opasswd;
        }
    }


}
void    ChangePasswd::doUpdateInfo()
{
        if(opasswd.length() <=0 )
            return;

        if(insoldpd->text().isEmpty()||opasswd.compare(insoldpd->text()) != 0)
        {
            QMessageBox  box;
            box.setWindowTitle("提示");
            box.setText("输入原密码不一致!");
            box.exec();
            return;
        }
        if(!insnewpd->text().isEmpty() && !insnewpd1->text().isEmpty())
        {
            QString  msg = insnewpd->text();
            if(msg.compare(insnewpd1->text()) == 0)
            {
                if(msg.compare(opasswd) == 0)
                {
                    QMessageBox  box;
                    box.setWindowTitle("提示");
                    box.setText("新密码原密码一致!");
                    box.exec();
                    insnewpd->clear();
                    insnewpd1->clear();
                    return;
                }

                emit  signals_modify(msg);
                QString  nmsg =username+":"+msg;//there must be  writen by this format,or  will happen questions
                QFile fileRe( "user.txt" );
                if (!fileRe.open(QIODevice::WriteOnly|QIODevice::Text)) {
                    QMessageBox::critical(NULL, "提示", "无法创建文件");
                    return;

                }

                QTextStream out(&fileRe);
                out<<nmsg<<endl;
                out.flush();
                fileRe.close();

            }
            else
            {
                QMessageBox  box;
                box.setWindowTitle("提示");
                box.setText("新密码前后两次输入不一致!");
                box.exec();
                insnewpd->clear();
                insnewpd1->clear();
            }
        }
        else
        {
            QMessageBox  box;
            box.setWindowTitle("提示");
            box.setText("新密码前后两次输入不一致!");
            box.exec();
        }
                insoldpd->clear();
                insnewpd->clear();
                insnewpd1->clear();

}
