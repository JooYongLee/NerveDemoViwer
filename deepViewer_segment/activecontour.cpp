#include "activecontour.h"

QPolygonF PolygonContour::RestoreOriginalDomain(QPolygonF *polygon, QRectF imgRect, QSize img_origin_size)
{
    qreal x_scale = img_origin_size.width() / imgRect.width() ;
    qreal y_scale = img_origin_size.height() / imgRect.height();
    QPolygonF src_domain_cntr;
    for( int ind = 0; ind < polygon->count(); ind++)
    {
        qreal pt_x = polygon->at(ind).x() - imgRect.left();
        qreal pt_y = polygon->at(ind).y() - imgRect.top();


        qreal convert_pt_x = pt_x * x_scale;
        qreal convert_pt_y = pt_y * y_scale;
        src_domain_cntr.push_back(QPointF(convert_pt_x, convert_pt_y));

    }
//    src_domain_cntr.closePath();
    return src_domain_cntr;
}


//    qDebug()<<__FUNCTION__<<src_domain_box.getClass()<<box->GetBoxClass();
