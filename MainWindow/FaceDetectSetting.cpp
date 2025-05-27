#include<QThread>



#include "FaceDetectSetting.h"
#include "ui_FaceDetectSetting.h"

FaceDetectSetting::FaceDetectSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FaceDetectSetting)
{
    tool_ = tool;
    ui->setupUi(this);
    setWindowTitle("FaceDetect");
    viewer_ = new ImageViewer(ui->frame);
    ui->horizontalLayout_4->addWidget(viewer_);
    viewer_->setImage(img);
    QThread::msleep(100);
    viewer_->centerImage();
    img_ = img.clone();
    ui->lineEdit_2->setEnabled(false);
    isSave_ = 0;
    ui->lineEdit->setEnabled(false);

}

FaceDetectSetting::~FaceDetectSetting()
{
    delete ui;
}

void FaceDetectSetting::on_FaceDetectSetting_accepted()
{

    IOConfig config;
    config.addData("personName", personName_);
    config.addData("isSave",isSave_);
    // 设置到 tool_
    tool_->setInputs(config);
}


void FaceDetectSetting::on_lineEdit_editingFinished()
{
    personName_ = ui->lineEdit->text();
}


void FaceDetectSetting::on_ToolType_currentTextChanged(const QString &arg1)
{

    if (arg1 == "人脸录入")
    {
        ui->lineEdit->setEnabled(true);
        isSave_ = 1;
    }
    else if (arg1 == "人脸识别")
    {
        ui->lineEdit->setEnabled(false);
        isSave_ = 0;
    }

}


void FaceDetectSetting::on_buttonBox_accepted()
{
     accept();
}

