/**
 *
 *  @file visibility.hpp
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
#ifndef CGRIDE_PROJECT_VISIBILITY_HPP
#define CGRIDE_PROJECT_VISIBILITY_HPP

#include <string_view>

namespace cgride::project
{
  /**
   * @enum Visibility
   * @brief Visibility of a target usage requirement.
   *
   * Visibility describes how include directories, compile definitions,
   * compile options and link requirements propagate between targets.
   */
  enum class Visibility
  {
    Private,
    Public,
    Interface
  };

  /**
   * @brief Convert a visibility value to a stable string.
   *
   * @param visibility Visibility value.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(Visibility visibility) noexcept
  {
    switch (visibility)
    {
    case Visibility::Private:
      return "Private";

    case Visibility::Public:
      return "Public";

    case Visibility::Interface:
      return "Interface";
    }

    return "Private";
  }

  /**
   * @brief Return true if the requirement applies to the target itself.
   *
   * @param visibility Visibility value.
   * @return True when the requirement is private or public.
   */
  [[nodiscard]] constexpr bool applies_to_self(Visibility visibility) noexcept
  {
    return visibility == Visibility::Private || visibility == Visibility::Public;
  }

  /**
   * @brief Return true if the requirement propagates to dependents.
   *
   * @param visibility Visibility value.
   * @return True when the requirement is public or interface.
   */
  [[nodiscard]] constexpr bool propagates_to_dependents(Visibility visibility) noexcept
  {
    return visibility == Visibility::Public || visibility == Visibility::Interface;
  }

} // namespace cgride::project

#endif // CGRIDE_PROJECT_VISIBILITY_HPP
