# Cgride Project

C++ project model with targets, sources, includes and links for Cgride.

Cgride Project is the logical project model module. It describes what a C++ project wants to build without selecting a compiler, generating build commands, executing tasks or printing terminal output.

## Purpose

This module provides the project-level types used by the Cgride engine:

- project model
- target model
- executable targets
- static library targets
- shared library targets
- interface library targets
- source files and source patterns
- include directories
- compile definitions
- compile options
- link options
- target-to-target links
- project validation

Cgride Project does not contain toolchain discovery, build planning, task execution, caching or CLI logic.

## Requirements

- C++23
- CMake 3.22 or newer
- Vix.cpp
- Cgride Core

## Build

```bash
vix build --build-target all
```

## Run tests

```bash
vix tests
```

## Release build

```bash
vix build --preset release --build-target all
```

## Install

```bash
vix build --build-target all
sudo cmake --install build-ninja --prefix /usr/local
```

## CMake usage

```cmake
find_package(cgride-project CONFIG REQUIRED)

target_link_libraries(my_tool PRIVATE cgride::project)
```

## Basic example

```cpp
#include <cgride/project/project.hpp>

int main()
{
  cgride::project::Project project("hello");

  auto &core = project.static_library("core");
  core.source("src/core.cpp");
  core.public_include_directory("include");

  auto &app = project.executable("app");
  app.source("src/main.cpp");
  app.link(core);

  return 0;
}
```

## Module boundary

Cgride Project may depend on:

- `cgride::core`

Cgride Project may be used by:

- `cgride::engine`
- `cgride::config`
- `cgride::cli`
- external runtimes
- frameworks
- IDE integrations
- developer tools

Cgride Project must not depend on:

- `cgride::toolchains`
- `cgride::graph`
- `cgride::executor`
- `cgride::cache`
- `cgride::engine`
- `cgride::config`
- `cgride::cli`

## Design rule

This module stores project intent only.

It should answer:

```txt
What does the project want to build?
```

It should not answer:

```txt
Which compiler should be used?
Which files are dirty?
Which commands should run?
How should output be printed?
```

Those decisions belong to higher-level Cgride modules.

## License

MIT
