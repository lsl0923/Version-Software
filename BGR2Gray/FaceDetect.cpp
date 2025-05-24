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
        if (!face_cascade.load("../../../model/haarcascade_frontalface_default.xml"))
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
int FaceDetect::runSub()
{
    QString dbFile = "../../../FaceData/faces.json";
    if(!validateInputs())
    {
        qDebug() << "Input ERROR!";
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    //0 输出图像，1运行结果代码，2时间
    qDebug() << "runSub FaceDetect";
    cv::Mat img;

    if(inputs_.getDataValue("image").type() == typeid(cv::Mat))
    {
        img = std::any_cast<cv::Mat>(inputs_.getDataValue("image"));
    }
    else
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image",img);
        outputs_.addData("errorCode",1);
        outputs_.addData("runTime",elapsed.count());
        qDebug() << "type error: need Mat";

    }

    if (img.empty())
    {
        qWarning() << "Received an empty Image!";
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image",img);
        outputs_.addData("errorCode",-1);
        outputs_.addData("runTime",elapsed.count());
        return -1;
    }



    int isSave;
    if(inputs_.getDataValue("isSave").type() == typeid(int))
    {
        isSave = std::any_cast<int>(inputs_.getDataValue("isSave"));
    }
    if(isSave == 1)
    {
        cv::Mat dst = img.clone();
        qDebug() << "Saving face to database...";

        // 检测人脸
        std::vector<cv::Rect> faceRectsTemp;
        cv::Mat dstTemp;
        faceDetect_FastMTCNN(img, dstTemp, faceRectsTemp);

        if (faceRectsTemp.empty())
        {
            qWarning() << "No face found to save!";
        }
        else
        {
            cv::Mat face = img(faceRectsTemp[0]).clone(); // 取第一张人脸区域
            cv::Mat feature;

            if (recognizer_.extractFeature(face, feature)) // 提取特征
            {
                QString name;
                if (inputs_.getDataValue("personName").type() == typeid(QString))
                    name = std::any_cast<QString>(inputs_.getDataValue("personName"));
                else
                    name = "Unnamed";

                FaceRecord record{name, feature};
                db_.add(record);

                // 可修改路径
                if (!db_.save(dbFile))
                {
                    cv::putText(dst, "NG", cv::Point(10, 20),
                                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                    qWarning() << "Failed to save face database!";
                }

                else
                {
                     qDebug() << "Saved face for:" << name;
                    cv::putText(dst, "OK", cv::Point(10, 20),
                                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                }

            }
            else
            {
                qWarning() << "Failed to extract face feature!";
                cv::putText(dst, "NG", cv::Point(10, 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
            }
        }


        outputs_.addData("image",dst);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("runTime",elapsed.count());
        outputs_.addData("errorCode",1);
        return 1;
    }




    cv::Mat dst;
      QString text("NG");
    std::vector<cv::Rect> faceRectsTemp;
    faceDetect_FastMTCNN(img, dst, faceRectsTemp);

    if (faceRectsTemp.empty())
    {
        qWarning() << "No face found to recognize!";
        // 可以选择返回错误码，下面添加输出数据示例
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        outputs_.addData("image", dst);
        outputs_.addData("errorCode", -1);
        outputs_.addData("runTime", elapsed.count());
        return -1;
    }
    else
    {
        cv::Mat face = img(faceRectsTemp[0]).clone();
        cv::Mat feature;

        if (recognizer_.extractFeature(face, feature))
        {
            // 匹配逻辑
            QString bestName = "Unknown";
            float bestScore = -1.0f;
            db_.load(dbFile);
            for (const auto& record : db_.records)
            {
                std::vector<float> dbFeature = record.feature;
                float sim = db_.cosineSimilarity(feature, dbFeature);

                if (sim > bestScore)
                {
                    bestScore = sim;
                    bestName = record.name;
                }
            }

            const float threshold = 0.35f;
            if (bestScore >= threshold)
            {
                text = QString("OK: %1 (%.2f)").arg(bestName).arg(bestScore);
                cv::putText(dst, "OK", cv::Point(10, 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
                qDebug() << "Matched:" << bestName << ", score:" << bestScore;
            }
            else
            {
                cv::putText(dst, "No Match", cv::Point(10, 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 255), 1);
                qDebug() << "No match found. Best score:" << bestScore;
            }
        }
        else
        {
            qWarning() << "Failed to extract face feature!";
            cv::putText(dst, "NG", cv::Point(10, 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 255), 1);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    outputs_.addData("image", dst);
    outputs_.addData("errorCode", 1);
    outputs_.addData("runTime", elapsed.count());
    outputs_.addData("text",text);
    return 1;

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
