# Eiku

Eiku is a lightweight 3D game engine written in pure C. Inspired by early software renderers such as the DOOM engine, Eiku aims to provide a minimal yet extensible foundation for building first-person games and prototypes.

## Features

- Software rendering pipeline with fixed-point math
- Basic 3D scene management (entities, components, events)
- Audio playback via mixer
- Input handling for keyboard and mouse
- Resource loading and management

## Getting Started

1. Clone the repository:

   ```sh
   git clone https://github.com/your-repo/eiku.git
   cd eiku
   ```

2. Build the engine:

   ```sh
   make
   ```

3. Link against the static library or load the shared object in your project:

   ```sh
   cc main.c -Iinclude -Ltarget/x86_64-linux-gnu/eiku/lib -leiku -o mygame
   ```

## Current Status

This project is in very early development. Core subsystems are being designed, and documentation/examples are not expected to come soon. Expect breaking changes as the API stabilizes.

## Contributing

Contributions and feedback are welcome! Please open issues or pull requests to propose features and fixes.

## Acknowledgments

### MiniLibX Inspiration

The X11 driver implementation in Eiku's core module is inspired by the excellent [MiniLibX (MLX)](https://github.com/42Paris/minilibx-linux) library developed by Olivier Crouzet and contributors at 42 School. While Eiku's implementation is original code written from scratch, we acknowledge:

- The MiniLibX team for the clean API design and effective X11 integration patterns
- The 42 School community for foundational graphics programming concepts
- The original MiniLibX authors for demonstrating elegant approaches to cross-platform graphics

**Important Note**: While inspired by MiniLibX's API design, Eiku's X11 driver is an independent implementation licensed under LGPL v3.0. No MiniLibX source code was copied or derived.

### Other Acknowledgments

- The DOOM engine for inspiring the software rendering approach
- The open-source community for X11 documentation and examples

## Licence

This project is licensed under the GNU Lesser General Public License v3.0. See [LICENCE.md](LICENCE.md) for details.
