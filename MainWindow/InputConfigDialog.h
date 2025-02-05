#ifndef INPUTCONFIGDIALOG_H
#define INPUTCONFIGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include"ToolsFlow.h"
#include<QListWidgetItem>

#include"ToolsFlow.h"
namespace Ui {
class InputConfigDialog;
}

class InputConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputConfigDialog(QWidget *parent = nullptr, std::string id = "");
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

private:
    Ui::InputConfigDialog *ui;
    std::string toolID_;
    std::vector<InputConfig> inputConfigs_; // 存储用户输入的配置
};

#endif // INPUTCONFIGDIALOG_H
