# Libs

This directory is intended for the project specific (private) libraries.
PlatformIO will compile them to static libraries and link to executable file.

The source code of each library should be placed in separate directory, like
`lib/private_lib/[here are source files]`.

For example, see how can be organized `Foo` and `Bar` libraries:

```txt
|--lib
|  |
|  |--Bar
|  |  |--docs
|  |  |--examples
|  |  |--src
|  |     |- Bar.c
|  |     |- Bar.h
|  |  |- library.json (optional, custom build options, etc) http://docs.platformio.org/page/librarymanager/config.html
|  |
|  |--Foo
|  |  |- Foo.c
|  |  |- Foo.h
|  |
|  |- readme.txt --> THIS FILE
|
|- platformio.ini
|--src
   |- main.c
```

Then in `src/main.c` you should use:

```cpp
#include <Foo.h>
#include <Bar.h>

// rest H/C/CPP code
```

PlatformIO will find your libraries automatically, configure preprocessor's
include paths and build them.

More information about PlatformIO Library Dependency Finder - <http://docs.platformio.org/page/librarymanager/ldf.html>
