GWEN
====

A fork of GWEN, the C++ GUI library.

The original library can be found here: https://github.com/garrynewman/GWEN

About
=====

This repository is a fork of GWEN which I use in my personal projects.  I became interested in understanding the implementation of GUI libraries.  So, I started converting GWEN to my own coding style in an effort to force myself to actually understand what the code was doing.  After a while, I had updated most of the library.

Since the original repository no longer appears active, I decided to upload this version in case anyone else was interested in using it.

This fork contains a subset of the original repository's widgets.  If I did not directly need the widget, I removed it.

Building
========

For my personal projects, I build everything I use from source with CMake.  So, I don't have any solution files to upload which would be of any value.  However, I included a sample CMakeLists file to act as a starting point.  You will need to modify it to operate in your CMake ecosystem.

As an alternative, you could just drag and drop all the files into an IDE.  There's nothing complicated about building the library.  Just make sure to update the include and linker directories accordingly.  Use the sample CMakeLists file as a guide.
