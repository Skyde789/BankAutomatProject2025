#include "menus.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

WithdrawMenu::WithdrawMenu(MainWindow *parent){
    MenuController=parent;
    inputController = MenuController->GetInputMenu();

    connect(MenuController->API, &RestAPI::ReplyReceived, this,
            &WithdrawMenu::ReplyHandling);

    connect(inputController, &InputMenu::OkPressed,
            this, &WithdrawMenu::InputMenuReplyHandling);
}

void WithdrawMenu::ReplyHandling(RestAPI::ReplyType reply){
    if (inputController->getType() != InputMenu::WithdrawType){
        return;
    }
    if(MenuController->GetActiveMenu() == MainWindow::InputMenuPage){
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 110){
                MenuController->API->GetAccountByNumber(MenuController->API->account.AccountNumber);
                if (QMessageBox::information(nullptr, "Success", "Withdraw Successful") == QMessageBox::Ok) {
                    MenuController->SwitchMenu(MainWindow::MainMenuPage);
                }

            }
        }
    }
        if (RestAPI::ErrorReply == reply){
            qDebug() << "Error Found";
            if (RestAPI::ErrorReply == reply){
                QMessageBox::critical(nullptr, "Error", MenuController->API->LatestError().errorMessage);
                qDebug()<<"Error Found";
        }
    }
}

void WithdrawMenu::InputMenuReplyHandling(InputMenu::InputPageType type, QString text){
    if (type != InputMenu::WithdrawType)
        return;

    RestAPI* api = MenuController->API;
    api->Withdraw(api->account.AccountNumber, text.toDouble());
}

