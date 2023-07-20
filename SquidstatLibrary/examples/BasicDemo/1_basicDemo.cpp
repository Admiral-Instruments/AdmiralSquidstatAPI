/** @page basics Basic Demo
 *
 *  @verbinclude 1_basicDemo.cpp
 */
/*
* The Admiral Instruments API gives more control of the device and gives you the tools to integrate running
* our experiments in your pipeline and automating your workflow.
* Our API lets you programmatically start an experiment, pause an experiment and stop an experiment.
* For example, you may want to start an experiment with our device automatically whenever another device you have
* reads a certain temperature. Among other things, whenever starting, pausing or stopping an experiment happens,
* our API also sends a signal that you can use to control your workflow.
* For example, you may choose to start the next step in your pipeline whenever the experiment stops.
*
* Below is an example for building a custom experiment
*
*/

#include "AisExperiment.h"
#include "AisDeviceTracker.h"
#include "AisInstrumentHandler.h"
#include "experiments/builder_elements/AisConstantCurrentElement.h"
#include "experiments/builder_elements/AisConstantPotElement.h"
#include <QCoreApplication>
#include <qdebug.h>

int main()
{

    // Environment setup
    char** test = nullptr;
    int args;
    QCoreApplication a(args, test); // this creates a non-GUI Qt application
    auto tracker = AisDeviceTracker::Instance(); // create a tracker that tracks connected devices

    /*
	* The basic building block of a custom experiment are the elements.
	* An element is an elementary experiment like Constant Voltage/Potential (CV) or Constant Current (CC).
	* A custom experiment can have one or more elements. The elements inside could be run one or more times.
	* A custom experiment can also contain another custom experiment as a sub-experiment.
	* The sub-experiment can be run one or more times as well.
	*
	* Let us go through building an example custom experiment with our API.
	*
	*/

    // Custom Experiment Example

    /* To build a custom experiment, we need at least one element.
	*  In the example below, we have two elements and a sub-experiment.
	*  The sub-experiment has the same two elements.
	*/

    // We create an element and set parameters based on the type of the element.
    // You can see a full list of the available elements in the directory: SquidstatLibrary\include\experiments\builder_elements
    // For now, we are only setting the required parameters.
    // You can get a complete list of settable parameters for any given element type by examining the corresponding header file in the builder_elements directory
    AisConstantPotElement cvElement( // constructing a constant potential element
        1, // voltage: 1v
        1, // sampling interval: 1s
        30 // duration: 30s
    );

    // We create another element of a different type
    AisConstantCurrentElement ccElement( // construct a constant current element and assign it to "ccElement"
        1, // current: 1A
        1, // sampling interval: 1s
        60 // duration: 60s
    );

    // We create a custom experiment and add the elements to it
    auto customExperiment = std::make_shared<AisExperiment>(); // at this point, it is an empty custom experiment, so, we add the elements we created to it.
    customExperiment->appendElement(ccElement, 1); // append the CC element to the end of the experiment and set it to run 1 time
    customExperiment->appendElement(cvElement, 1); // append the CV element to the end of the experiment and set it to run 1 time
    // Note: Elements are run in the order that they are added to the experiment

    // Next, we create a second experiment as a sub-experiment i.e. we are going to then add it to the main experiment.
    auto subExperiment = std::make_shared<AisExperiment>(); // this line creates a custom experiment, intended to be used as a sub-subExperiment
    subExperiment->appendElement(ccElement, 2); // append the CC element to the sub-experiment and set it to run 2 times
    subExperiment->appendElement(cvElement, 3); // append the CV element to the sub-experiment and set it to run 3 times
    customExperiment->appendSubExperiment(*subExperiment, 2); // append the sub-experiment to the main experiment and set the sub-experiment to run 2 times.
    // Again, the order adding/appending the elements and the sub-experiment here corresponds to the order at which they will run.
    // The sub-experiment and the elements it contains will be run after the elements already added to the main experiment

    // We create an additional constant voltage element with different duration and voltage setpoint
    AisConstantPotElement cvElement_2(
        4, // voltage: 4v
        1, // sampling interval: 1s
        10 // duration: 10s
    );

    customExperiment->appendElement(cvElement_2, 1); // this line appends to the main experiment the CV element to run 1 time after the sub-experiment

    /*This concludes creating the experiment. Next is how to control the workflow of the experiment*/
    /*
	* So far, we have only created the experiment. But we need to start it and control it.
	* This code section employs a callback mechanism specific to Qt, called signals and slots.
	* callbacks are used to take an action when a specified condition is met i.e. control the workflow.
	* For simplicity, we provided some common slots related to our API with comments inside, on what you can do.
	*
	* You can read more about Qt signals and slots in the following link: https://doc.qt.io/qt-5/signalsandslots.html
	* Reading this document should still cover most of what is needed. Basically, a signal can be emitted when an event happens.
	* If a slot is connected to that signal, whatever is inside that slot will be executed when the signal is emitted.
	* You can think of a signal as condition and a slot is what will be executed once the condition is met. The only difference is the order of execution.
	* Normal execution have sequential order. However, a slot can be emitted at anytime. Whenever that happens the slot will execute
	* no matter where the connection has been made (as long as a connection has been made prior).
	* That is how we can have extra control on how and when things are executed.
	*
	* An experiment is run on a specific channel of a device.
	*   You may have more than one device connected. A single device has up to 4 channels.
	*   Any channel on a specific device can run a single experiment at a time.
	*   To start an experiment, we specify the device and the channel and then start it.
	*   To stop or pause that experiment, we need to specify its corresponding device and channel.
	* We need to keep track of the device and channel for each experiment we start so we can control it later.
	*
	* We can control a device, including starting, pausing and stopping an experiment on a specific channel using a device 'handler'.
	*   A device handler can be created given a device name that we detect.
	*
	* We have two parts below: one that creates logic using signals and slots. The second part assigns that logic to an experiment handler which will discuss in a bit.
	* The first part below is creating some control flow logic that we can assign to a handler.
	* We can also create other logics in the same way that can be assigned to different handlers which can be used to control different devices.
	* If we only have one device, all the logic will be handled with one handler. We can then have further control within, based channels.
	*/

    /* Part one: creating control flow logic specific to a handler
	* The first part is a lambda function called "createLogic" which takes a handler as an argument and connects some of the handler signals to slots.
	* We have other signals related to a handler you can add, which you can find in the file "AisInstrumentHandler.h" in the directory: SquidstatLibrary\include
	* This example logic has four conditions on which we can perform other tasks.
	* That is, when we assign this logic to a specific handler, this logic will execute for that handler.
	* The four signals and slots below in the first part are examples for you to follow in order to add other connections.
	*/
    auto connectSignals = [=](const AisInstrumentHandler* handler) {
        QObject::connect(handler, &AisInstrumentHandler::activeDCDataReady, [=](uint8_t channel, const AisDCData& data) {
            // do something when DC data are received, such as writing to a CSV file output
            // THIS IS WHERE YOU RECEIVE DC DATA FROM THE DEVICE

            //example: print the data to the standard output as follows:
            qDebug() << "channel: " << (int)channel << "current :" << data.current << "   voltage: " << data.workingElectrodeVoltage << "   counter electrode : " << data.counterElectrodeVoltage << "  timestamp : " << data.timestamp;
        });

        QObject::connect(handler, &AisInstrumentHandler::activeACDataReady, [=](uint8_t channel, const AisACData&) {
            // do something when AC data are received
            // THIS IS WHERE YOU RECEIVE AC (EIS) DATA FROM THE DEVICE
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentNewElementStarting, [=](uint8_t channel, const AisExperimentNode&) {
            // do something when a new element is starting

            // for example, print to the standard output: "New element starting"
            qDebug() << "New element starting";
        });

        QObject::connect(handler, &AisInstrumentHandler::experimentStopped, [=](uint8_t channel) {
            // do something when the experiment has stopped or has been stopped. For example, you can invoke starting the next step in your workflow

            // print to the standard output: "Experiment stopped Signal "
            qDebug() << "Experiment Stopped Signal " << channel;
        });
    };

    // part two: connecting slots to device tracker signals; first when a device is connected and second when a device is disconnected.
    /*
	* This connects a slot to the device tracker's "newDeviceConnected" signal that provides the device name.
	* Because we have the device name, we can create a device handler and do whatever a handler can do.
	* In this slot example, we are creating a handler, assigning the previously created logic to this handler and then starting an experiment
	*/
    QObject::connect(tracker, &AisDeviceTracker::newDeviceConnected, &a, [=](const QString& deviceName) {
        // Do something when a new device is detected to be connected. The device name is given in the variable 'deviceName'

        // The following 5 lines start the experiment that we created
        auto& handler = tracker->getInstrumentHandler(deviceName); // create a device handler using the given device name
        connectSignals(&handler); // connect signals for device, once per device to handle all channels

        // before starting an experiment, you have the option of linking channels (cycler only) so that you can share the current over multiple channels
        // if using paralleled channels, setLinkedChannels MUST be called before each experiment that uses paralleled channels
        auto masterchannel = handler.setLinkedChannels({ 0, 1 }); // this does two things, first links the given channels and second returns the masterchannel used to control the combined output.
        auto error = handler.uploadExperimentToChannel(masterchannel, customExperiment); // upload to a specific channel (first arg) an experiment (second arg) on the given device
        if (error) {
            qDebug() << error.message();
            return;
        }
        error = handler.startUploadedExperiment(masterchannel); // start the previously uploaded experiment on a specific channel. The argument is the channel number
        // you may still use a single channel and change the 'masterchannel' to the channel number as such: handler->uploadExperimentToChannel(0, experiment);
        if (error) {
            qDebug() << error.message();
            return;
        }

        // Please refer to \ref AisInstrumentHandler for possible errors that may occur when performing operations such as uploading and starting an experiment.
        // For example, when uploading an experiment, AisInstrumentHandler::uploadExperimentToChannel may return an AisErrorCode::InvalidParameters error
        // if the parameters are out of range where you can display the message to check which parameter was not supported for your device.
    });

    /* This connects a slot to the device tracker's "deviceDisconnected" signal with the device name. */
    QObject::connect(tracker, &AisDeviceTracker::deviceDisconnected, &a, [=](const QString& deviceName) {
        // do something when a device has been disconnected. The device name is given in the variable 'deviceName'

        // for example, print to the standard output that the device given is disconnected
        qDebug() << deviceName << "is disconnected ";
    });

    /*
	* We still have not started the experiment, we've only created an experiment and setup callback functions via signals.
	* When we connect a device using the tracker as follows, the newDeviceConnected signal will be emitted with the device name.
	*   As a result, the slot we connected earlier (newDeviceConnected) will execute (connecting the additional signals and running the experiment).
	*/

    AisErrorCode error = tracker->connectToDeviceOnComPort("COM14"); // change the port number to yours. For example, in windows, you can find it from the device manager
    if (error != error.Success) {
        qDebug() << error.message();
    }

    a.exec();
}
