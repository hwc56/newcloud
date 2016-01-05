/************
*
*功能:Login  Class
*
*
*
**/
#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui>
#include <QDialog>
namespace Ui{
    class   Form;
}
class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QDialog *parent = 0);
    ~Login();

    void    initUserInfo();
    QString GetUserName()
    {
        return  username;
    }
signals:
    void    loginRequest(const QString &);//username
    void    signals_loginmsg(const QString &);
    void    check_cdms_signals();
public slots:
    void    emit_signals_checkcdms();
private:
        void   paintEvent(QPaintEvent  *);
private:
    Ui::Form  *ui;
    QString  username;
    QString  password;

};

#endif // LOGIN_H
