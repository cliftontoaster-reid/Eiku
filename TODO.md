# Eiku Game Engine - Development Roadmap

This document outlines the planned development roadmap for the Eiku game engine, prioritized by importance and dependencies.

## Project Overview

Eiku is a lightweight 3D game engine written in pure C, inspired by early software renderers like the DOOM engine. The goal is to provide a minimal yet extensible foundation for building first-person games and prototypes.

## Current Status (v0.1.0-exp.1)

### ✅ **Implemented Components**

- **Core System**: X11 windowing, context management, basic event handling
- **Platform Layer**: OS/architecture detection, API visibility macros
- **Image System**: Basic image creation, pixel manipulation, window rendering
- **Build System**: Makefile with static/shared library generation, testing framework
- **Example**: Basic color gradient demonstration

### 🚧 **Partially Implemented**

- **Core Events**: Basic hooks exist but need expansion for full input handling
- **Testing**: Framework setup but minimal test coverage

### ❌ **Not Yet Implemented**

- All major subsystems (audio, math, scene, rendering, input, resources)

## Development Roadmap

### Phase 1: Foundation Math Library (High Priority)

> Essential for all 3D operations

#### 1.1 Vector Mathematics

- [x] **Vector2/3/4 structures** (`include/math/`)
  - Basic vector types (int and float variants)
  - Addition, subtraction, multiplication, division
  - Dot product, cross product
  - Length, normalization
  - Distance calculations
- [x] **Vector utility functions**
  - Linear interpolation (lerp)
  - Reflection, refraction
  - Angle calculations

#### 1.2 Matrix Mathematics (Pushed down as optional)

- [ ] **Matrix3x3 and Matrix4x4 structures** (`include/math/matrix.h`)
  - Matrix creation and initialization
  - Matrix multiplication
  - Transpose, determinant, inverse
  - Identity matrix generation
- [ ] **Transformation matrices**
  - Translation, rotation, scaling
  - View matrix (lookAt)
  - Projection matrices (orthographic, perspective)
  - Model-view-projection pipeline

#### 1.3 Quaternions (Pushed down as optional)

- [ ] **Quaternion structure** (`include/math/quaternion.h`)
  - Creation from axis-angle, Euler angles
  - Quaternion multiplication
  - Normalization, conjugate, inverse
  - SLERP (spherical linear interpolation)
  - Conversion to/from rotation matrices

#### 1.4 Transform System (Pushed down as optional)

- [ ] **Transform structure** (`include/math/transform.h`)
  - Position, rotation, scale components
  - Local-to-world matrix generation
  - Parent-child transform hierarchies
  - Transform composition

### Phase 2: Input System (High Priority)

> Required for interactive applications

#### 2.1 Keyboard Input

- [x] **Keyboard state management** (`include/input/keyboard.h`)
  - Key state tracking (pressed, held, released)
  - Key code definitions and mapping
  - Modifier key support (Ctrl, Alt, Shift)
  - Text input handling

#### 2.2 Mouse Input

- [x] **Mouse state management** (`include/input/mouse.h`)
  - Button state tracking
  - Mouse position and delta tracking
  - Mouse wheel support
  - Mouse capture and relative mode

#### 2.3 Input Manager

- [ ] **Unified input system** (`include/input/input.h`)
  - Input polling and event processing
  - Action mapping system
  - Input buffer management
  - Frame-rate independent input

### Phase 3: Resource Management (Medium Priority)

> Foundation for asset loading

#### 3.1 File System

- [x] **File operations** (`include/res/filesystem.h`)
  - File reading/writing utilities
  - Path manipulation and validation
  - Directory traversal
  - Binary and text file support

#### 3.2 Resource Manager

- [x] **Resource loading system** (`include/res/resource.h`)
  - Generic resource interface
  - Resource caching and lifetime management
  - Reference counting
  - Hot-reloading support (development feature)

#### 3.3 Image Loading

- [ ] **Enhanced image support** (`include/res/image.h`)
  - XPM file format support (already declared)
  - PNG/JPEG loading (via external library)
  - Texture atlas support
  - Image format conversion utilities

### Phase 4: Scene Management (Medium Priority)

> Entity-Component-System architecture

#### 4.1 Entity System

- [ ] **Entity management** (`include/scene/entity.h`)
  - Entity creation and destruction
  - Entity ID system
  - Entity hierarchies (parent-child relationships)
  - Entity tagging and filtering

#### 4.2 Component System

- [ ] **Component architecture** (`include/scene/component.h`)
  - Base component interface
  - Component registration system
  - Component storage and iteration
  - Built-in components (Transform, Renderable, etc.)

#### 4.3 Event System

- [ ] **Event management** (`include/scene/event.h`)
  - Event queue and dispatching
  - Event types and handlers
  - Entity-to-entity messaging
  - System communication

#### 4.4 Scene Graph

- [ ] **Scene management** (`include/scene/scene.h`)
  - Scene loading and unloading
  - Scene switching
  - Scene serialization (save/load)
  - Multiple scene support

### Phase 5: Software Renderer (High Priority)

> Core 3D rendering capabilities

#### 5.1 Renderer Foundation

- [ ] **Renderer architecture** (`include/render/renderer.h`)
  - Renderer context and state
  - Rendering pipeline setup
  - Viewport and camera management
  - Frame buffer management

