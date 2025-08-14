#include "mainwindow.h"
#include "menus.h"
#include "ui_mainwindow.h"

#include<QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    API = new RestAPI();
    ptrRFID = new RFID_DLL(this);

    input = new InputMenu(this);
    main =new MainMenu(this);

    login =new LoginMenu(this);

    deposit=new DepositMenu(this);
    withdraw =new WithdrawMenu(this);
    transfer =new TransferMenu(this);
    transaction = new TransactionMenu(this);
    credit = new CreditMenu(this);

    foreach (QPushButton *button, this->findChildren<QPushButton*>()) {
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setOffset(3, 3);
        shadow->setColor(Qt::black);
        button->setGraphicsEffect(shadow);
        button->setStyleSheet(R"(
            QPushButton {
                background-color: #1f1f1f;
            }
            QPushButton:hover {
               background-color: #363636;
            }
        )");
    }
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "button" + QString::number(i);
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            button->setStyleSheet(R"(
            QPushButton {
                background-color: #1f1f1f;
                border-radius: 40px;
                color: white;
                font: bold 16px;
            }
            QPushButton:hover {
                background-color: #363636;
            }
            )");
        }
    }

    foreach (QLineEdit *lineEdit, this->findChildren<QLineEdit*>()) {
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setOffset(3, 3);
        shadow->setColor(Qt::black);
        lineEdit->setGraphicsEffect(shadow);
    }

    QStringList targetLabels = {"loginlabel","menulabel","transactionslabel","depositlabel","withdrawlabel","transferlabel","balancelabel"};

    foreach (QLabel *label, this ->findChildren<QLabel*>()){
        if (targetLabels.contains(label->objectName())){
            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(10);
            shadow->setOffset(3, 3);
            shadow->setColor(Qt::black);
            label->setGraphicsEffect(shadow);
        }
    }
    ui->OKbutton->setStyleSheet(R"(
    QPushButton {
        border: 4px solid #03822d;
        font: bold 16px;
        border-radius: 5px;
        padding: 5px;
        background-color: #5dba7d;
        color: #002800;
    }
    QPushButton:hover {
        border: 4px solid #399C5A;
        font: bold 16px;
        border-radius: 5px;
        padding: 5px;
        background-color: #7FC899;
        color: #002800;
    }
    )");
    ui->DELbutton->setStyleSheet(R"(
    QPushButton {
        border: 4px solid #610602;
        font: bold 16px;
        border-radius: 5px;
        padding: 5px;
        background-color: #D96762;
        color: #002800;
    }
    QPushButton:hover {
        border: 4px solid #833B38;
        font: bold 16px;
        border-radius: 5px;
        padding: 5px;
        background-color: #E18783;
        color: #002800;
    }
    )");
    SwitchMenu(FirstScreenPage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete API;
    //delete ptrRFID;
}

void MainWindow::SwitchMenu(Menu m)
{
    ui->stackedWidget->setCurrentIndex(m);
    if (m == FirstScreenPage){
        transaction->filters->setCurrentIndex(0);
    }
}
bool MainWindow::isActiveMenu(Menu m)
{
    if (ui->stackedWidget->currentIndex() == m)
        return true;
    else
        return false;
}

void MainWindow::LogOut()
{
    transaction->Reset();
    transaction->KillChildren();
    SwitchMenu(MainWindow::FirstScreenPage);
    API->LogOut();
    ui->nextAccount->setVisible(false);
    ui->previousAccount->setVisible(false);
    main->ResetFirstAccount();

}

MainWindow::Menu MainWindow::GetActiveMenu()
{
    return (Menu)ui->stackedWidget->currentIndex();
}
