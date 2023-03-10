# Projectme

Projection mapping of GLSL shaders to several rectangular surfaces, from 2015.

## Build

Dependencies are configured via a [vcpkg](https://vcpkg.io/) manifest, see `source/vcpkg.json`.

Generate build files via cmake e.g. for Windows x64:
```
> cmake -B build -S source -DCMAKE_TOOLCHAIN_FILE=<Path-to-VCPKG>\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
```

Tested with Visual Studio 2022 on Windows 10.

## Acknowledgement

- The software uses [QNodesEditor by Stanislaw Adazewski](https://adared.ch/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/)
  and includes source code from it in a version from 2012 which is licensed under BSD. Affected files:
    - `qneblock.h`
	- `qneconnection.h`
	- `qneport.h`
	- `qnodeseditor.h`
	- `qneblock.cpp`
	- `qneconnection.cpp`
	- `qneport.cpp`
	- `qnodeseditor.cpp`

- The source code contains a down-stripped version of "Distance transform of 1D and 2D images"
  by Pedro Felzenszwalb in a version from 2006 which is licensed under GPL-2.
  That particular version seems to be no longer online, but a more recent version 
  from 2012 can be found on the page [Distance Transforms of Sampled Functions](https://cs.brown.edu/people/pfelzens/dt/index.html).
  Affected files:
    - `DistanceTransformFelzenszwalb.h`

- The software uses `QSyntaxHighlighter` sample code from the examples of the Qt Toolkit
  from 2014 licensed under BSD. Affected files:
    - `Highlighter.h`
    - `Highlighter.h`

- The software uses selected icons from the [Tango Icon Library](http://tango.freedesktop.org/Tango_Icon_Library)
  which is released to the Public Domain. 

## History

### Oct 2017
* Update to Qt5

### Jan 2015
* Texture multiplexer (realized via muxer.glsl)
* Particle system: Mixture of velocity textures (can be realized via muxer)
* Custom module properties widget (ModuleParameterWidget)
	- ModuleBase supports now parameters and options, both of type Parameter.
	  The former can be edited live (like shader uniforms) while the latter 
	  requires an explicits update (like texture size).
	- Parse inputs, add controls for (specific) inputs (via QAutoGUI)
	- ShaderPrepocessor to automatically parse (marked) shader variables
* Spectrum analyzer module
* Particle system: Fade particles over lifetime
* Module selection in node editor

### Dec 2014
* Node editor / connection serialization
* Node editor (texture channel flow)
* Shader editor
* Dynamic module creation (factory pattern)
* UI to assign modules to render areas ("Mapper widget")
* Module viewer (render only the output of a single module)
