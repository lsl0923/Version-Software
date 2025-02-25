#include "ROIOptionBox.h"
#include <QPainter>
#include <QMouseEvent>

ROIOptionBox::ROIOptionBox(QWidget *parent) : QWidget(parent), selectedROI(-1)
{
    setFixedSize(220, 120);  // 固定大小
}

void ROIOptionBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制外部长方形框
    painter.setPen(Qt::black);
    painter.drawRect(10, 10, 200, 100);

    // 绘制多个正方形 ROI 选项
    for (int i = 0; i < 3; ++i) {
        QRect roiRect(20 + i * 60, 30, 40, 40);

        if (i == selectedROI) {
            painter.setBrush(Qt::cyan); // 选中颜色
        } else {
            painter.setBrush(Qt::NoBrush);
        }

        painter.drawRect(roiRect);
    }
}

void ROIOptionBox::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < 3; ++i) {
        QRect roiRect(20 + i * 60, 30, 40, 40);
        if (roiRect.contains(event->pos())) {
            selectedROI = i;
            update(); // 重新绘制
            emit roiSelected(i); // 发送信号
            return;
        }
    }
}
