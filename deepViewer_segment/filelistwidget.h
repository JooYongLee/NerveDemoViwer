#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H
#include <QListWidget>
#include <QWidget>
#include <QDebug>
class FileListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent=0)
        :QListWidget(parent)
    {
        this->setSelectionRectVisible(true);

//        QPalette p;
//        p.setColor(QPalette::Highlight, Qt::red);
//        QAbstractItemView::SelectionBehavior
//        this->setPalette(Qt::yellow);
//        this->setFo
//        this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    }
//    void selec


//    void currentRowChanged( const QModelIndex & current, const QModelIndex &    previous )
//    {
//        qDebug()<<__FUNCTION__<<this->item(current.row())->text();
//    }

//    void selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
//    {
//        ModelIndexList indexes  = selected.indexes();
//        foreach(const QModelIndex &index, indexes) {
//            QListWidgetItem *item = this->item(index.row());
//            // ...
//        }
//    }


};

#endif // FILELISTWIDGET_H
