#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

QSerialPort *serialport;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->text() = "";
    serialport = new QSerialPort(this);

    //Config for COM port
    serialport->setPortName("ttyUSB0");
    serialport->setBaudRate(QSerialPort::Baud115200);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setFlowControl(QSerialPort::NoFlowControl);

    if(serialport->open(QIODevice::ReadWrite) != true){
        qDebug("Error open");
        return ;
    }
    serialport->clear();
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::button_handler);
    connect(serialport, &QSerialPort::readyRead, this, &MainWindow::serialPort_Read);
}

MainWindow::~MainWindow()
{
    delete ui;
    serialport->close();
    delete serialport;
}

void MainWindow::button_handler(void)
{
    serialport->write(ui->lineEdit->text().toStdString().data());
    //serialport->write("Hello World.");
    ui->label->setEnabled(1);
    ui->label->setText("Message sent ");
}

void MainWindow::serialPort_Read()
{
    auto available_bytes = serialport->bytesAvailable();
    if (!available_bytes) {
        return;
    }
    std::vector<char> temp(available_bytes);
    serialport->read(temp.data(), available_bytes);
    std::copy(temp.begin(), temp.end(), std::back_inserter(buffer));
    auto it = std::find(buffer.begin(), buffer.end(), '.');
    if (it != std::end(buffer)) {
        QString message;
        std::copy(buffer.begin(), it + 1, std::back_inserter(message));
        ui->label->setText(message);
    }

}
