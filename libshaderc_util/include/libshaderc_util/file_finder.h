// Copyright 2015 The Shaderc Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIBSHADERC_UTIL_SRC_FILE_FINDER_H_
#define LIBSHADERC_UTIL_SRC_FILE_FINDER_H_

#include <string>
#include <vector>

namespace shaderc_util {

// Finds files within a search path.
class FileFinder {
 public:
  // Searches for a read-openable file based on filename, which must be
  // non-empty.  The search is attempted on filename prefixed by each element of
  // search_path() in turn.  The first hit is returned, or an empty string if
  // there are no hits.  Search attempts treat their argument the way
  // std::fopen() treats its filename argument, blind to whether the path is
  // absolute or relative.
  //
  // If a search_path() element is non-empty and not ending in a slash, then a
  // slash is inserted between it and filename before its search attempt. An
  // empty string in search_path() means that the filename is tried as-is.
  //
  // Usage advice: when searching #include files, you almost certainly want ""
  // to be the first element in search_path().  That way both relative and
  // absolute filenames will work as expected.  Note that a "." entry on the
  // search path may be prepended to an absolute filename (eg, "/a/b/c") to
  // create a relative result (eg, ".//a/b/c").
  std::string FindReadableFilepath(const std::string& filename) const;

  // Search path for Find().  Users may add/remove elements as desired.
  std::vector<std::string>& search_path() { return search_path_; }

 private:
  std::vector<std::string> search_path_;
};

}  // namespace shaderc_util

#endif  // LIBSHADERC_UTIL_SRC_FILE_FINDER_H_
