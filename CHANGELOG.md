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

### Changed

- Bootstrap X11 driver core with an API inspired by MiniLibX and add platform visibility macros (e.g. `EIKU_INT`) for internal symbols.
- Add GitHub issue templates, CODEOWNERS, and development tooling (Trunk linting/formatting and VS Code configuration).

### Deprecated

-

### Removed

-

### Fixed

- Correct README header formatting and standardize repository documentation formatting.

### Security

-
