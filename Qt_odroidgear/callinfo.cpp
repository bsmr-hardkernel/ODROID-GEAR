#include "callinfo.h"
#include "ui_callinfo.h"
#include <QTimer>

CallInfo::CallInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallInfo)
{
    temp = 0;
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    ringingImage = new QLabel(this);
    ringingImage->setPixmap(QPixmap("/home/pi/opi_watch/images/ic_ring_96.png"));
    ringingImage->setGeometry(110, 90, 96, 96);
    ringingImage->hide();
    ui->lIncomingCall->hide();

    idleImage = new QLabel(this);
    idleImage->setPixmap(QPixmap("/home/pi/opi_watch/images/ic_end_call.png"));
    idleImage->setGeometry(110, 90, 96, 96);

}

CallInfo::~CallInfo()
{
    delete ui;
}

void CallInfo::update()
{
    temp = !temp;
    if (temp)
            ringingImage->show();
    else
        ringingImage->hide();

}

void CallInfo::idleStatus()
{
    timer->stop();
    ui->lIncomingCall->hide();
    ringingImage->hide();
    ui->label_2->show();
    ui->lRecievedTime->show();
    idleImage->show();
}

void CallInfo::incomingStatus()
{
    timer->start(300);
    idleImage->hide();
    ui->lRecievedTime->hide();
    ui->label_2->hide();
    ui->lIncomingCall->show();
}

void CallInfo::setCall(QString pNumber, QString recievedTime)
{
    ui->lPhoneNumber->setText(pNumber);
    ui->lRecievedTime->setText(recievedTime);
}
