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


    void on_ToolsWight_doubleClicked(const QModelIndex &index);

protected:
    //void dropEvent(QDropEvent *event) override;
private:
    Ui::Form *ui;
     ImageViewer* imageViewer_; // 图像查看控件

    std::map<ToolsInfo, ManagerBase*> toolsMap_;

    // 1. 重新架构 tools toolsitem  toolsflow
    // 2. 输入输出数据泛型解决方案
    // 3. 工具改成树形结构，ToolsInfo 添加 类型和所在库，根据库更严格的创建工具
    std::vector<ToolsFlow> toolsFlows_;



    bool isExistsInToolsWight(const std::string toolName);
};

#endif // FORM_H
