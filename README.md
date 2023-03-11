# Projectme

Projection mapping of GLSL shaders to multiple quadrilaterals, from 2015.

## Build

Dependencies are configured via a [vcpkg](https://vcpkg.io/) manifest, see `source/vcpkg.json`.

Generate build files via cmake e.g. for Windows x64:
```
> cmake -B build -S source -DCMAKE_TOOLCHAIN_FILE=<Path-to-VCPKG>\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
```

Tested with Visual Studio 2022 on Windows 10.

## Acknowledgements

This software uses the following 3rd party components in original or modified form:

- [QAutoGUI](https://code.google.com/p/qautogui/) by Andrea Tagliasacchi licensed under a LGPL license.
- [QNodesEditor](https://adared.ch/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/)
  by Stanislaw Adazewski in a version from 2012, licensed under a BSD license.
- [webgl-noise](https://github.com/ashima/webgl-noise) by Stefan Gustavson from 2011, licensed under a MIT license.
- "Distance transform of 1D and 2D images" by Pedro Felzenszwalb in a version from 2006, licensed under a GPL-2 license.
  That particular version seems to be no longer online, but a more recent version 
  from 2012 can be found on the page [Distance Transforms of Sampled Functions](https://cs.brown.edu/people/pfelzens/dt/index.html).
- `QSyntaxHighlighter` sample code from the examples of the Qt Toolkit from 2014, licensed under a BSD license.
- Selected icons from the [Tango Icon Library](http://tango.freedesktop.org/Tango_Icon_Library) which is in the Public Domain. 

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
