#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QMainWindow>
#include <QTimer>

typedef struct Slave
{
    Slave(QString ip, quint16 port,
          QString name, bool stat):
        ipAdress(ip), portNr(port),
        fullSlaveName(name), status(stat)
    {}

    QString ipAdress;
    quint16 portNr = 3422; // Default
    QString fullSlaveName;
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
    void setSlaveStatus(unsigned slave, bool status);
    void printSlaves();
public:
    void getSlaveStatus();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void timerTimeOut();

private:
    QList<slave_t*> slaveList;
    QTcpSocket *tcpSocket;

private:
    Ui::MainWindow *ui;
    QTimer *pingTimer;
};

#endif // MAINWINDOW_H
