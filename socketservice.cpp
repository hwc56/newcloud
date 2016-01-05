#include "socketservice.h"
#include "cJSON.h"
#include "logging.h"
#include <QtGui>


SocketService::SocketService(QObject *parent) :
    QObject(parent)
{
    http = new  CHttpClient();
    httpcache = "";
    login_state = false;
    ReadUSB();
    connect(this,SIGNAL(signals_httperror(QString)),this,SLOT(displayErrorSlots(QString)));
    connect(this,SIGNAL(signals_gethostlist()),this,SLOT(gethostlist_slots()));
}
/*****
 *  Function: when change password , this  Slot will be  Called by the  
 *  signals_modify  signals of  mainview class . 
 *
 *
 ******/
void    SocketService::updateUserinfo(const QString  &newpass)
{
    if(userpass.length() > 0)
    {

        std::string  paswd = "oldPassword="+passwd+"&newPassword="+newpass.toLocal8Bit().data();
        std::string str11 = httphead+"ChangePassword";	//ChangePassword
        std::string str20 = paswd+"&"+userpass;
        std::string  strmg="";
        //qDebug()<<str20.c_str();
        http->Post(str11,str20, strmg);
        qDebug()<<strmg.c_str();
        if(strmg.length() <= 0)
        {
            LOG("error:SocketService::updateUserinfo  ChangePassword not respondly.");
            return ;
        }
        cJSON   *json  = cJSON_Parse(strmg.data());
        if(json == NULL)
        {
            LOG("error:updateuserinfo:JSON Pare error!");
            emit  signals_httperror("updateuserinfo:JSON Pare error!");
            return;
        }
        cJSON   *json_result = cJSON_GetObjectItem(json,"result");
        QString   result = json_result->valuestring;
        qDebug() <<"Result:"<<result;
        if(result.compare("true") == 0)
        {
            ResetUserInfo();
            emit this->signals_loginview(0);
        }
        else
        {
            cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

            emit  signals_httperror(json_error->valuestring);

        }
        cJSON_Delete(json);
    }
    else
    {  
        ResetUserInfo();
        //emit  signals_httperror("Please to Login.");
        emit this->signals_loginview(0);

    }
}
/*****
 *  Function: check the  cdms  if  can  be  used. 
 *
 *  this  function   will  be  called  in  the  slot  of  registertocdms or
 * 
 *  int  the  CheckNetworkState  method of  mainview  after   mainview  
 *  window  call the  show  method  
 ******/
void    SocketService::CheckService(const QString &dmsip)
{
    /*****check the  service  if  can  arrived*****/
    std::string   str1,str2,strmg="";
    //qDebug() <<"cdms:"<< dmsip;
    if(dmsip.length() <= 0)
        return;
    cdmsip =  dmsip.toLocal8Bit().data();
    httphead = "http://"+cdmsip+"/index.php?r=api/";
    str1 = httphead+"Hello";
    http->Post(str1, "", strmg);
    qDebug() << "hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {

        LOG("warning:cannot  arrive,please check the  network wireline!");
        emit  this->signals_connectError(1);
        return;
    }
    cJSON   *json  = cJSON_Parse(strmg.data());
    if(json == NULL)
    {
        LOG("error:checkservice:JSON Pare error!");
        emit  signals_httperror("checkservice:JSON Pare error!");
        return;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    QString   result = json_result->valuestring;
    qDebug() <<"Result:"<<result;
    if(result.compare("true") == 0)
    {
        /***********register information***************/
        emit  this->signals_register();

    }
    else
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

        emit  signals_httperror(json_error->valuestring);

        emit  this->signals_connectError(1);
    }
    cJSON_Delete(json);
}
/*****
 *  Function: to  register  the information  of  user. 
 *
 *
 *
 *
 ******/
