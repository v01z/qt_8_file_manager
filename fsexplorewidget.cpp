#include "fsexplorewidget.h"
#include <QDir>
#include <QRegExpValidator>
#include <QDebug>

FSExploreWidget::FSExploreWidget(QWidget *parent) : QWidget{ parent },
    gridLay{ new QGridLayout(this) },
    tree{ new QTreeView(this) },
    mainPath { nullptr },
    disckSelBox { nullptr },
    lePath { new QLineEdit(this) },
    tbGo { new QToolButton(this) },
    tbFind { new QToolButton(this) },
    model { nullptr },
    currentPath {}

{
    parent->setMinimumSize(500,600);

    this->setLayout(gridLay);

    gridLay->addWidget(tree, 1, 0, 10, 10);

    //Юзер кликает по дереву каталогов
    connect(tree, SIGNAL(clicked(QModelIndex)), this, SLOT(updatePath()));

    setMinimumSize(500, 600);

    gridLay->addWidget(lePath,0, 2, 1, 1);

    //Удалим слеш в начале пути
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^(?!\\/).{0,}$"), this);
    lePath->setValidator(validator);

    //Юзер нажал 'Enter' в поле LineEdit
    connect(lePath, SIGNAL(returnPressed()), this, SLOT(goPath()));

    gridLay->addWidget(tbGo, 0, 3, 1, 1);
    tbGo->setText("Go");
    connect(tbGo, SIGNAL(clicked()), this, SLOT(goPath()));

    gridLay->addWidget(tbFind, 0, 4, 1, 1);
    tbFind->setText("Find here");
    connect(tbFind, SIGNAL(clicked()), this, SLOT(showFindWindow()));

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
           rebuildModel(rootDir);
        }

       gridLay->addWidget(disckSelBox, 0, 0, 1, 2);
       connect(disckSelBox, SIGNAL(activated(int)), this, SLOT(chgDisk(int)));

   } else
    {
       mainPath = new QPushButton(this);
       mainPath->setText(rootDir);
       gridLay->addWidget(mainPath, 0, 0, 1, 2);
       connect(mainPath, SIGNAL(clicked()), this, SLOT(goMainPath()));

       rebuildModel(rootDir);
    }
}

void FSExploreWidget::chgDisk(int index)
{
   QFileInfoList list = QDir::drives();
   rebuildModel(list.at(index).path());
}

void FSExploreWidget::goMainPath()
{
   currentPath = rootDir;
   rebuildModel(currentPath);
   lePath->clear();
}


void FSExploreWidget::setNewModel(QStandardItemModel *newmodel)
{
    tree->setModel(newmodel);
    model = newmodel;
}

void FSExploreWidget::rebuildModel(QString str)
{
   QStandardItemModel *model = new QStandardItemModel(this);

   QList<QStandardItem*> items;
   items.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), str));

   model->appendRow(items);

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
   setNewModel(model);

   model->setHeaderData(0, Qt::Horizontal, "File system tree");
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
        rebuildModel(currentPath);
    else
    {
        lePath->clear();
        currentPath = rootDir;
        rebuildModel(currentPath);
    }
}

//Добавляем выделенную юзером диру в LineEdit
void FSExploreWidget::updatePath()
{
    QModelIndex index = tree->currentIndex();

    QVariant data = tree->model()->data(index);

    QString tempPath { currentPath + rootDir + data.toString() };

    if (QDir(tempPath).exists())
        lePath->setText(tempPath);
}

void FSExploreWidget::showFindWindow()
{

}
