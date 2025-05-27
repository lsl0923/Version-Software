#include<QDebug>
#include <opencv2/opencv.hpp>
#include <QCoreApplication>
#include<QLabel>
#include<QSize>
#include<QPoint>

#include "FaceDetect.h"



FaceDetect::FaceDetect()
    :recognizer_("../../../model/sphereface-model/","../../../model/Fast-MTCNN-master/model")
{
    setName();
    initInput();
}
void FaceDetect::initInput()
{

    IOConfig config;
    config.addData("personName", "Unknow");
    config.addData("isSave",0);
    // 设置到 tool_
    this->setInputs(config);
}
FaceDetect::~FaceDetect()
{
    // 确保有定义
}
void FaceDetect::setName()
{
    name_ = "FaceDetect";
}

void FaceDetect::faceDetect_FastMTCNN(cv::Mat& img, cv::Mat& dst, std::vector<cv::Rect>& faceRects)
{
    // 初始化 MTCNN 检测器（路径根据你本地模型位置修改）
    MTCNN detector("../../../model/Fast-MTCNN-master/model");

    // 复制输入图像到输出图像
    dst = img.clone();

    // 检测参数
    float factor = 0.709f;
    float threshold[3] = { 0.7f, 0.6f, 0.6f };
    int minSize = 12;

    // 开始计时
    double t = (double)cv::getTickCount();

    // 检测人脸
    std::vector<FaceInfo> faceInfo = detector.Detect_mtcnn(dst, minSize, threshold, factor, 3);

    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    std::cout << "Face detection time: " << t << "s" << std::endl;

    // 清空旧的人脸框
    faceRects.clear();

    // 遍历检测结果并绘制人脸框
    for (const auto& face : faceInfo) {
        int x = static_cast<int>(face.bbox.xmin);
        int y = static_cast<int>(face.bbox.ymin);
        int w = static_cast<int>(face.bbox.xmax - face.bbox.xmin + 1);
        int h = static_cast<int>(face.bbox.ymax - face.bbox.ymin + 1);
        cv::Rect rect(x, y, w, h);
        faceRects.push_back(rect);
        cv::rectangle(dst, rect, cv::Scalar(255, 0, 0), 2);
    }
}
void FaceDetect::faceDetect_Haar(cv::Mat& img, cv::Mat& dst, std::vector<cv::Rect>& faceRects)
{
    // 拷贝原图
    dst = img.clone();

    // 加载 Haar XML 分类器
    static cv::CascadeClassifier face_cascade;
    static bool loaded = false;
    if (!loaded)
    {
        if (!face_cascade.load("../../../model/Haar/haarcascade_frontalface_alt.xml"))
        {
            //QString currentDir = QCoreApplication::applicationDirPath();
            //qDebug() << "当前路径：" << currentDir;
             qDebug()<< "无法加载 Haar 分类器 XML 文件！";
            return;
        }
        loaded = true;
    }

    // 转灰度
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    // 检测人脸
    face_cascade.detectMultiScale(gray, faceRects, 1.1, 11, 0, cv::Size(30, 30));

    // 绘制检测框
    for (const auto& face : faceRects)
    {
        cv::rectangle(dst, face, cv::Scalar(255, 0, 0), 2);
    }
}
int FaceDetect::finalizeOutput(const cv::Mat& img, int code,
                               std::chrono::high_resolution_clock::time_point start)
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    outputs_.addData("image", img.clone());
    outputs_.addData("errorCode", code);
    outputs_.addData("runTime", elapsed.count());
    return code;
}
int FaceDetect::runSub()
{
    const QString dbFile = "../../../FaceData/faces.json";
    QString resultText = "NG";
    if (!validateInputs()) {
        qWarning() << "Input ERROR!";
        return -1;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    // 读取图像
    cv::Mat img;
    if (inputs_.getDataValue("image").type() == typeid(cv::Mat)) {
        img = std::any_cast<cv::Mat>(inputs_.getDataValue("image"));
    }

    if (img.empty()) {
        qWarning() << "Empty or invalid image!";
        return finalizeOutput(img, -1, start);
    }

    // 是否是保存模式
    int isSave = 0;
    if (inputs_.getDataValue("isSave").type() == typeid(int)) {
        isSave = std::any_cast<int>(inputs_.getDataValue("isSave"));
    }

    cv::Mat dst = img.clone();
    std::vector<cv::Rect> faceRects;
    cv::Mat tmp;
    faceDetect_FastMTCNN(img, dst, faceRects);
    //faceDetect_Haar(img,dst,faceRects);
    if (faceRects.empty())
    {
        qWarning() << "No face detected!";
        resultText = "No face detected!";
        outputs_.addData("messege", resultText);
        return finalizeOutput(dst, -1, start);
    }

    // 裁剪第一张人脸
    cv::Mat feature;

    if (!recognizer_.extractFeature(img, feature)) {
        qWarning() << "Failed to extract face feature!";
        cv::putText(dst, "NG", cv::Point(10, 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        return finalizeOutput(dst, -1, start);
    }

    // 保存模式
    if (isSave == 1)
    {
         resultText = "NG";
        QString name = "Unnamed";
        if (inputs_.getDataValue("personName").type() == typeid(QString)) {
            name = std::any_cast<QString>(inputs_.getDataValue("personName"));
        }

        db_.add(FaceRecord{name, feature});
        if (db_.save(dbFile)) {
            resultText = "Saved face for:" + name;
            qDebug() << "Saved face for:" << name;
            cv::putText(dst, "OK", cv::Point(10, 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        } else {
            resultText ="Failed to save database!";
            qWarning() << "Failed to save database!";
            cv::putText(dst, "NG", cv::Point(10, 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
        }
        outputs_.addData("messege", resultText);
        return finalizeOutput(dst, 1, start);
    }

    // 识别模式
     resultText = "NG";
    db_.load(dbFile);

    std::vector<float> featVec;
    if (feature.isContinuous()) {
        featVec.assign((float*)feature.datastart, (float*)feature.dataend);
    } else {
        for (int i = 0; i < feature.rows; ++i) {
            const float* rowPtr = feature.ptr<float>(i);
            featVec.insert(featVec.end(), rowPtr, rowPtr + feature.cols);
        }
    }

    QString bestName = "Unknown";
    float bestScore = -1.0f;
    for (const auto& record : db_.records) {
        float sim = db_.cosineSimilarity(featVec, record.feature);
        if (sim > bestScore) {
            bestScore = sim;
            bestName = record.name;
        }
    }

    const float threshold = 0.5f;
    if (bestScore >= threshold) {
         resultText = QString("OK: %1 (%2)").arg(bestName).arg(QString::number(bestScore, 'f', 6));

        cv::putText(dst, "OK", cv::Point(10, 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        qDebug() << "Matched:" << bestName << ", score:" << bestScore;
    } else {
      resultText = QString("No Match (%1)").arg(QString::number(bestScore, 'f', 6));
        cv::putText(dst, "No Match", cv::Point(10, 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 255), 1);
        qDebug() << "No match. Best score:" << bestScore;
    }

    outputs_.addData("messege", resultText);
    return finalizeOutput(dst, 1, start);
}

bool FaceDetect::validateInputs()
{
    const std::vector<IOData>& data = inputs_.data;
    // 查找是否存在名为 "image" 的输入
    auto it = std::find_if(data.begin(), data.end(), [](const IOData& io)
                           {
                               return io.name == "image";
                           });
    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'image'";
        return false;
    }

    // 检查 "image" 是否为 cv::Mat 类型
    try
    {
        const auto& imageInput = std::any_cast<cv::Mat>(it->value);
    }
    catch (const std::bad_any_cast&)
    {
        qDebug() << "Error: Input 'image' must be of type cv::Mat";
        return false;
    }

    it = std::find_if(data.begin(), data.end(), [](const IOData& io)
                      {
                          return io.name == "isSave";
                      });
    if (it == data.end())
    {
        qDebug() << "Error: Missing required input 'isSave'";
        return false;
    }
    // 验证通过
    return true;
}

std::vector<std::string> FaceDetect::getInputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    vec.push_back("personName");
    vec.push_back("ToolType");
    return vec;
}

std::vector<std::string> FaceDetect::getOutputsList()
{
    std::vector<std::string> vec;
    vec.push_back("image");
    vec.push_back("faceRects");
    return vec;
}

std::string FaceDetect::getInputType(const std::string& inputName)
{
    return "cv::Mat";
}

std::string FaceDetect::getOutputType(const std::string& outputName)
{
    if(outputName == "faceRects")
    {
        return "std::vector<cv::Rect>";
    }
    else
    {
        return "cv::Mat";
    }

}
