/**
 *
 *  @file target.hpp
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
#ifndef CGRIDE_PROJECT_TARGET_HPP
#define CGRIDE_PROJECT_TARGET_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <cgride/project/build_profile.hpp>
#include <cgride/project/requirement.hpp>
#include <cgride/project/source_set.hpp>
#include <cgride/project/target_kind.hpp>
#include <cgride/project/visibility.hpp>

namespace cgride::project
{
  /**
   * @enum CppStandard
   * @brief C++ language standard requested by a target.
   */
  enum class CppStandard
  {
    Cxx17,
    Cxx20,
    Cxx23,
    Cxx26
  };

  /**
   * @brief Convert a C++ standard value to a stable string.
   *
   * @param standard C++ standard.
   * @return Stable string representation.
   */
  [[nodiscard]] std::string_view to_string(CppStandard standard) noexcept;

  /**
   * @class TargetLink
   * @brief Logical link from one target to another target name.
   */
  class TargetLink
  {
  public:
    /**
     * @brief Construct an empty target link.
     */
    TargetLink() = default;

    /**
     * @brief Construct a target link.
     *
     * @param visibility Link visibility.
     * @param target_name Linked target name.
     */
    TargetLink(Visibility visibility, std::string target_name);

    /**
     * @brief Access the link visibility.
     */
    [[nodiscard]] Visibility visibility() const noexcept;

    /**
     * @brief Access the linked target name.
     */
    [[nodiscard]] const std::string &target_name() const noexcept;

    /**
     * @brief Return true if the link has a non-empty target name.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    Visibility visibility_{Visibility::Private};
    std::string target_name_{};
  };

  /**
   * @class Target
   * @brief Logical C++ build target.
   *
   * Target stores project intent only. It does not select a compiler, expand
   * source patterns, generate commands or execute a build.
   */
  class Target
  {
  public:
    /**
     * @brief Construct a target.
     *
     * @param name Target name.
     * @param kind Target kind.
     */
    Target(std::string name, TargetKind kind);

    /**
     * @brief Set the C++ language standard.
     *
     * @param standard C++ standard.
     * @return Reference to this target.
     */
    Target &cpp_standard(CppStandard standard) noexcept;

    /**
     * @brief Set the build profile.
     *
     * @param profile Build profile.
     * @return Reference to this target.
     */
    Target &build_profile(BuildProfile profile) noexcept;

    /**
     * @brief Add one source file.
     *
     * @param path Source file path.
     * @return Reference to this target.
     */
    Target &source(std::filesystem::path path);

    /**
     * @brief Add one source glob pattern.
     *
     * @param pattern Source glob pattern.
     * @return Reference to this target.
     */
    Target &sources(std::string pattern);

    /**
     * @brief Add many source files.
     *
     * @param paths Source file paths.
     * @return Reference to this target.
     */
    Target &source_files(std::vector<std::filesystem::path> paths);

    /**
     * @brief Add an include directory.
     *
     * @param path Include directory path.
     * @param visibility Requirement visibility.
     * @return Reference to this target.
     */
    Target &include_directory(std::filesystem::path path,
                              Visibility visibility = Visibility::Private);

    /**
     * @brief Add a public include directory.
     *
     * @param path Include directory path.
     * @return Reference to this target.
     */
    Target &public_include_directory(std::filesystem::path path);

    /**
     * @brief Add a compile definition.
     *
     * @param value Compile definition.
     * @param visibility Requirement visibility.
     * @return Reference to this target.
     */
    Target &compile_definition(std::string value,
                               Visibility visibility = Visibility::Private);

    /**
     * @brief Add a compile option.
     *
     * @param value Compile option.
     * @param visibility Requirement visibility.
     * @return Reference to this target.
     */
    Target &compile_option(std::string value,
                           Visibility visibility = Visibility::Private);

    /**
     * @brief Add a linker option.
     *
     * @param value Link option.
     * @param visibility Requirement visibility.
     * @return Reference to this target.
     */
    Target &link_option(std::string value,
                        Visibility visibility = Visibility::Private);

    /**
     * @brief Add a named system or external library.
     *
     * @param value Library name.
     * @param visibility Requirement visibility.
     * @return Reference to this target.
     */
    Target &link_library(std::string value,
                         Visibility visibility = Visibility::Private);

    /**
     * @brief Link this target to another target.
     *
     * @param target Target to link.
     * @param visibility Link visibility.
     * @return Reference to this target.
     */
    Target &link(const Target &target,
                 Visibility visibility = Visibility::Private);

    /**
     * @brief Link this target to a target by name.
     *
     * @param target_name Linked target name.
     * @param visibility Link visibility.
     * @return Reference to this target.
     */
    Target &link_named(std::string target_name,
                       Visibility visibility = Visibility::Private);

    /**
     * @brief Access the target name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Access the target kind.
     */
    [[nodiscard]] TargetKind kind() const noexcept;

    /**
     * @brief Access the requested C++ standard.
     */
    [[nodiscard]] CppStandard cpp_standard() const noexcept;

    /**
     * @brief Access the build profile.
     */
    [[nodiscard]] BuildProfile build_profile() const noexcept;

    /**
     * @brief Access the source set.
     */
    [[nodiscard]] const SourceSet &source_set() const noexcept;

    /**
     * @brief Access target requirements.
     */
    [[nodiscard]] const RequirementSet &requirements() const noexcept;

    /**
     * @brief Access target-to-target links.
     */
    [[nodiscard]] const std::vector<TargetLink> &target_links() const noexcept;

    /**
     * @brief Return true if the target has a non-empty name.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    std::string name_{};
    TargetKind kind_{TargetKind::Executable};
    CppStandard cpp_standard_{CppStandard::Cxx23};
    BuildProfile build_profile_{BuildProfile::Debug};
    SourceSet sources_{};
    RequirementSet requirements_{};
    std::vector<TargetLink> target_links_{};
  };

} // namespace cgride::project

#endif // CGRIDE_PROJECT_TARGET_HPP
