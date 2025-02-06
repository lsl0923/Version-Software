#ifndef INPUTCONFIGDIALOG_H
#define INPUTCONFIGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include"ToolsFlow.h"
#include<QListWidgetItem>
#include"ToolsBase.h"
#include"ToolsFlow.h"
namespace Ui {
class InputConfigDialog;
}

class InputConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputConfigDialog( std::string id ,cv::Mat img ,QWidget *parent = nullptr);
    ~InputConfigDialog();
    void addInput(std::shared_ptr<ToolsBase> tool, const std::vector<std::string>& previousTools,ToolsFlow& toolsflow);

    // 获取用户配置的输入项
    std::vector<InputConfig> getInputConfigs() const;
    void refreshGrayScaleForOutputs();
    void addNumericInput(const std::string& inputName);
private slots:
    void on_outputListWight_itemDoubleClicked(QListWidgetItem *item);
    void on_inputListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_toolListWight_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_inputListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::InputConfigDialog *ui;
    std::string toolID_;
    std::vector<InputConfig> inputConfigs_; // 存储用户输入的配置
    cv::Mat img_;
    std::shared_ptr<ToolsBase> tool_;
};

#endif // INPUTCONFIGDIALOG_H
