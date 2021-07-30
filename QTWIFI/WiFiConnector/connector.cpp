#include "connector.h"
#include "ui_connector.h"

//import library
#include <QSerialPort> // 如過這裡有錯誤，請到專案名稱檔 .pro 將 "QT += core gui" 加上 serialport，且將 qt version switch to qt5
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QtCore>
#include <QtGui>

Connector::Connector(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Connector)
{
    ui->setupUi(this);

    nodeMCU_port_name = "";
    nodeMCU_is_available = false;
    nodeMCU = new QSerialPort;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.productIdentifier() == nodeMCU_product_id)
            {
                if(serialPortInfo.vendorIdentifier() == nodeMCU_vendor_id)
                {
                    nodeMCU_port_name = serialPortInfo.portName();
                    nodeMCU_is_available = true;
                }
            }
        }
    }
    if(nodeMCU_is_available)
    {
      // open and configure serial port
      nodeMCU->setPortName(nodeMCU_port_name);
      nodeMCU->open(QSerialPort::WriteOnly);
      nodeMCU->setBaudRate(QSerialPort::Baud9600);
      nodeMCU->setDataBits(QSerialPort::Data8);
      nodeMCU->setParity(QSerialPort::NoParity);
      nodeMCU->setStopBits(QSerialPort::OneStop);
      nodeMCU->setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
      // give error message if not available
      QMessageBox::warning(this, "Port error", "Couldn't find the nodeMCU");
    }
}

Connector::~Connector()
{
    if(nodeMCU->isOpen())
    {
        nodeMCU->close();
    }
    delete ui;
}


void Connector::on_SendButton_clicked()
{
    //QMessageBox::information(this, "Title here", "hello world");
    //int value = 0;
    //value = ui->MsgNum->value();
    //ledControl::updateSendMsg(msg);
    //ledControl::updateSendMsg(QString("ssid"));
    int port = ui->Port->value();
    QString portNum = QString::number(port);
    QString wifiname = ui->WIFIname->toPlainText();
    QString wifiPassword = ui->Password ->toPlainText();
    QString IPaddress = ui->IPaddress->toPlainText();
    QString confirmHead = "confirmHead";
    QString test = confirmHead + ';' + wifiname + ':' + wifiPassword + '@' + IPaddress + '!' + portNum;
    Connector::updateSendMsg(confirmHead + ';' + wifiname + ':' + wifiPassword + '@' + IPaddress + '!' + portNum);
    qDebug() << test;
}

void Connector::updateSendMsg(QString Msg)
{
    if(nodeMCU->isWritable())
    {
        nodeMCU->write(Msg.toStdString().c_str());
        //nodeMCU->write(Msg.toUtf8().constData());
    }
    else
    {
        qDebug() << "Couldn't write to serial";
    }
}

