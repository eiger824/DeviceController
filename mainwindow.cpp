#include <QWidget>
#include <QPlainTextEdit>
#include <QDateTime>
#include <QList>
#include <QDebug>

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

static unsigned nrSlaves = 4;
static unsigned currentIndex = 0;
static QString currentSheet = "background-color: rgb(255, 255, 255);border: 3px solid;border-color: rgb(52, 101, 164);";
static QString notCurrentSheet = "background-color: rgb(255, 255, 255);";
static QList<slave_t *> onlineHosts;

void MainWindow::setButtonFormat(unsigned b, bool onFocus)
{
    if (b >= nrSlaves)
        return;
    switch (b)
    {
    case 0:
    {
        QPixmap pixmap("./rpi.jpg");
        QIcon icon(pixmap);
        ui->pushButton->setIcon(icon);
        QSize s(pixmap.rect().size());
        QSize bs(ui->pushButton->rect().size());
        s.setHeight(61);
        s.setWidth(120);
        ui->pushButton->setIconSize(s);
        if (onFocus)
            ui->pushButton->setStyleSheet(currentSheet);
        else
            ui->pushButton->setStyleSheet(notCurrentSheet);
        break;
    }
    case 1:
    {
        QPixmap pixmap("./bbb.jpe");
        QIcon icon(pixmap);
        ui->pushButton_2->setIcon(icon);
        QSize s(pixmap.rect().size());
        QSize bs(ui->pushButton_2->rect().size());
        s.setHeight(61);
        s.setWidth(120);
        ui->pushButton_2->setIconSize(s);
        if (onFocus)
            ui->pushButton_2->setStyleSheet(currentSheet);
        else
            ui->pushButton_2->setStyleSheet(notCurrentSheet);
        break;
    }
    case 2:
    {
        QPixmap pixmap("./wbd.jpe");
        QIcon icon(pixmap);
        ui->pushButton_3->setIcon(icon);
        QSize s(pixmap.rect().size());
        QSize bs(ui->pushButton_3->rect().size());
        s.setHeight(61);
        s.setWidth(120);
        ui->pushButton_3->setIconSize(s);
        if (onFocus)
            ui->pushButton_3->setStyleSheet(currentSheet);
        else
            ui->pushButton_3->setStyleSheet(notCurrentSheet);
        break;
    }
    default:
    {
        QPixmap pixmap("./tkb.jpe");
        QIcon icon(pixmap);
        ui->pushButton_4->setIcon(icon);
        QSize s(pixmap.rect().size());
        QSize bs(ui->pushButton_4->rect().size());
        s.setHeight(61);
        s.setWidth(120);
        ui->pushButton_4->setIconSize(s);
        if (onFocus)
            ui->pushButton_4->setStyleSheet(currentSheet);
        else
            ui->pushButton_4->setStyleSheet(notCurrentSheet);
        break;
    }
    }
}

void MainWindow::setSlaveStatus(unsigned index, bool status)
{
    if (index >= slaveList.size()) return;

    slave_t *slave = slaveList.at(index);
    slave->status = status;

    // Depending on the status change corresponding label
    switch (index)
    {
    case 0:
    {
        if (status)
            ui->label->setStyleSheet("background-color: rgb(0, 255, 0);");
        else
            ui->label->setStyleSheet("background-color: rgb(255, 0, 0);");
        break;
    }
    case 1:
    {
        if (status)
            ui->label_2->setStyleSheet("background-color: rgb(0, 255, 0);");
        else
            ui->label_2->setStyleSheet("background-color: rgb(255, 0, 0);");
        break;
    }
    case 2:
    {
        if (status)
            ui->label_3->setStyleSheet("background-color: rgb(0, 255, 0);");
        else
            ui->label_3->setStyleSheet("background-color: rgb(255, 0, 0);");
        break;
    }
    default:
    {
        if (status)
            ui->label_4->setStyleSheet("background-color: rgb(0, 255, 0);");
        else
            ui->label_4->setStyleSheet("background-color: rgb(255, 0, 0);");
        break;
    }
    }
}

void MainWindow::printSlaves()
{
    for (unsigned i=0; i<slaveList.size(); ++i)
    {
        slave_t *s = slaveList.at(i);
        appendOnSlave(i, "Host " + s->fullSlaveName
                            + " (" + s->ipAdress + ") is "
                            + (s->status? "online" : "offline"));
    }
}

void MainWindow::appendOnSlave(unsigned index, const QString &msg)
{
    QPlainTextEdit *pt;
    if (index == 0) pt = ui->plainTextEdit;
    else if (index == 1) pt = ui->plainTextEdit_2;
    else if (index == 2) pt = ui->plainTextEdit_3;
    else if (index == 3) pt = ui->plainTextEdit_4;
    pt->appendPlainText("[" + QDateTime::currentDateTime().toString() + "] " + msg);
}

