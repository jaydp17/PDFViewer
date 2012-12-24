#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>

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
    
public slots:
    
private:
    QRubberBand *rubberband;
    QPoint origin;
};

#endif // PAGEVIEW_H
