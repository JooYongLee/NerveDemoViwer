#ifndef ACTIVECONTOUR_H
#define ACTIVECONTOUR_H
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <QUuid>
#include <QGraphicsPolygonItem>
#include <QPoint>
#include <QPolygon>
#include <QList>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
class QLineEq
{
public:
    QLineEq(QPointF pt1, QPointF pt2)
    {
// x0 * x + y0 * y = c
        a = pt1.y() - pt2.y();
        b = pt2.x() - pt2.x();
        c = (pt1.x()*a + pt1.y()*b);
    }
    QLineEq(QPointF pt1, qreal vectorx, qreal vectory)
    {
        a = vectorx;
        b = vectory;
        c = pt1.x() * vectorx + pt1.y() * vectory;
    }

    ~QLineEq()
    {

    }
public:
    qreal a;
    qreal b;
    qreal c;
};
class SolveLineEq
{
public:
    SolveLineEq()
    {

    }
    ~SolveLineEq()
    {

    }
    static QPointF SolveLines( QLineEq line1, QLineEq line2)
    {
        qreal a1 = line1.a;
        qreal b1 = line1.b;
        qreal c1 = line1.c;
        qreal a2 = line2.a;
        qreal b2 = line2.b;
        qreal c2 = line2.c;
        qreal det = a1 * b2 -  b1* a2;

        qreal x0 = b2 * c1 - b1 * c2;
        qreal y0 = -a2*c1 + a1*c2;
        x0 /= det;
        y0 /= det;
        return QPointF(x0, y0);
    }
    static QPointF SolveNormal( QLineEq line, QPointF pt)
    {
        QLineEq normaline(pt, line.b, -line.a);
        return SolveLines(line, normaline);
    }
    static qreal CalcDist( QPointF pt1,
                           QPointF pt2)
    {
        return sqrt( (pt1.x() - pt2.x()) * (pt1.x() - pt2.x())+\
                     (pt1.y() - pt2.y()) * (pt1.y() - pt2.y())
                    );
    }

    static QPointF SolveBezierLine(QPointF pt1,
                                   QPointF pt2,
                                   QPointF pt3)
    {
        QPointF midpt = (pt1 + pt2)/2;
        QPointF midpt_radius = pt2 - midpt;
    // rotate 90, -90 degree
        QPointF p90rotate( -midpt_radius.y() + midpt.x(),
                            midpt_radius.x() + midpt.y());
        QPointF m90rotate(  midpt_radius.y() + midpt.x(),
                           -midpt_radius.x() + midpt.y());

//        QPointF direct = pt2 - pt1;
        if( CalcDist(pt3, p90rotate) > CalcDist(pt3, m90rotate))
        {
            return p90rotate;
        }
        else
        {
            return m90rotate;
        }
    }
};



class PolygonContour : public QGraphicsPathItem
{

//    Q_OBJECT
public:
    QPolygonF _polygon;
    QList<QGraphicsLineItem> pts;
    qreal prevpts;
    qreal newpts;
    QPolygonF _bezierPolygon;


public:
    PolygonContour(QGraphicsItem *parent = 0)
    {
        prevpts = -100000;
        newpts = -100000;
        QPen contourPen = QPen(QColor(130, 224, 170),2);
        this->setPen(contourPen);
    }
    ~PolygonContour()
    {

    }
    QPolygonF BezierCurve()
    {
        int n = _polygon.count();
        QPointF pt1 = _polygon.at(n-3);
        QPointF pt2 = _polygon.at(n-2);
        QPointF pt3 = _polygon.at(n-1);

        QLineEq line23(pt2,pt3);
//        QPointF con_pt = SolveLineEq::SolveNormal(line23,pt1);
        QPointF con_pt = SolveLineEq::SolveBezierLine(pt1,pt2,pt3);


        qDebug()<<__FUNCTION__<<pt1<<pt2<<pt3;
        qDebug()<<__FUNCTION__<<con_pt;

//        QPolygonF bezier;
        const int segment = 30;
        for( int i =0; i < segment; i++)
        {
            qreal t = i / qreal(segment-1);
            qreal invt = 1 -t;

            QPointF pos =  invt * invt * pt1
                                + 2 * invt * t * con_pt
                                + t * t * pt2;
            _bezierPolygon.push_back(pos);
        }
        return _bezierPolygon;
    }

    void PutPoints(QPointF pt)
    {
        qDebug()<<__FUNCTION__<<pt;
        _polygon.push_back(pt);
        QPainterPath contourpath;
        if( _polygon.count() >= 3)
        {
            contourpath.addPolygon(_polygon.toPolygon());
        }        
        this->setPath(contourpath);
    }
    void PopPoints()
    {
        if( _polygon.count() > 1 )
        {
            _polygon.pop_back();
            qDebug()<<__FUNCTION__<<_polygon;
            QPainterPath contourpath;
            contourpath.addPolygon(_polygon);
            this->setPath(contourpath);
        }
    }
    QPolygonF GetPolygon()
    {
        return _polygon;
    }

    void closePath()
    {
        QPainterPath contourpath;
        contourpath.addPolygon(_polygon);
        contourpath.closeSubpath();

        this->setPath(contourpath);
    }
    static QPolygonF RestoreOriginalDomain(QPolygonF *contour, QRectF imgRect, QSize img_origin_size);



    void reset()
    {
        _polygon.clear();
    }


};
#endif // ACTIVECONTOUR_H
