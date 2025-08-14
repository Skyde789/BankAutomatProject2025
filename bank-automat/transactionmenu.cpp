#include "menus.h"
#include "mainwindow.h"
#include "expandablebutton.h"
TransactionMenu::TransactionMenu(MainWindow *parent){
    MenuController=parent;
    auto ui= MenuController->GetUI();

    filters = ui->transactionFilter;
    container = ui->scrollArea->widget();
    layoutGroup  = new QVBoxLayout(container);
    layoutGroup->setAlignment(Qt::AlignTop);
    container->setLayout(layoutGroup);

    filters->addItem("All");
    filters->addItem("Deposit");
    filters->addItem("Withdraw");
    filters->addItem("Transfer");

    connect(filters, &QComboBox::currentTextChanged, this, &TransactionMenu::FilterUpdated);
    connect(MenuController->API, &RestAPI::ReplyReceived, this, &TransactionMenu::HandleReply);
    connect(ui->transactionBack, &QPushButton::clicked, this, &TransactionMenu::BackClicked);
    connect(ui->RefreshButton, &QPushButton::clicked, this, &TransactionMenu::RefreshClicked);
}

void TransactionMenu::HandleReply(RestAPI::ReplyType reply){
    if (reply== RestAPI::TransactionReply)
    {
        KillChildren();
        Reset();
        delete moreButton;
        moreButton = nullptr;
        FilterTransactions(filters->currentText());
    }
}

void TransactionMenu::KillChildren(){
    foreach (ExpandableButton *button, MenuController->findChildren<ExpandableButton*>()) {
        delete button;
    }
}

void TransactionMenu::SpawnChildren(vector<TransactionData> list){
    int i = 0;
    for(TransactionData data : list){
        if(i < currentIndex){
            i++;
            continue;
        }
        if(i >= maxIndex && !moreButton)
        {
            moreButton = new QPushButton("Load more...");
            layoutGroup->addWidget(moreButton);
            connect(moreButton, &QPushButton::clicked, this, &TransactionMenu::HandleMoreButtonClick);
            break;
        }

        SpawnChild(data);
        i++;
    }
    currentIndex = i;
}

void TransactionMenu::HandleMoreButtonClick(){
    maxIndex += maxIncrease;
    delete moreButton;
    moreButton = nullptr;
    FilterTransactions(filters->currentText());
}

void TransactionMenu::SpawnChild(TransactionData data){
    ExpandableButton *button = new ExpandableButton(container);
    button->Initialize(data, MenuController->API->account.AccountNumber);
    layoutGroup->addWidget(button);
}

void TransactionMenu::BackClicked(){
    MenuController->SwitchMenu(MainWindow::MainMenuPage);
}

void TransactionMenu::RefreshClicked(){
    KillChildren();
    Reset();
    MenuController->API->GetTransactionsFromAccountNumber(MenuController->API->account.AccountNumber);
}
void TransactionMenu::FilterUpdated(QString type){
    KillChildren();
    Reset();
    if(moreButton){
        delete moreButton;
        moreButton = nullptr;
    }
    FilterTransactions(type);
}
void TransactionMenu::FilterTransactions(QString type){
    if(type == "All")
        SpawnChildren(MenuController->API->transactions);
    else if(type == "Deposit")
        SpawnChildren(MenuController->API->FilterTransactionsByType(TransactionData::DepositTransaction));
    else if(type == "Withdraw")
        SpawnChildren(MenuController->API->FilterTransactionsByType(TransactionData::WithdrawTransaction));
    else if(type == "Transfer")
        SpawnChildren(MenuController->API->FilterTransactionsByType(TransactionData::TransferTransaction));
}
