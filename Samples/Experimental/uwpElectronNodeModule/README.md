# uwpElectronNodeModule

Steps to get started

1) Go to https://nodejs.org/en/ and Download the latest LTS version of Node.js for Windows.

To check if you have Node.js installed, run this command in your terminal:
    node -v

To confirm that you have npm installed you can run this command in your terminal:
    npm -v

npm is a separate project from Node.js, and tends to update more frequently. As a result, even if you’ve just downloaded Node.js (and therefore npm), you’ll probably need to update your npm. Luckily, npm knows how to update itself! To update your npm, type this into your terminal:

    npm install npm@latest -g


next we will install node-gyp. node-gyp is a cross-platform command-line tool written in Node.js for compiling native addon modules for Node.js. It contains a vendored copy of the gyp-next project that was previously used by the Chromium team, extended to support the development of Node.js native addons.

    npm install -g node-gyp

We will also need to Install all the required tools and configurations using Microsoft's windows-build-tools.

    npm install --global --production windows-build-tools 

from an elevated PowerShell or CMD.exe (run as Administrator).

2) Clone and build UWPELECTRONSAMPLE project

navigate to https://github.com/bertburtbert/uwpElectronNodeModule

paste  the following into your terminal to clone the repo
        
    git clone https://github.com/bertburtbert/uwpElectronNodeModule.git 

Change directories to the ElectronApp folder

    cd .\uwpElectronNodeModule\ElectronApp\

Install npm dependencies and recompile for electron
        
    npm install &&  node-gyp rebuild --target=9.0.5 --arch=x64 --dist-url=https://electronjs.org/headers

run the app

    npm start

