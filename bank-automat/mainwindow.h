#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "restapi.h"
#include "rfid_dll.h"
#include <QDebug>

class MainMenu;
class InputMenu;
class LoginMenu;
class DepositMenu;
class WithdrawMenu;
class TransferMenu;
class CreditMenu;
class TransactionMenu;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RestAPI *API;

    RFID_DLL * ptrRFID;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum Menu{  FirstScreenPage=0,
                InputMenuPage=1,
                MainMenuPage=2,
                TransactionsMenuPage=3,
                TransferMenuPage=4,
    };

    void SwitchMenu(Menu);
    auto GetUI(){return ui;}
    auto GetInputMenu() { return input; }
    bool isActiveMenu(Menu);
    Menu GetActiveMenu();
    void LogOut();

private:
    Ui::MainWindow *ui;

    MainMenu *main;

    LoginMenu *login;
    InputMenu *input;
    DepositMenu *deposit;
    WithdrawMenu *withdraw;
    TransferMenu *transfer;
    CreditMenu *credit;
    TransactionMenu *transaction;
};
#endif // MAINWINDOW_H
