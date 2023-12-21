# The Game of Life

## About
C program using Raylib to simulate 
[The Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## Usage
If you don't have [Raylib](https://github.com/raysan5/raylib) natively
installed, you first need a `.config` file that contains the paths to the 
local instance of the raylib library and include files. It should look like
the following:
```conf
LIBDIR=/path/to/raylib/library
INCLUDES=/path/to/raylib/headers
```

To build the program, simply run `make`. If you are on Windows, you will need
to copy the `raylib.dll` file into your current directory, if you haven't
done so already, or don't have Raylib natively installed.


## Controls
With the simulation running you have the following control options:

| **Key**     | **Action**       |
|-------------|------------------|
| `Space Bar` | Pause and Resume |
| `Enter`     | Restart          |
| `Esc`       | Quit             |

Also, you can press and hold the left mouse button and drag it around the
screen to turn cells to being live.

> [!NOTE]
> It's best to do this while the simulation is paused