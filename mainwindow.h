#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
//#include "secondwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket *socket;
    QByteArray Data;
    QString account;

private slots:
    void on_pushButton_clicked();

public slots:
    void slotReadyRead();
    void sockDisc();
    void onConnected();

private:
    Ui::MainWindow *ui;
    void SendToServer(QString str);
};
#endif // MAINWINDOW_H
