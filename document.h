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
    QImage renderPage(int index);
    int numPages();
    QRectF searchForward(QString text);

private:
    QString mFilename;
    int currentPage;
    Poppler::Document *pDoc;
    QRectF searchLocation;

    double physicalDpiX();
    double physicalDpiY();
};

#endif // DOCUMENT_H

