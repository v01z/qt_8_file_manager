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
#include <QListView>
#include <QTabWidget>
#include <QLabel>

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
   QTabWidget *tabWidgetArea;
   QWidget *tabExplore;
   QWidget *tabFind;
   QGridLayout *exploreGridLay;
   QGridLayout *findGridLay;
   QTreeView *exploreTreeView;
   QListView *findListView;
   QPushButton *mainPath;
   QComboBox *disckSelBox;
   QLineEdit *lePath;
   QLineEdit *leFileName;
   QToolButton *tbGo;
   QToolButton *tbFind;
   QStandardItemModel *model;
   QString currentPath;
   QLabel *dirLabel;

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
   void findFile();
   void on_tabWidgetArea_changed(int);
   //QString removeOneSlash(QString&);
  // void on_lePath_text_changed(QString&);

protected:
};

#endif // FSEXPLOREWIDGET_H
