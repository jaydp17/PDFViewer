#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QtCore>
#include <QDesktopWidget>
#include <QtGui>

#include <poppler-qt4.h>

class Document
{
public:
    Document();
    ~Document();
    QString currentFileName();
    int currentPageIndex();
    bool openFile(QString filename);
    QImage renderPage(int index, double scaleFactor);
    int numPages();
    QRectF searchDocument(QString text, Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitivity);
    QRectF searchPage(QString text, Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitivity);
    QList<QRectF> selectionText(QRectF rect);
    QString selectedText();
    void showTOC(QTreeWidget *tree, QTreeWidgetItem *parentItem);

private:
    QString mFilename;
    int currentPage;
    Poppler::Document *pDoc;
    QRectF searchLocation;
    QString prevSearchtext;
    QString mSelectedText;
    QMap<QString,QString> *refMapping;

    double physicalDpiX();
    double physicalDpiY();
    QList<QRectF> selectAllInBetween(QList<Poppler::TextBox*> textList, int index, QRectF fullselection);
    QImage renderPrivate(int index, double scaleFactor);
    void fillTOC(const QDomNode &parent, QTreeWidget *tree, QTreeWidgetItem *parentItem);
};

#endif // DOCUMENT_H

