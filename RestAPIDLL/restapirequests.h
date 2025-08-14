#ifndef RESTAPIREQUESTS_H
#define RESTAPIREQUESTS_H

#include "RESTAPIREQUESTS_global.h"
#include <QtNetwork>
#include <QJsonDocument>
#include <QNetworkAccessManager>


class RESTAPIREQUESTS_EXPORT RestAPIRequests : public QObject
{
    Q_OBJECT

public:
    RestAPIRequests();
    ~RestAPIRequests();

    // Rest API Requests
    void GetRequest(QString);
    void PostRequest(QString, QJsonObject);
    void PutRequest(QString, QJsonObject);

    void SetAuthToken(QByteArray x){authToken = x;}

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray response_data;
    QString url = "http://localhost:3000/";
    QByteArray authToken;

private slots:
    // Replies from the API
    void GetReply (QNetworkReply *reply);
signals:
    // Sends a received reply for the "RestAPI"-Class to handle it
    void SendReply(QJsonDocument reply);
};


#endif // RESTAPIREQUESTS_H