void    SocketService::registertocdms(const QString &registerdata)
{
    /*****check the  service  if  can  arrived*****/
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    //qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() > 0)
    {

        //qDebug() << "========="<<registerdata;
        QStringList     infos       = registerdata.split("\n");
        std::string     strqest     = httphead + "RegisterTc";
        std::string     user        = infos.at(0).toLocal8Bit().data();
        std::string     password    = infos.at(1).toLocal8Bit().data();
        std::string     mac         = infos.at(2).toLocal8Bit().data();
        std::string     name        = infos.at(3).toLocal8Bit().data();
        std::string     gateway     = infos.at(4).toLocal8Bit().data();
        std::string     mask        = infos.at(5).toLocal8Bit().data();
        std::string     ip          = infos.at(6).toLocal8Bit().data();
        std::string     dns         = infos.at(7).toLocal8Bit().data();
        std::string     method      = infos.at(8).toLocal8Bit().data();
        boxmac = mac;
        std::string     strparams   =   "username=" +user+"&password="+password+
            "&mac="     +mac+"&name="    +name+"&gateway="+gateway+
            "&mask="    +mask+"&ip="      +ip+"&DNS="    +dns+
            "&method="  +method+"&protocol="+"SPICY";
        //qDebug() << "========="<<strparams.c_str();
        std::string    respond="";
        http->Post(strqest, strparams, respond);
        qDebug() << "registerrespond:"<<respond.c_str();
        if(respond.length() <= 0)
        {
            emit  signals_httperror("server no  respond!");
            return;
        }
        cJSON   *json  = cJSON_Parse(respond.data());
        if(json == NULL)
        {
            emit  signals_httperror("registertocdms:JSON Pare error!");
            return;
        }
        cJSON   *json_result = cJSON_GetObjectItem(json,"result");
        QString   result = json_result->valuestring;
        //qDebug() <<"Result:"<<result;
        if(result.compare("true") == 0)
        {
            if(login_state == false)
                emit this->signals_loginview(0);
        }
        else
        {
            cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

            emit  signals_httperror(json_error->valuestring);

            emit  this->signals_connectError(1);
        }
        cJSON_Delete(json);

    }
    else
    {

        LOG("error:register to cdms failed for  the  internet connecting question.");
        emit  signals_httperror("register to cdms failed for  the  internet connecting question.");
        emit  this->signals_connectError(1);
    }
}
/*****
 *  Function: check  login  information  if  effect  for  server  terminal. 
 *
 *  this  slot  will be  called  when  the  signals_loginmsg  signal of  Login  class   send   out.
 *
 *
 ******/
void    SocketService::login_slots(const QString &userpasswd)
{
    /*****check the  service  if  can  arrived*****/
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    //qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);
        return;
    }

    ResetUserInfo();
    qDebug() << userpasswd;
    QStringList  usepasswd = userpasswd.split("\n");
    user = usepasswd.at(0).toLocal8Bit().data();
    passwd = usepasswd.at(1).toLocal8Bit().data();

    userpass = "username="+user+"&password="+passwd/*qsau_test_1"*/;



    qDebug()<<"socketservice:"<<user.c_str()<<":"<<passwd.c_str();
    std::string str10 = httphead+"login";
    //str10 = "http://10.3.3.59/index.php?r=api/login"; //login
    std::string str20 = userpass;//"username=qsau_test_1&password=qsau_test_1";

    qDebug()<<userpass.c_str();
    strloginCheck="";
    http->Post(str10, str20, strloginCheck);
    qDebug() << strloginCheck.c_str();
    if(strloginCheck.length() <= 0)
    {
        LOG("error:server no  respond when login_slots!");
        emit  signals_httperror("server no  respond when login!");
        return;
    }
    cJSON   *json  = cJSON_Parse(strloginCheck.data());
    if(json == NULL)
    {
        LOG("error:login:JSON Pare error!");
        emit  signals_httperror("login:JSON Pare error!");
        return;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    if(json_result == NULL)
    {
        emit  signals_httperror("JSON Pare error!");
        return ;
    }
    QString   result = json_result->valuestring;
    //qDebug() <<"Result:"<<result;
    if(result.compare("true") == 0)
    {
        login_state = true;
        emit   this->signals_gethostlist();
    }
    else
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

        emit  signals_httperror(json_error->valuestring);
        emit  signals_loginview(0);
    }
    cJSON_Delete(json);
}
/*****
 *  Function: to  get  the  virtual  pc  list. 
 *
 *  the  slot   will be  called by   the   signals_gethostlist  signal.
 *
 ******/
