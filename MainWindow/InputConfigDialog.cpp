#include "InputConfigDialog.h"
#include "ui_InputConfigDialog.h"
#include <QLineEdit>
#include<QDebug>
#include"ToolsFlow.h"
#include"ThresholdDialog.h"
#include"TemMatcherSetting.h"

InputConfigDialog::InputConfigDialog(std::string id ,cv::Mat img ,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputConfigDialog)
    , img_(img)
{
    ui->setupUi(this);
    toolID_ = id;

}

InputConfigDialog::~InputConfigDialog()
{
    delete ui;
}
void InputConfigDialog::addInput( std::shared_ptr<ToolsBase> tool, const std::vector<std::string>& previousTools, ToolsFlow& toolsFlow)
{
    tool_ = tool;
    // 显示所有输入到第一个 QListWidget
    for (const std::string& input : tool->getInputsList())
    {
        ui->inputListWidget->addItem(QString::fromStdString(input));
    }

    // 显示所有上游工具到第二个 QListWidget
    for (const std::string& toolId : previousTools)
    {
        auto tool = toolsFlow.findToolById(toolId);
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(tool->getName()), ui->toolListWight);
        item->setData(Qt::UserRole, QString::fromStdString(toolId)); // 绑定工具 ID，便于查找
    }

    connect(ui->toolListWight, &QListWidget::currentItemChanged, this,
            [this, &toolsFlow, tool](QListWidgetItem* current, QListWidgetItem* /*previous*/)
            {
                // 清空输出列表
                ui->outputListWight->clear();

                if (!current) return;

                // 获取选中的工具 ID
                std::string selectedToolId = current->data(Qt::UserRole).toString().toStdString();

                // 查找对应工具并获取输出
                std::shared_ptr<ToolsBase> selectedTool = toolsFlow.findToolById(selectedToolId);
                if (selectedTool)
                {
                    const std::vector<std::string>& outputs = selectedTool->getOutputsList();
                    for (const std::string& output : outputs)
                    {
                        // 这里需要知道当前选中的输入项
                        QListWidgetItem* selectedInputItem = ui->inputListWidget->currentItem();
                        if (!selectedInputItem) continue;

                        std::string inputName = selectedInputItem->text().toStdString();
                        if (ToolsBase::isMatchType(tool->getInputType(inputName), selectedTool->getOutputType(output)))
                        {
                            ui->outputListWight->addItem(QString::fromStdString(output));
                        }
                    }
                }
                refreshGrayScaleForOutputs();
            });
    // 监听输入变化，更新输出列表
    connect(ui->inputListWidget, &QListWidget::currentItemChanged, this, [this, &toolsFlow, tool](QListWidgetItem* current, QListWidgetItem* previous) {
        // 清空输出列表
        ui->outputListWight->clear();

        if (!current || !ui->toolListWight->currentItem()) return;

        // 获取选中的工具 ID 和输入类型
        std::string selectedToolId = ui->toolListWight->currentItem()->data(Qt::UserRole).toString().toStdString();
        std::string selectedInputType = tool->getInputType(current->text().toStdString());  // 获取输入类型

        // 查找对应工具并获取输出
        std::shared_ptr<ToolsBase> selectedTool = toolsFlow.findToolById(selectedToolId);
        if (selectedTool)
        {
            const std::vector<std::string>& outputs = selectedTool->getOutputsList();
            for (const std::string& output : outputs)
            {
                if (ToolsBase::isMatchType(selectedInputType, selectedTool->getOutputType(output)))
                {
                    ui->outputListWight->addItem(QString::fromStdString(output));
                }
            }
        }
        refreshGrayScaleForOutputs();
    });

}



std::vector<InputConfig> InputConfigDialog::getInputConfigs() const
{
    return inputConfigs_;
}

