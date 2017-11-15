#include <QPushButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static unsigned nrSlaves = 4;
static unsigned currentIndex = 0;
static QString currentSheet = "background-color: rgb(255, 255, 255);border: 3px solid;border-color: rgb(52, 101, 164);";
static QString notCurrentSheet = "background-color: rgb(255, 255, 255);";

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

void MainWindow::slaveUpdatedStatus(unsigned index, bool status)
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentIndex = 0;
    setButtonFormat(0, true);
    setButtonFormat(1, false);
    setButtonFormat(2, false);
    setButtonFormat(3, false);

    slaveList.append(new slave_t(QString("192.168.1.179"), 3422, QString("Raspberry Pi 3"), false));
    slaveList.append(new slave_t(QString("192.168.1.102"), 3422, QString("BeagleBone Black Wireless"), false));
    slaveList.append(new slave_t(QString("192.168.1.179"), 3422, QString("Wandboard Dual Rev. D1"), false));
    slaveList.append(new slave_t(QString("192.168.1.203"), 3422, QString("Tinker Board"), false));

    // Perform the address scanning on port 3422 to see which are online

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
