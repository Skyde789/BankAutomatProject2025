#ifndef RESTAPI_H
#define RESTAPI_H

#include "RESTAPI_global.h"
#include "restapirequests.h"
#include "datatypes.h"
#include <vector>
#include <algorithm>

using namespace std;

class RESTAPI_EXPORT RestAPI : public QObject
{
    Q_OBJECT


public:
    RestAPI();

    enum ReplyType { CardReply, AccountReply, CustomerReply, TransactionReply, ErrorReply, SuccessReply, NullReply};

    RestAPIRequests request;

    vector<TransactionData> transactions;
    vector<ErrorData> errors;
    vector<SuccessData> successes;
    AccountData account;
    CustomerData customer;
    CardData card;

    // GET Pyynnöt
    void GetAccountByNumber(int);
    void CheckAccountExists(int);
    void GetTransactionsFromAccountNumber(int);
    void GetCustomerByID (int);
    void GetCardByNumber(QString);

    // POST Pyynnöt
    void Transfer(int, int, double);
    void Withdraw(int, double);
    void Deposit(int, double);
    void Login(QString, QString);
    void LogOut();

    // PUT Pyynnöt
    void UpdateCreditLimit(int, int);

    // Helper / getter funktiot
    /// Returns a vector filled with transactions of a given type.
    vector<TransactionData> FilterTransactionsByType(TransactionData::TransactionType);
    ErrorData LatestError() { return errors.back(); }
    SuccessData LatestSuccess() { return successes.back(); }


signals:
    /// Event that triggers after the request has been handled
    /// Passes
    void ReplyReceived(ReplyType);
private slots:
    /// Listens to RestAPIRequests and handles the reply
    /// This also updates all of the data vectors based on the reply
    void HandleReply(QJsonDocument);
};

#endif // RESTAPI_H
