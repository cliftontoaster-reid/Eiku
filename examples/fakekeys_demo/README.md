# Fake Key Events Demo

This example demonstrates how to use the X11 XTest extension to generate fake keyboard events, based on the approach described in this [blog post](https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/).

## What it does

The demo sends various fake keyboard events to the currently focused window:

1. **Simple text**: Types "Hello"
2. **More text**: Adds " World!"
3. **Special keys**: Sends Enter key
4. **Modifier keys**: Types "CAPS" using Shift modifier
5. **Key combinations**: Sends Ctrl+A (select all)
6. **Copy command**: Sends Ctrl+C (copy)
7. **Window switching**: Sends Alt+Tab (this was problematic with the old XSendEvent method)
8. **Function keys**: Sends F1
9. **Arrow keys**: Sends Up, Down, Left, Right sequence

## Building

```bash
make
```

Or manually:

```bash
gcc main.c -o fakekeys_demo -lX11 -lXtst -lXext
```

## Running

```bash
make run
```

Or manually:

```bash
./fakekeys_demo
```

**Important**: Make sure you have a text editor or terminal window focused before running the demo, as the fake key events will be sent to whatever window has focus!

## Requirements

- X11 development libraries (`libx11-dev`)
- XTest extension library (`libxtst-dev`)
- XTest extension must be enabled on your X11 server (it usually is by default)

## Key Concepts

The XTest extension provides a cleaner and more reliable way to generate fake X11 events compared to the older `XSendEvent()` approach. The main advantages are:

1. **Better compatibility**: Works with Alt+Tab and other key combinations that XSendEvent struggles with
2. **Simpler API**: Just call `XTestFakeKeyEvent()` with the keycode and press/release flag
3. **More reliable**: Events are handled as if they came from real hardware

### Core Functions

- `XTestQueryExtension()`: Check if XTest is available
- `XTestFakeKeyEvent()`: Send fake key press/release events
- `XKeysymToKeycode()`: Convert key symbols to hardware keycodes
- `XSync()`: Ensure events are processed immediately

This approach is particularly useful for:

- Automated testing of GUI applications
- Creating virtual keyboards or input devices
- Accessibility tools
- Game automation and macros
