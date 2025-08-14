#include "menus.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

DepositMenu::DepositMenu(MainWindow *parent){
    MenuController=parent;
    inputController = MenuController->GetInputMenu();

    connect(MenuController->API, &RestAPI::ReplyReceived,
            this, &DepositMenu::ReplyHandling);

    connect(inputController, &InputMenu::OkPressed,
            this, &DepositMenu::InputMenuReplyHandling);
}

void DepositMenu::ReplyHandling(RestAPI::ReplyType reply){
    if (inputController->getType() != InputMenu::DepositType)
        return;

    if(MenuController->GetActiveMenu() == MainWindow::InputMenuPage){
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 100){
                MenuController->API->GetAccountByNumber(MenuController->API->account.AccountNumber);
                if (QMessageBox::information(nullptr, "Success", "Deposit Successful") == QMessageBox::Ok) {
                    MenuController->SwitchMenu(MainWindow::MainMenuPage);
                }
            }
        }
        if (RestAPI::ErrorReply == reply){
            QMessageBox::critical(nullptr, "Error", MenuController->API->LatestError().errorMessage);
            qDebug()<<"Error Found";
        }
    }
}

void DepositMenu::InputMenuReplyHandling(InputMenu::InputPageType type, QString text){
    if (type != InputMenu::DepositType)
        return;

    RestAPI* api = MenuController->API;
    api->Deposit(api->account.AccountNumber, text.toDouble());
}


