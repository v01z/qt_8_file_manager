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

class FSExploreWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FSExploreWidget(QWidget *parent = nullptr);
   void clearTree();
   void setNewExploreModel(QStandardItemModel*);
   void rebuildExploreModel(QString);
   void rebuildFindModel(QString);
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
   QStandardItemModel *modelExplore;
   QStandardItemModel *modelFind;
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

protected:
};

#endif // FSEXPLOREWIDGET_H
