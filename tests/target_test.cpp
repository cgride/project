/**
 *
 *  @file target_test.cpp
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

#include <cgride/project/target.hpp>

int main()
{
  {
    using cgride::project::CppStandard;
    using cgride::project::to_string;

    assert(to_string(CppStandard::Cxx17) == std::string_view("Cxx17"));
    assert(to_string(CppStandard::Cxx20) == std::string_view("Cxx20"));
    assert(to_string(CppStandard::Cxx23) == std::string_view("Cxx23"));
    assert(to_string(CppStandard::Cxx26) == std::string_view("Cxx26"));
  }

  {
    using cgride::project::TargetKind;

    assert(cgride::project::to_string(TargetKind::Executable) == std::string_view("Executable"));
    assert(cgride::project::to_string(TargetKind::StaticLibrary) == std::string_view("StaticLibrary"));
    assert(cgride::project::to_string(TargetKind::SharedLibrary) == std::string_view("SharedLibrary"));
    assert(cgride::project::to_string(TargetKind::InterfaceLibrary) == std::string_view("InterfaceLibrary"));

    assert(cgride::project::is_executable(TargetKind::Executable));
    assert(cgride::project::is_library(TargetKind::StaticLibrary));
    assert(cgride::project::is_library(TargetKind::SharedLibrary));
    assert(cgride::project::is_interface(TargetKind::InterfaceLibrary));

    assert(cgride::project::produces_artifact(TargetKind::Executable));
    assert(cgride::project::produces_artifact(TargetKind::StaticLibrary));
    assert(cgride::project::produces_artifact(TargetKind::SharedLibrary));
    assert(!cgride::project::produces_artifact(TargetKind::InterfaceLibrary));
  }

  {
    using cgride::project::BuildProfile;

    assert(cgride::project::to_string(BuildProfile::Debug) == std::string_view("Debug"));
    assert(cgride::project::to_string(BuildProfile::Release) == std::string_view("Release"));
    assert(cgride::project::to_string(BuildProfile::RelWithDebInfo) == std::string_view("RelWithDebInfo"));
    assert(cgride::project::to_string(BuildProfile::MinSizeRel) == std::string_view("MinSizeRel"));

    assert(!cgride::project::is_optimized(BuildProfile::Debug));
    assert(cgride::project::is_optimized(BuildProfile::Release));
    assert(cgride::project::is_optimized(BuildProfile::RelWithDebInfo));
    assert(cgride::project::is_optimized(BuildProfile::MinSizeRel));

    assert(cgride::project::has_debug_info(BuildProfile::Debug));
    assert(!cgride::project::has_debug_info(BuildProfile::Release));
    assert(cgride::project::has_debug_info(BuildProfile::RelWithDebInfo));
    assert(!cgride::project::has_debug_info(BuildProfile::MinSizeRel));

    assert(cgride::project::optimizes_for_size(BuildProfile::MinSizeRel));
    assert(!cgride::project::optimizes_for_size(BuildProfile::Release));
  }

  {
    cgride::project::TargetLink link(
        cgride::project::Visibility::Public,
        "core");

    assert(link.visibility() == cgride::project::Visibility::Public);
    assert(link.target_name() == "core");
    assert(link.valid());
  }

  {
    cgride::project::TargetLink link;

    assert(link.visibility() == cgride::project::Visibility::Private);
    assert(link.target_name().empty());
    assert(!link.valid());
  }

  {
    cgride::project::Target target(
        "app",
        cgride::project::TargetKind::Executable);

    assert(target.name() == "app");
    assert(target.kind() == cgride::project::TargetKind::Executable);
    assert(target.cpp_standard() == cgride::project::CppStandard::Cxx23);
    assert(target.build_profile() == cgride::project::BuildProfile::Debug);
    assert(target.source_set().empty());
    assert(target.requirements().empty());
    assert(target.target_links().empty());
    assert(target.valid());
  }

  {
    cgride::project::Target target(
        "app",
        cgride::project::TargetKind::Executable);

    target
        .cpp_standard(cgride::project::CppStandard::Cxx20)
        .build_profile(cgride::project::BuildProfile::Release);

    assert(target.cpp_standard() == cgride::project::CppStandard::Cxx20);
    assert(target.build_profile() == cgride::project::BuildProfile::Release);
  }

  {
    cgride::project::Target target(
        "app",
        cgride::project::TargetKind::Executable);

    target
        .source("src/main.cpp")
        .sources("src/**/*.cpp")
        .source_files({
            std::filesystem::path("src/app.cpp"),
            std::filesystem::path("src/server.cpp"),
        });

    assert(target.source_set().size() == 4);
    assert(target.source_set().entries()[0].kind() == cgride::project::SourceKind::File);
    assert(target.source_set().entries()[0].path() == std::filesystem::path("src/main.cpp"));
    assert(target.source_set().entries()[1].kind() == cgride::project::SourceKind::Pattern);
    assert(target.source_set().entries()[1].pattern() == "src/**/*.cpp");
    assert(target.source_set().entries()[2].path() == std::filesystem::path("src/app.cpp"));
    assert(target.source_set().entries()[3].path() == std::filesystem::path("src/server.cpp"));
  }

  {
    cgride::project::Target target(
        "core",
        cgride::project::TargetKind::StaticLibrary);

    target
        .include_directory("private/include")
        .public_include_directory("include")
        .compile_definition("CGRIDE_CORE=1")
        .compile_option("-Wall")
        .link_option("-pthread")
        .link_library("pthread", cgride::project::Visibility::Public);

    assert(target.requirements().size() == 6);

    assert(target.requirements().entries()[0].kind() == cgride::project::RequirementKind::IncludeDirectory);
    assert(target.requirements().entries()[0].visibility() == cgride::project::Visibility::Private);
    assert(target.requirements().entries()[0].path() == std::filesystem::path("private/include"));

    assert(target.requirements().entries()[1].kind() == cgride::project::RequirementKind::IncludeDirectory);
    assert(target.requirements().entries()[1].visibility() == cgride::project::Visibility::Public);
    assert(target.requirements().entries()[1].path() == std::filesystem::path("include"));

    assert(target.requirements().entries()[2].kind() == cgride::project::RequirementKind::CompileDefinition);
    assert(target.requirements().entries()[2].value() == "CGRIDE_CORE=1");

    assert(target.requirements().entries()[3].kind() == cgride::project::RequirementKind::CompileOption);
    assert(target.requirements().entries()[3].value() == "-Wall");

    assert(target.requirements().entries()[4].kind() == cgride::project::RequirementKind::LinkOption);
    assert(target.requirements().entries()[4].value() == "-pthread");

    assert(target.requirements().entries()[5].kind() == cgride::project::RequirementKind::LinkLibrary);
    assert(target.requirements().entries()[5].visibility() == cgride::project::Visibility::Public);
    assert(target.requirements().entries()[5].value() == "pthread");
  }

  {
    cgride::project::Target core(
        "core",
        cgride::project::TargetKind::StaticLibrary);

    cgride::project::Target app(
        "app",
        cgride::project::TargetKind::Executable);

    app
        .link(core, cgride::project::Visibility::Public)
        .link_named("network");

    assert(app.target_links().size() == 2);

    assert(app.target_links()[0].target_name() == "core");
    assert(app.target_links()[0].visibility() == cgride::project::Visibility::Public);

    assert(app.target_links()[1].target_name() == "network");
    assert(app.target_links()[1].visibility() == cgride::project::Visibility::Private);
  }

  {
    cgride::project::Target target(
        "",
        cgride::project::TargetKind::Executable);

    assert(!target.valid());
  }

  return 0;
}
