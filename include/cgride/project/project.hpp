/**
 *
 *  @file project.hpp
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
#ifndef CGRIDE_PROJECT_PROJECT_HPP
#define CGRIDE_PROJECT_PROJECT_HPP

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <cgride/project/target.hpp>
#include <cgride/project/target_kind.hpp>

namespace cgride::project
{
  /**
   * @class Project
   * @brief Logical C++ project model.
   *
   * Project owns targets and keeps them stable by storing them behind
   * unique pointers. References returned by target creation functions remain
   * valid for the lifetime of the project.
   */
  class Project
  {
  public:
    /**
     * @brief Construct an empty project.
     */
    Project() = default;

    /**
     * @brief Construct a named project.
     *
     * @param name Project name.
     */
    explicit Project(std::string name);

    Project(const Project &) = delete;
    Project &operator=(const Project &) = delete;

    Project(Project &&) noexcept = default;
    Project &operator=(Project &&) noexcept = default;

    /**
     * @brief Set the project name.
     *
     * @param name Project name.
     * @return Reference to this project.
     */
    Project &name(std::string name);

    /**
     * @brief Create a target.
     *
     * @param name Target name.
     * @param kind Target kind.
     * @return Reference to the created target.
     */
    Target &target(std::string name, TargetKind kind);

    /**
     * @brief Create an executable target.
     *
     * @param name Target name.
     * @return Reference to the created target.
     */
    Target &executable(std::string name);

    /**
     * @brief Create a static library target.
     *
     * @param name Target name.
     * @return Reference to the created target.
     */
    Target &static_library(std::string name);

    /**
     * @brief Create a shared library target.
     *
     * @param name Target name.
     * @return Reference to the created target.
     */
    Target &shared_library(std::string name);

    /**
     * @brief Create an interface library target.
     *
     * @param name Target name.
     * @return Reference to the created target.
     */
    Target &interface_library(std::string name);

    /**
     * @brief Find a target by name.
     *
     * @param name Target name.
     * @return Pointer to the target or nullptr.
     */
    [[nodiscard]] Target *find_target(std::string_view name) noexcept;

    /**
     * @brief Find a target by name.
     *
     * @param name Target name.
     * @return Pointer to the target or nullptr.
     */
    [[nodiscard]] const Target *find_target(std::string_view name) const noexcept;

    /**
     * @brief Return true if a target with this name exists.
     *
     * @param name Target name.
     * @return True if the target exists.
     */
    [[nodiscard]] bool has_target(std::string_view name) const noexcept;

    /**
     * @brief Access the project name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Access all targets.
     */
    [[nodiscard]] const std::vector<std::unique_ptr<Target>> &targets() const noexcept;

    /**
     * @brief Return true if the project has no targets.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return the number of targets.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Remove all targets.
     */
    void clear() noexcept;

  private:
    std::string name_{};
    std::vector<std::unique_ptr<Target>> targets_{};
  };

} // namespace cgride::project

#endif // CGRIDE_PROJECT_PROJECT_HPP
