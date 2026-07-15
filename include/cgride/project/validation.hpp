/**
 *
 *  @file validation.hpp
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
#ifndef CGRIDE_PROJECT_VALIDATION_HPP
#define CGRIDE_PROJECT_VALIDATION_HPP

#include <vector>

#include <cgride/core/error.hpp>
#include <cgride/core/result.hpp>
#include <cgride/project/project.hpp>

namespace cgride::project
{
  /**
   * @brief Validate a project and return every detected validation error.
   *
   * Validation checks the logical project model only. It does not expand
   * source patterns, inspect the filesystem, select a compiler or generate
   * build tasks.
   *
   * @param project Project to validate.
   * @return List of validation errors.
   */
  [[nodiscard]] std::vector<cgride::core::Error> validate_project_errors(const Project &project);

  /**
   * @brief Validate a project and return success or the first error.
   *
   * @param project Project to validate.
   * @return Success when the project model is valid, otherwise first error.
   */
  [[nodiscard]] cgride::core::Result<void> validate_project(const Project &project);

} // namespace cgride::project

#endif // CGRIDE_PROJECT_VALIDATION_HPP