void    SocketService::gethostlist_slots()
{
    vmsinfors.clear();
    /*****check the  service  if  can  arrived*****/
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    //qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);//if get  hostlist  failed  ,open the  networkconfig  window.
        return;
    }
    //QDateTime  time = QDateTime::currentDateTime();
    //QString  strtime = time.toString("MMddhhmmssddd");
    std::string str11 = httphead+"getHostList";//+strtime.toLocal8Bit().data();//"http://10.3.3.59/index.php?r=api/getHostList";	//getHostList
    std::string str20 = userpass;//"username=qsau_test_1&password=qsau_test_1";
    //check  username and  passwd if OK then  post
    if(userpass.length() > 0 && login_state == true)
    {
        strhostlist = "";
        http->Post(str11, str20, strhostlist);
        qDebug() <<"=============list:"<<strhostlist.c_str();
        if(strhostlist.length()<=0)
        {
            emit  signals_httperror("server not respond!");
            return;
        }
        cJSON   *json  = cJSON_Parse(strhostlist.data());
        cJSON   *json_result = cJSON_GetObjectItem(json,"result");
        if(json_result == NULL)
        {
            emit  signals_httperror("JSON Pare error!");
            return ;
        }
        QString   result = json_result->valuestring;
        //qDebug() <<"Result:"<<result;
        if(result.compare("true") == 0)
        {
           // emit    signals_tohostlistcheck(userpass.c_str());
            cJSON   *json_vmlist = cJSON_GetObjectItem(json,"params");
            if(json_vmlist == NULL)
            {
                return ;
            }
            cJSON   *json_array = cJSON_GetObjectItem(json_vmlist,"hosts");
            if(json_array == NULL)
            {
                return ;
            }
            int   array_size = cJSON_GetArraySize(json_array);

            QString   hostlist="",onevm="";
            for(int i=0;i < array_size;i++)
            {
                cJSON   *item = cJSON_GetArrayItem(json_array,i);
                //qDebug() <<i<<":"<< cJSON_Print(item);
                json_result = cJSON_GetObjectItem(item,"hostName");
                if(json_result != NULL)
                    result = json_result->valuestring;
                else
                    result = "unknown";
                //qDebug() <<result;
                hostlist += result + "\t";
                onevm += result + "\t";

                json_result = cJSON_GetObjectItem(item,"hostId");
                if(json_result != NULL)
                    result = json_result->valuestring;
                else
                    result = "unknown";
                //qDebug() <<result;
                hostlist += result + "\t";
                onevm += result + "\t";

                json_result = cJSON_GetObjectItem(item,"status");
                if(json_result != NULL)
                    result = json_result->valuestring;
                else
                    result = "unknown";
                //qDebug() <<result;
                hostlist += result + "\t";
                onevm += result + "\t";

                json_result = cJSON_GetObjectItem(item,"protocol");

                if(json_result != NULL)
                {
                    int  size = cJSON_GetArraySize(json_result);
                    for(int j=0;j < size;j++)
                    {
                        cJSON  *item_item = cJSON_GetArrayItem(json_result,j);
                        result = item_item->valuestring;
                        //qDebug() <<result;
                        if(result.length() > 0)
                        {
                            hostlist += result + "\t";
                            onevm += result + "\t";
                        }
                        else
                        {
                            hostlist += "unknown\t";
                            onevm += "unknown\t";


                        }
                    }
                }
                else
                {
                    hostlist += "unknown\t";
                    onevm += "unknown\t";

                }

                json_result = cJSON_GetObjectItem(item,"type");
                if(json_result != NULL)
                {//qDebug() <<json_result->valueint;
                    hostlist += QString::number(json_result->valueint) + "\n";
                    onevm += QString::number(json_result->valueint);
                }
                else
                {
                    hostlist += QString::number(-1) + "\n";
                    onevm += QString::number(-1);

                }
                vmsinfors.push_back(onevm);
                onevm="";
            }

            emit  this->signals_vmlist(hostlist); 

        }
        else
        {
            cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");
            emit  signals_httperror(json_error->valuestring);

        }


        cJSON_Delete(json);
    }else
    {
        ResetUserInfo();
        //emit  signals_httperror("Please to Login.");
        emit  signals_loginview(0);
    }


}
/*****
 *  Function: to  get  the  connecting  params  for  linking remote. 
 *
 *   the  slot  called  by   the  three signal  of  vm  button.
 *
 *
 ******/
