#pragma once

#include "datatypes.h"
#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QPropertyAnimation>
#include <qlabel.h>

class ButtonWithLabels : public QPushButton {
public:
    ButtonWithLabels(const QString &text1, const QString &text2, QWidget *parent = nullptr)
        : QPushButton(parent) {

        // Create the labels
        amountLabel = new QLabel(text1, this);
        dateLabel = new QLabel(text2, this);
        amountLabel->setStyleSheet("font-size: 18pt; color: green;");

        dateLabel->setStyleSheet("font-size: 14pt; color: grey;");
        // Create a horizontal layout for the button's content
        QHBoxLayout *layout = new QHBoxLayout(this);
        // Add labels inside the button's layout
        layout->addWidget(amountLabel);
        layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        layout->addWidget(dateLabel);

        // Set the layout for the button
        setLayout(layout);
    }

    void SetStyleSheets(bool gain){
        setStyleSheet(R"(
        QPushButton {
            background-color: #1f1f1f;
        }
        QPushButton:hover {
           background-color: #363636;
        }
        )");
        if(gain){
            amountLabel->setStyleSheet(R"(
            QLabel {
                font-size: 18pt;
                color: green;
                background-color: transparent;
            }
            )");
        }
        else{
            amountLabel->setStyleSheet(R"(
            QLabel {
                font-size: 18pt;
                color: red;
                background-color: transparent;
            }
            )");
        }
        dateLabel->setStyleSheet(R"(
        QLabel {
            font-size: 14pt;
            color: grey;
            background-color: transparent;
        }
        )");
    }
    QLabel *amountLabel;
    QLabel *dateLabel;
};

class ExpandableButton : public QWidget {
    Q_OBJECT
public:
    ExpandableButton(QWidget *parent = nullptr);

    void Initialize(TransactionData data, int accNum){
        transactionData = data;
        headerButton->amountLabel->setText((IsGain(data, accNum) ? "+"  : "-") + QString::number(data.amount));
        headerButton->dateLabel->setText(data.date);
        headerButton->SetStyleSheets(IsGain(data, accNum));
        transactionID->setText("Transaction ID: " + QString::number(data.ID));
        transactionType->setText("Transaction Type: " + data.TypeToString(data.type));
        transactionDetailedDate->setText("Date: " + data.detailedDate);
    };

    bool IsGain(TransactionData data, int accNum){
        if (data.type == TransactionData::DepositTransaction)
            return true;
        else if (data.type == TransactionData::WithdrawTransaction)
            return false;
        else if (data.type == TransactionData::TransferTransaction){
            if (data.receiverNumber == accNum)
                return true;
            else
                return false;
        }
    }

    TransactionData GetData(){ return transactionData; }
private slots:
    void toggleExpand();

private:
    ButtonWithLabels* headerButton;
    QFrame* expandableArea;
    QPropertyAnimation* animation;
    QLabel* transactionID;
    QLabel* transactionDetailedDate;
    QLabel* transactionType;
    TransactionData transactionData;
    bool isExpanded = false;
};


