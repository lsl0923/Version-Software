#ifndef TEMMATCHERSETTING_H
#define TEMMATCHERSETTING_H
#include"ROIOptionBox.h"
#include"ImageViewer.h"
#include <QDialog>
#include"ToolsBase.h"

namespace Ui
{
class TemMatcherSetting;
}

class TemMatcherSetting : public QDialog
{
    Q_OBJECT

public:
    explicit TemMatcherSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent = nullptr);
    ~TemMatcherSetting();

private slots:
    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_clicked();

private:
    Ui::TemMatcherSetting *ui;
    ImageViewer* viewer_;
    std::shared_ptr<ToolsBase> tool_;
    std::string mode_;
    double thresholdValue_;
    cv::Mat img_;
};

#endif // TEMMATCHERSETTING_H
