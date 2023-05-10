#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent,QTcpSocket *soc) :
    QDialog(parent),
    ui(new Ui::SecondWindow)

{
    ui->setupUi(this);
    socket = soc;
    ui->lineEdit->setPlaceholderText(QString("Enter message"));
    ui->lineEdit_2->setPlaceholderText(QString("Enter receiver"));
    //connect(socket, &QTcpSocket::connected, this, &SecondWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &SecondWindow::sockDisc);
    connect(socket, &QTcpSocket::readyRead, this, &SecondWindow::slotReadyRead);
}

SecondWindow::~SecondWindow()
{
    sockDisc();
    delete ui;
}

void SecondWindow::on_pushButton_clicked()
{

    QByteArray block;
    QVector<QString> package(2);
    package[1] = ui->lineEdit->text();
    package[0] = ui->lineEdit_2->text();
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    if (out.status() == QDataStream::Ok)
    {
        out << package;
        socket->write(block);
        ui->textBrowser->append(package[0]+":"+package[1]+"(send)");
    } else
    {
        ui->textBrowser->append(package[0]+":"+package[1]+"(error)");
    }
    ui->lineEdit->clear();
}

void SecondWindow::slotReadyRead()
{
    QByteArray data = socket->readAll();
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);
    QVector<QString> message;
    in >> message;
    ui->textBrowser->append(message[0]+":"+message[1]+"(received)");


}

void SecondWindow::sockDisc()
{
    ui->textBrowser->append("Server out");
    socket->deleteLater();
}