void    SocketService::getconnectinfo_slots(const QString &vminfo)
{

    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    //qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);
        return;
    }
    qDebug() <<"vminfo:"<<vminfo;

    QStringList   vmneeds = vminfo.split("\t");
    QString  params = "&type="+vmneeds.at(vmneeds.size()-1)+"&hostId="+vmneeds.at(1)+"&protocol="+vmneeds.at(3); 
    if(QString("0").compare(vmneeds.at(2))==0) 
        startvmslots(vmneeds.at(1));
    //this is for migrate 
    checkmigrate = "";
    checkmigrate = vmneeds.at(0)+"\n"+vmneeds.at(1);
    vmstatus = vmneeds.at(2).toLocal8Bit().data();
    logipslots(vmneeds.at(1));
    //qDebug() <<"params:"<<params;

    std::string str12 = httphead+"getConnectInfo";
    //"http://10.3.3.59/index.php?r=api/getConnectInfo";	//getConnectInfo
    std::string str21 = userpass+params.toLocal8Bit().data();    
    //"username=qsau_test_1&password=qsau_test_1&type=1&hostId=41&protocol=spicy";
    //check vm information is able to  use then post,this info contain hostId
    strConnectInfo = "";
    http->Post(str12, str21, strConnectInfo);

    qDebug() <<"*********ConnectInfo:"<<strConnectInfo.c_str();
    //qDebug() <<"params:"<<str21.c_str();
    if(strConnectInfo.length() <= 0)
    {
        LOG("error:Server not respond when GetConnectInfo.");
        emit  signals_httperror("Server not respond when Get Connectinfo.");
        return;
    }
   
    std::string jsondata = IfJsonData(strConnectInfo);
    cJSON   *json  = cJSON_Parse(jsondata.data());
    if(json == NULL)
    {
        emit  signals_httperror("JSON_Parse failed!");
        emit   this->signals_gethostlist();
        return ;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    QString   result = json_result->valuestring;
    qDebug() <<"Result:"<<result;
    if(result.compare("true") == 0)
    {
        QString  msg="";
        cJSON   *json_vmlist = cJSON_GetObjectItem(json,"params");
        cJSON   *json_ip = cJSON_GetObjectItem(json_vmlist,"ip");
        result = json_ip->valuestring;
        qDebug() <<"ip:"<<result;
        msg += result+"\n";
        cJSON   *json_port = cJSON_GetObjectItem(json_vmlist,"port");
        result = json_port->valuestring;
        qDebug() <<"port:"<<result;
        msg += result+"\n";


        cJSON   *json_usb = cJSON_GetObjectItem(json_vmlist,"usb");
        if(json_usb != NULL)
        {
            result = json_usb->valuestring;
            qDebug() <<"usb:"<<result;
            msg += result+"\n";
        }



        qDebug() << msg;
        LoginCloud(msg);
    }
    else
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");
        emit  signals_httperror(json_error->valuestring);

    }


    cJSON_Delete(json);
}

std::string    SocketService::IfJsonData(const std::string &strInfo)
{
    std::string   jsondata="";
    if(strInfo.length() > 0)
    {
    std::string::size_type  pos = 0;
    pos = strInfo.find_first_of('{');
    if(pos != std::string::npos )
    jsondata = strInfo.substr(pos);    
    }
    return  jsondata;
}
/*****
 *  Function:to  display  the  error  message. 
 *
 *
 *
 *
 ******/
