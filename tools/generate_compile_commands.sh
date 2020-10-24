#!/bin/bash
INSTALL_DIR="/usr/local/bin"
VERSION="0.4.3"

# Download and symlink.
(
  cd "${INSTALL_DIR}" \
  && curl -L "https://github.com/grailbio/bazel-compilation-database/archive/${VERSION}.tar.gz" | tar -xz \
  && ln -f -s "${INSTALL_DIR}/bazel-compilation-database-${VERSION}/generate.sh" bazel-compdb
)

bazel-compdb # This will generate compile_commands.json in your workspace root.

# # You can tweak some behavior with flags:
# # 1. To use the source dir instead of bazel-execroot for directory in which clang commands are run.
# bazel-compdb -s 
