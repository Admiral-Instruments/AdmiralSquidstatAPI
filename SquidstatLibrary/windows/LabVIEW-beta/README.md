# Introduction
This is a proof of concept for Squidstat integration into LabVIEW. It is not feature complete, but should include enough to get started using the Squidstat in LabVIEW.

*Note:* Control via LabVIEW is still in beta. We cannot guarantee the accuracy and stability of the system while using it. If you see any issues or if you would like to request that we implement a missing feature, please create a new discussion on our [GitHub Discussions page](https://github.com/Admiral-Instruments/AdmiralSquidstatAPI/discussions). We currently only support Windows for this beta release.

# How to Use
## If you are reading this on GitHub
Please follow the instructions in our [Documentation](https://admiral-instruments.github.io/AdmiralSquidstatAPI/_setup_lab_v_i_e_w.html)
## If you have already cloned/downloaded this folder
 1. Create or open an existing LabVIEW project.
 2. In the project explorer, right-click `My Computer` and navigate to `Add->Folder (Auto-populating)...`
 3. In the file browser pop-up, navigate to the cloned API and then into the `SquidstatLibrary/windows/LabVIEW-beta` directory in the API and select it.
 4. Open the `.vi` located at `LabVIEW-beta/Demo/Demo.vi`. Here you will find an example of handling AC and DC data. Our API utilizes LabVIEW User Events to facilitate the transfer of data into the rest of the system.
    - The event system in LabVIEW adds some overhead to the library overall. If you need faster response time, you can look at `Squidstat/SquidstatEventLoop.vi` for an example of the polling loop that creates these events. You can copy this loop where desired and remove the event structures. For future releases, this may become simplified.
 5. At the point you may choose to import the palette file located at `SquidstatLibrary/windows/LabVIEW-beta/SquidstatPalette.mnu`. This can be done via `Tools->Advanced->Edit Palette Set...`