#### 5.2 Primitive Rendering

- [ ] **2D/3D primitive drawing**
  - Line drawing (Bresenham's algorithm)
  - Triangle rasterization
  - Polygon filling
  - Circle and ellipse drawing

#### 5.3 3D Pipeline

- [ ] **3D rendering pipeline**
  - Vertex transformation (model → world → view → clip → screen)
  - Clipping (frustum and screen-space)
  - Backface culling
  - Z-buffer depth testing
  - Perspective-correct interpolation

#### 5.4 Texture System

- [ ] **Texture support** (`include/render/texture.h`)
  - Texture creation and management
  - Texture sampling and filtering
  - UV mapping
  - Texture atlas support

#### 5.5 Sprite System

- [ ] **2D sprite rendering** (`include/render/sprite.h`)
  - Sprite batching
  - Sprite animation
  - Billboard sprites for 3D
  - Sprite sorting and layering

### Phase 6: Shader System (Medium Priority)

> Programmable rendering pipeline

#### 6.1 Shader Framework

- [ ] **Software shader system** (`include/render/shader.h`)
  - Vertex shader interface
  - Fragment shader interface
  - Shader program management
  - Uniform variable system

#### 6.2 Built-in Shaders

- [ ] **Standard shader library**
  - Unlit/flat shading
  - Basic Phong lighting
  - Texture mapping shaders
  - Debug visualization shaders

### Phase 7: Audio System (Lower Priority)

> Sound and music support

#### 7.1 Audio Foundation

- [ ] **Audio context** (`include/audio/audio.h`)
  - Audio device initialization
  - Sample rate and format management
  - Audio buffer management
  - Platform-specific audio backend (ALSA/PulseAudio)

#### 7.2 Sound System

- [ ] **Sound playback** (`include/audio/sound.h`)
  - Sound effect loading (WAV, OGG)
  - Sound instance management
  - Volume and pitch control
  - 3D positional audio

#### 7.3 Audio Mixer

- [ ] **Audio mixing** (`include/audio/mixer.h`)
  - Multi-channel mixing
  - Real-time audio processing
  - Audio streaming
  - Music playback support

#### 7.4 Audio Manager

- [ ] **High-level audio API** (`include/res/audio_manager.h`)
  - Audio resource management
  - Audio groups and categories
  - Dynamic loading/unloading
  - Audio settings management

### Phase 8: Advanced Features (Future)

> Extended capabilities

#### 8.1 Algorithms

- [ ] **Utility algorithms** (`include/math/algos.h`)
  - Collision detection (AABB, sphere, ray-casting)
  - Spatial partitioning (octree, BSP tree)
  - Path-finding algorithms
  - Noise generation (Perlin, simplex)

#### 8.2 Bounding Volumes

- [ ] **Collision primitives** (`include/math/bounding_box.h`)
  - AABB (Axis-Aligned Bounding Box)
  - OBB (Oriented Bounding Box)
  - Bounding spheres
  - Frustum structures

#### 8.3 Performance Tools

- [ ] **Profiling and debugging**
  - Frame time measurement
  - Memory usage tracking
  - Render statistics
  - Debug visualization

### Phase 9: Platform Expansion (Future)

> Multi-platform support

#### 9.1 Windows Support

- [ ] **Win32 backend**
  - Window creation and management
  - Input handling
  - Audio backend (DirectSound/WASAPI)

#### 9.2 macOS Support

- [ ] **Cocoa backend**
  - NSWindow integration
  - Metal rendering backend (optional)
  - CoreAudio integration

## Implementation Guidelines

### Code Quality Standards

- Follow existing code style (42 School header format)
- Comprehensive error handling
- Memory leak prevention
- Unit tests for all major components
- Documentation for public APIs

### Performance Considerations

- Fixed-point math where appropriate for consistency
- Cache-friendly data structures
- Minimize memory allocations in hot paths
- Profile-guided optimization

### API Design Principles

- Consistent naming conventions (`eiku_*` prefix)
- Minimal dependencies between modules
- Clear separation of concerns
- Backward compatibility within major versions

## Examples and Documentation

### Planned Examples

- [ ] **Basic 3D scene** - Spinning cube with basic lighting
- [ ] **First-person camera** - WASD movement and mouse look
- [ ] **Sprite rendering** - 2D sprite animation demo
- [ ] **Audio demo** - Sound effects and music playback
- [ ] **Game prototype** - Simple FPS-style game demo

### Documentation Tasks

- [ ] API reference documentation
- [ ] Getting started tutorials
- [ ] Architecture overview
- [ ] Performance best practices
- [ ] Platform-specific build instructions

## Testing Strategy

### Unit Testing

- Math library functions (vectors, matrices, quaternions)
- Resource loading and management
- Scene graph operations
- Memory management

### Integration Testing

- Rendering pipeline
- Input handling
- Audio playback
- Cross-platform compatibility

### Performance Testing

- Rendering benchmarks
- Memory usage profiling
- Audio latency testing

---

**Note**: This roadmap is subject to change based on community feedback, technical challenges, and evolving requirements. The priority levels indicate the recommended order of implementation but can be adjusted based on specific project needs.
