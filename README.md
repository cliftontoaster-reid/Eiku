# Eiku

Eiku is a lightweight 3D game engine written in pure C. Inspired by early software renderers such as the DOOM engine, Eiku aims to provide a minimal yet extensible foundation for building first-person games and prototypes.

-## Features

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

## Licence

This project is licensed under the MIT Licence. See [LICENCE.md](LICENCE.md) for details.
