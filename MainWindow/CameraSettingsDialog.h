#ifndef CAMERASETTINGSDIALOG_H
#define CAMERASETTINGSDIALOG_H

#include <QDialog>
#include"ToolsBase.h"
namespace Ui {
class CameraSettingsDialog;
}

class CameraSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSettingsDialog(QWidget *parent = nullptr,std::shared_ptr<ToolsBase> tool = nullptr);
    ~CameraSettingsDialog();
    void onCameraSelectionChanged(int currentRow);

private slots:
    void on_CameraSettingsDialog_accepted();

private:
    Ui::CameraSettingsDialog *ui;
    std::shared_ptr<ToolsBase> tool_;
};

#endif // CAMERASETTINGSDIALOG_H
