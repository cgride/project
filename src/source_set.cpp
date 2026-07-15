/**
 *
 *  @file source_set.cpp
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
#include <cgride/project/source_set.hpp>

namespace cgride::project
{
  std::string_view to_string(SourceKind kind) noexcept
  {
    switch (kind)
    {
    case SourceKind::File:
      return "File";

    case SourceKind::Pattern:
      return "Pattern";
    }

    return "File";
  }

  SourceEntry SourceEntry::file(std::filesystem::path path)
  {
    SourceEntry entry;
    entry.kind_ = SourceKind::File;
    entry.path_ = std::move(path);
    return entry;
  }

  SourceEntry SourceEntry::pattern(std::string pattern)
  {
    SourceEntry entry;
    entry.kind_ = SourceKind::Pattern;
    entry.pattern_ = std::move(pattern);
    return entry;
  }

  SourceKind SourceEntry::kind() const noexcept
  {
    return kind_;
  }

  const std::filesystem::path &SourceEntry::path() const noexcept
  {
    return path_;
  }

  const std::string &SourceEntry::pattern() const noexcept
  {
    return pattern_;
  }

  bool SourceEntry::valid() const noexcept
  {
    switch (kind_)
    {
    case SourceKind::File:
      return !path_.empty();

    case SourceKind::Pattern:
      return !pattern_.empty();
    }

    return false;
  }

  SourceSet &SourceSet::file(std::filesystem::path path)
  {
    entries_.push_back(SourceEntry::file(std::move(path)));
    return *this;
  }

  SourceSet &SourceSet::files(std::vector<std::filesystem::path> paths)
  {
    for (auto &path : paths)
    {
      file(std::move(path));
    }

    return *this;
  }

  SourceSet &SourceSet::pattern(std::string pattern)
  {
    entries_.push_back(SourceEntry::pattern(std::move(pattern)));
    return *this;
  }

  SourceSet &SourceSet::patterns(std::vector<std::string> patterns)
  {
    for (auto &pattern : patterns)
    {
      this->pattern(std::move(pattern));
    }

    return *this;
  }

  const std::vector<SourceEntry> &SourceSet::entries() const noexcept
  {
    return entries_;
  }

  bool SourceSet::empty() const noexcept
  {
    return entries_.empty();
  }

  std::size_t SourceSet::size() const noexcept
  {
    return entries_.size();
  }

  void SourceSet::clear() noexcept
  {
    entries_.clear();
  }

} // namespace cgride::project
