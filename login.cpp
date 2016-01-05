/************
 *
 *功能:Login GUI Realize
 *
 *
 *
 **/

#include "login.h"
#include "ui_login.h"
#include <QFile>

/************
 *
 *功能:
 *
 *
 *
 **/
Login::Login(QDialog *parent) :
    QDialog(parent),
    ui(new  Ui::Form)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);

    QPalette palette;
    palette.setColor(QPalette::Background,QColor(0xff,0xff,0xff,0x00));
    this->setPalette(palette);
    this->setAutoFillBackground(true);


    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->checkBox->setText("记住密码");
    
    
    /** Set the  background  of the  Login  GUI**/
    ui->pushButton->setFlat(true);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton->setStyleSheet("background-color:transparent");
    ui->lineEdit->setStyleSheet("background-color:rgba(0,0,0,0)");
    ui->lineEdit_2->setStyleSheet("background-color:rgba(0,0,0,0)");
    ui->lineEdit->setFrame(false);
    //ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_2->setFrame(false);
    //ui->lineEdit_2->setFocusPolicy(Qt::NoFocus);
    ui->checkBox->setStyleSheet("font-size:12px; color:rgb(255,255,255)");


    this->initUserInfo();

    connect(ui->lineEdit,SIGNAL(textEdited(QString)),ui->lineEdit_2,SLOT(clear()));
    connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(repaint()));
    connect(ui->lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(repaint()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(emit_signals_checkcdms()));

}
/************
 *
 *功能:存储用户登录信息,启动CDMS检测
 *
 *
 *
 **/
void    Login::emit_signals_checkcdms()
{

    if(!ui->lineEdit->text().isEmpty())
    {

        if(ui->checkBox->checkState() == Qt::Checked )
        {

            QFile fileRe( "user.txt" );
            QString  msg =ui->lineEdit->text()+":"+ui->lineEdit_2->text();
            if (!fileRe.open(QIODevice::WriteOnly|QIODevice::Text)) {
                QMessageBox::critical(NULL, "提示", "无法创建文件");

            }

            QTextStream out(&fileRe);
            out<<msg<<endl;
            out.flush();

            fileRe.close();
        }

        
            this->username = ui->lineEdit->text();
            this->password = ui->lineEdit_2->text();
            emit  this->signals_loginmsg(ui->lineEdit->text()+"\n"+ui->lineEdit_2->text());
        
    }
    else
    {
        QMessageBox  box;
        box.setText("username is null.");
        box.exec();
    }







}
/************
 *
 *功能:读取本地存储的用户登录信息
 *
 **/
void    Login::initUserInfo()
{
    QFile file( "user.txt" );
    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QByteArray  line = file.readLine();
        if(line.length() > 0)
        {
            ui->checkBox->setCheckState(Qt::Checked);

            QString  info = line,mg="";
            QStringList  tmp;
            if(info.length() >= 0)
            {
                QStringList mc = info.split(":");
                if(!mc.isEmpty()&&mc.size()> 0)
                {
                    this->username = mc.at(0);
                    if(mc.size() > 1)
                        mg = mc.at(1);
                    if(!mg.isEmpty())
                    {
                        tmp = mg.split("\n");
                        this->password = tmp.at(0);

                        ui->lineEdit->setText(this->username);
                        ui->lineEdit_2->setText(this->password);
                    }
                }
            }
        }
        else
            ui->checkBox->setCheckState(Qt::Unchecked);

    }
}
void    Login::paintEvent(QPaintEvent  *)
{
        QFont font("Arial",14);
        QPainter    painter(this);
        painter.setFont(font);
        painter.setPen(QColor(Qt::white));
        painter.drawPixmap(QPoint(width()/2-140,height()/2+60),QPixmap("./images/login.png"));
        painter.drawText(width()/2-40,height()/2+82,"登  录");

        if(ui->lineEdit->text().isEmpty())
        painter.drawPixmap(QPoint(width()/2-140,height()/2-80),QPixmap("./images/user.png"));
        else
        painter.drawPixmap(QPoint(width()/2-140,height()/2-80),QPixmap("./images/userhover.png"));

        if(ui->lineEdit_2->text().isEmpty())
        painter.drawPixmap(QPoint(width()/2-140,height()/2-10),QPixmap("./images/passwd.png"));
        else
        painter.drawPixmap(QPoint(width()/2-140,height()/2-10),QPixmap("./images/passwdhover.png"));
}
Login::~Login()
{
    delete  ui;
}
