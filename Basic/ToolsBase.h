#ifndef TOOLSBASE_H
#define TOOLSBASE_H
#include<any>
#include "Basic_global.h"
#include<vector>
#include<memory>
#include"OutPutsBase.h"
#include<string>
#include<chrono>


class BASIC_EXPORT ToolsBase
{
public:
    ToolsBase();
    virtual ~ToolsBase() = default;
    int run();

    const std::vector<std::any>& getInputs() const;
    void setInputs(const std::vector<std::any>& inputs);

    std::shared_ptr<OutPutsBase> getOutputs() const;
    void setOutputs(const std::shared_ptr<OutPutsBase>& outputs);
    std::string generateUniqueTimestamp();
    std::string  getToolId();
     std::string getName();
protected:
    std::string toolId_;
    std::string name_;
    std::vector<std::any> inputs;
    std::shared_ptr<OutPutsBase> outputs;
    virtual int runSub() = 0;
};

#endif // TOOLSBASE_H
