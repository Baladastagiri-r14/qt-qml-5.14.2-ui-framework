#ifndef BUTTONWINDOW_H
#define BUTTONWINDOW_H

#include <QWidget>

class QPushButton;
class QQuickWidget;

class ButtonWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonWindow(QWidget *parent = nullptr);

private slots:
    void handleButtonClick();

private:
    QPushButton *button;
    QQuickWidget *quickWidget;
};

#endif // BUTTONWINDOW_H
