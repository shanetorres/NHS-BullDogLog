#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "officerdelegate.h"

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

    void on_offAddStudentButton_clicked();

    void on_offDeleteStudentButton_clicked();

    void officerDeleteRecord();

    void enableButtons();

private:
    Ui::mainWidget *ui;
    QStandardItemModel* currentStudentsModel;
    const int currentStudentCols = 6;
    officerDelegate *currentStudentsDelegate;
};
#endif // MAINWIDGET_H
