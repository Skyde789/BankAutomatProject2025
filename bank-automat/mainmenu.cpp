#include "menus.h"
#include "mainwindow.h"

MainMenu::MainMenu(MainWindow *parent){
    MenuController=parent;
    inputController=MenuController->GetInputMenu();
    api = MenuController->API;
    auto ui= MenuController->GetUI();

    connect(ui->transfer, &QPushButton::clicked, this, &MainMenu::OpenTransferMenu);
    connect(ui->transactions, &QPushButton::clicked, this, &MainMenu::OpenTransactionsMenu);
    connect(ui->deposit, &QPushButton::clicked, this, &MainMenu::OpenDepositMenu);
    connect(ui->withdraw, &QPushButton::clicked, this, &MainMenu::OpenWithdrawMenu);
    connect(ui->logout, &QPushButton::clicked, this, &MainMenu::LogOut);
    connect(MenuController->API, &RestAPI::ReplyReceived, this, &MainMenu::HandleReply);
    connect(ui->CreditLimit, &QPushButton::clicked, this, &MainMenu::OpenCreditMenu);
    connect(ui->nextAccount, &QPushButton::clicked, this, &MainMenu::SwapAccount);
    connect(ui->previousAccount, &QPushButton::clicked, this, &MainMenu::SwapAccount);

    MenuBrowser = ui->MenuBrowser;
    MenuBrowser->setAlignment(Qt::AlignCenter);

    ui->nextAccount->setVisible(false);
    ui->previousAccount->setVisible(false);
    ui->CreditLimit->setVisible(false);
}

void MainMenu::HandleReply(RestAPI::ReplyType MenuReply){
    if (MenuReply== RestAPI::AccountReply)
    {
        api->GetTransactionsFromAccountNumber(MenuController->API->account.AccountNumber);
        UpdateText();
        if(firstAccount && api->card.CardType == "double"){
            MenuController->GetUI()->nextAccount->setVisible(true);
        }

        if(api->card.CardType == "credit"){
            MenuController->GetUI()->CreditLimit->setVisible(true);
        }
        else
        {
            MenuController->GetUI()->CreditLimit->setVisible(false);
        }
    }
}

void MainMenu::OpenTransferMenu(){
    MenuController->SwitchMenu(MainWindow::InputMenuPage);
    inputController->Initialize(InputMenu::TransferAccountType);

}
void MainMenu::OpenTransactionsMenu(){
    MenuController->SwitchMenu(MainWindow::TransactionsMenuPage);

}
void MainMenu::OpenDepositMenu(){
    MenuController->SwitchMenu(MainWindow::InputMenuPage);
    inputController->Initialize(InputMenu::DepositType);
}
void MainMenu::OpenWithdrawMenu(){
    MenuController->SwitchMenu(MainWindow::InputMenuPage);
    inputController->Initialize(InputMenu::WithdrawType);
}
void MainMenu::LogOut(){
    MenuController->LogOut();
}

void MainMenu::OpenCreditMenu(){
    MenuController->SwitchMenu(MainWindow::InputMenuPage);
    inputController->Initialize(InputMenu::CreditType);
}

void MainMenu::UpdateText(){
    QString MenuText;
    MenuText += "<div align='center'>";
    MenuText += "Account: " + QString::number(api->account.AccountNumber);
    MenuText += "<br> Balance: " + QString::number(api->account.AccountBalance);
    if (api->account.AccountType == "credit"){
        MenuText += "<br> Credit Limit: " + QString::number(api->account.AccountCreditLimit);
    }
    MenuText += "</div>";
    MenuBrowser->setText(MenuText);
}

void MainMenu::SwapAccount(){
    if(firstAccount){
        firstAccount = false;
        api->GetAccountByNumber(api->card.CardAccNum2);
        MenuController->GetUI()->nextAccount->setVisible(false);
        MenuController->GetUI()->previousAccount->setVisible(true);
    }
    else
    {
        firstAccount = true;
        api->GetAccountByNumber(api->card.CardAccNum1);
        MenuController->GetUI()->nextAccount->setVisible(true);
        MenuController->GetUI()->previousAccount->setVisible(false);
    }
}


