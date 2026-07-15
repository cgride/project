/**
 *
 *  @file target_kind.hpp
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
#ifndef CGRIDE_PROJECT_TARGET_KIND_HPP
#define CGRIDE_PROJECT_TARGET_KIND_HPP

#include <string_view>

namespace cgride::project
{
  /**
   * @enum TargetKind
   * @brief Logical kind of a Cgride build target.
   */
  enum class TargetKind
  {
    Executable,
    StaticLibrary,
    SharedLibrary,
    InterfaceLibrary
  };

  /**
   * @brief Convert a target kind to a stable string.
   *
   * @param kind Target kind.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(TargetKind kind) noexcept
  {
    switch (kind)
    {
    case TargetKind::Executable:
      return "Executable";

    case TargetKind::StaticLibrary:
      return "StaticLibrary";

    case TargetKind::SharedLibrary:
      return "SharedLibrary";

    case TargetKind::InterfaceLibrary:
      return "InterfaceLibrary";
    }

    return "Executable";
  }

  /**
   * @brief Return true if the target kind produces a final executable.
   *
   * @param kind Target kind.
   * @return True for executable targets.
   */
  [[nodiscard]] constexpr bool is_executable(TargetKind kind) noexcept
  {
    return kind == TargetKind::Executable;
  }

  /**
   * @brief Return true if the target kind produces a library artifact.
   *
   * @param kind Target kind.
   * @return True for static or shared library targets.
   */
  [[nodiscard]] constexpr bool is_library(TargetKind kind) noexcept
  {
    return kind == TargetKind::StaticLibrary || kind == TargetKind::SharedLibrary;
  }

  /**
   * @brief Return true if the target kind only carries usage requirements.
   *
   * @param kind Target kind.
   * @return True for interface library targets.
   */
  [[nodiscard]] constexpr bool is_interface(TargetKind kind) noexcept
  {
    return kind == TargetKind::InterfaceLibrary;
  }

  /**
   * @brief Return true if the target kind produces a build artifact.
   *
   * @param kind Target kind.
   * @return True when the target produces an executable or library file.
   */
  [[nodiscard]] constexpr bool produces_artifact(TargetKind kind) noexcept
  {
    return kind == TargetKind::Executable ||
           kind == TargetKind::StaticLibrary ||
           kind == TargetKind::SharedLibrary;
  }

} // namespace cgride::project

#endif // CGRIDE_PROJECT_TARGET_KIND_HPP