void    SocketService::displayErrorSlots(const QString &mg)
{
    QMessageBox  box;
    box.setWindowTitle("提示");
    box.setText(mg);
    box.exec();
}

void    SocketService::startvmslots(const QString &vmid)//start   virtual  machine
{
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);
        return;
    }

    if(userpass.length() <= 0)
    {
        ResetUserInfo();
        //emit  signals_httperror("Please to Login.");
        emit  signals_loginview(0);
        return ;
    }

    QString   params = "&hostId="+ vmid;
    std::string   hostid = params.toLocal8Bit().data();

    std::string   httpinfo =  httphead + "StartHost";
    std::string   checkinfo = userpass + hostid; 
    std::string   strmsg = "";
    qDebug()<<"checkinfo:"<<checkinfo.c_str();
    http->Post(httpinfo, checkinfo, strmsg);
    //if(strmsg.length() <=0 )
    //{
    //  sleep(500);
    // http->Post(httpinfo, checkinfo, strmsg);
    //}
    qDebug() << "startvm:"<<strmsg.c_str();
    JSParse(strmsg);

}
void    SocketService::shutdownvmslots(const QString &vmid)//shutdown   virtual  machine
{
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);
        return;
    }
    if(userpass.length() <= 0)
    {
        ResetUserInfo();
        //emit  signals_httperror("Please to Login.");
        emit  signals_loginview(0);
        return ;
    }
    QString   params = "&hostId="+ vmid;
    std::string   hostid = params.toLocal8Bit().data();
    std::string   httpinfo =  httphead + "ForcedShutdown";
    std::string   checkinfo = userpass + hostid; 
    std::string   strmsg = "";

    qDebug()<<"checkinfo:"<<checkinfo.c_str();
    http->Post(httpinfo, checkinfo, strmsg);
    //if(strmsg.length() <= 0 )
    //{
    //sleep(100);
    //http->Post(httpinfo, checkinfo, strmsg);
    //}   
    qDebug() << "closevm:"<<strmsg.c_str();
    JSParse(strmsg);
}
void    SocketService::rebootvmslots(const QString &vmid)//reboot  virtual machine
{
    std::string str1 = httphead+"Hello";
    std::string strmg = "";
    http->Post(str1, "", strmg);
    qDebug() << "regidter hello:"<<strmg.c_str();
    if(strmg.length() <= 0)
    {
        emit  this->signals_connectError(1);
        return;
    }
    if(userpass.length() <= 0)
    {
        ResetUserInfo();
        //emit  signals_httperror("Please to Login.");
        emit  signals_loginview(0);
        return ;
    }
    QString   params = "&hostId="+ vmid;
    std::string   hostid = params.toLocal8Bit().data();
    std::string   httpinfo =  httphead + "ForcedReboot";
    std::string   checkinfo = userpass + hostid; 
    std::string   strmsg = "";

    qDebug()<<"checkinfo:"<<checkinfo.c_str();
    http->Post(httpinfo, checkinfo, strmsg);
    // if(strmsg.length() <= 0)
    //{
    //sleep(500);
    //http->Post(httpinfo, checkinfo, strmsg);
    //}
    qDebug() << "closevm:"<<strmsg.c_str();
    JSParse(strmsg);

}
#if 0
void    SocketService::sleep(int   msec)
{
    QTime  dt = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dt)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
#endif
void    SocketService::JSParse(const std::string  &strmsg)
{

    std::string jsondata = IfJsonData(strmsg);
    cJSON   *json  = cJSON_Parse(jsondata.data());
    if(json == NULL)
    {
        emit   this->signals_gethostlist();
        return;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    if(json_result == NULL)
    {
        emit  signals_httperror("JSON Pare error!");
        return ;
    }
    QString   result = json_result->valuestring;
    if(result.compare("true") == 0)
    {
        emit   this->signals_gethostlist();
    }
    else
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

        emit  signals_httperror(json_error->valuestring);
    }
    cJSON_Delete(json);

}
#if 1
/***********
 *
 * the  function  is check if  able to  connect  the  server.
 *
 *
 *
 *
 *************/
