
#ifndef Gray2BGR_H
#define Gray2BGR_H
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QObject>
#include "ToolsBase.h"
#include "BGR2Gray_global.h"


class BGR2GRAY_EXPORT Gray2BGR :public ToolsBase
{


public:

    Gray2BGR();
    virtual ~Gray2BGR() override;

    // 重写 runSub 方法
    int runSub() override;
    bool validateInputs(const std::vector<IOData>& data) override;
    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    void setName() override;
};


#endif // Gray2BGR_H
