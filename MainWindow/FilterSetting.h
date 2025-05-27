#ifndef FILTERSETTING_H
#define FILTERSETTING_H

#include <QDialog>
#include"ImageViewer.h"
#include <QDialog>
#include"ToolsBase.h"
namespace Ui {
class FilterSetting;
}

class FilterSetting : public QDialog
{
    Q_OBJECT

public:
    explicit FilterSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent = nullptr);
    ~FilterSetting();
    void updateImg();
private slots:
    void on_threshold_4_textChanged(const QString &arg1);

    void on_comboBox_4_currentTextChanged(const QString &arg1);

    void on_FilterSetting_accepted();

private:
    Ui::FilterSetting *ui;
    ImageViewer* viewer_;
    std::shared_ptr<ToolsBase> tool_;
    int mode_;
    int size_;
    cv::Mat img_;
};

#endif // FILTERSETTING_H
