#ifndef FSEXPLOREWIDGET_H
#define FSEXPLOREWIDGET_H


#include <QWidget>
#include <QGridLayout>
#include <QTreeView>
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QApplication>
#include <QLineEdit>
#include <QToolButton>

//Q_PROPERTY(QStandardItemModel *model READ getCurrentModel WRITE setNewModel)

class FSExploreWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FSExploreWidget(QWidget *parent = nullptr);
   void clearTree();
   QStandardItemModel *getCurrentModel()const
   {
       return model;

   }

   void setNewModel(QStandardItemModel *newmodel);
   void rebuildModel(QString str);
private:
   QGridLayout *gridLay;
   QTreeView *tree;
   QPushButton *mainPath;
   QComboBox *disckSelBox;
   QLineEdit *lePath;
   QToolButton *tbGo;
   QStandardItemModel *model;
//   QString currentPath;

#if defined (__unix__)
   inline static const QString rootDir { '/' };
#elif
   QString rootDir;
#endif //unix

private slots:
   void chgDisk(int index);
   void goMainPath();
   void goPath();
   void updatePath();

protected:
};

#endif // FSEXPLOREWIDGET_H
