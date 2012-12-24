#include "pageview.h"

PageView::PageView() : QLabel()
{
    this->setAlignment(Qt::AlignCenter);
}

void PageView::mousePressEvent(QMouseEvent *ev)
{
    origin = ev->pos();
    if(!rubberband){
        rubberband = new QRubberBand(QRubberBand::Rectangle,this);
    }
    rubberband->setGeometry(QRect(origin, QSize()));
    rubberband->show();
}

void PageView::mouseMoveEvent(QMouseEvent *ev)
{
    rubberband->setGeometry(QRect(origin, ev->pos()).normalized());
}

void PageView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!rubberband->size().isEmpty()){
        QRectF rect(rubberband->rect());
        // TODO : create a rectangle with coordinates relative to the top-left corner of the image within the label
    }
}


