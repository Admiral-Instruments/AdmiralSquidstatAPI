/*
* This demo is part 2 and assumes familiarity with 1_BasicDemo.cpp.
* This demo shows how to run a sequence of experiments and controlling
* when to stop an experiment and start another based on external conditions.
* For simplicity, we are assuming having a single device connected and we are running on a single channel.
* So, we will not have to keep track of devices and channels.
* We will just focus on running and controlling the workflow of different experiments.
*/

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include <QCoreApplication>
#include <QTimer>
#include <qdebug.h>

int main()
{
    // Environment Setup
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test); // this creates a non-GUI Qt application

    // constructing a constant potential element with required arguments
    AisConstantPotElement cvElement(
        5, // voltage: 1v
        1, // sampling interval: 1s
        10 // duration: 10s
    );

    // constructing a constant current element with required arguments
    AisConstantCurrentElement ccElement(
        0.002,  // current: 2mA
        1,      // sampling interval: 1s
        10      // duration: 10s
    );

    auto experimentA = std::make_shared<AisExperiment>(); // create a custom experiment
    experimentA->appendElement(cvElement, 1); // append to experimentA, the created CV element and set it to run 1 time

    auto experimentB = std::make_shared<AisExperiment>(); // create a second experiment
    experimentB->appendElement(ccElement, 1); // append to experimentB, the created CC element and set it to run 1 time

    auto experimentC = std::make_shared<AisExperiment>(); // create a third experiment
    experimentC->appendElement(cvElement, 2); // append to experimentC, the created CV element and set it to run 2 times

    /*
    * Now we have the experiments set up. Next we will create the logic for the sequence of experiments.
    * We will be using timers as external conditions to control the workflow. You may substitute that with your own conditions.
    *
    * The following lambda function creates a logic and assigns it to the given handler.
    * We will call this function after a newDeviceConnected signal has been emitted and a handler has been created.
    * The logic controls the workflow as follows:
    * # Start the first timer
    * # once the timer times out, start Experiment A
    * # once Experiment A completes, start the second timer
    * # once the second timer times out, start Experiment B
    * # start a third timer to stop Experiment B early
    * # once the third timer out, stop Experiment B
    * # start a fourth timer
    * # once the fourth timer times out, start Experiment C
    * # once Experiment C completes, start Experiment B
    */

    // Lambda function
    auto createLogic = [&](const AisInstrumentHandler* handler) {
        QTimer* timer1 = new QTimer(); // the first timer is used in lieu of the first external condition
        timer1->setSingleShot(true);
        timer1->start(1000);

        QObject::connect(timer1, &QTimer::timeout, [=]() {
            qDebug() << "Initial condition met. Starting Experiment A ";
            handler->uploadExperimentToChannel(0, experimentA);
            handler->startUploadedExperiment(0);

            // once the first experiment is completed (Experiment A), start the next experiment (Experiment B).
            // this signal will be emitted for any experiment not just A so, we will track of the sequence with experimentStep
            // once an experiment has completed or has been stopped, continue to the next experimentStep
            QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [&](uint8_t channel) {
                static int experimentStep = 0;
                qDebug() << "Experiment Step " << experimentStep << " Completed";

                experimentStep++; //increment the experiment step
                if (experimentStep == 1) {
                    // Wait for external start condition
                    QTimer* timer = new QTimer(); // the timer is used in lieu of an external condition
                    timer->setSingleShot(true);
                    timer->start(10000); // when this timer times out, the next experiment (Experiment B) will start

                    // Create an external condition that will stop the upcoming experiment early
                    QTimer* StopEarlyTimer = new QTimer();
                    StopEarlyTimer->setSingleShot(true);
                    QObject::connect(StopEarlyTimer, &QTimer::timeout, [&]() {
                        qDebug() << "External early stop condition met";
                        handler->stopExperiment(0); // Once the external condition is met, experiment B will stop, and the experimentCompleted signal will be emitted
                    });

                    QObject::connect(timer, &QTimer::timeout, [&, StopEarlyTimer]() {
                        qDebug() << "External condition met, starting experiment B";
                        handler->uploadExperimentToChannel(0, experimentB); // start Experiment B
                        handler->startUploadedExperiment(0);
                        StopEarlyTimer->start(2000);
                    });
                } else if (experimentStep == 2) {
                    QTimer* timer = new QTimer(); // the timer is used in lieu of an external condition
                    timer->setSingleShot(true);
                    timer->start(10000); // when this timer times out, the next experiment (Experiment C) will start

                    QObject::connect(timer, &QTimer::timeout, [&]() {
                        qDebug() << "External condition met, starting Experiment C ";
                        handler->uploadExperimentToChannel(0, experimentC); // start Experiment C
                        handler->startUploadedExperiment(0);
                    });
                } else if (experimentStep == 3) {
                    QTimer* timer = new QTimer(); // the timer is used in lieu of an external condition
                    timer->setSingleShot(true);
                    timer->start(10000); // when this timer times out, the next experiment (Experiment B) will start

                    QObject::connect(timer, &QTimer::timeout, [&]() {
                        qDebug() << "External condition met, starting Experiment B ";
                        handler->uploadExperimentToChannel(0, experimentB); // start Experiment B
                        handler->startUploadedExperiment(0);
                    });
                }
            });
        });
    };

    // this is a signal-slot connection where the slot assigns the logic to the device handler when newDeviceConnected signal is emitted.
    auto tracker = AisDeviceTracker::Instance(); // create a tracker that tracks connected devices
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [&](const QString& deviceName) {
        auto& handler = tracker->getInstrumentHandler(deviceName);
        createLogic(&handler); // controlling experiments is to be done only after a device handler has been assigned. That is why it is placed inside this slot.
    });

    // here we have a signal and slot for when a device has been disconnected
    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        qDebug() << deviceName << "is disconnected ";
    });

    tracker->connectToDeviceOnComPort("COM18"); // change the port number to your device. For example, in windows, you can find it from the device manager

    a.exec();
}
