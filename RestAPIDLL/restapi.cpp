#include "restapi.h"


RestAPI::RestAPI()
{
    connect(&request, &RestAPIRequests::SendReply, this, &RestAPI::HandleReply);
}

void RestAPI::GetAccountByNumber(int accNum)
{
    request.GetRequest("accounts/" + QString::number(accNum));
}

void RestAPI::CheckAccountExists(int accNum)
{
    request.GetRequest("accounts/check/" + QString::number(accNum));
}


void RestAPI::GetCustomerByID(int luku)
{
    request.GetRequest("customer/" + QString::number(luku));
}


//
// TRANSACTIONS
//
vector<TransactionData> RestAPI::FilterTransactionsByType(TransactionData::TransactionType type)
{
    vector<TransactionData> filtered;
    copy_if(transactions.begin(), transactions.end(), std::back_inserter(filtered), [type](TransactionData& x) {
        return x.type == type;
    });
    return filtered;
}

void RestAPI::GetTransactionsFromAccountNumber(int accNum)
{
    request.GetRequest("transactions/account/" + QString::number(accNum));
}

void RestAPI::Transfer(int receiver, int sender, double amount)
{
    // Creating a jsonObj and filling it with the correct data
    // same as "body" in Postman
    QJsonObject jsonObj;
    jsonObj.insert("receiver", receiver);
    jsonObj.insert("sender", sender);
    jsonObj.insert("amount", amount);
    jsonObj.insert("type", "transfer");

    request.PostRequest("transactions/transfer/", jsonObj);
}

void RestAPI::Withdraw(int account, double amount)
{
    // Creating a jsonObj and filling it with the correct data
    // same as "body" in Postman
    QJsonObject jsonObj;
    jsonObj.insert("account", account);
    jsonObj.insert("amount", amount);
    jsonObj.insert("type", "withdraw");

    request.PostRequest("transactions/withdraw/", jsonObj);
}

void RestAPI::Deposit(int account, double amount)
{
    // Creating a jsonObj and filling it with the correct data
    // same as "body" in Postman
    QJsonObject jsonObj;
    jsonObj.insert("account", account);
    jsonObj.insert("amount", amount);
    jsonObj.insert("type", "deposit");

    request.PostRequest("transactions/deposit/", jsonObj);
}
//
// CARDS / LOGIN
//
void RestAPI::GetCardByNumber(QString cardNum)
{
    request.GetRequest("cards/" + cardNum);
}


void RestAPI::Login(QString cardNumber, QString cardPin)
{
    QJsonObject jsonObj;
    jsonObj.insert("card_number", cardNumber);
    jsonObj.insert("card_pin", cardPin);

    request.PostRequest("login/", jsonObj);
}

void RestAPI::LogOut()
{
    request.SetAuthToken("");
    card = CardData();
    customer = CustomerData();
    transactions.clear();
    account = AccountData();
}

void RestAPI::UpdateCreditLimit(int account, int limit)
{
    // Creating a jsonObj and filling it with the correct data
    // same as "body" in Postman
    QJsonObject jsonObj;
    jsonObj.insert("credit_limit", limit);

    request.PutRequest("accounts/credit/" + QString::number(account), jsonObj);
}

