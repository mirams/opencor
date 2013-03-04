//==============================================================================
// Single cell simulation view graph panel plot widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelplotwidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>

//==============================================================================

#include <float.h>

//==============================================================================

#include "qwt_painter.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_directpainter.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_renderer.h"
#include "qwt_scale_div.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelPlotCurve::SingleCellSimulationViewGraphPanelPlotCurve() :
    QwtPlotCurve()
{
    // Customise it a bit

    setPen(QPen(Qt::darkBlue));
    setRenderHint(QwtPlotItem::RenderAntialiased);
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::SingleCellSimulationViewGraphPanelPlotWidget(QWidget *pParent) :
    QwtPlot(pParent),
    mCurves(QList<SingleCellSimulationViewGraphPanelPlotCurve *>()),
    mAction(None),
    mOriginPoint(QPoint()),
    mMinX(100.0),
    mMaxX(300.0),
    mMinY(0.0),
    mMaxY(1000.0),
    mFixedAxisX(false),
    mFixedAxisY(false)
{
    // Get ourselves a direct painter

    mDirectPainter = new QwtPlotDirectPainter(this);

    // Speedup painting on X11 systems
    // Note: this can only be done on X11 systems...

    if (QwtPainter::isX11GraphicsSystem())
        canvas()->setAttribute(Qt::WA_PaintOnScreen, true);

    // Customise ourselves a bit

    setCanvasBackground(Qt::white);

    // We don't want a frame around ourselves

    qobject_cast<QwtPlotCanvas *>(canvas())->setFrameShape(QFrame::NoFrame);

    // Set our axes' local minimum/maximum values

    setLocalMinX(mMinX);
    setLocalMaxX(mMaxX);
    setLocalMinY(mMinY);
    setLocalMaxY(mMaxY);

    // Attach a grid to ourselves

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(this);
}

//==============================================================================

SingleCellSimulationViewGraphPanelPlotWidget::~SingleCellSimulationViewGraphPanelPlotWidget()
{
    // Delete some internal objects

    delete mDirectPainter;
}

//==============================================================================

bool SingleCellSimulationViewGraphPanelPlotWidget::eventFilter(QObject *pObject,
                                                               QEvent *pEvent)
{
    // Default handling of the event

    bool res = QwtPlot::eventFilter(pObject, pEvent);

    // We want to handle a double mouse click, but for some reasons to override
    // mouseDoubleClickEvent() doesn't work, so...

    if(pEvent->type() == QEvent::MouseButtonDblClick)
        handleMouseDoubleClickEvent(static_cast<QMouseEvent *>(pEvent));

    // We are all done, so...

    return res;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::handleMouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Copy the contents of the plot to the clipboard, in case we double-clicked
    // using the left mouse button

    if (pEvent->button() == Qt::LeftButton) {
        // Render the plot to an image

        QwtPlotRenderer renderer;
        QImage image = QImage(size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);

        renderer.render(this, &painter, rect());

        // Set the image to the clipboard

        QApplication::clipboard()->setImage(image);
    }
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::minX() const
{
    // Return our minimum X value

    return mMinX;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMinX(const double &pMinX)
{
    // Set our minimum X value

    mMinX = pMinX;
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::maxX() const
{
    // Return our maximum X value

    return mMaxX;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMaxX(const double &pMaxX)
{
    // Set our maximum X value

    mMaxX = pMaxX;
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::minY() const
{
    // Return our minimum Y value

    return mMinY;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMinY(const double &pMinY)
{
    // Set our minimum Y value

    mMinY = pMinY;
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::maxY() const
{
    // Return our maximum Y value

    return mMaxY;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setMaxY(const double &pMaxY)
{
    // Set our maximum Y value

    mMaxY = pMaxY;
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMinX() const
{
    // Return our local minimum X value

    return axisScaleDiv(QwtPlot::xBottom).lowerBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMinX(const double &pLocalMinX)
{
    // Set our local minimum X value
    // Note: to use setAxisScale() on its own is not sufficient unless we were
    //       to replot ourselves immediately after, but we don't necessarily
    //       want to do that, so instead we also use setAxisScaleDiv() to make
    //       sure that our local minimum X value is indeed taken into account
    //       (i.e. we can retrieve it using localMinX()). Also, we must call
    //       setAxisScaleDiv() before setAxisScale() to make sure that the axis
    //       data is not considered as valid which is important when it comes to
    //       plotting ourselves...

    setAxisScaleDiv(QwtPlot::xBottom, QwtScaleDiv(pLocalMinX, localMaxX()));
    setAxisScale(QwtPlot::xBottom, pLocalMinX, localMaxX());
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMaxX() const
{
    // Return our local maximum X value

    return axisScaleDiv(QwtPlot::xBottom).upperBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMaxX(const double &pLocalMaxX)
{
    // Set our local maximum X value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::xBottom, QwtScaleDiv(localMinX(), pLocalMaxX));
    setAxisScale(QwtPlot::xBottom, localMinX(), pLocalMaxX);
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMinY() const
{
    // Return our local minimum Y value

    return axisScaleDiv(QwtPlot::yLeft).lowerBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMinY(const double &pLocalMinY)
{
    // Set our local minimum Y value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv(pLocalMinY, localMaxY()));
    setAxisScale(QwtPlot::yLeft, pLocalMinY, localMaxY());
}

//==============================================================================

double SingleCellSimulationViewGraphPanelPlotWidget::localMaxY() const
{
    // Return our local maximum Y value

    return axisScaleDiv(QwtPlot::yLeft).upperBound();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setLocalMaxY(const double &pLocalMaxY)
{
    // Set our local maximum Y value
    // Note: see the corresponding note in setLocalMinX()...

    setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv(localMinY(), pLocalMaxY));
    setAxisScale(QwtPlot::yLeft, localMinY(), pLocalMaxY);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setFixedAxisX(const bool &pFixedAxisX)
{
    // Specify whether our X axis is to be fixed

    mFixedAxisX = pFixedAxisX;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setFixedAxisY(const bool &pFixedAxisY)
{
    // Specify whether our Y axis is to be fixed

    mFixedAxisY = pFixedAxisY;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::setAxes(const double &pMinX,
                                                           const double &pMaxX,
                                                           const double &pMinY,
                                                           const double &pMaxY,
                                                           const bool &pCanReplot,
                                                           const bool &pForceMinMaxValues,
                                                           const bool &pUpdateMinMaxValues,
                                                           const bool &pResetMinMaxValues)
{
    // Update our axes

    double oldLocalMinX = localMinX();
    double oldLocalMaxX = localMaxX();
    double oldLocalMinY = localMinY();
    double oldLocalMaxY = localMaxY();

    double newLocalMinX = pMinX;
    double newLocalMaxX = pMaxX;
    double newLocalMinY = pMinY;
    double newLocalMaxY = pMaxY;

    // Retrieve the bounding rectangle for all our curves (that is, as long as
    // they have some data)

    QRectF boundingRect = QRectF();

    foreach (SingleCellSimulationViewGraphPanelPlotCurve *curve, mCurves)
        if (curve->dataSize())
            boundingRect |= curve->boundingRect();

    // Update the minimum/maximum values of our axes, should we have retrieved a
    // valid bounding rectangle

    if (boundingRect != QRectF()) {
        // Optimise our bounding rectangle

        double minX = boundingRect.left();
        double maxX = boundingRect.right();
        double minY = boundingRect.top();
        double maxY = boundingRect.bottom();

        double powerX = qPow(10.0, qMax(minX?qFloor(log10(qAbs(minX))):0, maxX?qFloor(log10(qAbs(maxX))):0));
        double powerY = qPow(10.0, qMax(minY?qFloor(log10(qAbs(minY))):0, maxY?qFloor(log10(qAbs(maxY))):0));

        minX = powerX*qFloor(minX/powerX);
        maxX = powerX*qCeil(maxX/powerX);
        minY = powerY*qFloor(minY/powerY);
        maxY = powerY*qCeil(maxY/powerY);

        // Update the minimum/maximum values of our axes, if required

        if (!mFixedAxisX) {
            if (pResetMinMaxValues) {
                mMinX = minX;
                mMaxX = maxX;
            } else if (pUpdateMinMaxValues) {
                mMinX = qMin(mMinX, minX);
                mMaxX = qMax(mMaxX, maxX);
            }
        }

        if (!mFixedAxisY) {
            if (pResetMinMaxValues) {
                mMinY = minY;
                mMaxY = maxY;
            } else if (pUpdateMinMaxValues) {
                mMinY = qMin(mMinY, minY);
                mMaxY = qMax(mMaxY, maxY);
            }
        }
    }

    // Make sure that the new local minimum/maximum values of our axes fit
    // within the minimum/maximum values of our axes

    if (pForceMinMaxValues) {
        newLocalMinX = mMinX;
        newLocalMaxX = mMaxX;
        newLocalMinY = mMinY;
        newLocalMaxY = mMaxY;
    } else {
        newLocalMinX = qMax(newLocalMinX, mMinX);
        newLocalMaxX = qMin(newLocalMaxX, mMaxX);
        newLocalMinY = qMax(newLocalMinY, mMinY);
        newLocalMaxY = qMin(newLocalMaxY, mMaxY);
    }

    // Update the local minimum/maximum of our axes, if needed

    bool needReplot = false;

    if (newLocalMinX != oldLocalMinX) {
        setLocalMinX(newLocalMinX);

        needReplot = true;
    }

    if (newLocalMaxX != oldLocalMaxX) {
        setLocalMaxX(newLocalMaxX);

        needReplot = true;
    }

    if (newLocalMinY != oldLocalMinY) {
        setLocalMinY(newLocalMinY);

        needReplot = true;
    }

    if (newLocalMaxY != oldLocalMaxY) {
        setLocalMaxY(newLocalMaxY);

        needReplot = true;
    }

    // Replot ourselves, if needed and allowed

    if (needReplot && pCanReplot)
        replotNow();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::scaleAxes(const double &pScalingFactorX,
                                                             const double &pScalingFactorY)
{
    // Determine the local minimum/maximum values of our two axes

    double rangeX = localMaxX()-localMinX();
    double centerX = localMinX()+0.5*rangeX;
    double rangeOverTwoX = 0.5*pScalingFactorX*rangeX;

    double rangeY = localMaxY()-localMinY();
    double centerY = localMinY()+0.5*rangeY;
    double rangeOverTwoY = 0.5*pScalingFactorY*rangeY;

    // Rescale our two axes

    setAxes(centerX-rangeOverTwoX, centerX+rangeOverTwoX,
            centerY-rangeOverTwoY, centerY+rangeOverTwoY);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::checkAxes(const bool &pCanReplot,
                                                             const bool &pForceMinMaxValues,
                                                             const bool &pUpdateMinMaxValues)
{
    // Check our axes by trying to set them

    setAxes(localMinX(), localMaxX(), localMinY(), localMaxY(),
            pCanReplot, pForceMinMaxValues, pUpdateMinMaxValues);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::resetAxes(const bool &pCanReplot)
{
    // Reset our axes by trying to set them

    setAxes(0.0, 0.0, 0.0, 0.0, pCanReplot, true, false, true);
}

//==============================================================================

static const double NoScalingFactor  = 1.0;
static const double ScalingInFactor  = 0.57;
static const double ScalingOutFactor = 1.0/ScalingInFactor;

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseMoveEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseMoveEvent(pEvent);

    // Determine how much we have moved our mouse since last time

    int deltaX = pEvent->pos().x()-mOriginPoint.x();
    int deltaY = pEvent->pos().y()-mOriginPoint.y();

    // Carry out the action

    switch (mAction) {
    case Pan: {
        // Pan ourselves, if needed

        if (deltaX || deltaY) {
            // Compute the shifts in the X and Y directions

            double shiftX = -deltaX*(plotLayout()->canvasRect().width()?(localMaxX()-localMinX())/plotLayout()->canvasRect().width():0.0);
            double shiftY =  deltaY*(plotLayout()->canvasRect().height()?(localMaxY()-localMinY())/plotLayout()->canvasRect().height():0.0);

            // Determine our new local minimum/maximum values for our axes

            double newLocalMinX = localMinX()+shiftX;
            double newLocalMaxX = localMaxX()+shiftX;
            double newLocalMinY = localMinY()+shiftY;
            double newLocalMaxY = localMaxY()+shiftY;

            // Make sure that our new local minimum/maximum values for our axes
            // are within our local minimum/maximum values

            if (newLocalMinX < minX()) {
                newLocalMinX = minX();
                newLocalMaxX = newLocalMinX+localMaxX()-localMinX();
            } else if (newLocalMaxX > maxX()) {
                newLocalMaxX = maxX();
                newLocalMinX = newLocalMaxX-localMaxX()+localMinX();
            }

            if (newLocalMinY < minY()) {
                newLocalMinY = minY();
                newLocalMaxY = newLocalMinY+localMaxY()-localMinY();
            } else if (newLocalMaxY > maxY()) {
                newLocalMaxY = maxY();
                newLocalMinY = newLocalMaxY-localMaxY()+localMinY();
            }

            // Set our new local minimum/maximum values for our axes which will
            // replot ourselves as a result

            setAxes(newLocalMinX, newLocalMaxX, newLocalMinY, newLocalMaxY);
        }

        break;
    }
    case Zoom: {
        // Rescale ourselves (which will replot ourselves as a result)

        if (deltaX || deltaY)
            scaleAxes(deltaX?
                          (deltaX > 0)?
                              ScalingInFactor:
                              ScalingOutFactor:
                          NoScalingFactor,
                      deltaY?
                          (deltaY > 0)?
                              ScalingInFactor:
                              ScalingOutFactor:
                          NoScalingFactor);

        break;
    }
    default:
        // None

        ;
    }

    // Reset our point of origin

    mOriginPoint = pEvent->pos();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mousePressEvent(pEvent);

    // Check which action we can carry out

    if (   (pEvent->button() == Qt::LeftButton)
        && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to pan

        mAction = Pan;
    } else if (   (pEvent->button() == Qt::RightButton)
               && (pEvent->modifiers() == Qt::NoModifier)) {
        // We want to zoom in/out

        mAction = Zoom;
    } else {
        // None of the actions we can carry out, so...

        mAction = None;

        return;
    }

    // Keep track of the mouse position and make sure that we track the mouse

    mOriginPoint = pEvent->pos();

    setMouseTracking(true);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::mouseReleaseEvent(pEvent);

    // Check whether we need to carry out an action

    if (mAction == None)
        return;

    // Stop tracking the mouse

    setMouseTracking(false);

    // We are done carrying out an action, so...

    mAction = None;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Default handling of the event

    QwtPlot::wheelEvent(pEvent);

    // The only action we support using the wheel is zooming in/out, but this
    // requires no modifiers being used

    if (pEvent->modifiers() != Qt::NoModifier)
        return;

    // Make sure that there is a need to zoom in/out

    if (!pEvent->delta())
        return;

    // Zoom in/out by scaling our two axes

    double scalingFactor = (pEvent->delta() > 0)?ScalingInFactor:ScalingOutFactor;

    scaleAxes(scalingFactor, scalingFactor);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::replotNow()
{
    // Replot ourselves, but only if the local minimum/maximum values of our
    // axes are valid

    if (   (qAbs(localMinX()) == DBL_MAX) || (qAbs(localMaxX()) == DBL_MAX)
        || (qAbs(localMinY()) == DBL_MAX) || (qAbs(localMaxY()) == DBL_MAX))
        return;

    replot();

    // Make sure that the replotting occurs immediately
    // Note: this is needed when running a simulation since, otherwise,
    //       replotting won't occur immediately (because of threading)...

    qApp->processEvents();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::attach(SingleCellSimulationViewGraphPanelPlotCurve *pCurve)
{
    // Make sure that the given curve is not already attached to us

    if (mCurves.contains(pCurve))
        return;

    // Attach the given curve to ourselves

    pCurve->attach(this);

    // Add the given curve to our list of curves

    mCurves << pCurve;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::detach(SingleCellSimulationViewGraphPanelPlotCurve *pCurve)
{
    // Check that the given curve is attached to us

    if (!mCurves.contains(pCurve))
        return;

    // Detach the given curve from ourselves

    pCurve->detach();

    // Remove the given curve from our list of curves

    mCurves.removeOne(pCurve);
}

//==============================================================================

void SingleCellSimulationViewGraphPanelPlotWidget::drawCurveSegment(SingleCellSimulationViewGraphPanelPlotCurve *pCurve,
                                                                    const qulonglong &pFrom,
                                                                    const qulonglong &pTo)
{
    // Make sure that we have a curve segment to draw

    if (pFrom == pTo)
        return;

    // Determine the minimum/maximum X/Y values of our new data

    double xMin;
    double xMax;
    double yMin;
    double yMax;

    for (qulonglong i = pFrom; i <= pTo; ++i)
        if (i == pFrom) {
            xMin = xMax = pCurve->data()->sample(i).x();
            yMin = yMax = pCurve->data()->sample(i).y();
        } else {
            double xVal = pCurve->data()->sample(i).x();
            double yVal = pCurve->data()->sample(i).y();

            xMin = qMin(xMin, xVal);
            xMax = qMax(xMax, xVal);

            yMin = qMin(yMin, yVal);
            yMax = qMax(yMax, yVal);
        }

    // Check which curve segment we are dealing with and whether our X/Y axis
    // can handle the minimum/maximum X/Y values of our new data

    if (   !pFrom
        || (xMin < minX()) || (xMax > maxX())
        || (yMin < minY()) || (yMax > maxY()))
        // Either it's our first curve segment and/or our X/Y axis cannot handle
        // the minimum/maximum X/Y values of our new data, so check our axes

        checkAxes(true, true, true);
    else
        // Our X/Y axis can handle the X/Y min/max of our new data, so just draw
        // our new curve segment

        mDirectPainter->drawSeries(pCurve, pFrom, pTo);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
