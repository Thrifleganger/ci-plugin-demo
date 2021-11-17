### Running the installer

- To run the installer script, you need to download [Inno Setup](https://jrsoftware.org/isdl.php).
- Install the downloaded application locally. 
- Make sure that the application is compiled and built with Release configuration, and the VST3 file is copied to this directory
- Open `ci-plugin-demo-installer.iss` in Inno Setup Compiler, and compile the script.
- The executable should be build in `./Output/ci-plugin-demo-<version>.exe`