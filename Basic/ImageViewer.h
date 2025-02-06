
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <opencv2/opencv.hpp>

class ImageViewer : public QWidget {
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
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent  *event) override;

private:
    void updateImageDisplay(); // 更新当前显示的图像
    std::vector<cv::Mat> images_;
    cv::Mat image_;
    QPoint offset_;
    double scaleFactor_;
    QPoint lastPos_;
    int parent_width_;
    int parent_height_;
    QString coordinateText_;
    int currentImageIndex_; // 当前显示的图像索引
    double maxSize_;
    double minSize_;
};

#endif // IMAGEVIEWER_H
