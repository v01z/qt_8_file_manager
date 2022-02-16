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
#include <QThread>
#include <QDirIterator>

class FSExploreWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FSExploreWidget(QWidget *parent = nullptr);
   void clearTree();
   void setNewExploreModel(QStandardItemModel*);
   void rebuildExploreModel(QString);
   //void rebuildFindModel(QString);
   void rebuildFindModel();

   friend class ThreadRunner;

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
   QString fileNameToFind;


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

class ThreadRunner : public QThread
{
private:
    //FSExploreWidget *fseExploreWidget;
    FSExploreWidget *outerObj;
    QString path;
public:
    ThreadRunner(FSExploreWidget*, QString&);
    void run();

};

#endif // FSEXPLOREWIDGET_H
