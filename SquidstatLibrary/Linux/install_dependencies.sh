#!/bin/bash

# Function to check the status of the last command and exit if it failed
check_status() {
    if [ $? -ne 0 ]; then
        echo -e "\e[31mError: Operation failed. Exiting...\e[0m"
        exit 1
    fi
}

# Update and upgrade the system
echo "Updating and upgrading the system..."
sudo apt-get update && sudo apt-get upgrade -y
check_status

# Add the current user to the 'dialout' group
echo "Adding the user to the dialout group..."
sudo usermod -a -G dialout $USER
check_status

# Create udev rules for hidraw and usb devices
echo "Creating udev rules for hidraw and usb devices..."

# HID raw device rule
echo 'SUBSYSTEM=="hidraw", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="003c", MODE="0660", GROUP="dialout"' | sudo tee /etc/udev/rules.d/99-admiral-instruments-squidstat.rules > /dev/null
check_status

# USB device rule
echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="003c", MODE="0660", GROUP="dialout"' | sudo tee -a /etc/udev/rules.d/99-admiral-instruments-squidstat.rules > /dev/null
check_status

# Reload udev rules to apply changes
echo "Reloading udev rules..."
sudo udevadm control --reload-rules
check_status

# Install required dependencies
echo "Installing required dependencies..."
sudo apt-get install -y build-essential libgl1-mesa-dev libxcb-cursor0
check_status

# Completion message
echo -e "\e[32mDependencies installed and rules created successfully!\e[0m"
echo -e "\n\e[1;33m=============================================================\e[0m"
echo -e "\e[1;33m PLEASE RESTART YOUR COMPUTER FOR THE CHANGES TO TAKE EFFECT. \e[0m"
echo -e "\e[1;33m=============================================================\e[0m"
