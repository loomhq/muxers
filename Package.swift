// swift-tools-version:5.5

import PackageDescription

#if os(iOS)
  let libraryType = Product.Library.LibraryType.dynamic
#else
  let libraryType: Product.Library.LibraryType? = nil
#endif

let package = Package(
  name: "Muxers",
  platforms: [
    // keep in sync with the official Loom device compatibility guide
    // https://support.loom.com
    // /hc/en-us/articles/360002219798-Loom-device-compatibility-
    .macOS(.v10_14),
    .iOS(.v14),
  ],
  products: [
    .library(
      name: "Muxers",
      type: libraryType,
      targets: [
        "TsMuxer",
      ]
    )
  ],
  targets: [
    .target(
      name: "TsMuxer",
      path: "tsmuxer",
      exclude: ["tests", "BUILD.bazel"]
    ),
  ],
  cxxLanguageStandard: .cxx14
)
