#include "ThresholdDialog.h"
#include "ui_ThresholdDialog.h"
#include"QDebug"
ThresholdDialog::ThresholdDialog(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent)
    : QDialog(parent), tool_(tool), thresholdValue_(128),ui(new Ui::ThresholdDialog)
{
    ui->setupUi(this);
    setWindowTitle("二值化阈值调整");
    viewer_ = new ImageViewer(ui->frame);
    ui->verticalLayout_3->addWidget(viewer_);
    viewer_->setImage(img);
    viewer_->centerImage();
    img_ = img.clone();
}

ThresholdDialog::~ThresholdDialog()
{
    delete ui;
}

void ThresholdDialog::updateThreshold()
{
    thresholdValue_ = ui->threshold->value();
    mode_ = ui->comboBox->currentText().toStdString();

    // 获取输入图像
    cv::Mat inputImage = img_;
    if (inputImage.empty())
    {
        qDebug() << "输入图像为空！";
        return;
    }

    // 转换模式
    int cvMode = cv::THRESH_BINARY;
    if (mode_ == "THRESH_BINARY_INV") cvMode = cv::THRESH_BINARY_INV;
    else if (mode_ == "THRESH_TRUNC") cvMode = cv::THRESH_TRUNC;
    else if (mode_ == "THRESH_TOZERO") cvMode = cv::THRESH_TOZERO;
    else if (mode_ == "THRESH_TOZERO_INV") cvMode = cv::THRESH_TOZERO_INV;
    // 应用二值化
    cv::Mat outputImage;
    cv::threshold(inputImage, outputImage, thresholdValue_, 255, cvMode);
    // 显示预览
    viewer_->setImage(outputImage);

}

void ThresholdDialog::on_threshold_textChanged(const QString &arg1)
{
    updateThreshold();
}


void ThresholdDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    updateThreshold();
}


void ThresholdDialog::on_ThresholdDialog_accepted()
{

    IOConfig config;
    config.addData("ThresholdValue", thresholdValue_);
    int cvMode = cv::THRESH_BINARY;
    if (mode_ == "THRESH_BINARY_INV") cvMode = cv::THRESH_BINARY_INV;
    else if (mode_ == "THRESH_TRUNC") cvMode = cv::THRESH_TRUNC;
    else if (mode_ == "THRESH_TOZERO") cvMode = cv::THRESH_TOZERO;
    else if (mode_ == "THRESH_TOZERO_INV") cvMode = cv::THRESH_TOZERO_INV;
    config.addData("ThresholdMode", static_cast<int>(cvMode));

    // 设置到 tool_
    tool_->setInputs(config);
}

