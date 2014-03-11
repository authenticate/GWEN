GWEN
====

A fork of GWEN, a C++ GUI library.

The original library can be found here: https://github.com/garrynewman/GWEN

About
=====

This repository is a fork of GWEN which I use in my personal projects.  I became interested in understanding the implementation of GUI libraries.  So, I started converting GWEN to my own coding style in an effort to force myself to actually understand what the code was doing.  After a while, I had updated most of the library.

Since the original repository no longer appears active, I decided to upload this version in case anyone else was interested in using it.

This fork contains a subset of the original repository's widgets.  If I did not directly need the widget, I removed it.  In addition, the only supported rendering and input systems are both SDL2.

Building
========

For my personal projects, I build everything I use from source with CMake.  So, I don't have any solution files to upload which would be of any value.  However, I included a sample CMakeLists file to act as a starting point.  You will need to modify it to operate in your CMake ecosystem.

As an alternative, you could just drag and drop all the files into an IDE.  There's nothing complicated about building the library.  Just make sure to update the include and linker directories accordingly.  Use the sample CMakeLists file as a guide.

Integrating
===========

Here's some pseudo code to get started:

```

// Somewhere in your code base while the application is starting up...

// Create the renderer.
gwen_renderer = new Gwen::Renderer::SDL2(...);
gwen_renderer->Initialize();

// Create the skin.
gwen_skin = new Gwen::Skin::TexturedBase(gwen_renderer);
gwen_skin->Initialize(...;

// Create the canvas.
gwen_canvas = new Gwen::Controls::Canvas(gwen_skin);

// Create the input system.
gwen_input = new Gwen::Input::SDL2();
gwen_input->Initialize(gwen_canvas);

...

YourGameLoop()
{
    SDL_Event the_event;
    while (SDL_PollEvent(&the_event))
    {
        // Process events.
        gwen_input->ProcessEvent(the_event);

        ...

    }

    ...

    // Draw the canvas.
    gwen_canvas->RenderCanvas();
}

// Make sure to clean up all those pointers created with new!

...

```

In addition, to add a control:

```

button = new Gwen::Controls::Button(gwen_canvas);
button->SetText("Hello World");

```
