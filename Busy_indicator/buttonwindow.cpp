#include "buttonwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QQuickWidget>

ButtonWindow::ButtonWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    button = new QPushButton("Show Busy Indicator", this);
    layout->addWidget(button);

    quickWidget = new QQuickWidget(this);
    quickWidget->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    layout->addWidget(quickWidget);

    connect(button, &QPushButton::clicked, this, &ButtonWindow::handleButtonClick);
}

void ButtonWindow::handleButtonClick()
{
    quickWidget->rootObject()->setProperty("running", true); // Start the busy indicator
}
