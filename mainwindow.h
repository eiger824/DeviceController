#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

typedef struct Slave
{
    Slave(QString, quint16, QString, bool) {}
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
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    QList<slave_t*> slaveList;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
