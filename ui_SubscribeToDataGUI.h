/********************************************************************************
** Form generated from reading UI file 'SubscribeToDataGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBSCRIBETODATAGUI_H
#define UI_SUBSCRIBETODATAGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_subscribetodata
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_strategyInstructions;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_cancel;
    QListView *listView;
    QSplitter *splitter;

    void setupUi(QWidget *subscribetodata)
    {
        if (subscribetodata->objectName().isEmpty())
            subscribetodata->setObjectName(QStringLiteral("subscribetodata"));
        subscribetodata->resize(616, 407);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(subscribetodata->sizePolicy().hasHeightForWidth());
        subscribetodata->setSizePolicy(sizePolicy);
        subscribetodata->setStyleSheet(QStringLiteral("background-color: rgb(59, 63, 85);"));
        horizontalLayout = new QHBoxLayout(subscribetodata);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox_strategyInstructions = new QGroupBox(subscribetodata);
        groupBox_strategyInstructions->setObjectName(QStringLiteral("groupBox_strategyInstructions"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_strategyInstructions->sizePolicy().hasHeightForWidth());
        groupBox_strategyInstructions->setSizePolicy(sizePolicy1);
        groupBox_strategyInstructions->setMinimumSize(QSize(165, 0));
        groupBox_strategyInstructions->setMaximumSize(QSize(200, 16777215));
        groupBox_strategyInstructions->setBaseSize(QSize(200, 0));
        groupBox_strategyInstructions->setStyleSheet(QStringLiteral("color:white;"));
        widget = new QWidget(groupBox_strategyInstructions);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(17, 118, 129, 128));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_ok = new QPushButton(widget);
        pushButton_ok->setObjectName(QStringLiteral("pushButton_ok"));
        pushButton_ok->setStyleSheet(QLatin1String("color:white;\n"
"background-color: rgb(79, 79, 79);"));

        verticalLayout->addWidget(pushButton_ok);

        pushButton_cancel = new QPushButton(widget);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));
        pushButton_cancel->setStyleSheet(QLatin1String("color:white;\n"
"background-color: rgb(79, 79, 79);"));

        verticalLayout->addWidget(pushButton_cancel);


        horizontalLayout->addWidget(groupBox_strategyInstructions);

        listView = new QListView(subscribetodata);
        listView->setObjectName(QStringLiteral("listView"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(5);
        sizePolicy2.setVerticalStretch(5);
        sizePolicy2.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(listView);

        splitter = new QSplitter(subscribetodata);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(splitter);


        retranslateUi(subscribetodata);

        QMetaObject::connectSlotsByName(subscribetodata);
    } // setupUi

    void retranslateUi(QWidget *subscribetodata)
    {
        subscribetodata->setWindowTitle(QApplication::translate("subscribetodata", "Subscribe to data", 0));
#ifndef QT_NO_WHATSTHIS
        subscribetodata->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        groupBox_strategyInstructions->setTitle(QString());
        pushButton_ok->setText(QApplication::translate("subscribetodata", "ok", 0));
        pushButton_cancel->setText(QApplication::translate("subscribetodata", "cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class subscribetodata: public Ui_subscribetodata {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBSCRIBETODATAGUI_H
