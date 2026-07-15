/**
 *
 *  @file build_profile.hpp
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
#ifndef CGRIDE_PROJECT_BUILD_PROFILE_HPP
#define CGRIDE_PROJECT_BUILD_PROFILE_HPP

#include <string_view>

namespace cgride::project
{
  /**
   * @enum BuildProfile
   * @brief High-level build profile requested by a project or command.
   */
  enum class BuildProfile
  {
    Debug,
    Release,
    RelWithDebInfo,
    MinSizeRel
  };

  /**
   * @brief Convert a build profile to a stable string.
   *
   * @param profile Build profile.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(BuildProfile profile) noexcept
  {
    switch (profile)
    {
    case BuildProfile::Debug:
      return "Debug";

    case BuildProfile::Release:
      return "Release";

    case BuildProfile::RelWithDebInfo:
      return "RelWithDebInfo";

    case BuildProfile::MinSizeRel:
      return "MinSizeRel";
    }

    return "Debug";
  }

  /**
   * @brief Return true if the profile is optimized for runtime performance.
   *
   * @param profile Build profile.
   * @return True for release-like profiles.
   */
  [[nodiscard]] constexpr bool is_optimized(BuildProfile profile) noexcept
  {
    return profile == BuildProfile::Release ||
           profile == BuildProfile::RelWithDebInfo ||
           profile == BuildProfile::MinSizeRel;
  }

  /**
   * @brief Return true if the profile should normally include debug information.
   *
   * @param profile Build profile.
   * @return True for debug and release-with-debug-info profiles.
   */
  [[nodiscard]] constexpr bool has_debug_info(BuildProfile profile) noexcept
  {
    return profile == BuildProfile::Debug ||
           profile == BuildProfile::RelWithDebInfo;
  }

  /**
   * @brief Return true if the profile is optimized for smaller binary size.
   *
   * @param profile Build profile.
   * @return True for minimum-size release profiles.
   */
  [[nodiscard]] constexpr bool optimizes_for_size(BuildProfile profile) noexcept
  {
    return profile == BuildProfile::MinSizeRel;
  }

} // namespace cgride::project

#endif // CGRIDE_PROJECT_BUILD_PROFILE_HPP
