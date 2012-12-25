#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>
#include <QRectF>

class PageView : public QLabel
{
    Q_OBJECT
public:
    PageView();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    
signals:
    void rubberBandSelection(QRectF rect);
public slots:
    
private:
    QRubberBand *rubberband;
    QPoint origin;
    QRectF rref;
};

#endif // PAGEVIEW_H
