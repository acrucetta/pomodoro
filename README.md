# Pomodoro Timer with Website Blocker

## Overview
This project is a Pomodoro Timer implemented in C. The Pomodoro technique is a time management method that uses a timer to break work into intervals, traditionally 25 minutes in length, separated by short breaks. This program also includes a feature to block distracting websites during these work intervals by modifying the system's `/etc/hosts` file.

## Features
- **Pomodoro Timer:** Standard 25-minute focus timer with console output for time remaining.
- **Website Blocking:** Automatically blocks a list of specified websites during the focus period.
- **Signal Handling:** Gracefully handles `SIGINT` (Ctrl+C) signal to unblock websites and exit the program.

## Requirements
- Unix-like operating system (Linux, macOS)
- Root privileges for modifying the `/etc/hosts` file
- GCC or another C compiler for building the program

## Installation
1. Clone this repository to your local machine.
   ```
   git clone acrucetta/pomodoro
   ```
2. Navigate to the cloned directory.
   ```
   cd pomodoro
   ```

## Usage

1. Compile the program.
   ```
   gcc -o pomodoro_timer main.c
   ```
2. Run the program with root privileges.
   ```
   sudo ./pomodoro_timer s
   ```
   The 's' argument starts the timer and activates website blocking.

3. When the timer is finished, the program will automatically unblock the websites and exit.

4. To exit the program before the timer is finished, press `Ctrl+C` to send the `SIGINT` signal.

## Configuring Websites to Block
Edit the `websites.txt` file in the project directory to list the websites you want to block, one per line. Example:
```
facebook.com
youtube.com
twitter.com
```

## Important Notes
- The program requires root privileges to modify the `/etc/hosts` file.
- Make sure to use the program responsibly, as it modifies a system file.


## Application
i
- User starts pomodoro timer for 25 minutes
- The program will start a timer and show the user the end time
- The program will load a list of websites it wants to block from the environmental variables or a config file
- The programm will start a proxy server and block any requests for websites the user doesn't want to visit
- Once the timer is finished, the proxy server will shut down and allow the user to visit those websites again
