#ifndef  __CHANGEPASSWORD__
#define  __CHANGEPASSWORD__
#include <QWidget>
#include <QtGui>
class   ChangePasswd:public   QWidget
{
    Q_OBJECT
    public:
        ChangePasswd(QWidget  *parent = 0);
    signals:
        void    signals_modify(const QString &);
        void    docancel();
    private slots:
            void    doUpdateInfo();
    private:
        QLineEdit    *insoldpd ;
        QLineEdit    *insnewpd ;
        QLineEdit    *insnewpd1;
        QString         opasswd ;
        QString         username;

};
#endif
