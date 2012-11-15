//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#include "singlecellsimulationviewsimulationworker.h"

//==============================================================================

#include <QTime>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationWorker::SingleCellSimulationViewSimulationWorker(const double &pStartingPoint,
                                                                                   const double &pEndingPoint,
                                                                                   const double &pPointInterval) :
    mActive(false),
    mPausing(false),
    mStartingPoint(pStartingPoint),
    mEndingPoint(pEndingPoint),
    mPointInterval(pPointInterval)
{
}

//==============================================================================

bool SingleCellSimulationViewSimulationWorker::isPausing() const
{
    // Return whether we are pausing

    return mPausing;
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::run()
{
    // Make sure that we are not already active

    if (mActive)
        return;

    // We are about to run, so...

    mActive = true;

    // Let people know that we are running

    emit running();

    // Start our timer

    QTime timer;
    int totalElapsedTime = 0;

    timer.start();

    // Our main work loop

    bool increasingPoints = mEndingPoint > mStartingPoint;
    const double oneHundredOverPointRange = 100.0/(mEndingPoint-mStartingPoint);
    int voiCounter = 0;
    double currentPoint = mStartingPoint;

    while ((currentPoint != mEndingPoint) && mActive) {
        // Pretend that we are doing something

        qDebug("[%06d] voi = %f...", voiCounter, currentPoint);

        // Let people know about our progress

        emit progress((currentPoint-mStartingPoint)*oneHundredOverPointRange);

        // Check whether we should be pausing

        mPauseMutex.lock();
            if(mPausing) {
                // We have been asked to pause, so do just that after having
                // stopped our timer

                totalElapsedTime += timer.elapsed();

                mPauseCondition.wait(&mPauseMutex);

                // Let people know that we are running again

                emit running();

                // Restart our timer

                timer.restart();
            }
        mPauseMutex.unlock();

        // Go to the next point, if needed

        ++voiCounter;

        currentPoint = increasingPoints?
                           qMin(mEndingPoint, mStartingPoint+voiCounter*mPointInterval):
                           qMax(mEndingPoint, mStartingPoint+voiCounter*mPointInterval);
    }

    // Pretend that we are doing something with the last point

    qDebug("[%06d] voi = %f...", voiCounter, currentPoint);

    // Let people know about our final progress

    emit progress(100.0);

    // Let people know about how long it took to complete the work

    totalElapsedTime += timer.elapsed();

    emit elapsedTime(totalElapsedTime);

    // We are done, so let people know about it, after making sure that mActive
    // has been reset (in case the worker actually completed its job)

    mActive = false;

    emit progress(0.0);
    emit finished();
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::pause()
{
    // Check that we are active and not already pausing

    if (!mActive || mPausing)
        return;

    // Pause ourselves

    mPauseMutex.lock();
        mPausing = true;
    mPauseMutex.unlock();

    // Let people know that we are pausing

    emit pausing();
}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::resume()
{
    // Check that we are active and pausing

    if (!mActive || !mPausing)
        return;

    // Resume ourselves

    mPauseMutex.lock();
        mPausing = false;
    mPauseMutex.unlock();

    mPauseCondition.wakeAll();}

//==============================================================================

void SingleCellSimulationViewSimulationWorker::stop()
{
    // Check that we are active

    if (!mActive)
        return;

    // Resume ourselves, if needed

    if (mPausing)
        resume();

    // Leave it to our run() method to handle the stop by setting mActive to
    // false

    mActive = false;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
