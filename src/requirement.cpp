/**
 *
 *  @file requirement.cpp
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
#include <cgride/project/requirement.hpp>

namespace cgride::project
{
  std::string_view to_string(RequirementKind kind) noexcept
  {
    switch (kind)
    {
    case RequirementKind::IncludeDirectory:
      return "IncludeDirectory";

    case RequirementKind::CompileDefinition:
      return "CompileDefinition";

    case RequirementKind::CompileOption:
      return "CompileOption";

    case RequirementKind::LinkOption:
      return "LinkOption";

    case RequirementKind::LinkLibrary:
      return "LinkLibrary";
    }

    return "CompileOption";
  }

  Requirement Requirement::include_directory(Visibility visibility,
                                             std::filesystem::path path)
  {
    Requirement requirement;
    requirement.kind_ = RequirementKind::IncludeDirectory;
    requirement.visibility_ = visibility;
    requirement.path_ = std::move(path);
    return requirement;
  }

  Requirement Requirement::compile_definition(Visibility visibility,
                                              std::string value)
  {
    Requirement requirement;
    requirement.kind_ = RequirementKind::CompileDefinition;
    requirement.visibility_ = visibility;
    requirement.value_ = std::move(value);
    return requirement;
  }

  Requirement Requirement::compile_option(Visibility visibility,
                                          std::string value)
  {
    Requirement requirement;
    requirement.kind_ = RequirementKind::CompileOption;
    requirement.visibility_ = visibility;
    requirement.value_ = std::move(value);
    return requirement;
  }

  Requirement Requirement::link_option(Visibility visibility,
                                       std::string value)
  {
    Requirement requirement;
    requirement.kind_ = RequirementKind::LinkOption;
    requirement.visibility_ = visibility;
    requirement.value_ = std::move(value);
    return requirement;
  }

  Requirement Requirement::link_library(Visibility visibility,
                                        std::string value)
  {
    Requirement requirement;
    requirement.kind_ = RequirementKind::LinkLibrary;
    requirement.visibility_ = visibility;
    requirement.value_ = std::move(value);
    return requirement;
  }

  RequirementKind Requirement::kind() const noexcept
  {
    return kind_;
  }

  Visibility Requirement::visibility() const noexcept
  {
    return visibility_;
  }

  const std::string &Requirement::value() const noexcept
  {
    return value_;
  }

  const std::filesystem::path &Requirement::path() const noexcept
  {
    return path_;
  }

  bool Requirement::valid() const noexcept
  {
    switch (kind_)
    {
    case RequirementKind::IncludeDirectory:
      return !path_.empty();

    case RequirementKind::CompileDefinition:
    case RequirementKind::CompileOption:
    case RequirementKind::LinkOption:
    case RequirementKind::LinkLibrary:
      return !value_.empty();
    }

    return false;
  }

  RequirementSet &RequirementSet::include_directory(Visibility visibility,
                                                    std::filesystem::path path)
  {
    entries_.push_back(Requirement::include_directory(visibility, std::move(path)));
    return *this;
  }

  RequirementSet &RequirementSet::compile_definition(Visibility visibility,
                                                     std::string value)
  {
    entries_.push_back(Requirement::compile_definition(visibility, std::move(value)));
    return *this;
  }

  RequirementSet &RequirementSet::compile_option(Visibility visibility,
                                                 std::string value)
  {
    entries_.push_back(Requirement::compile_option(visibility, std::move(value)));
    return *this;
  }

  RequirementSet &RequirementSet::link_option(Visibility visibility,
                                              std::string value)
  {
    entries_.push_back(Requirement::link_option(visibility, std::move(value)));
    return *this;
  }

  RequirementSet &RequirementSet::link_library(Visibility visibility,
                                               std::string value)
  {
    entries_.push_back(Requirement::link_library(visibility, std::move(value)));
    return *this;
  }

  RequirementSet &RequirementSet::add(Requirement requirement)
  {
    entries_.push_back(std::move(requirement));
    return *this;
  }

  const std::vector<Requirement> &RequirementSet::entries() const noexcept
  {
    return entries_;
  }

  bool RequirementSet::empty() const noexcept
  {
    return entries_.empty();
  }

  std::size_t RequirementSet::size() const noexcept
  {
    return entries_.size();
  }

  void RequirementSet::clear() noexcept
  {
    entries_.clear();
  }

} // namespace cgride::project
