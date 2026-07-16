/**
 *
 *  @file validation_test.cpp
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
#include <string_view>

#include <cgride/project/validation.hpp>

int main()
{
  {
    cgride::project::Project project;

    auto errors = cgride::project::validate_project_errors(project);

    assert(errors.size() == 1);
    assert(errors[0].code() == cgride::core::ErrorCode::InvalidArgument);
    assert(errors[0].message() == "Project has no targets.");

    auto result = cgride::project::validate_project(project);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
  }

  {
    cgride::project::Project project("hello");

    auto &app = project.executable("app");
    app.source("src/main.cpp");

    auto errors = cgride::project::validate_project_errors(project);

    assert(errors.empty());

    auto result = cgride::project::validate_project(project);

    assert(result);
  }

  {
    cgride::project::Project project("empty-target");

    project.executable("");

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());
    assert(errors[0].code() == cgride::core::ErrorCode::InvalidArgument);
    assert(errors[0].message() == "Project contains a target with an empty name.");
  }

  {
    cgride::project::Project project("duplicate-targets");

    project.executable("app").source("src/main.cpp");
    project.static_library("app").source("src/app.cpp");

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Project contains duplicate target names." &&
          error.detail().has_value() &&
          error.detail().value() == "app")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("missing-sources");

    project.executable("app");

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Target has no sources." &&
          error.detail().has_value() &&
          error.detail().value() == "app")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("interface-target");

    project.interface_library("headers")
        .public_include_directory("include");

    auto errors = cgride::project::validate_project_errors(project);

    assert(errors.empty());

    auto result = cgride::project::validate_project(project);

    assert(result);
  }

  {
    cgride::project::Project project("invalid-source");

    project.executable("app").source({});

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Target contains an invalid source entry." &&
          error.detail().has_value() &&
          error.detail().value() == "app")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("invalid-requirement");

    project.executable("app")
        .source("src/main.cpp")
        .compile_option("");

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Target contains an invalid requirement." &&
          error.detail().has_value() &&
          error.detail().value() == "app")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("unknown-link");

    project.executable("app")
        .source("src/main.cpp")
        .link_named("core");

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::NotFound &&
          error.message() == "Target link refers to an unknown target." &&
          error.detail().has_value() &&
          error.detail().value() == "core")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("self-link");

    auto &app = project.executable("app");
    app.source("src/main.cpp");
    app.link(app);

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidArgument &&
          error.message() == "Target cannot link to itself." &&
          error.detail().has_value() &&
          error.detail().value() == "app")
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);
  }

  {
    cgride::project::Project project("cycle");

    auto &core = project.static_library("core");
    auto &net = project.static_library("net");
    auto &app = project.executable("app");

    core.source("src/core.cpp").link(net);
    net.source("src/net.cpp").link(app);
    app.source("src/main.cpp").link(core);

    auto errors = cgride::project::validate_project_errors(project);

    assert(!errors.empty());

    bool found = false;

    for (const auto &error : errors)
    {
      if (error.code() == cgride::core::ErrorCode::InvalidState &&
          error.message() == "Target dependency cycle detected." &&
          error.detail().has_value())
      {
        found = true;
      }
    }

    if (!found)
    {
      return 1;
    }

    assert(found);

    auto result = cgride::project::validate_project(project);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidState ||
           result.error().code() == cgride::core::ErrorCode::InvalidArgument);
  }

  {
    cgride::project::Project project("valid-links");

    auto &core = project.static_library("core");
    auto &app = project.executable("app");

    core.source("src/core.cpp");
    app.source("src/main.cpp").link(core);

    auto errors = cgride::project::validate_project_errors(project);

    assert(errors.empty());

    auto result = cgride::project::validate_project(project);

    assert(result);
  }

  {
    cgride::project::Project project("multiple-errors");

    project.executable("app");
    project.executable("app");
    project.executable("server").source({}).link_named("missing");

    auto errors = cgride::project::validate_project_errors(project);

    assert(errors.size() >= 3);
  }

  return 0;
}
