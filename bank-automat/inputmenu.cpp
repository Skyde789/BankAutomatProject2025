#include "menus.h"
#include "mainwindow.h"
#include <QMessageBox>
InputMenu::InputMenu(MainWindow *parent){
    MenuController=parent;
    auto ui= MenuController->GetUI();

    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "button" + QString::number(i);
        QPushButton *button = MenuController->findChild<QPushButton*>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &InputMenu::HandleNumberClick);
        }
    }

    header = ui->inputMenuLabel;
    header2 = ui->inputMenuLabel_2;
    textBox = ui->pinline;
    textBox->setAlignment(Qt::AlignCenter);
    header->setAlignment(Qt::AlignCenter);
    header2->setAlignment(Qt::AlignCenter);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &InputMenu::TimerEnd);
    connect(ui->OKbutton, &QPushButton::clicked, this, &InputMenu::HandleConfirmClick);
    connect(ui->DELbutton, &QPushButton::clicked, this, &InputMenu::HandleDeleteClick);
    connect(ui->inputBackButton, &QPushButton::clicked, this, &InputMenu::HandleBackClick);
}

void InputMenu::Initialize(InputPageType t){
    currentType = t;

    timer->setInterval(30000);
    timer->start();

    switch(currentType){

    case LoginType:
        header->setText("Login");
        header2->setText("Enter Pin:");
        textBox->setMaxLength(4);
        textBox->setEchoMode(QLineEdit::Password);
        break;
    case WithdrawType:
        header->setText("Withdraw");
        header2->setText("Amount:");
        textBox->setMaxLength(10);
        textBox->setEchoMode(QLineEdit::Normal);
        break;
    case DepositType:
        header->setText("Deposit");
        header2->setText("Amount:");
        textBox->setMaxLength(10);
        textBox->setEchoMode(QLineEdit::Normal);
        break;
    case TransferAccountType:
        header->setText("Transfer");
        header2->setText("Receiving account:");
        textBox->setMaxLength(10);
        textBox->setEchoMode(QLineEdit::Normal);
        break;
    case TransferAmountType:
        header->setText("Transfer");
        header2->setText("Amount:");
        textBox->setMaxLength(10);
        textBox->setEchoMode(QLineEdit::Normal);
        break;
    case CreditType:
        header->setText("Credit Limit");
        header2->setText("Amount:");
        textBox->setMaxLength(10);
        textBox->setEchoMode(QLineEdit::Normal);
        break;
    }
}

void InputMenu::HandleNumberClick(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
    QString current = textBox->text();
        if (current.length() < textBox->maxLength()) {
            textBox->setText(current + button->text());
        }
    }


    timer->start();
}

void InputMenu::HandleConfirmClick(){
    timer->stop();
    emit OkPressed(currentType, textBox->text());
    textBox->clear();
}

void InputMenu::TimerEnd(){
    if (QMessageBox::information(nullptr, "Idle", "Too long idle") == QMessageBox::Ok) {
        switch(currentType){
        case LoginType:
            MenuController->SwitchMenu(MainWindow::FirstScreenPage);
            break;
        default:
            MenuController->SwitchMenu(MainWindow::MainMenuPage);
        }
    }
}

void InputMenu::HandleDeleteClick(){
    QString pin = textBox->text();
    pin.chop(1);
    textBox->setText(pin);
}
void InputMenu::HandleBackClick(){
    timer->stop();
    switch(currentType){

    case LoginType:
        MenuController->SwitchMenu(MainWindow::FirstScreenPage);
        break;
    case WithdrawType:
        MenuController->SwitchMenu(MainWindow::MainMenuPage);
        break;
    case DepositType:
        MenuController->SwitchMenu(MainWindow::MainMenuPage);
        break;
    case TransferAccountType:
        MenuController->SwitchMenu(MainWindow::MainMenuPage);
        break;
    case TransferAmountType:
        Initialize(TransferAccountType);
        break;
    case CreditType:
        MenuController->SwitchMenu(MainWindow::MainMenuPage);
    }
}
