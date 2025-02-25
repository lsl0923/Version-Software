#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H
#include"ImageViewer.h"
#include <QDialog>
#include"ToolsBase.h"
namespace Ui
{
class ThresholdDialog;
}

class ThresholdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThresholdDialog(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent = nullptr);
    ~ThresholdDialog();
      void updateThreshold();
private slots:
    void on_threshold_textChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_ThresholdDialog_accepted();

private:
    Ui::ThresholdDialog *ui;
    ImageViewer* viewer_;
    std::shared_ptr<ToolsBase> tool_;
    std::string mode_;
    double thresholdValue_;
    cv::Mat img_;
};

#endif // THRESHOLDDIALOG_H
