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

#ifndef SHADERC_H_
#define SHADERC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

enum shaderc_shader_kind {
  shaderc_glsl_vertex_shader,
  shaderc_glsl_fragment_shader
};

// Usage examples:
//
// Aggressively release compiler resources, but spend time in initialization
// for each new use.
//      shaderc_compiler_t compiler = shaderc_compiler_initialize();
//      shader_spv_module_t module = shaderc_compile_into_spv(compiler,
//                    "int main() {}", 13, shaderc_glsl_vertex_shader, "main");
//      // Do stuff with module compilation results.
//      shaderc_module_release(module);
//      shaderc_compiler_release(compiler);
//
// Keep the compiler object around for a long time, but pay for extra space
// occupied.
//      shaderc_compiler_t compiler = shaderc_compiler_initialize();
//      // On the same, other or multiple simultaneous threads.
//      shader_spv_module_t module = shaderc_compile_into_spv(compiler,
//                    "int main() {}", 13, shaderc_glsl_vertex_shader, "main");
//      // Do stuff with module compilation results.
//      shaderc_module_release(module);
//      // Once no more compilations are to happen.
//      shaderc_compiler_release(compiler);

// An opaque handle to an object that manages all compiler state.
typedef struct shaderc_compiler* shaderc_compiler_t;

// Returns a shaderc_compiler_t that can be used to compile modules.
// A return of NULL indicates that there was an error initializing the compiler.
// Any function operating on shaderc_compiler_t must offer the basic
// thread-safety guarantee.
// [http://herbsutter.com/2014/01/13/gotw-95-solution-thread-safety-and-synchronization/]
// That is: concurrent invocation of these functions on DIFFERENT objects needs
// no synchronization; concurrent invocation of these functions on the SAME
// object requires synchronization IF AND ONLY IF some of them take a non-const
// argument.
shaderc_compiler_t shaderc_compiler_initialize();

// Releases the resources held by the shaderc_compiler_t.
// After this call it is invalid to make any future calls to functions
// involving this shaderc_compiler_t.
void shaderc_compiler_release(shaderc_compiler_t);

// An opaque handle to the results of a call to shaderc_compile_into_spv().
typedef struct shaderc_spv_module* shaderc_spv_module_t;

// Takes a GLSL source string and the associated shader type, compiles it into
// SPIR-V, and returns a shaderc_spv_module that contains the results of the
// compilation.  The entry_point_name null-terminated string
// defines the name of the entry point to associate with this GLSL source.
// May be safely called from multiple threads without explicit synchronization.
// If there was failure in allocating the compiler object NULL will be returned.
shaderc_spv_module_t shaderc_compile_into_spv(const shaderc_compiler_t compiler,
                                              const char* source_text,
                                              int source_text_size,
                                              shaderc_shader_kind shader_kind,
                                              const char* entry_point_name);

// The following functions, operating on shaderc_spv_module_t objects, offer
// only the basic thread-safety guarantee.

// Releases the resources held by module.  It is invalid to use module for any
// further operations.
void shaderc_module_release(shaderc_spv_module_t module);

// Returns true if the result in module was a successful compilation.
bool shaderc_module_get_success(const shaderc_spv_module_t module);

// Returns the number of bytes in a SPIR-V module.
size_t shaderc_module_get_length(const shaderc_spv_module_t module);

// Returns a pointer to the start of the SPIR-V bytes.
// This is guaranteed to be castable to a uint32_t*.
const char* shaderc_module_get_bytes(const shaderc_spv_module_t module);

// Returns a null-terminated string that contains any error messages generated
// during the compilation.
const char* shaderc_module_get_error_message(const shaderc_spv_module_t module);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // SHADERC_H_