void InputConfigDialog::on_outputListWight_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    // 获取输出项的文本（即输出 ID）
    std::string outputId = item->text().toStdString();

    // 获取当前选中的工具
    QListWidgetItem* selectedToolItem = ui->toolListWight->currentItem();
    if (!selectedToolItem) return;

    // 获取所选工具的 ID
    std::string selectedToolId = selectedToolItem->data(Qt::UserRole).toString().toStdString();

    // 获取输入项的名称（假设输入项是根据选中的输入名称列表更新的）
    QListWidgetItem* inputItem = ui->inputListWidget->currentItem();
    if (!inputItem) return;

    std::string inputName = inputItem->text().toStdString();;

    // 将选择的输出添加至输入配置中
    bool inputExists = false;
    for (auto& config : inputConfigs_)
    {
        if (config.inputName == inputName)
        {
            // 如果存在，则更新 selectedToolId 和 outputId
            config.sourceToolId = selectedToolId;
            config.sourceOutputId = outputId;
            inputExists = true;
            break;
        }
    }

    // 如果不存在，则添加新的配置
    if (!inputExists)
    {
        InputConfig newConfig(toolID_, selectedToolId, inputName, outputId);
        inputConfigs_.push_back(newConfig);
    }

    item->setSelected(false);
    refreshGrayScaleForOutputs();

}
void InputConfigDialog::refreshGrayScaleForOutputs()
{
    QListWidgetItem* inputname = ui->inputListWidget->currentItem();
    QListWidgetItem* outputntool = ui->toolListWight->currentItem();


    if (!inputname || !outputntool) return;

    std::string selectedInput = inputname->text().toStdString();
    std::string selectedToolId = outputntool->data(Qt::UserRole).toString().toStdString();

    // 遍历输出列表，设置默认状态
    for (int i = 0; i < ui->outputListWight->count(); ++i)
    {
        QListWidgetItem* outputItem = ui->outputListWight->item(i);
        outputItem->setBackground(Qt::white);
        outputItem->setFlags(outputItem->flags() | Qt::ItemIsSelectable); // 允许选择
    }

    // 遍历 inputConfigs_，匹配当前选择的 inputName 和 toolId，并灰化
    for (const auto& config : inputConfigs_)
    {
        if (config.inputName == selectedInput) // 只检查当前输入相关的配置
        {
            for (int i = 0; i < ui->outputListWight->count(); ++i)
            {
                QListWidgetItem* outputItem = ui->outputListWight->item(i);

                if (outputItem->text().toStdString() == config.sourceOutputId &&
                    selectedToolId == config.sourceToolId)
                {
                    outputItem->setBackground(Qt::lightGray);  // 设置灰度背景
                    outputItem->setFlags(outputItem->flags() & ~Qt::ItemIsSelectable); // 禁用选择
                }
            }
        }
    }
}


void InputConfigDialog::on_inputListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

    refreshGrayScaleForOutputs();
}


void InputConfigDialog::on_toolListWight_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    refreshGrayScaleForOutputs();
}

void InputConfigDialog::addNumericInput(const std::string& inputName)
{
    // 创建 QListWidgetItem
    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(inputName));
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable); // 让 item 不能被选中
    ui->inputListWidget->addItem(item);

    // 创建 QLineEdit 作为输入框
    QLineEdit* inputEdit = new QLineEdit();
    inputEdit->setValidator(new QDoubleValidator(this)); // 限制输入为数值
    inputEdit->setPlaceholderText("Enter number...");

    // 在 QListWidget 中设置 Widget
    ui->inputListWidget->setItemWidget(item, inputEdit);

    // 监听输入变化
    connect(inputEdit, &QLineEdit::textChanged, this, [this, inputName](const QString& newValue) {
        qDebug() << "Input" << QString::fromStdString(inputName) << "changed to" << newValue;

        bool found = false;
        for (auto& config : inputConfigs_)
        {
            if (config.inputName == inputName)
            {
                config.value = newValue.toDouble();
                found = true;
                break;
            }
        }
        if (!found)
        {
            inputConfigs_.emplace_back(toolID_, "", inputName, "");
            inputConfigs_.back().value = newValue.toDouble();
        }
    });
}

void InputConfigDialog::on_inputListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString input = item->text();
    if(tool_->getName() == "BinaryThresholdTool" && input != "image")
    {
        //addNumericInput((input));
        ThresholdDialog setThresholdDilog(tool_,img_);
        setThresholdDilog.exec();
    }
    else if(tool_->getName() == "TemMatcher" && input != "image")
    {
        TemMatcherSetting temmatchersetting(tool_,img_);
        temmatchersetting.exec();
    }
}

