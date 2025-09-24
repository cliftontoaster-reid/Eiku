# Fake Mouse Events Demo

This example demonstrates how to use the X11 XTest extension to generate fake mouse events, based on the approach described in this [blog post](https://bharathisubramanian.wordpress.com/2010/04/01/x11-fake-mouse-events-generation-using-xtest/).

## What it does

The demo sends various fake mouse events to demonstrate comprehensive mouse control:

1. **Basic Movement**: Moves cursor to screen center
2. **Relative Movement**: Draws a square pattern using relative coordinates
3. **Button Clicks**: Demonstrates left, middle, and right mouse button clicks
4. **Double-Click**: Shows proper double-click timing
5. **Drag Operations**: Press, drag, and release sequences
6. **Scroll Wheel**: Vertical and horizontal scrolling events
7. **Circle Drawing**: Complex movement pattern creating a circle
8. **Button Combinations**: Multiple buttons pressed simultaneously
9. **Rapid Clicking**: Fast clicking sequences
10. **Pattern Tracing**: Movement combined with clicking to trace patterns

## Building

```bash
make
```

Or manually:

```bash
gcc main.c -o fakemouse_demo -lX11 -lXtst -lXext
```

## Running

```bash
make run
```

Or manually:

```bash
./fakemouse_demo
```

**Important**: This demo will move your mouse cursor and generate clicks! Make sure you're prepared for the cursor to move around your screen and potentially interact with desktop elements.

## Requirements

- X11 development libraries (`libx11-dev`)
- XTest extension library (`libxtst-dev`)
- XTest extension must be enabled on your X11 server (it usually is by default)

## Key Mouse Event Functions

The XTest extension provides powerful mouse event generation:

### Movement Functions

- `XTestFakeMotionEvent()`: Absolute mouse positioning
- `XTestFakeRelativeMotionEvent()`: Relative mouse movement

### Button Functions

- `XTestFakeButtonEvent()`: Mouse button press/release events

### Scroll Wheel

Scroll wheel events are handled as special button events:

- **Button 4**: Scroll up
- **Button 5**: Scroll down
- **Button 6**: Scroll left (horizontal)
- **Button 7**: Scroll right (horizontal)

### Button Mapping

- **Button 1**: Left mouse button
- **Button 2**: Middle mouse button (scroll wheel click)
- **Button 3**: Right mouse button
- **Buttons 4-7**: Scroll wheel directions
- **Button 8+**: Extra mouse buttons

## Use Cases

This approach is particularly useful for:

- **Automated GUI testing**: Simulating user interactions
- **Accessibility tools**: Providing alternative input methods
- **Game automation**: Automated clicking and movement
- **Remote control applications**: Controlling mouse from external sources
- **Presentation tools**: Automated mouse movement for demos
- **Load testing**: Generating high-frequency mouse events

## Advanced Features Demonstrated

- **Complex Patterns**: Circle drawing using trigonometric calculations
- **Timing Control**: Proper delays for double-clicks and smooth movement
- **Button Combinations**: Multiple simultaneous button presses
- **Drag Operations**: Press-move-release sequences
- **Pattern Tracing**: Combining movement with clicking for complex interactions

The demo showcases the full capabilities of XTest mouse event generation, providing a foundation for any application requiring programmatic mouse control.
