#ifndef TOOLSFLOW_H
#define TOOLSFLOW_H

#include<string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include "ToolsBase.h"



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

    // 获取最终输出
    std::vector<std::any> getFinalOutput() const;

    void setFlowID(std::string id);
    std::string getFlowID();


private:
    std::string flowID_;
    std::vector<std::shared_ptr<ToolsBase>> tools_; // 工具列表
    std::vector<std::any> initialInput_;                          // 初始输入
    std::vector<std::any> finalOutput_;                           // 最终输出
};

#endif // TOOLS_FLOW_H
