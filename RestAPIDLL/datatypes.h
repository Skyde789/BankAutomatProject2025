#ifndef DATATYPES_H
#define DATATYPES_H

#include "DATATYPES_global.h"
#include "qdatetime.h"
#include <qstring.h>
#include <QMetaEnum>

struct DATATYPES_EXPORT SuccessData{
public:
    SuccessData(){}
    SuccessData(int code, QString msg){
        successCode = code;
        successMessage = msg;
    }

    int successCode;
    QString successMessage;

    QString ToString(){
        return "| success_code: " +  QString::number(successCode) +
               " | success_message: " + successMessage;
    }
};

struct DATATYPES_EXPORT ErrorData{
    public:
        ErrorData(){}
        ErrorData(int code, QString msg){
            errorCode = code;
            errorMessage = msg;
        }

        int errorCode;
        QString errorMessage;

        QString ToString(){
            return "| error_code: " +  QString::number(errorCode) +
                   " | error_message: " + errorMessage;
        }
};

struct DATATYPES_EXPORT TransactionData
{
public:
    enum TransactionType { DepositTransaction = 0, WithdrawTransaction = 1, TransferTransaction = 2 };

    // Constructors
    TransactionData(){}
    TransactionData(int id, int receiver, int sender, double am, TransactionType t, QString d)
    {
        ID = id;
        receiverNumber = receiver;
        senderNumber= sender;
        amount = am;
        type = t;

        QDateTime dt = QDateTime::fromString(d, Qt::ISODate);

        dt = dt.toLocalTime();

        date = dt.toString("dd.MM.yyyy");
        detailedDate = dt.toString("dd.MM.yyyy hh:mm:ss");
    }

    ~TransactionData(){}

    // Transaction Table datas
    int ID;
    int receiverNumber;
    int senderNumber;
    double amount;

    TransactionType type;
    QString date;
    QString detailedDate;

    QString lol[3] = {"Deposit", "Withdraw", "Transfer"};

    QString ToString(){
        return "| transaction_id: " +  QString::number(ID) +
               " | transaction_receiver: " + QString::number(receiverNumber) +
               " | transaction_sender: " + QString::number(senderNumber) +
               " | transaction_amount: " + QString::number(amount) +
               " | transaction_type: " + TypeToString(type) +
               " | transaction_date: " + date;
    }

    QString TypeToString(TransactionType t) { return lol[t]; }

    TransactionType ConvertQStringToType(QString text)
    {
        TransactionType t = DepositTransaction;

        if(text.contains("deposit", Qt::CaseInsensitive)){
            t = DepositTransaction;
        }
        else if(text.contains("withdraw", Qt::CaseInsensitive)){
            t = WithdrawTransaction;
        }
        else if(text.contains("transfer", Qt::CaseInsensitive)){
            t = TransferTransaction;
        }

        return t;
    }

};

struct DATATYPES_EXPORT AccountData{
public:
    AccountData(){}
    AccountData(int number, double balance, QString type, int limit, int id, QString state){
        AccountNumber = number;
        AccountBalance = balance;
        AccountType = type;
        AccountCreditLimit = limit;
        AccountCustomerID = id;
        AccountState = state;
    }

    int AccountNumber;
    double AccountBalance;
    QString AccountType;
    int AccountCreditLimit;
    int AccountCustomerID;
    QString AccountState;

    QString ToString(){
        return "| account_number: " +  QString::number(AccountNumber) +
               " | account_balance: " + QString::number(AccountBalance) +
               " | account_type: " + AccountType +
               " | account_credit_limit: " + QString::number(AccountCreditLimit) +
               " | account_customer_id: " + QString::number(AccountCustomerID) +
               " | account_state: " + AccountState;
    }
};

struct DATATYPES_EXPORT CustomerData{
public:
    CustomerData(){}
    CustomerData(int id, QString fname, QString lname, QString address, QString phonenumber){

        CustomerID = id;
        CustomerFname = fname;
        CustomerLname = lname;
        CustomerAddress = address;
        CustomerPhoneNumber = phonenumber;
    }

    int CustomerID;
    QString CustomerFname;
    QString CustomerLname;
    QString CustomerAddress;
    QString CustomerPhoneNumber;

    QString ToString(){
        return "| customer_id: " +  QString::number(CustomerID) +
               " | customer_fname: " + CustomerFname +
               " | customer_lname: " + CustomerLname +
               " | customer_address: " + CustomerAddress +
               " | customer_phone_number: " + CustomerPhoneNumber ;
    }
};
struct DATATYPES_EXPORT CardData{
public:
    CardData(){}
    CardData(QString number, QString pin, int ownerId, int attempts, QString state, int accnum1, int accnum2, QString type){
        CardNumber = number;
        CardPin = pin;
        CardOwnerId = ownerId;
        CardAttempts = attempts;
        CardState = state;
        CardAccNum1 = accnum1;
        CardAccNum2 = accnum2;
        CardType = type;
    }

    QString CardNumber;
    QString CardPin;
    int CardOwnerId;
    int CardAttempts;
    QString CardState;
    int CardAccNum1;
    int CardAccNum2;
    QString CardType;

    QString ToString(){
        return "| card_number: " +  CardNumber +
               " | card_pin: " + CardPin +
               " | card_owner_id: " + QString::number(CardOwnerId) +
               " | card_attempts: " + QString::number(CardAttempts) +
               " | card_state: " + CardState +
               " | card_account_number1: " + QString::number(CardAccNum1) +
               " | card_account_number2: " + QString::number(CardAccNum2) +
               " | card_type: " + CardType;
    }
};
#endif // DATATYPES_H
