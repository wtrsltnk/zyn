#include "Dial.h"
#include <QMenu>
#include <QMouseEvent>
#include <QtDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QConicalGradient>
#include <math.h>
#include "Menu.h"

//drawstyles: 0 - piechart
//            1 - rotated dial
//            2 - rectangular
static int drawStyle = 1;

Dial::Dial(QWidget *parent)
    :QDial(parent),
      m_originalMouseY(-1),
      m_isConnected(false)
{
    setMouseTracking(false);
    setMinimum(0);
    setMaximum(127);
    setPageStep(32);

    ControlHelper *helper = new ControlHelper(this);

    connect(this, SIGNAL(sliderMoved(int)),
            helper, SLOT(setValue(int)));
    connect(helper, SIGNAL(valueChanged(int)),
            this, SLOT(setValue(int)));

    connect(helper, SIGNAL(connected(GenControl *)),
            this, SLOT(slotConnected(GenControl *)));
    connect(helper, SIGNAL(disconnected()),
            this, SLOT(slotDisconnected()));

    new Menu(this, helper);
}

void Dial::mousePressEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton)) {
        m_originalMouseY = event->y();
        m_originalValueOnPress = value();
        event->accept();
        setSliderDown(true);
        return;
    }
    QDial::mousePressEvent(event);
}

void Dial::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::RightButton)) {
        QDial::mouseReleaseEvent(event);
    }
    else {
        m_originalMouseY = -1;
        setSliderDown(false);
    }
}

void Dial::mouseMoveEvent(QMouseEvent *event)
{
    if(isSliderDown()) {
        setValue(m_originalValueOnPress
                 + float(m_originalMouseY - event->y())
                 * 0.5
                );
        event->accept();
    }
    //QDial::mouseMoveEvent(event);
}

void Dial::slotConnected(GenControl *control)
{
    m_isConnected = true;
    update();
}

void Dial::slotDisconnected()
{
    m_isConnected = false;
    update();
}

void Dial::paintEvent(class QPaintEvent *event)
{
    QPainter p(this);

    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    if(r.width() < r.height())
        r.setHeight(r.width());
    else
    if(r.height() < r.width())
        r.setWidth(r.height());

    r.setSize(r.size() * 0.9);
    r.moveCenter(rect().center());

    float v = (float(value()) / (maximum() - minimum()));

    if(drawStyle == 0) {
        QConicalGradient grad(r.center(), 270 - 13);
        p.setBrush(QColor(Qt::white));

        p.drawEllipse(r);

        grad.setColorAt(1, Qt::yellow);
        grad.setColorAt(0.5, QColor(255, 128, 0));
        grad.setColorAt(0, Qt::red);
        p.setBrush(grad);
        p.drawPie(r, 255 * 16, -v * 16);
        //p.drawPoint(10 *
    }
    else
    if(drawStyle == 1) {
        //p.setBrush(QColor(Qt::white));

        //center circle
        p.drawEllipse(r);

        p.translate(r.center());

        for(float i = -PI / 4; i <= (PI + PI / 4); i += PI / 6)

            //draw lots of markers
            p.drawLine(
                r.width() * 0.4 * cos(i), -r.height() * 0.4 * sin(i),
                r.width() * 0.5 * cos(i), -r.height() * 0.5 * sin(i));



        QRect smallRect = r;
        smallRect.setSize(smallRect.size() * 0.6);
        smallRect.moveCenter(QPoint(0, 0));

        if (m_isConnected)
            p.setBrush(palette().alternateBase());
        else
            p.setBrush(palette().dark());

        p.drawPie(smallRect, 225 * 16, -v * 270 * 16);
        //p.rotate(v);

        //QPoint point = r.center();
        //point.setY(point.y() - r.height() / 2);

        //p.drawEllipse(QPoint(0, r.height() / 3), 4, 4);
    }
    else
    if(drawStyle == 2) {
        r = rect();
        p.drawRect(r);
        //p.setBrush(palette().alternateBase());
        p.setBrush(QColor(Qt::white));
        p.drawRect(r.x(), r.height() + r.y() - v * r.height(),
                   r.width(), v * r.height());
    }
}

void Dial::wheelEvent(class QWheelEvent *event)
{
    QDial::wheelEvent(event);
    emit sliderMoved(value());
}

void Dial::setControl(GenControl *control)
{}

void Dial::slotUpdateSource()
{}

#include "Dial.moc"
