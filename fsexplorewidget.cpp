#include "fsexplorewidget.h"
#include <QDir>
#include <QRegExpValidator>
#include <QDebug>


FSExploreWidget::FSExploreWidget(QWidget *parent) : QWidget{ parent },
    tabWidgetArea { new QTabWidget(this) },
    tabExplore { new QWidget(this) },
    tabFind { new QWidget(this) },
    exploreGridLay{ new QGridLayout(this) },
    findGridLay { new QGridLayout(this) },
    exploreTreeView{ new QTreeView(this) },
    findListView { new QListView(this) },
    mainPath { nullptr },
    disckSelBox { nullptr },
    lePath { new QLineEdit(this) },
    leFileName{ new QLineEdit(this) },
    tbGo { new QToolButton(this) },
    tbFind { new QToolButton(this) },
    modelExplore { new QStandardItemModel(this) },
    modelFind { new QStandardItemModel(this) },
    currentPath {},
    dirLabel{ new QLabel(this) },
    fileNameToFind{}

{
    parent->setMinimumSize(800,600);
    this->setMinimumSize(parent->minimumSize());
    tabWidgetArea->setMinimumSize(parent->minimumSize());

    //******************* Init Explorer Tab Area *************************

    tabWidgetArea->addTab(tabExplore, "Explore file system");
    tabExplore->setLayout(exploreGridLay);
    exploreGridLay->addWidget(exploreTreeView, 1, 0, 10, 10);

    //Юзер кликает по дереву каталогов
    connect(exploreTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(updatePath()));

    exploreGridLay->addWidget(lePath,0, 2, 1, 1);

    //Не дадим юзеру возможность вбивать первым символом слеш
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^(?!\\/).{0,}$"), this);
    lePath->setValidator(validator);

    //Юзер нажал 'Enter' в поле LineEdit
    connect(lePath, SIGNAL(returnPressed()), this, SLOT(goPath()));

    exploreGridLay->addWidget(tbGo, 0, 3, 1, 1);
    tbGo->setText("Go");
    connect(tbGo, SIGNAL(clicked()), this, SLOT(goPath()));

   if(QSysInfo::productType() == "windows")
   {
       disckSelBox = new QComboBox(this);
       QFileInfoList list = QDir::drives();
       QFileInfoList::const_iterator listdisk = list.begin();
       int amount = list.count();
       for (int i = 0; i < amount; i++)
       {
           disckSelBox->addItem(listdisk->path());
           listdisk++;
       }

        if (amount > 0)
       {

//rootDir для unix у нас константный и компилятор тут ругается;
//успокоим его
#if !defined(__unix__)
           rootDir = (list.at(0).path());
#endif
           rebuildExploreModel(rootDir);
        }

       exploreGridLay->addWidget(disckSelBox, 0, 0, 1, 2);
       connect(disckSelBox, SIGNAL(activated(int)), this, SLOT(chgDisk(int)));

   } else
    {
       mainPath = new QPushButton(this);
       mainPath->setText(rootDir);
       exploreGridLay->addWidget(mainPath, 0, 0, 1, 2);
       connect(mainPath, SIGNAL(clicked()), this, SLOT(goMainPath()));

       rebuildExploreModel(rootDir);
    }

    //******************* Init Find Tab Area ****************************

    tabWidgetArea->addTab(tabFind, "Find file/directory");
    connect(tabWidgetArea, SIGNAL(currentChanged(int)), this, SLOT(on_tabWidgetArea_changed(int)));

    tabFind->setLayout(findGridLay);
    findGridLay->addWidget(findListView, 2, 0, 10, 10);

    QLabel *infoDirLabel = new QLabel(this);
    infoDirLabel->setText("We will search in: ");
    findGridLay->addWidget(infoDirLabel, 0, 0, 1, 1);

    findGridLay->addWidget(dirLabel, 0, 1, 1, 1);

    QLabel *findLabel = new QLabel(this);
    findLabel->setText("Enter file/directory name:");

    findGridLay->addWidget(findLabel, 1, 0, 1, 1);

    findGridLay->addWidget(leFileName, 1, 1, 1, 1);

    connect(leFileName, SIGNAL(returnPressed()), this, SLOT(findFile()));

    findGridLay->addWidget(tbFind, 1, 2, 1, 1);
    tbFind->setText("Find");
    connect(tbFind, SIGNAL(clicked()), this, SLOT(findFile()));
}

void FSExploreWidget::chgDisk(int index)
{
   QFileInfoList list = QDir::drives();
   rebuildExploreModel(list.at(index).path());
}

void FSExploreWidget::goMainPath()
{
   currentPath = rootDir;
   rebuildExploreModel(currentPath);
   lePath->clear();
}


void FSExploreWidget::setNewExploreModel(QStandardItemModel *newModelExplore)
{
    exploreTreeView->setModel(newModelExplore);
    modelExplore = newModelExplore;
}