void    SocketService::isableconnectslots(const QString &vmid)
{
    QString   params = "&hostId="+ vmid;
    std::string   hostid = params.toLocal8Bit().data();
    std::string   httpinfo =  httphead + "GetCbConnect";
    std::string   checkinfo = userpass + hostid; 
    std::string   strmsg = "";

    qDebug()<<"checkinfo:"<<checkinfo.c_str();
    http->Post(httpinfo, checkinfo, strmsg);
    qDebug() << "GetCbConnect:"<<strmsg.c_str();
    cJSON   *json  = cJSON_Parse(strmsg.data());
    if(json == NULL)
    {
        return;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    if(json_result == NULL)
    {
        emit  signals_httperror("JSON Pare error!");
        return ;
    }
    QString   result = json_result->valuestring;
    if(result.compare("true") == 0)
    {
        qDebug()<<"vmid:"<<vmid;
        QString  vmmg="";
        for(int i=0;i<vmsinfors.size();i++)
            if(vmid.compare(vmsinfors[i].split("\t").at(1))==0) 
            {
                qDebug()<<i<<":"<<vmsinfors[i];
                vmmg += vmsinfors[i];
                break;
            }
        qDebug()<<"vmmg:"<<vmmg;
        getconnectinfo_slots(vmmg);
    }
    else
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

        emit  signals_httperror(json_error->valuestring);
    }
    cJSON_Delete(json);


}
void        SocketService::ReadUSB()
{
        localusbinfo="";
       usb_init();
       usb_find_busses();
       usb_find_devices();

       struct  usb_bus *pbus = NULL,*usb_busses=NULL;
       struct  usb_device  *dev = NULL;

       usb_busses = usb_get_busses();
       int  c,i,a;
        char  buffer[2048];
       for(pbus = usb_busses;pbus != NULL;pbus = pbus->next)
       {
           for(dev = pbus->devices;dev != NULL;dev = dev->next)
           {
                for(c = 0;c < dev->descriptor.bNumConfigurations;c++)
                {
                    for(i=0;i < dev->config[c].bNumInterfaces;i++)
                    {
                        for(a=0; a < dev->config[c].interface[i].num_altsetting;a++)
                        {
                            switch(dev->config[c].interface[i].altsetting[a].bInterfaceClass)
                            {
                                /**********************
                                1－audio：表示一个音频设备。
                                2－communication device：通讯设备，如电话，moden等等。
                                3－HID：人机交互设备，如键盘，鼠标等。
                                6－image图象设备，如扫描仪，摄像头等，有时数码相 机也可归到这一类。
                                7－打印机类。如单向，双向打印机等。
                                8－mass storage海量存储类。如数码相机大多数都归这一类。
                                9－hub类。
                                11－chip card/smart card。
                                255－vendor specific.厂家的自定义类，主要用于一些特殊的设备，如接口转接卡等。 
                                *******************/
                                case 1:
                                    printf("audio %x:%x\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
                                break;
                                case 2:
                                    printf("comminication device %x:%x\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
                                break;
                                case 3:
                                {
                                    memset(buffer,0,sizeof(buffer));
                                    sprintf(buffer,"-1,0x%x,0x%x,-1,0|",dev->descriptor.idVendor,dev->descriptor.idProduct);
                                    printf("HID %x:%x\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
                                    localusbinfo += buffer;


                                break;
                                }
                                case 6:
                                break;
                                case 7:
                                break;   
                                case 8:
                                break;
                                case 9:
                                break;
                                case 11:
                                break;   
                                case 255:
                                break;
                            }
                        }
                    }

                }
            }
        }
        qDebug()<<"usbinfo:"<<localusbinfo.c_str();

}
void        SocketService::LoginCloud(const QString &msg)
{

    if(msg.length() > 0)
    {
        QStringList  ipport = msg.split("\n");
        //send the spicy ip to  migrate for  termal migrate
        emit  signals_startmigrate(ipport.at(0)+" -p "+ipport.at(1)+"\n"+user.c_str()+"\n"+localusbinfo.c_str()+"\n"+checkmigrate);
        if(ipport.size() >= 3)
        {
            /*********
    ['/usr/bin/spicy', '-h', u'10.5.0.31', '-p', u'5992', '-f', '--title', u'VM::huangweichang_1', 
    u'--spice-usbredir-auto-redirect-filter=-1,0xc45,0x760b,-1,0|-1,0x93a,0x2510,-1,0|-1,-1,-1,-1,1', 
    u'--spice-usbredir-redirect-on-connect=-1,0xc45,0x760b,-1,0|-1,0x93a,0x2510,-1,0|-1,-1,-1,-1,1']
            ***********/
            QString  str;
            //str = QString("/usr/bin/spicy -h %1 -p %2 -f --title  VM::%3 --spice-usbredir-auto-redirect-filter=\"%4\"    --spice-usbredir-redirect-on-connect=\"%5\"").arg(ipport.at(0)).arg(ipport.at(1)).arg(user.c_str()).arg(localusbinfo.c_str()+ipport.at(2)).arg(localusbinfo.c_str()+ipport.at(2));
            str = QString("/root/Downloads/spicy/gtk/.libs/spicy -h %1 -p %2 -f --title  VM::%3 ").arg(ipport.at(0)).arg(ipport.at(1)).arg(user.c_str());
            qDebug() << str;
            emit    this->signals_startspicy(str);
        }
    }



}

#endif



void     SocketService::modifycdmsSlots(const QString &newip)
{
    cdmsip =  newip.toLocal8Bit().data();
    httphead = "http://"+cdmsip+"/index.php?r=api/";
}


void            SocketService::logipslots(const QString &vmid)
{
    QString   params = "&hostId="+ vmid;
    std::string   hostid = params.toLocal8Bit().data();
    std::string   hostmac = "&boxId=" + boxmac;
    std::string   hoststatus = "&status=" + vmstatus;
    std::string   httpinfo =  httphead + "iChange";
    std::string   checkinfo = userpass + hostid + hostmac + hoststatus; 
    std::string   strmsg = "";

    qDebug()<<"checkinfo:"<<checkinfo.c_str();
    http->Post(httpinfo, checkinfo, strmsg);
    qDebug() << "logip:"<<strmsg.c_str();
    cJSON   *json  = cJSON_Parse(strmsg.data());
    if(json == NULL)
    {
        return;
    }
    cJSON   *json_result = cJSON_GetObjectItem(json,"result");
    if(json_result == NULL)
    {
        emit  signals_httperror("JSON Pare error!");
        return ;
    }
    QString   result = json_result->valuestring;
    if(result.compare("false") == 0)
    {
        cJSON   *json_error = cJSON_GetObjectItem(json,"errorMsg");

        emit  signals_httperror(json_error->valuestring);

    }
    cJSON_Delete(json);


}
#if  0
QString       SocketService::encryptiondata(QString data)
{
    qDebug() << "data:"<<data;
    QString   encryptdata;
    encryptdata.clear();
    if(data.length() > 0)
    {
        char  *array = data.toLatin1().data();
        for(int i=0;i < strlen(array);i++)
        {
            *(array+i) ^= '\n';
        }
        encryptdata = array;
    }
    qDebug() << "encryptdata:"<<encryptdata;
    return  encryptdata;
}

QString    SocketService::encryptionbymd5(QString msg)
{
    QString md5="";
    QByteArray bb;
    if(!msg.isEmpty())
    {
        bb = QCryptographicHash::hash ( msg.toAscii(), QCryptographicHash::Md5 );
        md5.append(bb.toHex());
    }
    return  md5;
}
#endif
