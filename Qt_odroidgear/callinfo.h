#ifndef CALLINFO_H
#define CALLINFO_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {
class CallInfo;
}

class CallInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CallInfo(QWidget *parent = 0);
    ~CallInfo();
    void setCall(QString pNumber, QString recievedTime);
    void incomingStatus(void);
    void idleStatus(void);
    //void recievedState(void);
    QTimer *timer;
    QLabel *ringingImage;
    QLabel *idleImage;
private:
    Ui::CallInfo *ui;

    int temp;


public slots:
    void update();
};

#endif // CALLINFO_H
