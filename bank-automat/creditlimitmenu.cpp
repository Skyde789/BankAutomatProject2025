#include "menus.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

CreditMenu::CreditMenu(MainWindow *parent){
    MenuController=parent;
    inputController = MenuController->GetInputMenu();

    connect(MenuController->API, &RestAPI::ReplyReceived, this,
            &CreditMenu::ReplyHandling);

    connect(inputController, &InputMenu::OkPressed,
            this, &CreditMenu::InputMenuReplyHandling);
}

void CreditMenu::ReplyHandling(RestAPI::ReplyType reply){
    if (inputController->getType() != InputMenu::CreditType){
        return;
    }
    if(MenuController->GetActiveMenu() == MainWindow::InputMenuPage){
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 203){
                MenuController->API->GetAccountByNumber(MenuController->API->account.AccountNumber);
                if (QMessageBox::information(nullptr, "Success", "Credit Limit Updated") == QMessageBox::Ok) {
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

void CreditMenu::InputMenuReplyHandling(InputMenu::InputPageType type, QString text){
    if (type != InputMenu::CreditType)
        return;

    RestAPI* api = MenuController->API;
    api->UpdateCreditLimit(api->account.AccountNumber, text.toInt());
    qDebug() << "Testi";
}

