#include "expandablebutton.h"
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QSizePolicy>
#include <QFrame>
#include <QPushButton>

ExpandableButton::ExpandableButton(QWidget *parent) : QWidget(parent), isExpanded(false) {
    // Header button (fixed part)
    headerButton = new ButtonWithLabels("+1000", "12.2.2025", this);
    headerButton->setFixedHeight(50);
    headerButton->amountLabel->connect(headerButton, &QPushButton::clicked, this, &ExpandableButton::toggleExpand);


    // Expandable area
    expandableArea = new QFrame(this);
    expandableArea->setStyleSheet("background-color: #212121;");
    expandableArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Add some example content
    QGridLayout* contentLayout = new QGridLayout(expandableArea);

    transactionID = new QLabel("1234545456");
    transactionDetailedDate = new QLabel("Even More Info 2");
    transactionType = new QLabel("Even More Info");

    transactionDetailedDate->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    contentLayout->addWidget(transactionID, 0, 0);
    contentLayout->addWidget(transactionDetailedDate, 0, 1);
    contentLayout->addWidget(transactionType, 1, 0);

    // Set max height to 0 to start collapsed
    expandableArea->setMaximumHeight(0);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(headerButton);
    mainLayout->addWidget(expandableArea);

    // Animation
    animation = new QPropertyAnimation(expandableArea, "maximumHeight", this);
    animation->setDuration(125); // You can tweak the duration
}

void ExpandableButton::toggleExpand() {
    isExpanded = !isExpanded;

    int startHeight = expandableArea->maximumHeight();
    int endHeight = isExpanded ? expandableArea->sizeHint().height() : 0;

    animation->stop();
    animation->setStartValue(startHeight);
    animation->setEndValue(endHeight);
    animation->start();
}
