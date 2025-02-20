#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"form.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Form* form_;

private slots:
    void onActionOpenOpenCV();
    void onActionOpenIKapCV();
    void onActionOpenImg();
    void onActionSaveImg();

};

#endif // MAINWINDOW_H
