/**
 *
 *  @file project_test.cpp
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
#include <string>

#include <cgride/project/project.hpp>

int main()
{
  {
    cgride::project::Project project;

    assert(project.name().empty());
    assert(project.empty());
    assert(project.size() == 0);
    assert(project.targets().empty());
    assert(project.find_target("app") == nullptr);
    assert(!project.has_target("app"));
  }

  {
    cgride::project::Project project("hello");

    assert(project.name() == "hello");

    project.name("server");

    assert(project.name() == "server");
  }

  {
    cgride::project::Project project("hello");

    auto &app = project.executable("app");
    (void)app;

    assert(project.size() == 1);
    assert(!project.empty());

    assert(app.name() == "app");
    assert(app.kind() == cgride::project::TargetKind::Executable);

    assert(project.find_target("app") == &app);
    assert(project.has_target("app"));
    assert(project.find_target("missing") == nullptr);
  }

  {
    cgride::project::Project project("libraries");

    auto &core = project.static_library("core");
    auto &shared = project.shared_library("shared");
    auto &headers = project.interface_library("headers");
    (void)core;
    (void)shared;
    (void)headers;

    assert(project.size() == 3);

    assert(core.name() == "core");
    assert(core.kind() == cgride::project::TargetKind::StaticLibrary);

    assert(shared.name() == "shared");
    assert(shared.kind() == cgride::project::TargetKind::SharedLibrary);

    assert(headers.name() == "headers");
    assert(headers.kind() == cgride::project::TargetKind::InterfaceLibrary);

    assert(project.find_target("core") == &core);
    assert(project.find_target("shared") == &shared);
    assert(project.find_target("headers") == &headers);
  }

  {
    cgride::project::Project project("custom");

    auto &target = project.target(
        "plugin",
        cgride::project::TargetKind::SharedLibrary);
    (void)target;

    assert(project.size() == 1);
    assert(target.name() == "plugin");
    assert(target.kind() == cgride::project::TargetKind::SharedLibrary);
    assert(project.find_target("plugin") == &target);
  }

  {
    cgride::project::Project project("stable");

    auto &first = project.executable("first");
    (void)first;

    for (int index = 0; index < 32; ++index)
    {
      project.executable("target_" + std::to_string(index));
    }

    assert(project.size() == 33);
    assert(first.name() == "first");
    assert(project.find_target("first") == &first);
  }

  {
    cgride::project::Project project("clear");

    project.executable("app");
    project.static_library("core");

    assert(project.size() == 2);
    assert(!project.empty());

    project.clear();

    assert(project.empty());
    assert(project.size() == 0);
    assert(project.find_target("app") == nullptr);
    assert(!project.has_target("core"));
  }

  {
    cgride::project::Project project("links");

    auto &core = project.static_library("core");
    auto &app = project.executable("app");

    core.source("src/core.cpp");
    app.source("src/main.cpp").link(core);

    assert(project.size() == 2);
    assert(app.target_links().size() == 1);
    assert(app.target_links()[0].target_name() == "core");
  }

  return 0;
}
