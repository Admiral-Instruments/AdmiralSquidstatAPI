# AdmiralSquidstatAPI

This repo includes the API bundle for public use as well as documentation for using the API.
The Admiral Instruments API gives more control of [our potentiostats](https://www.admiralinstruments.com/products), and gives you the
tools to integrate running our experiments in your pipeline and automating your workflow.

To read the documentation, please visit [the docs website](https://admiral-instruments.github.io/AdmiralSquidstatAPI).

The most convenient way to build and run the API and the provided examples is to utilize CMake with the CMake files we provide. After downloading the SquidstatLibrary folder, open it with an IDE with CMake support. For example, with Visual Studio 2019, if you have the C++ package installed with CMake, you can open the folder and the IDE will generate the project for you automatically. To then run any of the provided examples, set that example as the 'start up item'.