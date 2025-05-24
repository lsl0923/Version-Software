#ifndef FACEDETECTSETTING_H
#define FACEDETECTSETTING_H

#include <QDialog>
#include<opencv2/opencv.hpp>

#include"ImageViewer.h"
#include"ToolsBase.h"
namespace Ui {
class FaceDetectSetting;
}

class FaceDetectSetting : public QDialog
{
    Q_OBJECT

public:
    explicit FaceDetectSetting(std::shared_ptr<ToolsBase> tool,cv::Mat img, QWidget *parent = nullptr);
    ~FaceDetectSetting();

private slots:
    void on_FaceDetectSetting_accepted();

    void on_lineEdit_editingFinished();

    void on_ToolType_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::FaceDetectSetting *ui;
    ImageViewer* viewer_;
    std::shared_ptr<ToolsBase> tool_;
    double thresholdValue_;
    cv::Mat img_;
    int isSave_;
    QString personName_;
};

#endif // FACEDETECTSETTING_H
