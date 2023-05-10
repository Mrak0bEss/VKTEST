#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include<QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class SecondWindow; }
QT_END_NAMESPACE

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr, QTcpSocket *soc = nullptr);
    ~SecondWindow();
    void onConnected();
    void sockDisc();
    void slotReadyRead();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SecondWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
};


#endif // SECONDWINDOW_H
