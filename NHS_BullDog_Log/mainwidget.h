#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

private slots:

    void on_adminButton_clicked();

    void on_officerButton_clicked();

    void on_quitButton_clicked();

    void on_offMenuButton_clicked();

private:
    Ui::mainWidget *ui;
};

#endif // MAINWIDGET_H
