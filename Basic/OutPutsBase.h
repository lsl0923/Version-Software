#ifndef OUTPUTSBASE_H
#define OUTPUTSBASE_H

#include <vector>
#include <any>
#include<string>
#include<map>
class OutPutsBase
{
public:
    // 默认构造函数
    OutPutsBase();


    // 初始化函数
    void init();
    double getErrorCode() const;

    void setErrorCode(double errorCode);

    double getRunTime() const ;

    void setRunTime(double runTime);
    void addOutput(const std::string& outputId, const std::any& value);
    std::any getOutput(const std::string& outputId) const;

protected:
    std::map<std::string, std::any> outputs_;
    // 受保护的子类初始化接口
    virtual void initSub() = 0;
    double errorCode_;
    double runTime_;
    // 成员变量

};

#endif // OUTPUTSBASE_H
