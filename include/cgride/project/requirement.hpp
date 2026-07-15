/**
 *
 *  @file requirement.hpp
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
#ifndef CGRIDE_PROJECT_REQUIREMENT_HPP
#define CGRIDE_PROJECT_REQUIREMENT_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <cgride/project/visibility.hpp>

namespace cgride::project
{
  /**
   * @enum RequirementKind
   * @brief Kind of usage requirement stored on a target.
   */
  enum class RequirementKind
  {
    IncludeDirectory,
    CompileDefinition,
    CompileOption,
    LinkOption,
    LinkLibrary
  };

  /**
   * @brief Convert a requirement kind to a stable string.
   *
   * @param kind Requirement kind.
   * @return Stable string representation.
   */
  [[nodiscard]] std::string_view to_string(RequirementKind kind) noexcept;

  /**
   * @class Requirement
   * @brief One target usage requirement.
   *
   * A requirement stores a value and a visibility. It does not translate the
   * value to compiler or linker flags. Translation belongs to the toolchain
   * layer.
   */
  class Requirement
  {
  public:
    /**
     * @brief Create an include directory requirement.
     *
     * @param visibility Requirement visibility.
     * @param path Include directory path.
     * @return Requirement.
     */
    [[nodiscard]] static Requirement include_directory(Visibility visibility,
                                                       std::filesystem::path path);

    /**
     * @brief Create a compile definition requirement.
     *
     * @param visibility Requirement visibility.
     * @param value Compile definition.
     * @return Requirement.
     */
    [[nodiscard]] static Requirement compile_definition(Visibility visibility,
                                                        std::string value);

    /**
     * @brief Create a compile option requirement.
     *
     * @param visibility Requirement visibility.
     * @param value Compile option.
     * @return Requirement.
     */
    [[nodiscard]] static Requirement compile_option(Visibility visibility,
                                                    std::string value);

    /**
     * @brief Create a link option requirement.
     *
     * @param visibility Requirement visibility.
     * @param value Link option.
     * @return Requirement.
     */
    [[nodiscard]] static Requirement link_option(Visibility visibility,
                                                 std::string value);

    /**
     * @brief Create a named link library requirement.
     *
     * @param visibility Requirement visibility.
     * @param value Library name.
     * @return Requirement.
     */
    [[nodiscard]] static Requirement link_library(Visibility visibility,
                                                  std::string value);

    /**
     * @brief Access the requirement kind.
     */
    [[nodiscard]] RequirementKind kind() const noexcept;

    /**
     * @brief Access the requirement visibility.
     */
    [[nodiscard]] Visibility visibility() const noexcept;

    /**
     * @brief Access the string value.
     *
     * This value is meaningful for definitions, options and link libraries.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Access the path value.
     *
     * This value is meaningful for include directories.
     */
    [[nodiscard]] const std::filesystem::path &path() const noexcept;

    /**
     * @brief Return true if the requirement contains a non-empty value.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    RequirementKind kind_{RequirementKind::CompileOption};
    Visibility visibility_{Visibility::Private};
    std::string value_{};
    std::filesystem::path path_{};
  };

  /**
   * @class RequirementSet
   * @brief Collection of target usage requirements.
   */
  class RequirementSet
  {
  public:
    /**
     * @brief Add an include directory.
     *
     * @param visibility Requirement visibility.
     * @param path Include directory path.
     * @return Reference to this requirement set.
     */
    RequirementSet &include_directory(Visibility visibility, std::filesystem::path path);

    /**
     * @brief Add a compile definition.
     *
     * @param visibility Requirement visibility.
     * @param value Compile definition.
     * @return Reference to this requirement set.
     */
    RequirementSet &compile_definition(Visibility visibility, std::string value);

    /**
     * @brief Add a compile option.
     *
     * @param visibility Requirement visibility.
     * @param value Compile option.
     * @return Reference to this requirement set.
     */
    RequirementSet &compile_option(Visibility visibility, std::string value);

    /**
     * @brief Add a link option.
     *
     * @param visibility Requirement visibility.
     * @param value Link option.
     * @return Reference to this requirement set.
     */
    RequirementSet &link_option(Visibility visibility, std::string value);

    /**
     * @brief Add a named link library.
     *
     * @param visibility Requirement visibility.
     * @param value Library name.
     * @return Reference to this requirement set.
     */
    RequirementSet &link_library(Visibility visibility, std::string value);

    /**
     * @brief Add a requirement directly.
     *
     * @param requirement Requirement value.
     * @return Reference to this requirement set.
     */
    RequirementSet &add(Requirement requirement);

    /**
     * @brief Access all requirements in insertion order.
     */
    [[nodiscard]] const std::vector<Requirement> &entries() const noexcept;

    /**
     * @brief Return true if the requirement set has no entries.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return the number of requirements.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Remove all requirements.
     */
    void clear() noexcept;

  private:
    std::vector<Requirement> entries_{};
  };

} // namespace cgride::project

#endif // CGRIDE_PROJECT_REQUIREMENT_HPP
