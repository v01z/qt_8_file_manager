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

class ThreadRunner : public QThread
{
    Q_OBJECT
private:
    QString dirPath;
    QString fileNameToSearch;
    bool shouldTerminate;
public:
    ThreadRunner(const QString&, const QString&);
    void run();
signals:
    void fileIsFound(const QFileInfo&);
    void searchFinished();
public slots:
   void terminate();
};


class FSExploreWidget : public QWidget
{
   Q_OBJECT
public:
   explicit FSExploreWidget(QWidget *parent = nullptr);
   ~FSExploreWidget();

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
   QSharedPointer<ThreadRunner> threadRunner;
   QLabel *showFindResultLabel;
   size_t countOfFoundItems;
   bool searchInProgress;

   void clearTree();
   void setNewExploreModel(QStandardItemModel*);
   void rebuildExploreModel(const QString&);
   void rebuildFindModel(const QString&);
   inline void updateFindGUI();


#if defined (__unix__)
   inline static const QString rootDir { '/' };
#elif
   QString rootDir;
#endif //unix

private slots:
   void changeDisk(int index);
   void goMainPath();
   void goPath();
   void updatePath();
   void findFile();
   void on_tabWidgetArea_changed(int);

   void addItemToModelFind(const QFileInfo&);
   void applyFoundResultToView();

signals:
   void terminateChildThread();

};


#endif // FSEXPLOREWIDGET_H
