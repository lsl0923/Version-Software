#ifndef CAMERATOOL_H
#define CAMERATOOL_H
#include <QPluginLoader>
#include "ToolsBase.h"
#include "CameraManager.h"
#include "CameraBase.h"
#include <memory>

class CameraTool : public ToolsBase
{
public:
    explicit CameraTool(const CameraType type);
    ~CameraTool() override;

    std::vector<std::string> getInputsList() override;
    std::vector<std::string> getOutputsList() override;
    std::string getInputType(const std::string& inputName) override;
    std::string getOutputType(const std::string& outputName) override;
    void setCameraFromManager(int cameraIndex);
    void setCameraFromManager(CameraID id);
protected:
    int runSub() override;
    void setName() override;
    bool validateInputs() override;

private:
    std::shared_ptr<CameraManager> cameraManager_;
    std::shared_ptr<CameraBase> camera_;
};

#endif // CAMERATOOL_H
