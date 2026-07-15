/**
 *
 *  @file target.cpp
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
#include <cgride/project/target.hpp>

namespace cgride::project
{
  std::string_view to_string(CppStandard standard) noexcept
  {
    switch (standard)
    {
    case CppStandard::Cxx17:
      return "Cxx17";

    case CppStandard::Cxx20:
      return "Cxx20";

    case CppStandard::Cxx23:
      return "Cxx23";

    case CppStandard::Cxx26:
      return "Cxx26";
    }

    return "Cxx23";
  }

  TargetLink::TargetLink(Visibility visibility, std::string target_name)
      : visibility_(visibility),
        target_name_(std::move(target_name))
  {
  }

  Visibility TargetLink::visibility() const noexcept
  {
    return visibility_;
  }

  const std::string &TargetLink::target_name() const noexcept
  {
    return target_name_;
  }

  bool TargetLink::valid() const noexcept
  {
    return !target_name_.empty();
  }

  Target::Target(std::string name, TargetKind kind)
      : name_(std::move(name)),
        kind_(kind)
  {
  }

  Target &Target::cpp_standard(CppStandard standard) noexcept
  {
    cpp_standard_ = standard;
    return *this;
  }

  Target &Target::build_profile(BuildProfile profile) noexcept
  {
    build_profile_ = profile;
    return *this;
  }

  Target &Target::source(std::filesystem::path path)
  {
    sources_.file(std::move(path));
    return *this;
  }

  Target &Target::sources(std::string pattern)
  {
    sources_.pattern(std::move(pattern));
    return *this;
  }

  Target &Target::source_files(std::vector<std::filesystem::path> paths)
  {
    sources_.files(std::move(paths));
    return *this;
  }

  Target &Target::include_directory(std::filesystem::path path,
                                    Visibility visibility)
  {
    requirements_.include_directory(visibility, std::move(path));
    return *this;
  }

  Target &Target::public_include_directory(std::filesystem::path path)
  {
    return include_directory(std::move(path), Visibility::Public);
  }

  Target &Target::compile_definition(std::string value,
                                     Visibility visibility)
  {
    requirements_.compile_definition(visibility, std::move(value));
    return *this;
  }

  Target &Target::compile_option(std::string value,
                                 Visibility visibility)
  {
    requirements_.compile_option(visibility, std::move(value));
    return *this;
  }

  Target &Target::link_option(std::string value,
                              Visibility visibility)
  {
    requirements_.link_option(visibility, std::move(value));
    return *this;
  }

  Target &Target::link_library(std::string value,
                               Visibility visibility)
  {
    requirements_.link_library(visibility, std::move(value));
    return *this;
  }

  Target &Target::link(const Target &target, Visibility visibility)
  {
    return link_named(target.name(), visibility);
  }

  Target &Target::link_named(std::string target_name, Visibility visibility)
  {
    target_links_.emplace_back(visibility, std::move(target_name));
    return *this;
  }

  const std::string &Target::name() const noexcept
  {
    return name_;
  }

  TargetKind Target::kind() const noexcept
  {
    return kind_;
  }

  CppStandard Target::cpp_standard() const noexcept
  {
    return cpp_standard_;
  }

  BuildProfile Target::build_profile() const noexcept
  {
    return build_profile_;
  }

  const SourceSet &Target::source_set() const noexcept
  {
    return sources_;
  }

  const RequirementSet &Target::requirements() const noexcept
  {
    return requirements_;
  }

  const std::vector<TargetLink> &Target::target_links() const noexcept
  {
    return target_links_;
  }

  bool Target::valid() const noexcept
  {
    return !name_.empty();
  }

} // namespace cgride::project
