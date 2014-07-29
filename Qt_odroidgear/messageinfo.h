#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H

#include <QWidget>
#include <QTime>
#include <QLabel>

namespace Ui {
class MessageInfo;
}

class MessageInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageInfo(QWidget *parent = 0);
    ~MessageInfo();
    void setSMS(QString pNumber, QString smsMessage, QString recievedTime);
    
private:
    Ui::MessageInfo *ui;
    QLabel *smsImage;
};

#endif // MESSAGEINFO_H