void RestAPI::HandleReply(QJsonDocument reply)
{
    ReplyType t = NullReply;
    QJsonArray replyArray;
    QJsonObject replyObject;

    qDebug()<< reply;

    // If the reply is empty, emit a NullReply
    if(reply.isEmpty() || reply.isNull()){
        qDebug() << "Null reply";
        emit ReplyReceived(t);
        return;
    }

    // Depending on the reply, we need to parse the data differently.
    // Essentially the same thing in both of these checks, we are finding what kind of reply did we get (Account, Error etc..)
    if (reply.isArray())
    {
        replyArray = reply.array();
        for (const QString &key : replyArray[0].toObject().keys()) {
            if (key.contains("card", Qt::CaseInsensitive)) {
                t = CardReply;
                qDebug() << "Found card key";
                break;
            }
            if (key.contains("account", Qt::CaseInsensitive)) {
                t = AccountReply;
                qDebug() << "Found account key";
                break;
            }
            if (key.contains("customer", Qt::CaseInsensitive)) {
                t = CustomerReply;
                qDebug() << "Found customer key";
                break;
            }
            if (key.contains("transaction", Qt::CaseInsensitive)) {
                t = TransactionReply;
                qDebug() << "Found transaction key";
                break;
            }
            if (key.contains("error", Qt::CaseInsensitive)) {
                t = ErrorReply;
                qDebug() << "Found error key";
                break;
            }
            if (key.contains("success", Qt::CaseInsensitive)){
                t = SuccessReply;
                qDebug() << "Found success key";
                break;
            }
        }
    }
    else if (reply.isObject())
    {
        replyObject = reply.object();
        for (const QString &key : replyObject.keys()) {
            if (key.contains("card", Qt::CaseInsensitive)) {
                t = CardReply;
                qDebug() << "Found card key";
                break;
            }
            if (key.contains("account", Qt::CaseInsensitive)) {
                t = AccountReply;
                qDebug() << "Found account key";
                break;
            }
            if (key.contains("customer", Qt::CaseInsensitive)) {
                t = CustomerReply;
                qDebug() << "Found customer key";
                break;
            }

            if (key.contains("transaction", Qt::CaseInsensitive)) {
                t = TransactionReply;
                qDebug() << "Found transaction key";
                break;
            }
            if (key.contains("error", Qt::CaseInsensitive)) {
                t = ErrorReply;
                qDebug() << "Found error key";
                break;
            }
            if (key.contains("success", Qt::CaseInsensitive)){
                t = SuccessReply;
                qDebug() << "Found success key";
                break;
            }
        }
    }

    // Reply handling
    switch(t){
    case AccountReply:
        // Converting the reply into a TransactionData datatype
        if (reply.isArray()){
            AccountData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = AccountData(replyObject["account_number"].toInt(),
                                       replyObject["account_balance"].toString().toDouble(),
                                       replyObject["account_type"].toString(),
                                        replyObject["account_credit_limit"].toInt(),
                                       replyObject["account_customer_id"].toInt(),
                                       replyObject["account_state"].toString());
                account=temp;
            }
        }
        else
        {
            AccountData temp;
                temp = AccountData(replyObject["account_number"].toInt(),
                                       replyObject["account_balance"].toString().toDouble(),
                                       replyObject["account_type"].toString(),
                                        replyObject["account_credit_limit"].toInt(),
                                       replyObject["account_customer_id"].toInt(),
                                       replyObject["account_state"].toString());
                account=temp;
        }
        break;
    case CardReply:
        if (reply.isArray()){
            CardData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = CardData(replyObject["card_number"].toString(),
                                   replyObject["card_pin"].toString(),
                                   replyObject["card_owner_id"].toInt(),
                                   replyObject["card_attempts"].toInt(),
                                   replyObject["card_state"].toString(),
                                   replyObject["card_account_number"].toInt(),
                                   replyObject["card_account_number2"].toInt(),
                                   replyObject["card_type"].toString());
                card=temp;
            }
        }
        else
        {
            CardData temp;
            temp = CardData(replyObject["card_number"].toString(),
                            replyObject["card_pin"].toString(),
                            replyObject["card_owner_id"].toInt(),
                            replyObject["card_attempts"].toInt(),
                            replyObject["card_state"].toString(),
                            replyObject["card_account_number"].toInt(),
                            replyObject["card_account_number2"].toInt(),
                            replyObject["card_type"].toString());
            card=temp;
        }
        break;
    case CustomerReply:
        if (reply.isArray()){
            CustomerData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = CustomerData(replyObject["customer_id"].toInt(),
                                   replyObject["customer_fname"].toString(),
                                   replyObject["customer_lname"].toString(),
                                   replyObject["customer_address"].toString(),
                                   replyObject["customer_phone_number"].toString());
                customer=temp;
            }
        }
        else
        {
            CustomerData temp;
            temp = CustomerData(replyObject["customer_id"].toInt(),
                                replyObject["customer_fname"].toString(),
                                replyObject["customer_lname"].toString(),
                                replyObject["customer_address"].toString(),
                                replyObject["customer_phone_number"].toString());
            customer=temp;
        }
        break;
    case TransactionReply:
        // clearing the old data
        transactions.clear();

        // Converting the reply into a TransactionData datatype
        if (reply.isArray()){
            TransactionData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = TransactionData(replyObject["transaction_id"].toInt(),
                                       replyObject["transaction_receiver"].toInt(),
                                       replyObject["transaction_sender"].toInt(),
                                       replyObject["transaction_amount"].toString().toDouble(),
                                       temp.ConvertQStringToType(replyObject["transaction_type"].toString()),
                                       replyObject["transaction_date"].toString());
                transactions.push_back(temp);
            }
        }
        else
        {
            TransactionData temp;
            temp = TransactionData(replyObject["transaction_id"].toInt(),
                                   replyObject["transaction_receiver"].toInt(),
                                   replyObject["transaction_sender"].toInt(),
                                   replyObject["transaction_amount"].toString().toDouble(),
                                   temp.ConvertQStringToType(replyObject["transaction_type"].toString()),
                                   replyObject["transaction_date"].toString());
            transactions.push_back(temp);
        }
        break;
    case ErrorReply:
        // Converting the reply into a ErrorData datatype
        if (reply.isArray()){
            ErrorData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = ErrorData(replyObject["errorCode"].toInt(),
                                 replyObject["message"].toString());
                errors.push_back(temp);
            }
        }
        else
        {
            ErrorData temp;
            temp = ErrorData(replyObject["errorCode"].toInt(),
                             replyObject["message"].toString());
            errors.push_back(temp);
        }
        break;
    case NullReply:
        break;
    case SuccessReply:
        // Converting the reply into a ErrorData datatype
        if (reply.isArray()){
            SuccessData temp;
            foreach (const QJsonValue &value, replyArray) {
                replyObject = value.toObject();
                temp = SuccessData(replyObject["successCode"].toInt(),
                                 replyObject["message"].toString());
                successes.push_back(temp);
            }
        }
        else
        {
            SuccessData temp;
            temp = SuccessData(replyObject["successCode"].toInt(),
                             replyObject["message"].toString());
            successes.push_back(temp);
        }
        if(LatestSuccess().successCode == 500){
            request.SetAuthToken(LatestSuccess().successMessage.toUtf8());
        }
        break;
    }

    // finally emit the received reply type
    // Listening to this event ensures that the data is actually updated.
    emit ReplyReceived(t);
}
