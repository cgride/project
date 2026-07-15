/**
 *
 *  @file source_set.hpp
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
#ifndef CGRIDE_PROJECT_SOURCE_SET_HPP
#define CGRIDE_PROJECT_SOURCE_SET_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cgride::project
{
  /**
   * @enum SourceKind
   * @brief Kind of source entry stored in a source set.
   */
  enum class SourceKind
  {
    File,
    Pattern
  };

  /**
   * @brief Convert a source kind to a stable string.
   *
   * @param kind Source kind.
   * @return Stable string representation.
   */
  [[nodiscard]] std::string_view to_string(SourceKind kind) noexcept;

  /**
   * @class SourceEntry
   * @brief Represents one source file or source pattern.
   *
   * A source entry does not expand glob patterns by itself. Expansion belongs
   * to the planning layer, where the project root and filesystem rules are
   * known.
   */
  class SourceEntry
  {
  public:
    /**
     * @brief Create a source file entry.
     *
     * @param path Source file path.
     * @return Source entry.
     */
    [[nodiscard]] static SourceEntry file(std::filesystem::path path);

    /**
     * @brief Create a source pattern entry.
     *
     * @param pattern Source glob pattern.
     * @return Source entry.
     */
    [[nodiscard]] static SourceEntry pattern(std::string pattern);

    /**
     * @brief Access the source kind.
     */
    [[nodiscard]] SourceKind kind() const noexcept;

    /**
     * @brief Access the source file path.
     *
     * This value is meaningful when kind() is SourceKind::File.
     */
    [[nodiscard]] const std::filesystem::path &path() const noexcept;

    /**
     * @brief Access the source pattern.
     *
     * This value is meaningful when kind() is SourceKind::Pattern.
     */
    [[nodiscard]] const std::string &pattern() const noexcept;

    /**
     * @brief Return true if the entry contains a non-empty value.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    SourceKind kind_{SourceKind::File};
    std::filesystem::path path_{};
    std::string pattern_{};
  };

  /**
   * @class SourceSet
   * @brief Collection of source files and source patterns for a target.
   *
   * SourceSet stores the logical source declarations made by the project
   * model. It does not inspect the filesystem or decide which files need
   * recompilation.
   */
  class SourceSet
  {
  public:
    /**
     * @brief Add one source file.
     *
     * @param path Source file path.
     * @return Reference to this source set.
     */
    SourceSet &file(std::filesystem::path path);

    /**
     * @brief Add many source files.
     *
     * @param paths Source file paths.
     * @return Reference to this source set.
     */
    SourceSet &files(std::vector<std::filesystem::path> paths);

    /**
     * @brief Add one source glob pattern.
     *
     * @param pattern Source glob pattern.
     * @return Reference to this source set.
     */
    SourceSet &pattern(std::string pattern);

    /**
     * @brief Add many source glob patterns.
     *
     * @param patterns Source glob patterns.
     * @return Reference to this source set.
     */
    SourceSet &patterns(std::vector<std::string> patterns);

    /**
     * @brief Access all source entries in insertion order.
     */
    [[nodiscard]] const std::vector<SourceEntry> &entries() const noexcept;

    /**
     * @brief Return true if the source set has no entries.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return the number of source entries.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Remove all source entries.
     */
    void clear() noexcept;

  private:
    std::vector<SourceEntry> entries_{};
  };

} // namespace cgride::project

#endif // CGRIDE_PROJECT_SOURCE_SET_HPP
