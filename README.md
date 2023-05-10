# VKTEST
# Клиент - серверное приложение
# Описание:
Это MVP так сказать) реализовывать я буду более адекватный функционал, но пока то, что успел.

# Сначала о сервере:
его работа реализована так что всех пользователей и их сокеты мы закидываем в мапу, где потом с ними и работаем. 
```cpp
  QMap<QTcpSocket*, QString> clients;
```
При получении какого то сообщения (в том числе и при подключении) пользователь отправляет пакет из двух строк, тот кому мы отправляем и сам текст. При подключении для server отправляется логин пользователя. Если же мы отправляем не server, а другого адресата, то он ищется в мапе и ему отправляется сообщение. Это основной функционал сервера.
```cpp
void Server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
        QByteArray data = socket->readAll();
       
        QVector<QString> package(2);
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_15);
        in >> package;
        qDebug() << "Received message of "<<socket->socketDescriptor() <<" for" << package[0] << ":" << package[1];
        if (clients[socket] == "")
        {
            clients[socket] = package[1];
        } else
            if (package[0] !="srever")
        {
            //отправка сообщения по логину
            QString recipient = package[0];
            package[0] = clients[socket];
            //QString message = package[1];
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (it.value() == recipient)
                {
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_5_15);
                    out << package;
                    it.key()->write(block);
                    break;
                }
            }
        }

}
```
# Теперь о клиенте:
При запуске клиента открывается основное окно для ввода логина. Введя логин и нажав кнопку enter происходит попытка подключения к серверу. В это время мы отправляем на сервер логин, и если все хорошо то переходим на окно чата. иначе переписываем логин и повторяем попытку входа.
```cpp
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
```
В окне secondwindow у нас появляется поле ввода сообщения, адресата, и text browser  для отображения переписки. Отправка и получение сообщений происходит следующим образом: получаем пакет из двух строк, того, кто отправил или кому мы отправляем и сами данные для отправки, и выводим в text browser соответвующее сообщение.
```cpp
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
```

При отключении клиента, на сервере выходит соответсвующее сообщение и наоборот.
```cpp
void SecondWindow::sockDisc()
{
    ui->textBrowser->append("Server out");
    socket->deleteLater();
}
void Server::sockDisc()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    qDebug()<<clients[socket]<<" is out";
    clients.remove(socket);
    socket->deleteLater();

}
```
