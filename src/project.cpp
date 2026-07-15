/**
 *
 *  @file project.cpp
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
#include <cgride/project/project.hpp>

namespace cgride::project
{
  Project::Project(std::string name)
      : name_(std::move(name))
  {
  }

  Project &Project::name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Target &Project::target(std::string name, TargetKind kind)
  {
    auto target = std::make_unique<Target>(std::move(name), kind);
    auto &reference = *target;

    targets_.push_back(std::move(target));

    return reference;
  }

  Target &Project::executable(std::string name)
  {
    return target(std::move(name), TargetKind::Executable);
  }

  Target &Project::static_library(std::string name)
  {
    return target(std::move(name), TargetKind::StaticLibrary);
  }

  Target &Project::shared_library(std::string name)
  {
    return target(std::move(name), TargetKind::SharedLibrary);
  }

  Target &Project::interface_library(std::string name)
  {
    return target(std::move(name), TargetKind::InterfaceLibrary);
  }

  Target *Project::find_target(std::string_view name) noexcept
  {
    for (auto &target : targets_)
    {
      if (target && target->name() == name)
      {
        return target.get();
      }
    }

    return nullptr;
  }

  const Target *Project::find_target(std::string_view name) const noexcept
  {
    for (const auto &target : targets_)
    {
      if (target && target->name() == name)
      {
        return target.get();
      }
    }

    return nullptr;
  }

  bool Project::has_target(std::string_view name) const noexcept
  {
    return find_target(name) != nullptr;
  }

  const std::string &Project::name() const noexcept
  {
    return name_;
  }

  const std::vector<std::unique_ptr<Target>> &Project::targets() const noexcept
  {
    return targets_;
  }

  bool Project::empty() const noexcept
  {
    return targets_.empty();
  }

  std::size_t Project::size() const noexcept
  {
    return targets_.size();
  }

  void Project::clear() noexcept
  {
    targets_.clear();
  }

} // namespace cgride::project
