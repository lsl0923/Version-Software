
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include"Basic_global.h"
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <opencv2/opencv.hpp>

enum PaintMode
{
    PaintMode_None = 0,        // 不绘制
    PaintMode_FreeDraw,        // 自由绘制（鼠标画线）
    PaintMode_Rectangle,       // 绘制矩形
    PaintMode_Ellipse,         // 绘制椭圆
    PaintMode_Line,            // 绘制直线
    PaintMode_Polygon,         // 绘制多边形
    PaintMode_Erase            // 橡皮擦模式
};





class BASIC_EXPORT ImageViewer : public QWidget {
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    void setImage(const cv::Mat &mat);
    void centerImage();
    cv::Mat getImage() const;
    void loadImages(const QStringList &filePaths); // 加载多张图片
    void nextImage();                              // 显示下一张图片
    void prevImage();                              // 显示上一张图片
    void normalize();
    void setMode(PaintMode mode);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent  *event) override;

private:
    void updateImageDisplay(); // 更新当前显示的图像
    void drawFreeLine(QPoint point);
    void drawRectangle(QPoint start, QPoint end);
    void drawEllipse(QPoint start, QPoint end);
    void drawLine(QPoint start , QPoint end);
    void erase(QPoint pos, int size);


    std::vector<cv::Mat> images_;
    cv::Mat image_;
    cv::Mat overlay_;
    QPoint offset_;
    double scaleFactor_;
    QPoint lastPos_;
    QPoint tempStartPos_ ;
    int parent_width_;
    int parent_height_;
    QString coordinateText_;
    int currentImageIndex_; // 当前显示的图像索引
    double maxSize_;
    double minSize_;
    bool drawing_;
    PaintMode paintmode_;
};

#endif // IMAGEVIEWER_H
