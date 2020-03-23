workspace(name = "spleeter")

load("@spleeter//third_party:dependencies.bzl", "third_party_dependencies")

third_party_dependencies()

## Load bazel skylib as Submodule
load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

## Load bazel rules as Submodule
load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies", "rules_closure_toolchains")

rules_closure_dependencies()

rules_closure_toolchains()

## Load TensorFlow as Submodule
load("@org_tensorflow//tensorflow:workspace.bzl", "tf_workspace")

tf_workspace()
