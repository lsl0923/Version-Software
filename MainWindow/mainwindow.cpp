#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"form.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    form_ = new Form();
    ui->formLayout->addWidget(form_);
    connect(ui->actionOpenCV, &QAction::triggered, this, &MainWindow::onActionOpenOpenCV);
    connect(ui->actionIKapCV, &QAction::triggered, this, &MainWindow::onActionOpenIKapCV);
    connect(ui->actionOpenImg, &QAction::triggered, this, &MainWindow::onActionOpenImg);
    connect(ui->action_SaveImg, &QAction::triggered, this, &MainWindow::onActionSaveImg);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onActionOpenOpenCV()
{
    form_->onActionOpenUSB();
}

void MainWindow::onActionOpenIKapCV()
{

}


void MainWindow::onActionSaveImg()
{

}


void MainWindow::onActionOpenImg()
{
    form_->onActionOpenImg();
}
