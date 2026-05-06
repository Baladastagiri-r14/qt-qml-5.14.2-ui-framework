#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickItem>
#include <QWidget>
#include <QVBoxLayout>
#include <QtQuickWidgets/QQuickWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QWidget *centralWidget = new QWidget(this);
//    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
//    QQuickWidget *qmlWidget = new QQuickWidget();
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    qmlWidget->setSource(QUrl("qrc:/main.qml"));
    layout->addWidget(qmlWidget);
    setCentralWidget(centralWidget);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked,this, &MainWindow::on_pushButton_3_clicked);
    connect(ui->pushButton_4,&QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked) ;
    connect(ui->pushButton_5,&QPushButton::clicked, this, &MainWindow::on_pushButton_5_clicked) ;
    connect(ui->pushButton_6,&QPushButton::clicked, this, &MainWindow::on_pushButton_6_clicked) ;
    connect(ui->pushButton_7,&QPushButton::clicked, this, &MainWindow::on_pushButton_7_clicked) ;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked");
}

void MainWindow::on_pushButton_2_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked1");
}

void MainWindow::on_pushButton_3_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked3");
}

void MainWindow::on_pushButton_4_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked4");
}

void MainWindow::on_pushButton_5_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked5");
}

void MainWindow::on_pushButton_6_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked6");
}


void MainWindow::on_pushButton_7_clicked()
{
    QMetaObject::invokeMethod(this->qmlWidget->rootObject(), "qmlButtonClicked7");
}

