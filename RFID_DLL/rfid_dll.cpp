#include "rfid_dll.h"
#include <QDebug>

RFID_DLL::RFID_DLL(QObject * parent) : QObject(parent)
{
    qDebug()<<"RFID DLL created";
    port = new QSerialPort(this);
}

RFID_DLL::~RFID_DLL()
{
    qDebug()<<"RFID DLL destroyed";
}

void RFID_DLL::startListening(const QString &portName) {
    if (!port) {
        port = new QSerialPort(this);
    }

    // If already connected, disconnect first to prevent scanning twice
    if (port->isOpen()) {
        port->close();
        qDebug() << "Existing port closed.";
    }

    // Disconnect any previous connections
    disconnect(port, &QSerialPort::readyRead, this, &RFID_DLL::handleSerialData);

    port->setPortName(portName);
    port->setBaudRate(QSerialPort::Baud9600);

    if (port->open(QIODevice::ReadOnly)) {
        qDebug() << "RFID reader connected to" << portName;
        connect(port, &QSerialPort::readyRead, this, &RFID_DLL::handleSerialData);
    } else {
        qDebug() << "Failed to open port:" << portName << "-" << port->errorString();
    }
}

void RFID_DLL::handleSerialData() {
    QByteArray data = port->readAll();
    QString cardId = QString(data).trimmed();

    //qDebug() << "RFID read:" << cardId;

    if (!cardId.isEmpty()) {
        cardId = cardId.remove("-").remove("\r").remove("\n").remove(">");
        emit sendSignalToExe(cardId);
    }
}
    //This finds the COM port with the RFID reader and outputs the port number
QString RFID_DLL::findRFIDPortBySerial(const QString& targetSerialNumber) {
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : serialPortInfos) {
        qDebug() << "Checking port:" << info.portName()
        << "with serial:" << info.serialNumber();

        if (info.serialNumber() == targetSerialNumber) {
            return info.portName();
        }
    }

    qDebug() << "RFID reader not found!";
    return QString(); // Not found
}

