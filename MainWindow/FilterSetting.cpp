#include "FilterSetting.h"
#include "ui_FilterSetting.h"
#include<QDebug>
#include<QThread>
FilterSetting::FilterSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent)
    : QDialog(parent), tool_(tool), size_(3),ui(new Ui::FilterSetting)
{
    ui->setupUi(this);
    setWindowTitle("二值化阈值调整");
    viewer_ = new ImageViewer(ui->frame_4);
    ui->verticalLayout_10->addWidget(viewer_);
    viewer_->setImage(img);
    QThread::msleep(100);
    viewer_->centerImage();
    img_ = img.clone();
}

FilterSetting::~FilterSetting()
{
    delete ui;
}

void FilterSetting::on_threshold_4_textChanged(const QString &arg1)
{
    updateImg();
}

void FilterSetting::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    updateImg();
}

void FilterSetting::updateImg()
{
    size_ = ui->threshold_4->value(); // 滤波核大小
    QString filterType = ui->comboBox_4->currentText();

    if (filterType == "中值滤波")
    {
        mode_ = 0;
    }
    else if (filterType == "均值滤波")
    {
        mode_ = 1;
    }
    else if (filterType == "高斯滤波")
    {
        mode_ = 2;
    }
    else
    {
        qDebug() << "未知滤波类型：" << filterType;
        return;
    }

    // 检查图像有效性
    cv::Mat inputImage = img_;
    if (inputImage.empty())
    {
        qDebug() << "输入图像为空！";
        return;
    }

    // 滤波处理
    cv::Mat outputImage;
    int ksize = size_;
    if (ksize % 2 == 0) ksize += 1; // OpenCV 要求奇数核

    if (mode_ == 0) // 中值滤波
    {
        cv::medianBlur(inputImage, outputImage, ksize);
    }
    else if (mode_ == 1) // 均值滤波
    {
        cv::blur(inputImage, outputImage, cv::Size(ksize, ksize));
    }
    else if (mode_ == 2) // 高斯滤波
    {
        cv::GaussianBlur(inputImage, outputImage, cv::Size(ksize, ksize), 0);
    }

    // 显示预览图像
    viewer_->setImage(outputImage);
}
void FilterSetting::on_FilterSetting_accepted()
{
    IOConfig config;
    config.addData("size", size_);
    config.addData("type", static_cast<int>(mode_));

    tool_->setInputs(config);
}
