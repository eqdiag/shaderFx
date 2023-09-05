# shaderFx

An application for playing around with shader visual effects on 3d meshes.

Uses my custom math library: [gMath](https://github.com/eqdiag/gMath)

All dependencies are self-contained in this project using git's submodule system.

## Installation

```
git clone https://github.com/eqdiag/shaderFx
cd shaderFx
mkdir build
cd build
cmake ..
make
```


## Keyboard Controls
  * `ESC` Exit program

## Mouse Controls
  * `mouseDrag` Rotates camera
  * `mouseScroll` Zoom camera in/out

## Vertex/Fragment Shader Effects
  *  `Wax` Applies a !["matcap"]([https://digitalrune.github.io/DigitalRune-Documentation/html/9a8c8b37-b996-477a-aeab-5d92714be3ca.htm](https://digitalrune.github.io/DigitalRune-Documentation/html/9a8c8b37-b996-477a-aeab-5d92714be3ca.htm) effect.
  *  `Iridescence`  A rainbow iridescence effect that changes in time.
  *  `Sphere Morph`  A linear morph between a mesh and unit sphere to demonstrate vertex shader effects.

## Post-Processing Shader Effects
  *  `Invert`  A linear morph between a mesh and unit sphere to demonstrate vertex shader effects.
  *  `Vignette`  A radial "vignette" film effect.
  *  `Blur`  An image blurring effect.
  *  `Sobel`  A sobel filter, often used for edge detection algorithms.
  *  `Noise`  A "wavy" effect achieved using a noise source.

# Screenshots
![App](/screenshots/main.PNG "Application")
![Wax](/screenshots/wax.gif "Wax")
![Iridescence](/screenshots/iri.gif "Iridescence")
![Morph](/screenshots/morph.gif "Morph")
![Invert](/screenshots/invert.gif "Invert")
![Vignette](/screenshots/vignette.gif "Vignette")
![Blur](/screenshots/blur.gif "Blur")
![Sobel](/screenshots/sobel.gif "Sobel")
![Noise](/screenshots/noise.gif "Noise")


## Potential Future Addtiions
  * More cool visual effects I learn about :)
                       
## Copyright and Attribution
Copyright (c) 2023 eqdiag. Released under the [MIT License](https://github.com/eqdiag/shaderFx/blob/main/LICENSE.md).
