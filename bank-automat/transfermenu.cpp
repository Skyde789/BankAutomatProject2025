#include "menus.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

TransferMenu::TransferMenu(MainWindow *parent){
    MenuController=parent;
    inputController = MenuController->GetInputMenu();

    connect(MenuController->API, &RestAPI::ReplyReceived, this,
            &TransferMenu::ReplyHandling);

    connect(inputController, &InputMenu::OkPressed,
            this, &TransferMenu::InputMenuReplyHandling);
}

void TransferMenu::ReplyHandling(RestAPI::ReplyType reply){
    if(MenuController->GetActiveMenu() != MainWindow::InputMenuPage)
        return;

    if (inputController->getType() == InputMenu::TransferAccountType){       
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 204){
                qDebug() << "TA Right Success Code";
                inputController->Initialize(InputMenu::TransferAmountType);
            }
        }

        if (RestAPI::ErrorReply == reply){
            QMessageBox::critical(nullptr, "Error", MenuController->API->LatestError().errorMessage);
            qDebug()<<"Error Found";
        }

    }
    else if (inputController->getType() == InputMenu::TransferAmountType){       
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 120){
                MenuController->API->GetAccountByNumber(MenuController->API->account.AccountNumber);
                qDebug() << "T Right Success Code";
                if (QMessageBox::information(nullptr, "Success", "Transfer Successful") == QMessageBox::Ok) {
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

void TransferMenu::InputMenuReplyHandling(InputMenu::InputPageType type, QString text){

    RestAPI* api = MenuController->API;

    if (type == InputMenu::TransferAmountType){
        api->Transfer(receiverNumber.toInt(), api->account.AccountNumber, text.toDouble());

    }

    else if (type == InputMenu::TransferAccountType){
        api->CheckAccountExists(text.toInt());
        receiverNumber=text;
    }

}

