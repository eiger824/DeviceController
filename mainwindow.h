#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QMainWindow>
#include <QTimer>

typedef struct Slave
{
    Slave(QString ip, quint16 port,
          QString name, unsigned id, bool stat):
        ipAdress(ip), portNr(port),
        fullSlaveName(name), slaveID(id), status(stat)
    {}

    QString ipAdress;
    quint16 portNr = 3422; // Default
    QString fullSlaveName;
    unsigned slaveID;
    bool status = false; // Offline by default
} slave_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void setButtonFormat(unsigned b, bool onFocus);
    void slaveUpdatedStatus(unsigned index, bool status);
    void setSlaveStatus(unsigned index, bool status);
    void appendOnSlave(unsigned index, const QString& msg);
    void updateOnlineSlaves();
    bool pingSlave(slave_t *s);
public:
    void getSlaveStatus();
    void printSlaves();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void timerTimeOut();
    void statusTimerOut();

private:
    QList<slave_t*> slaveList;
    QTcpSocket *tcpSocket;

private:
    Ui::MainWindow *ui;
    QTimer *pingTimer;
    QTimer *statusTimer;
};

#endif // MAINWINDOW_H
