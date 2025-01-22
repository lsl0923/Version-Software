#include <QVBoxLayout>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QPluginLoader>
#include<QTreeView>
#include<QStandardItemModel>

#include "ImageViewer.h"
#include "ui_form.h"
#include "ToolsBase.h"
#include "form.h"
#include "managerbase.h"
#include "toolitem.h"


Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    // TODO 封装优化
    qRegisterMetaType<ToolsInfo>();
    qRegisterMetaTypeStreamOperators<ToolsInfo>("ToolsInfo");

    ui->setupUi(this);
    imageViewer_ = new ImageViewer(ui->frame);
    // 获取中心控件并设置布局

    ui->horizontalLayout->addWidget(imageViewer_);
    // 加载测试图像
    cv::Mat testImage = cv::imread("/home/lsl/图片/截图/s.png");
    if (!testImage.empty()) {
        imageViewer_->setImage(testImage);
    } else
    {
        qWarning() << "Failed to load test image.";
    }
    toolsFlows_ = std::vector<ToolsFlow>(5);
    QStandardItemModel* model = new QStandardItemModel;
    ui->ToolsWight->setModel(model);
    QString pluginPath = "/home/lsl/Code/BGR2Gray/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libBGR2Gray.so";
    loadPlugin(pluginPath);
}

Form::~Form()
{
    delete ui;
}






bool Form::isExistsInToolsWight(const std::string toolName)
{
    QAbstractItemModel* model = ui->ToolsWight->model(); // 获取模型
    if (!model)
    {
        qWarning() << "Model is null!";
        return false;
    }
    bool exists = false;
    QString toolNameQString = QString::fromStdString(toolName);
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0); // 获取第 0 列的索引
        QString itemText = model->data(index, Qt::DisplayRole).toString();
        if (itemText == toolNameQString)
        {
            return true; // 找到匹配项
        }

    }
    return exists;
}
/*
 void Form::loadPluginC(const QString& path)
{
     QLibrary lib(path);
    if (!lib.load()) {

        return ;
    }
    GetToolsListPtr getToolList =(GetToolsListPtr) lib.resolve("getToolsList");
    CreateToolsPtr createTools=(CreateToolsPtr) lib.resolve("createTools");

    if(getToolList != nullptr && createTools != nullptr)
    {
           std::vector<ToolsInfo> info = getToolList();
        for (auto i : info)
        {
            if (!isExistsInToolsWight(i.name))
            {
                ui->ToolsWight->addItem(QString::fromStdString(i.name));

                // 这里不再存储工具插件实例，而是通过插件创建工具实例
                ToolsBase* toolInstance = createTools(QString::fromStdString(i.name));
                if (toolInstance)
                {
                    toolsMap_[i.name] = toolInstance;  // 存储独立的工具实例
                }
            }
        }
    }
}
*/
void Form::loadPlugin(const QString& path)
{
    QPluginLoader loader(path);
    QObject* plugin = loader.instance();

    if (plugin) {
        ManagerBase* manager = dynamic_cast<ManagerBase*>(plugin);
        if (manager) {
            qDebug() << "Plugin loaded successfully as ToolsBase.";
            std::vector<ToolsInfo> info = manager->getToolsList();

            // 获取模型
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->ToolsWight->model());

            for (auto i : info) {
                if (!isExistsInToolsWight(i.name))
                {

                    QStandardItem *item = new QStandardItem(QString::fromStdString(i.name));

                    // 设置 item 的 data，用于存储 ToolsInfo
                    item->setData(QVariant::fromValue(i), Qt::UserRole);

                    // 查找父节点或创建父节点 (根据 type 创建)
                    QStandardItem *parentItem = nullptr;
                    QModelIndexList parentIndexes = model->match(
                        model->index(0, 0), Qt::DisplayRole, QString::fromStdString(i.type), -1, Qt::MatchRecursive
                        );
                    if (parentIndexes.isEmpty())
                    {
                        // 如果没有找到父节点，创建一个新的父节点
                        parentItem = new QStandardItem(QString::fromStdString(i.type));
                        parentItem->setFlags(parentItem->flags() & ~Qt::ItemIsDragEnabled);
                        model->appendRow(parentItem);
                    } else
                    {
                        parentItem = model->itemFromIndex(parentIndexes.first());
                    }

                    // 将 item 添加到对应的 parentItem 下
                    parentItem->appendRow(item);

                    toolsMap_[i] = manager;  // 存储独立的工具Manager
                }
            }
        } else
        {
            qWarning() << "Failed to cast plugin to ToolsBase.";
        }
    } else
    {
        qWarning() << "Failed to load plugin:" << loader.errorString();
    }
}


