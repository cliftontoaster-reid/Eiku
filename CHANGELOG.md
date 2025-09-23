# Changelog

All notable changes to this project will be documented in this file.

This project adheres to [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
It also adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Implement core X11 driver lifecycle: `eiku_init()` and `eiku_destroy()` to initialize and tear down X11 resources and shared memory (see `src/core/core/eiku_init.c`, `src/core/core/eiku_destroy.c`).
- Add internal X11 helper implementations for visual selection, SHM detection, and RGB conversion (see `src/core/core/eiku_int_get_visual.c`, `src/core/core/eiku_int_deal_shm.c`, `src/core/core/eiku_int_rgb_conversion.c`).
- Integrate the Criterion testing framework and add Makefile targets to download, build, and run unit tests (`criterion` target, `test` / `run_tests`).
- Support conventional shared library naming (`libeiku.so`) and update build/linker flags for X11 (`-lX11 -lXext`).
- Add comprehensive development roadmap in `TODO.md` with detailed implementation phases covering math library, input system, resource management, scene management, software renderer, shader system, audio system, and advanced features.
- Implement window creation and management with `eiku_new_window()` and `eiku_window_set_title()` functions.
- Add complete image module with X11 shared memory support (`src/res/image/`) including image creation, destruction, pixel manipulation, and window rendering.
- Create comprehensive example system with colourscreen demo showcasing window creation, pixel-level drawing, and RGB gradient generation.
- Implement comprehensive GitHub Actions CI/CD pipeline with multi-compiler support, automated testing, static analysis, security scanning, and automated releases.
- Add extensive test suite covering core initialization, window management, image operations, error handling, and integration scenarios.
- Add CodeQL security analysis workflow with advanced C/C++ security scanning and dependency review.
- **Implement complete vector mathematics library** (`include/math/vec.h`, `src/math/vec/`) with 2D/3D/4D vector types (int and float variants) supporting arithmetic operations, geometric calculations, linear interpolation, and utility functions - fulfilling Phase 1.1 of the development roadmap.
- **Add professional build system enhancements** with debug/release build modes, compiler optimization (zig cc > clang > gcc priority), mold linker support, security hardening (\_FORTIFY_SOURCE, relro, now), and comprehensive sanitizers (address, undefined behavior).

### Changed

- Bootstrap X11 driver core with an API inspired by MiniLibX and add platform visibility macros (e.g. `EIKU_INT`) for internal symbols.
- Add GitHub issue templates, CODEOWNERS, and development tooling (Trunk linting/formatting and VS Code configuration).
- Refactor image functionality from core module into dedicated `res/image.h` module for better code organization.
- Improve build system with comprehensive .gitignore, examples target, and enhanced Makefile structure.
- Update CI workflow to use Ubuntu 22.04 and fix examples build target.
- Enhance window creation to use borderless windows for cleaner appearance.

### Deprecated

-

### Removed

-

### Fixed

- Correct README header formatting and standardize repository documentation formatting.
- Fix syntax errors in test assertion messages across all test files.
- Add compile-time error for unsupported platforms in `eiku_init.c`.
- Update issue template formatting and fix CI/CD workflow permissions.
- Correct examples build target in CI and add missing Makefile target.
- **Fix alpha channel handling in 32-bit X11 visuals** to ensure proper opaque image rendering across different display configurations.
- **Remove duplicate return statement** in `eiku_pixel_put()` function that caused unreachable code.
- **Update core initialization tests** to handle flexible colormap selection based on X11 visual capabilities.

### Security

-
