#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QtCore>
#include <QDesktopWidget>

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
    QString selectionText(QRectF rect);

private:
    QString mFilename;
    int currentPage;
    Poppler::Document *pDoc;
    QRectF searchLocation;
    QString prevSearchtext;

    double physicalDpiX();
    double physicalDpiY();
    QImage renderPrivate(int index, double scaleFactor);
};

#endif // DOCUMENT_H

