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
    mode_ = 1;
}

FaceDetectSetting::~FaceDetectSetting()
{
    delete ui;
}


void FaceDetectSetting::update()
{
    IOConfig config;
    config.addData("personName", personName_);
    config.addData("isSave",isSave_);
    config.addData("mode",mode_);
    config.addData("image",img_);
    // 设置到 tool_
    tool_->setInputs(config);
    tool_->run();
    cv::Mat img;
    if (tool_->getOutputs().getDataValue("image").type() == typeid(cv::Mat)) {
        img = std::any_cast<cv::Mat>(tool_->getOutputs().getDataValue("image"));
    }
    viewer_->setImage(img);
}
void FaceDetectSetting::on_FaceDetectSetting_accepted()
{

    IOConfig config;
    config.addData("personName", personName_);
    config.addData("isSave",isSave_);
    config.addData("mode",mode_);

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
        ui->mode->setEnabled(false);
        isSave_ = 1;
    }
    else if (arg1 == "人脸识别")
    {
        ui->lineEdit->setEnabled(false);
        ui->mode->setEnabled(true);
        isSave_ = 0;
    }
    update();
}


void FaceDetectSetting::on_buttonBox_accepted()
{
     accept();
}


void FaceDetectSetting::on_mode_currentTextChanged(const QString &arg1)
{
    if (arg1 == "全局特征匹配")
    {
        mode_ = 1;
    }
    else if (arg1 == "局部关键点匹配")
    {
        mode_ = 0;
    }
     update();
}

