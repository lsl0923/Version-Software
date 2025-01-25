#ifndef TOOLSFLOW_H
#define TOOLSFLOW_H

#include<string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include "ToolsBase.h"

struct InputConfig {
    std::string targetToolId;    // 目标工具的 ID
    std::string sourceToolId;    // 来源工具的 ID
    std::string sourceOutputId;  // 来源工具输出的 ID

    InputConfig(const std::string& target, const std::string& source, const std::string& output)
        : targetToolId(target), sourceToolId(source), sourceOutputId(output) {}
};


class ToolsFlow
{
public:
    // 构造函数和析构函数
    ToolsFlow();
    ~ToolsFlow();

    // 添加工具到流程
    void addTool(const std::shared_ptr<ToolsBase>& tool);

    // 设置初始输入
    void setInitialInput(const  std::vector<std::any>& input);

    // 执行工具流
    bool executeFlow();

    void setInput(const std::string& targetToolId, const std::string& sourceToolId, const std::string& sourceOutputId);

    // 获取最终输出
    std::vector<std::any> getFinalOutput() const;

    void setFlowID(std::string id);
    std::string getFlowID();
    bool setToolInputs(const std::shared_ptr<ToolsBase>& tool);
    // 通过工具 ID 查找工具
    std::shared_ptr<ToolsBase> findToolById(const std::string& toolId) const;
private:
    std::vector<InputConfig> inputConfigs_;   // 存储输入配置
    std::string flowID_;
    std::vector<std::shared_ptr<ToolsBase>> tools_; // 工具列表
    std::vector<std::any> initialInput_;                          // 初始输入
    std::vector<std::any> finalOutput_;                           // 最终输出
   // std::map<std::string, std::shared_ptr<ToolsBase>> toolsMapID;  // 工具的映射：工具ID -> 工具实例
};

#endif // TOOLS_FLOW_H
