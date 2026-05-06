#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickItem>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    qmlView = new QQuickView();
    qmlView->setResizeMode(QQuickView::SizeRootObjectToView);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qmlView->setSource(QUrl("qrc:/widget.qml"));
    qmlView->show();
}