//RunButton
void Form::on_pushButton_clicked()
{
    cv::Mat img = imageViewer_->getImage();
    if (img.empty()) {
        qWarning() << "Initial image is empty!";
        return;
    }
    std::vector<std::any> initialInputs;
    initialInputs.push_back(img);
    // 设置工具流的初始输入
    toolsFlows_[0].setInitialInput(initialInputs);

    // 执行工具流
    if (toolsFlows_[0].executeFlow())
    {
        // 获取最终输出并更新图像
        std::vector<std::any> finalOutput = toolsFlows_[0].getFinalOutput();
        if(finalOutput[0].type() == typeid(cv::Mat))
        {
            imageViewer_->setImage(std::any_cast<cv::Mat>(finalOutput[0]));

        }
        qDebug() << "Tool flow executed successfully!";
    } else {
        qWarning() << "Tool flow execution failed!";
    }
}




//delete
void Form::on_pushButton_4_clicked()
{
    // 获取选中的项
    QListWidgetItem* selectedItem = ui->RunWidget->currentItem();

    delete selectedItem;

}

//center
void Form::on_pushButton_2__clicked()
{
    imageViewer_->centerImage();
}




void Form::on_RunWidget_itemChanged(QListWidgetItem *item)
{
    if (!item)
    {
        qWarning() << "No item found in RunWidget.";
        return;
    }

    // 将 QListWidgetItem 转换为 ToolItem


    ToolItem* toolItem = dynamic_cast<ToolItem*>(item);
    if (!toolItem)
    {
        qWarning() << "Item is not a ToolItem.";
        return;
    }
    // 获取工具的基本信息
    ToolsInfo toolInfo = toolItem->getToolInfo();
    if (toolInfo.name == "")
    {
        return;
    }

    auto it = toolsMap_.find(toolInfo);
    if (it != toolsMap_.end())
    {
        ManagerBase* manager = it->second;  // 获取对应的插件 manager
        if (manager)
        {
            ToolsBase* tool = manager->createTools(toolInfo);
            if (tool)
            {
                std::shared_ptr<ToolsBase> toolInstance(tool);
                toolItem->setToolInstance(toolInstance);
                toolsFlows_[0].addTool(toolInstance);
                qDebug() << "Tool instance created for:"  ;
            }
            else
            {
                qWarning() << "Failed to create tool instance for:" ;
            }
        }
    }
    else
    {
        qWarning() << "Tool not found in toolsMap for:" ;
    }
}


void Form::on_ToolsWight_doubleClicked(const QModelIndex &index)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->ToolsWight->model());
    if (!model)
    {
        return;
    }

    QStandardItem* item = model->itemFromIndex(index);
    if (!item)
    {
        return;
    }

    // 将 QStandardItem 转换为 ToolItem

    QVariant data = item->data(Qt::UserRole);

    ToolsInfo toolInfo = data.value<ToolsInfo>();
    if (toolInfo.name.empty() && toolInfo.library.empty())
    {
        return;
    }


    // 创建新的 ToolItem（在 RunWidget 中显示的 ToolItem）
    ToolItem* newItem = new ToolItem(toolInfo);
    // 将新项添加到 RunWidget 中
    ui->RunWidget->addItem(newItem);
    // 设置新项的工具信息
    newItem->setData(Qt::UserRole, QVariant::fromValue(toolInfo));

}


