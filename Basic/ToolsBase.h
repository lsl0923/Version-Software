#ifndef TOOLSBASE_H
#define TOOLSBASE_H
#include<any>
#include "Basic_global.h"
#include<vector>
#include<memory>

#include<string>
//#include<chrono>


struct IOData
{
    std::string name; // 数据的名称
    std::any value;   // 数据的值

    IOData(const std::string& name, const std::any& value)
        : name(name), value(value) {}
};

struct IOConfig
{
    std::vector<IOData> data; // 存储输入或输出的数据

    // 添加数据
    void addData(const std::string& name, const std::any& value)
    {
        for (auto& io : data)
        {
            if (io.name == name)
            {

                io.value = value;
                return;
            }
        }
        data.emplace_back(name, value);
    }



    // 查找数据
    std::any getDataValue(const std::string& name) const
    {
        for (const auto& d : data) {
            if (d.name == name) {
                return d.value; // 找到匹配项，返回值
            }
        }
        return {}; // 没有找到，返回空值
    }


    // 获取所有数据的名称列表
    std::vector<std::string> getDataNames() const
    {
        std::vector<std::string> names;
        for (const auto& d : data) {
            names.push_back(d.name);
        }
        return names;
    }
};

class BASIC_EXPORT ToolsBase
{
public:
    ToolsBase();
    virtual ~ToolsBase();
    int run();

    const IOConfig& getInputs() const;
    void setInputs(const IOConfig& newInputs);

    const IOConfig& getOutputs() const;
    void setOutputs(const IOConfig& newOutputs);
    std::string generateUniqueTimestamp();
    std::string  getToolId();
    std::string getName();
    virtual std::vector<std::string> getInputsList() = 0;
    virtual std::vector<std::string> getOutputsList() = 0;
protected:
    virtual int runSub() = 0;
    virtual void setName() = 0;
    virtual bool validateInputs(const std::vector<IOData>& data) = 0;
    IOConfig outputs_;
    IOConfig inputs_;
    std::string name_;
    std::string toolId_;
private:



};

#endif // TOOLSBASE_H
