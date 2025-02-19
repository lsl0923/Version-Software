#include <QVBoxLayout>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QPluginLoader>
#include<QTreeView>
#include<QStandardItemModel>
#include <QMenu>
#include <QFileDialog>
#include <QStringList>


#include "ImageViewer.h"
#include "ui_form.h"
#include "ToolsBase.h"
#include "form.h"
#include "managerbase.h"
#include "toolitem.h"
#include"IndexDelegate.h"
#include"InputConfigDialog.h"
#include "CameraManager.h"
#include "CameraSettingsDialog.h"
Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    // TODO 封装优化
    qRegisterMetaType<ToolsInfo>();
   // qRegisterMetaTypeStreamOperators<ToolsInfo>("ToolsInfo");

    ui->setupUi(this);

    imageViewer_ = new ImageViewer(ui->frame);
    // 获取中心控件并设置布局
    ui->RunWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    // 连接右键菜单信号
    connect( ui->RunWidget, &QListWidget::customContextMenuRequested, this, &Form::onRightClick);
    // 为 QListWidget 设置自定义委托
    IndexDelegate* delegate = new IndexDelegate(this);
    ui->RunWidget->setItemDelegate(delegate);

    ui->horizontalLayout->addWidget(imageViewer_);
    // 加载测试图像
    cv::Mat testImage = cv::imread("/home/lsl/图片/截图/s.png");
    if (!testImage.empty())
    {
        imageViewer_->setImage(testImage);
    }
    else
    {
        qWarning() << "Failed to load test image.";
    }
    toolsFlows_ = std::vector<ToolsFlow>(5);
    QStandardItemModel* model = new QStandardItemModel;
    ui->ToolsWight->setModel(model);
    QString pluginPath = "/home/lsl/Code/BGR2Gray/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libBGR2Gray.so";
    loadPlugin(pluginPath);
    pluginPath = "/home/lsl/Code/ImageLoad/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libImageLoad.so";
    loadPlugin(pluginPath);
    cameraManager_ = NULL;
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
        if (manager)
        {
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
                    }
                    else
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
    if (img.empty())
    {
        qWarning() << "Initial image is empty!";
        return;
    }
    IOConfig  initialInputs;
    initialInputs.addData("image",img);
    // 设置工具流的初始输入
    toolsFlows_[0].setInitialInput(initialInputs);

    // 执行工具流
    if (toolsFlows_[0].executeFlow())
    {
        // 获取最终输出并更新图像
        IOConfig finalOutput = toolsFlows_[0].getFinalOutput();
        if(finalOutput.getDataValue("image").type() == typeid(cv::Mat))
        {
            imageViewer_->setImage(std::any_cast<cv::Mat>(finalOutput.getDataValue("image")));

        }
    qDebug() << "Tool flow executed successfully!";
    }
    else
    {
        qWarning() << "Tool flow execution failed!";
    }
    //imageViewer_->nextImage();
}