void FSExploreWidget::rebuildExploreModel(QString str)
{
   modelExplore->clear();

   QList<QStandardItem*> items;
   items.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), str));

   modelExplore->appendRow(items);

   //****************** заполнение списка директорий *****************

   QDir dir(str);
   dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);
   QStringList list = dir.entryList();

   int amount = list.count();

   QList<QStandardItem*>folders;

   for (int i = 0; i < amount; i++)
   {
       QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), list.at(i));
       folders.append(f);
   }

   items.at(0)->appendRows(folders);

   dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Files);
   list = dir.entryList();

   amount = list.count();

   //****************** заполнение списка файлов *******************

   QList<QStandardItem*>files;

   for (int i = 0; i < amount; i++)
   {
       QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), list.at(i));
       files.append(f);
   }

   items.at(0)->appendRows(files);
   setNewExploreModel(modelExplore);

   modelExplore->setHeaderData(0, Qt::Horizontal, "File system tree");
}

void FSExploreWidget::goPath()
{
    currentPath =  rootDir + lePath->text();

    if (currentPath.length() > 1)
#if !defined(__unix__) //windoze
        if (currentPath[1] == '\\')
#else //(__unix__)
        if(currentPath[1] == '/')
#endif
              currentPath.remove(1, 1);

    if (QDir(currentPath).exists())
        rebuildExploreModel(currentPath);
    else
    {
        lePath->clear();
        currentPath = rootDir;
        rebuildExploreModel(currentPath);
    }
}

//Добавляем выделенную юзером диру в LineEdit
void FSExploreWidget::updatePath()
{
    QModelIndex index = exploreTreeView->currentIndex();

    QVariant data = exploreTreeView->model()->data(index);

    QString tempPath { currentPath + rootDir + data.toString() };

    if (QDir(tempPath).exists())
        lePath->setText(tempPath);
}

void FSExploreWidget::findFile()
{
    if (currentPath.isEmpty())
            currentPath = rootDir;

    //QString fileNameToFind { leFileName->text() };
    fileNameToFind = leFileName->text();
    if (fileNameToFind.isEmpty())
        return;

    //rebuildFindModel(fileNameToFind);
    rebuildFindModel();

}

void FSExploreWidget::on_tabWidgetArea_changed(int index)
{
    if (index == 1)
    {
        if (currentPath.isEmpty())
                currentPath = rootDir;

        dirLabel->setText(currentPath);

    }
}

//void FSExploreWidget::rebuildFindModel(QString fileNameToFind)
void FSExploreWidget::rebuildFindModel()
{
    modelFind->clear();
    /*
    QString tempFileName;

    QDirIterator dirIterator(currentPath, QDirIterator::Subdirectories);
    size_t i{};

    while (dirIterator.hasNext())
    {
        tempFileName = dirIterator.next();
        QFileInfo fileInfo { tempFileName };

        if (fileInfo.fileName().contains(fileNameToFind, Qt::CaseInsensitive))
        {
            QStandardItem *foundItem;
            if (fileInfo.isDir())
            {
             foundItem = std::move
                ( new QStandardItem(QIcon(QApplication
                    ::style()->standardIcon(QStyle::SP_DirIcon)),fileInfo.filePath()));
            }
            else
            {
             foundItem = std::move
                ( new QStandardItem(QIcon(QApplication
                    ::style()->standardIcon(QStyle::SP_FileIcon)),fileInfo.filePath()));
            }

            modelFind->appendRow(foundItem);

        }
    }

    */
    ThreadRunner threadRunner(this, fileNameToFind);
    threadRunner.start();
    qDebug() << "Hello from outer " << thread()->currentThreadId();
    threadRunner.wait();

    findListView->setModel(modelFind);
    /*
//    ThreadMaker *threadMaker = new ThreadMaker(this);
    ThreadMaker threadMaker { this, fileNameToFind };
//    ThreadRunner threadMaker { this, fileNameToFind };
//    threadMaker.run();
    threadMaker.start();
    */
    /*
    threadMaker.wait();
    */
}

ThreadRunner::ThreadRunner(FSExploreWidget *outer, QString &path)
    : outerObj { outer }, path { path } {}

void ThreadRunner::run()
{
    //outerObj->modelFind->clear();
    QString tempFileName;

    QDirIterator dirIterator(outerObj->currentPath, QDirIterator::Subdirectories);
    size_t i{};

    while (dirIterator.hasNext())
    {
        tempFileName = dirIterator.next();
        QFileInfo fileInfo { tempFileName };



        if (fileInfo.fileName().contains(path, Qt::CaseInsensitive))
        {
            QStandardItem *foundItem;
            if (fileInfo.isDir())
            {
             foundItem = std::move
                ( new QStandardItem(QIcon(QApplication
                    ::style()->standardIcon(QStyle::SP_DirIcon)),fileInfo.filePath()));
            }
            else
            {
             foundItem = std::move
                ( new QStandardItem(QIcon(QApplication
                    ::style()->standardIcon(QStyle::SP_FileIcon)),fileInfo.filePath()));
            }

           outerObj-> modelFind->appendRow(foundItem);
        }
    }

    qDebug() << "Hello from inner" << thread()->currentThreadId();
    //outerObj->findListView->setModel(outerObj->modelFind);
}
