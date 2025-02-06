#include "CameraSettingsDialog.h"
#include "ui_CameraSettingsDialog.h"

CameraSettingsDialog::CameraSettingsDialog(QWidget *parent ,std::shared_ptr<ToolsBase> tool)
    : QDialog(parent)
    , ui(new Ui::CameraSettingsDialog)
{
     ui->setupUi(this);
    tool_ = tool;
    std::vector<std::string> list = tool->getInputsList();
    for(int i = 0;i<list.size();++i)
    {
         ui->CameraList->addItem(QString::fromStdString(list[i]));
    }
    ui->verticalLayout->setEnabled(false);
    connect(ui->CameraList, &QListWidget::currentRowChanged, this, &CameraSettingsDialog::onCameraSelectionChanged);
}

CameraSettingsDialog::~CameraSettingsDialog()
{
    delete ui;
}
void CameraSettingsDialog::onCameraSelectionChanged(int currentRow)
{
    // 设置相机参数控件的可用性
    ui->verticalLayout->setEnabled(true);
}

void CameraSettingsDialog::on_CameraSettingsDialog_accepted()
{
    int selectedIndex = ui->CameraList->currentRow();

    if (selectedIndex >= 0)
    {
        // 选择了相机，应用参数
        std::string selectedCamera =( ui->CameraList->item(selectedIndex)->text()).toStdString();
        // 设置相机参数
        int exposure = ui->setExposure->value();
        bool option1 = ui->radioButton->isChecked();

        IOConfig config;
        // 添加数据到 IOConfig
        config.addData("Camera",selectedCamera);
        config.addData("Exposure", exposure);
        config.addData("snap", option1);

        tool_->setInputs(config);
    }

}

