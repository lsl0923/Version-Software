#include "TemMatcherSetting.h"
#include "ui_TemMatcherSetting.h"

TemMatcherSetting::TemMatcherSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TemMatcherSetting)
{
    ui->setupUi(this);
    setWindowTitle("设置模式图");
    viewer_ = new ImageViewer(ui->frame);
    ui->horizontalLayout->addWidget(viewer_);
    viewer_->setImage(img);

    viewer_->centerImage();
    img_ = img.clone();
}

TemMatcherSetting::~TemMatcherSetting()
{
    delete ui;
}

void TemMatcherSetting::on_radioButton_2_clicked()
{
    viewer_->setMode(PaintMode_Ellipse);
}


void TemMatcherSetting::on_radioButton_3_clicked()
{
    viewer_->setMode(PaintMode_Rectangle);
}


void TemMatcherSetting::on_radioButton_clicked()
{
    viewer_->setMode(PaintMode_Erase);
}

