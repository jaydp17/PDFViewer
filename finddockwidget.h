#ifndef FINDDOCKWIDGET_H
#define FINDDOCKWIDGET_H

#include <QDockWidget>
#include <QtGui>

#include <poppler-qt4.h>

namespace Ui {
class FindDockWidget;
}

class FindDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit FindDockWidget(QWidget *parent = 0);
    ~FindDockWidget();
    Poppler::Page::SearchMode caseSensitivity();
    bool isFullDocumentSearch();
    void show();

signals:
    void search(const QString text);
    void nextClicked(const QString text);
    void prevClicked(const QString text);
    void caseSensToggled(bool checked);
    void fullDocToggled(bool checked);

    
private:
    Ui::FindDockWidget *ui;
    QString searchBoxStyle;

private slots:
    void searchReturnPressed();
    void searchNext();
    void searchPrev();
    void caseSensChanged(bool checked);
    void fullDocChanged(bool checked);
};

#endif // FINDDOCKWIDGET_H
