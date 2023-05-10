#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include "secondwindow.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::sockDisc);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    ui->lineEdit->setPlaceholderText(QString("Type your login"));

}

MainWindow::~MainWindow()
{
    delete ui;
    //sockDisc();
}

void MainWindow::on_pushButton_clicked()
{

    account = ui->lineEdit->text();
    if (account!= "users" && account != "message" && account != "server")
    {

        socket->connectToHost("127.0.0.1",3333);
        SecondWindow *s = new SecondWindow(nullptr,socket);
        s->show();
        disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
        disconnect(socket, &QTcpSocket::disconnected, this, &MainWindow::sockDisc);
        close();
    } else
    {
        QMessageBox::critical(this, "Error", "This name is reserved");
    }

}
void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    //out << str;
    //socket->write(Data);
}

void MainWindow::onConnected()
{
    QByteArray block;
    QVector<QString> package(2);
    package[0] = "server";
    package[1] = account;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << package;
    socket->write(block);

}

void MainWindow::slotReadyRead()
{
    qDebug()<<"waitConnect";
    if (socket->waitForConnected(500))
    {
        qDebug()<<"connected";
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        qDebug() << Data;
    }

}
void MainWindow::sockDisc()
{
    qDebug()<<"server disconnect";
    socket->deleteLater();
}

