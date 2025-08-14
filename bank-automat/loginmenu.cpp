#include "menus.h"
#include "mainwindow.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

LoginMenu::LoginMenu(MainWindow *parent){
    MenuController=parent;

    inputController = MenuController->GetInputMenu();
    connect(MenuController->API, &RestAPI::ReplyReceived, this,
            &LoginMenu::ReplyHandling);

    // Using serial number to loop through COM ports to find RFID reader
    QString rfidSerial = "OL56E09005B8EBC";
    QString detectedPort = MenuController->ptrRFID->findRFIDPortBySerial(rfidSerial);

    if (!detectedPort.isEmpty()) {
        MenuController->ptrRFID->startListening(detectedPort);
    } else {
        qDebug() << "Could not find RFID reader.";
    }

    connect(MenuController->ptrRFID, &RFID_DLL::sendSignalToExe, this, &LoginMenu::onCardScanned);

    connect(inputController, &InputMenu::OkPressed, this, &LoginMenu::InputMenuReplyHandling);
    MenuController->GetUI()->TestButton->setVisible(true);
    connect(MenuController->GetUI()->TestButton, &QPushButton::clicked, this, &LoginMenu::Test);
}

void LoginMenu::onCardScanned(QString cardId){
    if (MenuController->isActiveMenu(MainWindow::FirstScreenPage)!= true)
        return;
    qDebug() << "Card detected:" << cardId;
    MenuController->API->GetCardByNumber(cardId);
}

void LoginMenu::ReplyHandling(RestAPI::ReplyType reply){
    if (!MenuController->isActiveMenu(MainWindow::FirstScreenPage)
            && !MenuController->isActiveMenu(MainWindow::InputMenuPage))
        return;

    if(MenuController->GetActiveMenu() == MainWindow::FirstScreenPage) {
        if (RestAPI::CardReply == reply){
            MenuController->SwitchMenu(MainWindow::InputMenuPage);
            inputController->Initialize(InputMenu::LoginType);
        }
    }

    if(MenuController->GetActiveMenu() == MainWindow::InputMenuPage){
        if (RestAPI::SuccessReply == reply){
            if (MenuController->API->LatestSuccess().successCode == 500){
                MenuController->SwitchMenu(MainWindow::MainMenuPage);
                MenuController->API->GetAccountByNumber(MenuController->API->card.CardAccNum1);
            }
        }
            if (RestAPI::ErrorReply == reply){
                if (MenuController->API->LatestError().errorCode == 5001){
                QMessageBox::critical(nullptr, "Error", "Wrong PIN");
                }
            }
        }
}

void LoginMenu::InputMenuReplyHandling(InputMenu::InputPageType type, QString text){
    if (type != InputMenu::LoginType)
        return;

    RestAPI* api = MenuController->API;
    api->Login(api->card.CardNumber, text);
}

void LoginMenu::Test(){
    MenuController->API->GetCardByNumber("122");
}
