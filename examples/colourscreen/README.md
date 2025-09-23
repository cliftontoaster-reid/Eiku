# Color Gradient Demo

A beautiful demonstration of the Eiku graphics library showcasing real-time gradient rendering using pixel-level drawing operations.

## Overview

This example creates an 800x600 window and fills it with a smooth color gradient where:

- **Red component** increases from left to right
- **Green component** increases from top to bottom
- **Blue component** creates a radial pattern from the center

The result is a stunning RGB gradient that demonstrates the power and precision of the Eiku pixel drawing system.

## Features Demonstrated

- ✅ **Window Creation** - Creating and managing application windows
- ✅ **Pixel Drawing** - Direct pixel manipulation and rendering
- ✅ **Color Calculations** - Mathematical color gradient generation
- ✅ **Real-time Rendering** - Progressive drawing with progress feedback
- ✅ **Resource Management** - Proper initialization and cleanup

## Building and Running

### Prerequisites

Make sure the Eiku library is built first:

```bash
cd ../..  # Go to Eiku root directory
make all  # Build the Eiku library
```

### Build the Demo

```bash
make        # Build the demo
# or
make all    # Same as above
```

### Run the Demo

```bash
make run    # Build and run the demo
```

### Clean Build Files

```bash
make clean  # Remove build artifacts
```

### Get Help

```bash
make help   # Show detailed help information
```

## How It Works

### 1. Initialization

The demo initializes the Eiku graphics system and creates an 800x600 window.

### 2. Gradient Calculation

For each pixel at position (x, y), the color is calculated using:

```c
// Normalize coordinates to 0.0-1.0 range
float norm_x = (float)x / (float)(width - 1);
float norm_y = (float)y / (float)(height - 1);

// Calculate color components
int red = (int)(norm_x * 255.0f);           // Left to right gradient
int green = (int)(norm_y * 255.0f);         // Top to bottom gradient

// Blue: radial gradient from center
float distance = sqrt((norm_x - 0.5)² + (norm_y - 0.5)²);
int blue = (int)((1.0f - distance) * 255.0f);

// Combine into RGB color
int color = (red << 16) | (green << 8) | blue;
```

### 3. Rendering

The demo draws each pixel individually using `eiku_pixel_put()`, showing progress every 50 lines.

### 4. Display

The completed gradient is displayed for 30 seconds, allowing you to appreciate the smooth color transitions.

## Expected Output

When you run the demo, you should see:

1. **Console Output**: Progress messages and completion status
2. **Graphics Window**: A beautiful gradient with:
   - Colors transitioning smoothly across the entire window
   - Red intensity increasing from left to right
   - Green intensity increasing from top to bottom
   - Blue intensity highest at the center, decreasing radially

## Technical Details

- **Window Size**: 800×600 pixels (480,000 individual pixels drawn)
- **Color Format**: 24-bit RGB (0xRRGGBB)
- **Rendering Method**: Direct pixel manipulation via `eiku_pixel_put()`
- **Performance**: Progress feedback every 50 lines for user feedback

## Troubleshooting

### Build Issues

**Error**: `Eiku library not found`

```bash
cd ../.. && make all
```

**Error**: `Cannot find include files`

- Ensure you're running from the `examples/colourscreen/` directory
- Check that the Eiku library was built successfully

### Runtime Issues

**Error**: `Failed to initialize Eiku context`

- Ensure you have X11 display available (not running headless)
- Check that required X11 libraries are installed

**Error**: `Failed to create window`

- Verify display server is running
- Check available memory and display resources

## Learning Outcomes

After running this demo, you'll understand:

1. **Basic Eiku Usage** - How to initialize the library and create windows
2. **Pixel-Level Graphics** - How to draw individual pixels with specific colors
3. **Color Mathematics** - How to calculate smooth gradients programmatically
4. **Resource Management** - Proper cleanup and error handling
5. **Real-time Rendering** - Techniques for providing user feedback during long operations

## Next Steps

Try modifying the gradient calculation in `calculate_gradient_color()` to create different effects:

- **Circular gradients**: Use polar coordinates
- **Wave patterns**: Add sine/cosine functions
- **Multiple colors**: Create more complex color interpolations
- **Animation**: Add time-based color shifts

This example provides a solid foundation for exploring more advanced graphics programming with Eiku!
