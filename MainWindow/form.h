#ifndef FORM_H
#define FORM_H
#include<map>
#include <QWidget>
#include"ImageViewer.h"

#include"ToolsFlow.h"
#include"QListWidget"
#include"managerbase.h"
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void loadPlugin(const QString& path);
    void loadPluginC(const QString& path);
private slots:

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2__clicked();

    void on_RunWidget_itemChanged(QListWidgetItem *item);

    void setInputMenu(const std::string& toolId, QMenu* inputMenu);
    void on_ToolsWight_doubleClicked(const QModelIndex &index);
    void onRightClick(const QPoint& pos);
protected:
    //void dropEvent(QDropEvent *event) override;
private:
    Ui::Form *ui;
    ImageViewer* imageViewer_; // 图像查看控件

    std::map<ToolsInfo, ManagerBase*> toolsMap_;


    std::vector<ToolsFlow> toolsFlows_;



    bool isExistsInToolsWight(const std::string toolName);
};

#endif // FORM_H
