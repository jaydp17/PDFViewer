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
    QRectF searchForward(QString text);
    QString selectionText(QRectF rect);

private:
    QString mFilename;
    int currentPage;
    Poppler::Document *pDoc;
    QRectF searchLocation;

    double physicalDpiX();
    double physicalDpiY();
    QImage renderPrivate(int index, double scaleFactor);
};

#endif // DOCUMENT_H

