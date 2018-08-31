# libCanvas #
A tiny library to draw graphics in pure C.  
  
## Why it's there ##
As a former VB lover, I found that controlling forms and draw things on them is much harder in pure C.  
So I wrote this thing to ease my further tasks.  
  
## Usage ##
```#include <libCanvas.h>```  
See `libCanvas.h` and `Demo.c` for more details.  
  
## How to compile ##
Only tested under TCC on win32. Must have w32api package installed.  
`tcc -shared libCanvas.c Internal.c`  
Place `libCanvas.def` under your library path, then  
`tcc -llibCanvas Demo.c`  
  
## License ##
BSD-3-Clause
