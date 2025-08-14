#ifndef RFID_DLL_H
#define RFID_DLL_H

#include "RFID_DLL_global.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>

class RFID_DLL_EXPORT RFID_DLL : public QObject
{
    Q_OBJECT
public:
    RFID_DLL(QObject * parent = nullptr);
    ~RFID_DLL();
    void startListening(const QString &portName);
    QString findRFIDPortBySerial(const QString& targetSerialNumber);

    QSerialPort * port;

private slots:
    void handleSerialData();

signals:
    void sendSignalToExe(QString); //Emits the card ID
};

#endif // RFID_DLL_H
