#include "restapirequests.h"
#include <QDebug>

using namespace std;

RestAPIRequests::RestAPIRequests() {
    manager = new QNetworkAccessManager(this);
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &RestAPIRequests::GetReply);
}

RestAPIRequests::~RestAPIRequests() { }

void RestAPIRequests::GetRequest(QString requestURL)
{
    QString site_url=url + requestURL;
    QNetworkRequest request(site_url);

    QByteArray myToken="Bearer "+authToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    reply = manager->get(request);
}

void RestAPIRequests::PostRequest(QString requestURL, QJsonObject jsonbObj)
{

    QString site_url=url + requestURL;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer "+authToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    reply = manager->post(request, QJsonDocument(jsonbObj).toJson());
}

void RestAPIRequests::PutRequest(QString requestURL, QJsonObject jsonbObj)
{
    QString site_url=url + requestURL;
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray myToken="Bearer "+authToken;
    request.setRawHeader(QByteArray("Authorization"),(myToken));

    reply = manager->put(request, QJsonDocument(jsonbObj).toJson());
}

void RestAPIRequests::GetReply(QNetworkReply *reply)
{
    response_data=reply->readAll();
    //qDebug()<<"DATA : "+response_data;
    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);

    emit SendReply(json_doc);

    reply->deleteLater();
    //manager->deleteLater();
}
