#include "Manager.h"
#include "CameraTool.h"
#include <QDebug>

Manager::Manager()
{
    toolsInfo_.push_back( ToolsInfo("CameraTool", "libImageLoad.so", "读图", ""));
    qDebug() << "Manager initialized.";
}

ToolsBase* Manager::createTools(const ToolsInfo& pluginName)
{
    qDebug() << "Creating tool for plugin:" << QString::fromStdString(pluginName.name);

    if (pluginName.name == "CameraTool")
    {
        return new CameraTool(QString("/home/lsl/Code/OpenCVCamera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libOpenCVCamera.so"));
    }

    qDebug() << "Unknown tool requested.";
    return nullptr;
}