void MainWindow::updateOnlineSlaves()
{
    // Clear online hosts
    onlineHosts.clear();
    for (unsigned i=0; i<slaveList.size(); ++i)
    {
        slave_t* s = slaveList.at(i);
        if (s->status) onlineHosts.push_back(s);
    }
}

bool MainWindow::pingSlave(slave_t *s)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(s->ipAdress, s->portNr);
    // Wait at most 200 ms
    tcpSocket->waitForConnected(200);
    char data[4];
    data[0] = s->slaveID; // Host ID, destination
    data[1] = 0x00; // HP host, source
    data[1] = 0x1a; // Ping
    data[2] = 0x00;
    tcpSocket->write(data, 3);
    if (!tcpSocket->waitForBytesWritten(1000))
        return false;

    // Read Pong
    char rsp[4];
    quint64 nb = tcpSocket->read(rsp, 3);
    rsp[nb] = 0x00;
    // Check if valid
    {
        return false;
    }
    qDebug() << "Destination ID (0x00) correct";
    if (rsp[1] != s->slaveID)
    {
        return false;
    }
    qDebug() << "Source ID (" + QString::number(s->slaveID) + ") correct";
    if (rsp[2] != 0x1b)
    {
        return false;
    }
    qDebug() << "Pong command received";
    return true;
}

void MainWindow::getSlaveStatus()
{
    // Do the address scanning here
    for (unsigned i=0; i<slaveList.size(); ++i)
    {
        cerr << "Connecting to : " << slaveList.at(i)->ipAdress.toStdString();
        tcpSocket->abort();
        tcpSocket->connectToHost(slaveList.at(i)->ipAdress,
                                 slaveList.at(i)->portNr);
        // Wait at most half second
       tcpSocket->waitForConnected(100);
       quintptr sockfd = tcpSocket->socketDescriptor();
       if (sockfd != -1)
       {
           cerr << " [ok]" << endl;
       }
       else
       {
           cerr << " [fail]" << endl;
       }
        setSlaveStatus(i, ((sockfd != -1) ? true : false));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (currentIndex != 0)
    {
        currentIndex = 0;
        setButtonFormat(0, true);
        setButtonFormat(1, false);
        setButtonFormat(2, false);
        setButtonFormat(3, false);
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (currentIndex != 1)
    {
        currentIndex = 1;
        setButtonFormat(0, false);
        setButtonFormat(1, true);
        setButtonFormat(2, false);
        setButtonFormat(3, false);
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if (currentIndex != 2)
    {
        currentIndex = 2;
        setButtonFormat(0, false);
        setButtonFormat(1, false);
        setButtonFormat(2, true);
        setButtonFormat(3, false);
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if (currentIndex != 3)
    {
        currentIndex = 3;
        setButtonFormat(0, false);
        setButtonFormat(1, false);
        setButtonFormat(2, false);
        setButtonFormat(3, true);
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MainWindow::timerTimeOut()
{
    // Ping to all online hosts
    for (unsigned i=0; i<onlineHosts.size(); ++i)
    {
        slave_t *s = onlineHosts.at(i);
        appendOnSlave(i, "About to ping " + s->fullSlaveName);
        if (pingSlave(s))
        {
            appendOnSlave(i, "Pong received from ("
                          + s->fullSlaveName + "): SUCCESS");
        }
        else
        {
            appendOnSlave(i, "ERROR encountered when sending PING");
        }
    }
}

void MainWindow::statusTimerOut()
{
    // Query states
    getSlaveStatus();
    // Print the (maybe) updated statuses
    printSlaves();

    updateOnlineSlaves();
    qDebug() << "Online hosts: " << onlineHosts.size();
    if (onlineHosts.size() == slaveList.size())
    {
        if (statusTimer->isActive())
            statusTimer->stop();
    }
    else
    {
        // Check if at least one connected to start the ping-pong
        if (!onlineHosts.isEmpty())
        {
            if (!pingTimer->isActive())
                pingTimer->start(10000);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    tcpSocket(new QTcpSocket(this)),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentIndex = 0;
    setButtonFormat(0, true);
    setButtonFormat(1, false);
    setButtonFormat(2, false);
    setButtonFormat(3, false);

    slaveList.append(new slave_t(QString("192.168.1.179"), 3422, QString("Raspberry Pi 3"), 1, false));
    slaveList.append(new slave_t(QString("192.168.1.102"), 3422, QString("BeagleBone Black Wireless"), 2, false));
    slaveList.append(new slave_t(QString("192.168.1.179"), 3422, QString("Wandboard Dual Rev. D1"), 3, false));
    slaveList.append(new slave_t(QString("192.168.1.203"), 3422, QString("Tinker Board"), 4, false));

    pingTimer = new QTimer(this);
    connect(pingTimer, SIGNAL(timeout()),
            this, SLOT(timerTimeOut()));

    statusTimer = new QTimer(this);
    connect(statusTimer, SIGNAL(timeout()),
            this, SLOT(statusTimerOut()));

    // Status timer will start first until at least one slave is up
    statusTimer->start(5000);
}
