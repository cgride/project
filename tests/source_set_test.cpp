/**
 *
 *  @file source_set_test.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/cgride/project
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Cgride
 *
 */
#include <cassert>
#include <filesystem>
#include <string_view>
#include <vector>

#include <cgride/project/source_set.hpp>

int main()
{
  {
    using cgride::project::SourceKind;
    using cgride::project::to_string;

    assert(to_string(SourceKind::File) == std::string_view("File"));
    assert(to_string(SourceKind::Pattern) == std::string_view("Pattern"));
  }

  {
    auto entry = cgride::project::SourceEntry::file("src/main.cpp");

    assert(entry.kind() == cgride::project::SourceKind::File);
    assert(entry.path() == std::filesystem::path("src/main.cpp"));
    assert(entry.pattern().empty());
    assert(entry.valid());
  }

  {
    auto entry = cgride::project::SourceEntry::pattern("src/**/*.cpp");

    assert(entry.kind() == cgride::project::SourceKind::Pattern);
    assert(entry.path().empty());
    assert(entry.pattern() == "src/**/*.cpp");
    assert(entry.valid());
  }

  {
    auto entry = cgride::project::SourceEntry::file({});

    assert(entry.kind() == cgride::project::SourceKind::File);
    assert(!entry.valid());
  }

  {
    auto entry = cgride::project::SourceEntry::pattern("");

    assert(entry.kind() == cgride::project::SourceKind::Pattern);
    assert(!entry.valid());
  }

  {
    cgride::project::SourceSet sources;

    assert(sources.empty());
    assert(sources.size() == 0);

    sources.file("src/main.cpp");

    assert(!sources.empty());
    assert(sources.size() == 1);
    assert(sources.entries()[0].kind() == cgride::project::SourceKind::File);
    assert(sources.entries()[0].path() == std::filesystem::path("src/main.cpp"));
  }

  {
    cgride::project::SourceSet sources;

    sources.pattern("src/**/*.cpp");

    assert(sources.size() == 1);
    assert(sources.entries()[0].kind() == cgride::project::SourceKind::Pattern);
    assert(sources.entries()[0].pattern() == "src/**/*.cpp");
  }

  {
    cgride::project::SourceSet sources;

    sources.files({
        std::filesystem::path("src/main.cpp"),
        std::filesystem::path("src/app.cpp"),
    });

    assert(sources.size() == 2);
    assert(sources.entries()[0].path() == std::filesystem::path("src/main.cpp"));
    assert(sources.entries()[1].path() == std::filesystem::path("src/app.cpp"));
  }

  {
    cgride::project::SourceSet sources;

    sources.patterns({
        "src/**/*.cpp",
        "include/**/*.hpp",
    });

    assert(sources.size() == 2);
    assert(sources.entries()[0].pattern() == "src/**/*.cpp");
    assert(sources.entries()[1].pattern() == "include/**/*.hpp");
  }

  {
    cgride::project::SourceSet sources;

    sources
        .file("src/main.cpp")
        .pattern("src/**/*.cpp");

    assert(sources.size() == 2);

    sources.clear();

    assert(sources.empty());
    assert(sources.size() == 0);
  }

  return 0;
}
