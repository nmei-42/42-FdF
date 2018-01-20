# 42-FdF
An introduction project to the graphics branch @ 42.

Reads in a height map to generate a 3D model that the user can interact with!

### Installation
To generate an executable for this project run `make` in the root of the project directory. If the `make` command fails, check if the minilibx folder is empty. If so, you may have to run `git submodule update --init --recursive` to initialize and get the files for the minilibx submodule.

This project will only work on MacOS El Capitan/Sierra/HighSierra(maybe) and even then, no promises! 

### Usage
After running `make` to generate the fdf executable you can run the program with `./fdf [valid_fdf_file]`.

A valid fdf file will consist of a matrix of number delimited by spaces. Each number's position represents an (x , y) coordinate with the value of the number itself representing a height (z).

Valid fdf files will always have the same number of elements per row.

Example:
```
$> cat -e 42.fdf
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0$
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0$
0  0 10 10  0  0 10 10  0  0  0 10 10 10 10 10  0  0  0$
0  0 10 10  0  0 10 10  0  0  0  0  0  0  0 10 10  0  0$
0  0 10 10  0  0 10 10  0  0  0  0  0  0  0 10 10  0  0$
0  0 10 10 10 10 10 10  0  0  0  0 10 10 10 10  0  0  0$
0  0  0 10 10 10 10 10  0  0  0 10 10  0  0  0  0  0  0$
0  0  0  0  0  0 10 10  0  0  0 10 10  0  0  0  0  0  0$
0  0  0  0  0  0 10 10  0  0  0 10 10 10 10 10 10  0  0$
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0$
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0$
$>
```

### Output
The program will attempt to render the model described by the fdf file in a graphical window.

![Example:](https://github.com/nmei-42/42-FdF/blob/master/example_maps/42%20FdF.png)

The user can interact with the graphical model with the following commands:

```
Mouse click and drag: Rotate x/y axes
Mouse scroll up/down: Zoom in/out
Arrow keys: Translate x/y axes
'z/x' keys: Rotates z axis
c: Change color modes
p: Turn on/off point only mode
q: Turn on/off Z-buffering
r: Reset map translations, rotations, and scalings
esc: Quit :(
```
