#include "opiwatch.h"
#include "analogclock.h"
#include "messageinfo.h"
#include "callinfo.h"
#include <QtDebug>
#include <QPushButton>
#include <QTimer>
#include <QPixmap>
#include <QLabel>

OpiWatch::OpiWatch(QWidget *parent) :
    QWidget(parent)
{
    analogClock = new AnalogClock(this);
    //analogClock->hide();
    messageInfo = new MessageInfo(this);
    messageInfo->hide();
    callInfo = new CallInfo(this);
    callInfo->hide();

    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
    tcpServer->listen(QHostAddress::Any, 8080);

    lBattery = new QLabel(analogClock);
    lBattery->setPixmap(QPixmap("/home/odroid/work/qt4/opi_watch/images/ic_battery.png"));
    lBattery->setGeometry(0, 0, 48, 48);

    lBlueTooth = new QLabel(analogClock);
    lBlueTooth->setPixmap(QPixmap("/home/odroid/work/qt4/opi_watch/images/ic_bluetooth.png"));
    lBlueTooth->setGeometry(272, 0, 48, 48);
    lBlueTooth->hide();

    lnBlueTooth = new QLabel(analogClock);
    lnBlueTooth->setPixmap(QPixmap("/home/odroid/work/qt4/opi_watch/images/ic_n_bluetooth.png"));
    lnBlueTooth->setGeometry(272, 0, 48, 48);

    lSMS = new QLabel(analogClock);
    lSMS->setPixmap(QPixmap("/home/odroid/work/qt4/opi_watch/images/ic_sms.png"));
    lSMS->setGeometry(0, 192, 48, 48);

    lCall = new QLabel(analogClock);
    lCall->setPixmap(QPixmap("/home/odroid/work/qt4/opi_watch/images/ic_call.png"));
    lCall->setGeometry(272, 192, 48, 48);

    fw = new QFileSystemWatcher(this);
    fw->addPath("/home/odroid/work/qt4/opi_watch/test.txt");

    connect(fw, SIGNAL(fileChanged(QString)), this, SLOT(updateFile()));
}

void OpiWatch::updateFile()
{
    analogClock->hide();
    messageInfo->show();
}

void OpiWatch::acceptConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));
    lnBlueTooth->hide();
    lBlueTooth->show();
}

void OpiWatch::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_S) {
        analogClock->hide();
        callInfo->hide();
        messageInfo->show();
    } else if (event->key() == Qt::Key_C) {
        analogClock->hide();
        messageInfo->hide();
        callInfo->show();
    } else if (event->key() == Qt::Key_H) {
        callInfo->hide();
        messageInfo->hide();
        analogClock->show();
    }


}

void OpiWatch::readTcpData()
{
    time = new QTime();
    date = new QDate();

    int i = 1;
    QByteArray buffer;
    QString phoneNumber = 0;
    QString message = 0;
    QString currentTime = 0;
    buffer = tcpSocket->readLine();
    qDebug() << buffer;
    if ((buffer[0] == 'n')) {
        while (buffer[i] != 'e')
            phoneNumber += buffer[i++];
        i++;
        if (buffer[i] == 'm') {
            for (i += 1; i < buffer.size(); i++) {
                message += buffer[i];
            }
            currentTime = time->currentTime().toString();
            currentTime.append(", ");
            currentTime.append(date->currentDate().toString());
            messageInfo->setSMS(phoneNumber, message, currentTime);
            analogClock->hide();
            callInfo->hide();
            messageInfo->show();
        } else if (buffer[i] == 's') {
            qDebug() << i;
            if (buffer[i+1] == 'i') {
                //IDLE
                callInfo->idleStatus();
                messageInfo->hide();
                callInfo->hide();
                analogClock->show();
            } else if (buffer[i+1] == 'r') {
                currentTime = time->currentTime().toString();
                currentTime.append(", ");
                currentTime.append(date->currentDate().toString());
                callInfo->setCall(phoneNumber, currentTime);
                callInfo->incomingStatus();
                messageInfo->hide();
                analogClock->hide();
                callInfo->show();
            } else if (buffer[i+1] == 'o'){
                callInfo->timer->stop();
                callInfo->ringingImage->show();
            }
        }
    }
}
