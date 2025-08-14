#ifndef MENUS_H
#define MENUS_H

#include "ui_mainwindow.h"
#include <QPushButton>
#include "restapi.h"
#include "mainwindow.h"
//
// INPUT
//
class InputMenu : public QObject {
    Q_OBJECT
public:
    enum InputPageType {LoginType, WithdrawType, DepositType, TransferAccountType, TransferAmountType, CreditType };
    InputMenu(MainWindow*);
    void Initialize(InputPageType);
    void HandleNumberClick();
    void HandleConfirmClick();
    void HandleDeleteClick();
    void HandleBackClick();
    void TimerEnd();
    InputPageType getType(){ return currentType; }
signals:
    void OkPressed(InputPageType, QString);
private:
    QLabel* header;
    QLabel* header2;
    MainWindow *MenuController;
    QLineEdit* textBox;
    QTimer *timer;
    InputPageType currentType=LoginType;
};
//
// MAIN
//
class MainMenu: public QObject
{
    Q_OBJECT
public:
    MainMenu(MainWindow *parent = nullptr);
    void OpenTransferMenu();
    void OpenTransactionsMenu();
    void OpenDepositMenu();
    void OpenWithdrawMenu();
    void LogOut();
    void UpdateText();
    void HandleReply(RestAPI::ReplyType);
    void OpenCreditMenu();
    void SwapAccount();
    void ResetFirstAccount(){firstAccount = true;}

private:
    MainWindow *MenuController;
    InputMenu* inputController;
    QTextBrowser* MenuBrowser;
    bool firstAccount = true;
    RestAPI* api;
};
//
// LOGIN
//
class LoginMenu : public QObject {
    Q_OBJECT
public:
    LoginMenu(MainWindow *parent = nullptr);
    void ScanCard();
    void OpenEnterPin(QString cardId);
    void OpenMainMenu();
    void ReplyHandling(RestAPI::ReplyType);
    void InputMenuReplyHandling(InputMenu::InputPageType, QString);
    void Test();
private slots:
    void onCardScanned(QString cardId);
private:
    MainWindow *MenuController;
    InputMenu* inputController;
};
//
// DEPOSIT
//
class DepositMenu : public QObject {
    Q_OBJECT
public:
    DepositMenu(MainWindow *parent =nullptr);
    void ReplyHandling(RestAPI::ReplyType);
    void InputMenuReplyHandling(InputMenu::InputPageType, QString);
private:
    MainWindow *MenuController;
    InputMenu* inputController;

};
//
// WITHDRAW
//
class WithdrawMenu: public QObject
{
    Q_OBJECT
public:
    WithdrawMenu(MainWindow *parent = nullptr);
    void ReplyHandling(RestAPI::ReplyType);
    void InputMenuReplyHandling(InputMenu::InputPageType, QString);

private:
    MainWindow *MenuController;
    InputMenu* inputController;

};
//
// CREDIT
//
class CreditMenu: public QObject
{
    Q_OBJECT
public:
    CreditMenu(MainWindow *parent = nullptr);
    void ReplyHandling(RestAPI::ReplyType);
    void InputMenuReplyHandling(InputMenu::InputPageType, QString);
private:
    MainWindow *MenuController;
    InputMenu* inputController;
};
//
// TRANSFER
//
class TransferMenu: public QObject
{
    Q_OBJECT
public:
    TransferMenu(MainWindow *parent = nullptr);
    void ReplyHandling(RestAPI::ReplyType);
    void InputMenuReplyHandling(InputMenu::InputPageType, QString);
private:
    MainWindow *MenuController;
    InputMenu *inputController;
    QString receiverNumber;
};
//
// TRANSACTIONS
//
class TransactionMenu: public QObject
{
    Q_OBJECT
public:
    TransactionMenu(MainWindow *parent = nullptr);
    void HandleReply(RestAPI::ReplyType);
    void HandleMoreButtonClick();
    void KillChildren();
    void SpawnChild(TransactionData);
    void SpawnChildren(vector<TransactionData>);
    void BackClicked();
    void RefreshClicked();
    QComboBox *filters;
    void FilterTransactions(QString);
    void FilterUpdated(QString);
    void Reset(){
        if(MenuController->GetActiveMenu() != MainWindow::TransactionsMenuPage)
            filters->setCurrentIndex(0);
        maxIndex = 10;
        currentIndex = 0;
    }

private:
    MainWindow *MenuController;
    QVBoxLayout *layoutGroup;
    QWidget *container;
    QPushButton *moreButton = nullptr;
    int currentIndex = 0;
    int maxIndex = 10;
    int maxIncrease = 10;

};
#endif // MENUS_H