//delete
void Form::on_pushButton_4_clicked()
{
    // 获取选中的项
    QListWidgetItem* selectedItem = ui->RunWidget->currentItem();
    if(selectedItem == nullptr)
    {
        return;
    }
    ToolItem *tool = dynamic_cast< ToolItem *> (selectedItem);
    ToolsFlow& flow = toolsFlows_[0];
    flow.removeToolById(tool->getToolInstance()->getToolId());
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

void Form::showInputConfigDialog(ToolItem* toolItem)
{
    if (!toolItem) return;

    // 获取当前工具的实例
    std::shared_ptr<ToolsBase> tool = toolItem->getToolInstance();
    if (!tool) return;
    if(tool->getName() == "CameraTool")
    {
        CameraSettingsDialog camerasettingsdialog(nullptr,tool);  // 创建对话框
        camerasettingsdialog.exec();
        return;
    }


    InputConfigDialog dialog(tool->getToolId(),imageViewer_->getImage());
    // 获取当前工具的输入列表
    const std::vector<std::string>& inputs = tool->getInputsList();
    if (inputs.empty())
    {
        qDebug() << "No inputs required for this tool.";
        return;
    }

    // 获取当前工具之前的所有工具
    std::vector<std::string> previousToolIds;
    ToolsFlow& flow = toolsFlows_[0];  // 假设取第一个 Flow
    std::string toolId = tool->getToolId();
    bool isCurrentToolFound = false;

    for (const auto& flowTool : flow.getTools()) {
        if (!flowTool) continue;

        if (flowTool->getToolId() == toolId) {
            isCurrentToolFound = true;
            break;
        }

        // 记录工具 ID 以供选择
        previousToolIds.push_back(flowTool->getToolId());
    }

    if (!isCurrentToolFound)
    {
        qDebug() << "Tool ID not found in the current flow!";
        return;
    }

    // **调用 `addInput`，一次性传入所有输入参数**
    dialog.addInput(tool, previousToolIds, flow);

    // 显示对话框
    if (dialog.exec() == QDialog::Accepted)
    {
        // 获取用户配置的输入
        std::vector<InputConfig> inputConfigs = dialog.getInputConfigs();

        // 记录输入配置
        for ( InputConfig& config : inputConfigs)
        {
            flow.setInput(config);
        }

        qDebug() << "Inputs configured for tool:" << QString::fromStdString(toolId);
    }
}


void Form::onRightClick(const QPoint& pos) {
    QListWidgetItem* item = ui->RunWidget->itemAt(pos);
    ToolItem* toolItem = dynamic_cast<ToolItem*>(item);
    if (!toolItem) return; // 确保 toolItem 存在

    QMenu contextMenu;
    QAction* actionSetInput = new QAction("Set Input", &contextMenu);

    // 绑定点击 "Set Input" 时，直接弹出输入配置窗口
    connect(actionSetInput, &QAction::triggered, this, [this, toolItem]() {
        showInputConfigDialog(toolItem);
    });

    contextMenu.addAction(actionSetInput);
    contextMenu.exec(ui->RunWidget->mapToGlobal(pos));
}

void Form::setInputMenu(const std::string& toolId, QMenu* inputMenu) {
    // 确保 Flow 存在
    if (toolsFlows_.empty()) {
        qDebug() << "No tools flow available!";
        return;
    }

    // 查找指定工具
    ToolsFlow& flow = toolsFlows_[0];  // 假定选定第一个 Flow
    std::shared_ptr<ToolsBase> tool = flow.findToolById(toolId);
    if (!tool) {
        qDebug() << "Tool not found with ID:" << QString::fromStdString(toolId);
        return;
    }

    // 获取工具的输入需求列表
    std::vector<std::string> inputs = tool->getInputsList();
    if (inputs.empty())
    {
        QAction* noInputAction = new QAction("No inputs required", inputMenu);
        noInputAction->setEnabled(false);
        inputMenu->addAction(noInputAction);
        return;
    }

    // 动态获取位于当前工具之前的工具输出列表
    std::vector<std::shared_ptr<ToolsBase>> previousTools;
    bool isCurrentToolFound = false;

    for (const auto& flowTool : flow.getTools())
    {
        if (!flowTool) continue;

        // 遍历到当前工具时停止
        if (flowTool->getToolId() == toolId)
        {
            isCurrentToolFound = true;
            break;
        }

        // 收集当前工具
        previousTools.push_back(flowTool);
    }

    if (!isCurrentToolFound)
    {
        qDebug() << "Tool ID not found in the current flow!";
        return;
    }

    // 为每个输入需求创建子菜单
    for (const std::string& inputName : inputs)
    {
        QMenu* inputSubMenu = new QMenu(QString::fromStdString(inputName), inputMenu);
        int index = 1;
        // 遍历位于当前工具之前的工具，生成子菜单
        for (const auto& prevTool : previousTools)
        {
            std::vector<std::string> outputList = prevTool->getOutputsList();
            if (outputList.empty())
            {
                continue;
            }

            QString toolMenuName = QString("%1 (%2)")
                                       .arg(QString::fromStdString(prevTool->getName()))
                                       .arg(index);
            index++;
            QMenu* toolMenu = new QMenu(toolMenuName, inputSubMenu);

            for (const std::string& outputId : outputList)
            {
                QString actionText = QString::fromStdString(outputId);
                QAction* outputAction = new QAction(actionText, toolMenu);

                // 绑定选中逻辑：将指定输出配置为当前工具的输入
               /// TODO:: 判断数据类型兼容性
                connect(outputAction, &QAction::triggered, this, [toolId, inputName, prevTool, outputId, &flow]()
                    {
                    qDebug() << "Configuring input for tool ID:" << QString::fromStdString(toolId)
                    << "Input:" << QString::fromStdString(inputName)
                    << "Using output from tool ID:" << QString::fromStdString(prevTool->getToolId())
                    << "Output ID:" << QString::fromStdString(outputId);

                    // 执行输入配置逻辑
                  flow.setInput(toolId, prevTool->getToolId(), inputName, outputId);

                });

                toolMenu->addAction(outputAction);
            }

            if (toolMenu->isEmpty())
            {
                QAction* noOutputsAction = new QAction("No outputs available", toolMenu);
                noOutputsAction->setEnabled(false);
                toolMenu->addAction(noOutputsAction);
            }

            inputSubMenu->addMenu(toolMenu);
        }

        if (inputSubMenu->isEmpty())
        {
            QAction* noToolsAction = new QAction("No tools available", inputSubMenu);
            noToolsAction->setEnabled(false);
            inputSubMenu->addAction(noToolsAction);
        }

        inputMenu->addMenu(inputSubMenu);
    }
}


void Form::on_LoadImage_clicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select Images", "", "Images (*.png *.jpg *.bmp)");
    if (!filePaths.isEmpty())
    {
        imageViewer_->loadImages(filePaths);
    }
}


void Form::on_nextImage_clicked()
{
    imageViewer_->nextImage();
}





void Form::on_pushButton_6_clicked()
{
    if (!cameraManager_)
    {
        cameraManager_ = std::make_shared<CameraManager>(CameraType::OpenCV);
    }

    if (cameraManager_ && cameraManager_->GetCameraList().size() > 0)
    {
        std::shared_ptr<CameraBase> camera = cameraManager_->GetCameraList()[0];

        if (camera->IsConnected())  // 如果相机已连接，先断开
        {
           // camera->StopGrab();
            camera->DisConnectCamera();
            disconnect(camera.get(), &CameraBase::newImageReceived, this, &Form::updateImage);
            return;
        }
        camera->StartGrab();

        bool connected = connect(camera.get(), &CameraBase::newImageReceived, this, &Form::updateImage, Qt::DirectConnection);
    }
    else
    {
        qDebug() << "No cameras found!";
    }
}

void Form::updateImage(cv::Mat img)
{
    imageViewer_->setImage(img);
    update();
}

