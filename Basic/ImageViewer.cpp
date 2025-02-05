#include "ImageViewer.h"
#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>
ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent), scaleFactor_(1.0), offset_(0, 0)
{
    setMouseTracking(true);
    QWidget *parentWidget = this->parentWidget();
    if (parentWidget)
    {
        parent_width_ = 877;
        parent_height_ = 730;
        qDebug() << "Parent size (width, height):" << parent_width_ << parent_height_;
    }

}


// 转换 OpenCV Mat 到 QImage
QImage MatToQImage(const cv::Mat& mat)
{
    switch (mat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_ARGB32);

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_RGB888);

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_Grayscale8);
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image(mat.data,
                     mat.cols, mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_Indexed8 );

        image.setColorTable(sColorTable);
#endif

        return image;
    }

        // wrong
    default:
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        break;
    }
    return QImage();
}


void ImageViewer::setImage(const cv::Mat &mat)
{


    image_ = mat.clone();
    // 根据窗口大小调整缩放比例
   /* if (parent_width_ > 0 && parent_height_ > 0)
    {
        double widthRatio = static_cast<double>(parent_width_) / image_.cols;
        double heightRatio = static_cast<double>(parent_height_) / image_.rows;
        scaleFactor_ = std::min(widthRatio, heightRatio); // 确保图片适应窗口，不超出范围
    }*/

    // 更新偏移量以居中
   // centerImage();
    update();
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    QImage image = MatToQImage(image_);
    QPainter painter(this);
    QRect targetRect(offset_, QSize(image.width() * scaleFactor_, image.height() * scaleFactor_));
    painter.drawImage(targetRect, image);

    if (!coordinateText_.isEmpty()) {
        QFont font = painter.font();
        font.setPointSize(10); // 设置字体大小
        painter.setFont(font);

        int margin = 10; // 距离左下角的边距
        QPoint textPos(margin, height() - margin); // 坐标显示位置
        painter.setPen(Qt::black);                // 白色字体
        painter.drawText(textPos, coordinateText_); // 绘制坐标文本
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
    lastPos_ = event->pos();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::MiddleButton)
    {
        // 仅在中键按下时进行拖动操作
        offset_ += event->pos() - lastPos_;
        lastPos_ = event->pos();
    }

    // 始终更新鼠标坐标信息
    QPointF imageCoords = (event->pos() - offset_) / scaleFactor_;
    coordinateText_ = QString("X: %1, Y: %2")
                          .arg(static_cast<int>(imageCoords.x()))
                          .arg(static_cast<int>(imageCoords.y()));

    update(); // 刷新界面以显示更新后的坐标
}


void ImageViewer::wheelEvent(QWheelEvent *event) {
    QPoint mousePos = event->pos();

    // 缩放前的图像在控件中的坐标
    QPointF beforeScalePoint = (mousePos - offset_) / scaleFactor_;
    if (event->angleDelta().y() > 0 && scaleFactor_<5)
    {
        scaleFactor_ *= 1.15;

    }
    else if(event->angleDelta().y() <= 0 && scaleFactor_>0.2)
    {
        scaleFactor_ /= 1.15;
    }
    QPointF afterScalePoint = beforeScalePoint * scaleFactor_;
    offset_ = (mousePos - afterScalePoint.toPoint());
    update();
}

void ImageViewer::centerImage()
{
    // 获取父控件的尺寸（QFrame），如果已经在 showEvent 中获取并保存了父控件尺寸
    int parentWidth = parent_width_;   // 从 showEvent 中保存的父控件宽度
    int parentHeight = parent_height_; // 从 showEvent 中保存的父控件高度

    if (parentWidth <= 0 || parentHeight <= 0)
    {
        qDebug() << "Invalid parent widget size.";
        return;
    }
     if (parent_width_ > 0 && parent_height_ > 0)
    {
        double widthRatio = static_cast<double>(parent_width_) / image_.cols;
        double heightRatio = static_cast<double>(parent_height_) / image_.rows;
        scaleFactor_ = std::min(widthRatio, heightRatio); // 确保图片适应窗口，不超出范围
    }
    // 计算缩放后的图像宽度和高度
    int imageWidth = image_.cols * scaleFactor_;
    int imageHeight = image_.rows * scaleFactor_;

    // 计算偏移量，使图像居中
    int offsetX = (parentWidth - imageWidth) / 2;
    int offsetY = (parentHeight - imageHeight) / 2;

    offset_ = QPoint(offsetX, offsetY);

    // 刷新界面，触发重新绘制
    update();
}


cv::Mat ImageViewer::getImage() const
{
    return image_;
}

void ImageViewer::resizeEvent(QResizeEvent  *event)
{
    QWidget::resizeEvent(event); // 调用基类的showEvent

    // 获取父控件的尺寸（QFrame）
    QWidget *parentWidget = this->parentWidget();
    if (parentWidget)
    {
        parent_width_ = parentWidget->width();
        parent_height_ = parentWidget->height();
        qDebug() << "Parent size (width, height):" << parent_width_ << parent_height_;
    }
    else
    {
        qDebug() << "No parent widget found.";
    }
}






void ImageViewer::updateImageDisplay()
{
    if (currentImageIndex_ >= 0 && currentImageIndex_ < images_.size())
    {
        const cv::Mat &mat = images_.at(currentImageIndex_);
        setImage(mat);
    }
}
void ImageViewer::loadImages(const QStringList &filePaths)
{
    images_.clear();             // 清空之前的图像
    currentImageIndex_ = -1;     // 重置索引


    for (const QString &filePath : filePaths)
    {
        cv::Mat img = cv::imread(filePath.toStdString());
        if (!img.empty())
        {
            images_.push_back(img); // 加载成功则保存
        }
        else
        {
            qDebug() << "Failed to load image:" << filePath;
        }
    }

    if (!images_.empty())
    {
        currentImageIndex_ = 0;  // 默认显示第一张图像
        updateImageDisplay();
    }
}
void ImageViewer::nextImage()
{
    if (currentImageIndex_ >= 0 && currentImageIndex_ < images_.size() - 1)
    {
        currentImageIndex_++;
        updateImageDisplay();
    }
    else
    {
        currentImageIndex_ = 0;
         updateImageDisplay();
    }
}

void ImageViewer::prevImage()
{
    if (currentImageIndex_ > 0)
    {
        currentImageIndex_--;
        updateImageDisplay();
    }
    else
    {
        currentImageIndex_ =images_.size()-1;
        updateImageDisplay();
    }
}
