#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>
#include <QDockWidget>
#include <QtGui>
#include <QtCore>
#include <QRectF>

#include "document.h"

class PageView : public QLabel
{
    Q_OBJECT
public:
    PageView();
    void setDocumentPointer(Document *p);
    void highlight(QRectF rect);
    void highlight(QList<QRectF> list);
    void setCurrentPixmap(QPixmap pxmap);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    
signals:
    void rubberBandSelection(QRectF rect);
    void textSelected();
    void getDocumentPointer();

public slots:
    
private:
    QRubberBand *rubberband;
    QPoint origin;
    QRectF rref;
    Document *mDoc;
    QPixmap currentPxmap;
};

#endif // PAGEVIEW_H
