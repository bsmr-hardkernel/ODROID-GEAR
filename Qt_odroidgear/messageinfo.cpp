#include "messageinfo.h"
#include "ui_messageinfo.h"

MessageInfo::MessageInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageInfo)

{
    ui->setupUi(this);
    smsImage = new QLabel(this);
    smsImage->setPixmap(QPixmap("/home/odroid/work/opi_watch/images/ic_sms_96.png"));
    smsImage->setGeometry(240, 5, 64, 64);
}

MessageInfo::~MessageInfo()
{
    delete ui;
}

void MessageInfo::setSMS(QString pNumber, QString smsMessage, QString recievedTime)
{
    ui->lPhoneNumber->setText(pNumber);
    ui->message->setText(smsMessage);
    ui->lRecievedTime->setText(recievedTime);
}
