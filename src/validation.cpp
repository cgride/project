/**
 *
 *  @file validation.cpp
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
#include <cgride/project/validation.hpp>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cgride::project
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    enum class VisitState
    {
      Visiting,
      Visited
    };

    void validate_target_sources(const Target &target,
                                 std::vector<Error> &errors)
    {
      if (is_interface(target.kind()))
      {
        return;
      }

      if (target.source_set().empty())
      {
        errors.emplace_back(
            ErrorCode::InvalidArgument,
            "Target has no sources.",
            std::string(target.name()));
        return;
      }

      for (const auto &entry : target.source_set().entries())
      {
        if (!entry.valid())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Target contains an invalid source entry.",
              std::string(target.name()));
        }
      }
    }

    void validate_target_requirements(const Target &target,
                                      std::vector<Error> &errors)
    {
      for (const auto &requirement : target.requirements().entries())
      {
        if (!requirement.valid())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Target contains an invalid requirement.",
              std::string(target.name()));
        }
      }
    }

    void validate_target_links(const Project &project,
                               const Target &target,
                               std::vector<Error> &errors)
    {
      for (const auto &link : target.target_links())
      {
        if (!link.valid())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Target contains an invalid target link.",
              std::string(target.name()));
          continue;
        }

        if (link.target_name() == target.name())
        {
          errors.emplace_back(
              ErrorCode::InvalidArgument,
              "Target cannot link to itself.",
              std::string(target.name()));
          continue;
        }

        if (!project.has_target(link.target_name()))
        {
          errors.emplace_back(
              ErrorCode::NotFound,
              "Target link refers to an unknown target.",
              link.target_name());
        }
      }
    }

    bool visit_target_for_cycle(
        const Project &project,
        const Target &target,
        std::unordered_map<std::string, VisitState> &states,
        std::vector<std::string> &stack,
        std::vector<Error> &errors)
    {
      const auto current = target.name();

      auto state = states.find(current);

      if (state != states.end() && state->second == VisitState::Visiting)
      {
        stack.push_back(current);

        std::string cycle;

        for (std::size_t index = 0; index < stack.size(); ++index)
        {
          if (index > 0)
          {
            cycle += " -> ";
          }

          cycle += stack[index];
        }

        errors.emplace_back(
            ErrorCode::InvalidState,
            "Target dependency cycle detected.",
            cycle);

        stack.pop_back();
        return true;
      }

      if (state != states.end() && state->second == VisitState::Visited)
      {
        return false;
      }

      states[current] = VisitState::Visiting;
      stack.push_back(current);

      for (const auto &link : target.target_links())
      {
        const auto *next = project.find_target(link.target_name());

        if (next == nullptr)
        {
          continue;
        }

        if (visit_target_for_cycle(project, *next, states, stack, errors))
        {
          stack.pop_back();
          states[current] = VisitState::Visited;
          return true;
        }
      }

      stack.pop_back();
      states[current] = VisitState::Visited;

      return false;
    }

    void validate_cycles(const Project &project,
                         std::vector<Error> &errors)
    {
      std::unordered_map<std::string, VisitState> states;
      std::vector<std::string> stack;

      for (const auto &target : project.targets())
      {
        if (!target)
        {
          continue;
        }

        visit_target_for_cycle(project, *target, states, stack, errors);
      }
    }

  } // namespace

  std::vector<cgride::core::Error> validate_project_errors(const Project &project)
  {
    std::vector<Error> errors;

    if (project.empty())
    {
      errors.emplace_back(
          ErrorCode::InvalidArgument,
          "Project has no targets.");
      return errors;
    }

    std::unordered_set<std::string> names;

    for (const auto &target : project.targets())
    {
      if (!target)
      {
        errors.emplace_back(
            ErrorCode::InvalidState,
            "Project contains a null target.");
        continue;
      }

      if (!target->valid())
      {
        errors.emplace_back(
            ErrorCode::InvalidArgument,
            "Project contains a target with an empty name.");
        continue;
      }

      if (!names.insert(target->name()).second)
      {
        errors.emplace_back(
            ErrorCode::InvalidArgument,
            "Project contains duplicate target names.",
            std::string(target->name()));
        continue;
      }

      validate_target_sources(*target, errors);
      validate_target_requirements(*target, errors);
      validate_target_links(project, *target, errors);
    }

    validate_cycles(project, errors);

    return errors;
  }

  cgride::core::Result<void> validate_project(const Project &project)
  {
    const auto errors = validate_project_errors(project);

    if (!errors.empty())
    {
      return errors.front();
    }

    return cgride::core::Result<void>::ok();
  }

} // namespace cgride::project
