#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QDialog>

#include <QSerialPort>// step1: import library

QT_BEGIN_NAMESPACE
namespace Ui { class Connector; }
QT_END_NAMESPACE

class Connector : public QDialog
{
    Q_OBJECT

public:
    Connector(QWidget *parent = nullptr);
    ~Connector();

private slots:

    void on_SendButton_clicked();
    void updateSendMsg(QString);

private:
    Ui::Connector *ui;

    QSerialPort *nodeMCU; // step2: add serial port as *nodeMCU
    static const quint16 nodeMCU_vendor_id = 4292;
    static const quint16 nodeMCU_product_id = 60000;
    QString nodeMCU_port_name;
    bool nodeMCU_is_available;
};
#endif // CONNECTOR_H
