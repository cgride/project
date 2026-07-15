/**
 *
 *  @file requirement_test.cpp
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

#include <cgride/project/requirement.hpp>
#include <cgride/project/visibility.hpp>

int main()
{
  {
    using cgride::project::RequirementKind;
    using cgride::project::to_string;

    assert(to_string(RequirementKind::IncludeDirectory) == std::string_view("IncludeDirectory"));
    assert(to_string(RequirementKind::CompileDefinition) == std::string_view("CompileDefinition"));
    assert(to_string(RequirementKind::CompileOption) == std::string_view("CompileOption"));
    assert(to_string(RequirementKind::LinkOption) == std::string_view("LinkOption"));
    assert(to_string(RequirementKind::LinkLibrary) == std::string_view("LinkLibrary"));
  }

  {
    using cgride::project::Visibility;

    assert(cgride::project::to_string(Visibility::Private) == std::string_view("Private"));
    assert(cgride::project::to_string(Visibility::Public) == std::string_view("Public"));
    assert(cgride::project::to_string(Visibility::Interface) == std::string_view("Interface"));

    assert(cgride::project::applies_to_self(Visibility::Private));
    assert(cgride::project::applies_to_self(Visibility::Public));
    assert(!cgride::project::applies_to_self(Visibility::Interface));

    assert(!cgride::project::propagates_to_dependents(Visibility::Private));
    assert(cgride::project::propagates_to_dependents(Visibility::Public));
    assert(cgride::project::propagates_to_dependents(Visibility::Interface));
  }

  {
    auto requirement = cgride::project::Requirement::include_directory(
        cgride::project::Visibility::Public,
        "include");

    assert(requirement.kind() == cgride::project::RequirementKind::IncludeDirectory);
    assert(requirement.visibility() == cgride::project::Visibility::Public);
    assert(requirement.path() == std::filesystem::path("include"));
    assert(requirement.value().empty());
    assert(requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::compile_definition(
        cgride::project::Visibility::Private,
        "CGRIDE_DEBUG=1");

    assert(requirement.kind() == cgride::project::RequirementKind::CompileDefinition);
    assert(requirement.visibility() == cgride::project::Visibility::Private);
    assert(requirement.value() == "CGRIDE_DEBUG=1");
    assert(requirement.path().empty());
    assert(requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::compile_option(
        cgride::project::Visibility::Private,
        "-Wall");

    assert(requirement.kind() == cgride::project::RequirementKind::CompileOption);
    assert(requirement.value() == "-Wall");
    assert(requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::link_option(
        cgride::project::Visibility::Private,
        "-pthread");

    assert(requirement.kind() == cgride::project::RequirementKind::LinkOption);
    assert(requirement.value() == "-pthread");
    assert(requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::link_library(
        cgride::project::Visibility::Public,
        "pthread");

    assert(requirement.kind() == cgride::project::RequirementKind::LinkLibrary);
    assert(requirement.visibility() == cgride::project::Visibility::Public);
    assert(requirement.value() == "pthread");
    assert(requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::compile_option(
        cgride::project::Visibility::Private,
        "");

    assert(!requirement.valid());
  }

  {
    auto requirement = cgride::project::Requirement::include_directory(
        cgride::project::Visibility::Private,
        {});

    assert(!requirement.valid());
  }

  {
    cgride::project::RequirementSet requirements;

    assert(requirements.empty());
    assert(requirements.size() == 0);

    requirements
        .include_directory(cgride::project::Visibility::Public, "include")
        .compile_definition(cgride::project::Visibility::Private, "CGRIDE_DEBUG=1")
        .compile_option(cgride::project::Visibility::Private, "-Wall")
        .link_option(cgride::project::Visibility::Private, "-pthread")
        .link_library(cgride::project::Visibility::Public, "pthread");

    assert(!requirements.empty());
    assert(requirements.size() == 5);

    assert(requirements.entries()[0].kind() == cgride::project::RequirementKind::IncludeDirectory);
    assert(requirements.entries()[1].kind() == cgride::project::RequirementKind::CompileDefinition);
    assert(requirements.entries()[2].kind() == cgride::project::RequirementKind::CompileOption);
    assert(requirements.entries()[3].kind() == cgride::project::RequirementKind::LinkOption);
    assert(requirements.entries()[4].kind() == cgride::project::RequirementKind::LinkLibrary);

    requirements.clear();

    assert(requirements.empty());
    assert(requirements.size() == 0);
  }

  {
    cgride::project::RequirementSet requirements;

    requirements.add(
        cgride::project::Requirement::compile_option(
            cgride::project::Visibility::Private,
            "-Wextra"));

    assert(requirements.size() == 1);
    assert(requirements.entries()[0].value() == "-Wextra");
  }

  return 0;
}
