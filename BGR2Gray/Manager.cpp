#include "Manager.h"
#include <QDir>
#include"ToolsBase.h"
#include<QDebug>

#include"BGR2Gray.h"
#include"Gray2BGR.h"
#include"BinaryThresholdTool.h"
#include "BGR2Gray_global.h"
Manager::Manager()
{
    toolsInfo_.push_back( ToolsInfo("BGR2Gray", "libBGR2Gary.so", "预处理", "Converts BGR images to grayscale."));
    toolsInfo_.push_back( ToolsInfo("Gray2BGR", "libBGR2Gary.so", "Filter", "Test Filter"));
    toolsInfo_.emplace_back("BinaryThresholdTool", "ImageProcessing", "预处理", "Performs binary thresholding on images");
}



ToolsBase* Manager::createTools(const ToolsInfo& pluginName)
{
    if (pluginName.name == "BGR2Gray")
    {
        return new BGR2Gray();
    }
    else if (pluginName.name == "Gray2BGR")
    {
        return new Gray2BGR();
    }
    else if(pluginName.name == "BinaryThresholdTool")
    {
        return new BinaryThresholdTool();
    }
    return nullptr;
}


//

std::vector<ToolsInfo>  g_toolInfos;
extern "C" BGR2GRAY_EXPORT std::vector<ToolsInfo> getToolsList()
{
    if(g_toolInfos.size() <= 0)
    {
        g_toolInfos.push_back(ToolsInfo("BGR2Gray"));

        g_toolInfos.push_back(ToolsInfo("Gray2BGR"));
    }
    return g_toolInfos;
}

extern "C" BGR2GRAY_EXPORT ToolsBase*  createTools(const QString& pluginName)
{
    if (pluginName == "BGR2Gray") {
        return new BGR2Gray();
    }
    else if (pluginName == "Gray2BGR") {
        return new Gray2BGR();
    }
    return nullptr;
}
