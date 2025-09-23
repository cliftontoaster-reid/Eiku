# Eiku Fast Colorscreen Example

This example demonstrates high-performance rendering using the Eiku game engine's image buffer approach.

## Overview

While the original `colourscreen` example draws pixels directly to the window (which requires individual X11 calls for each pixel), this `colourscreen_fast` example uses a much more efficient approach:

1. **Create an image buffer** with `eiku_new_image()`
2. **Draw all pixels to the buffer** using `eiku_image_pixel_put()`
3. **Transfer the entire buffer** to the window with `eiku_put_image_to_window()`

## Performance Benefits

- **Faster rendering**: Writing to memory is much faster than X11 calls
- **Single transfer**: Only one X11 operation instead of 480,000 individual pixel draws
- **Better responsiveness**: No window synchronization delays during drawing
- **Scalable**: Performance improvement increases with larger images

## Building and Running

From the Eiku root directory:

```bash
make examples/colourscreen_fast
./examples/colourscreen_fast/colourscreen_fast
```

Or from this directory:

```bash
make
./colourscreen_fast
```

## What You'll See

The same beautiful RGB gradient as the original example:

- **Red component**: Increases from left to right
- **Green component**: Increases from top to bottom
- **Blue component**: Radial pattern from center

But rendered much faster using the image buffer technique!

## Code Highlights

### Fast Image Buffer Rendering

```c
// Create image buffer
t_eiku_image *image = eiku_new_image(ctx, width, height);

// Draw to buffer (fast!)
for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int color = calculate_gradient_color(x, y, width, height);
        eiku_image_pixel_put(image, x, y, color);  // Memory write
    }
}

// Transfer to window (single operation!)
eiku_put_image_to_window(ctx, window, image, 0, 0);
```

### Performance Comparison

This example includes timing measurements so you can see the performance difference compared to the original approach.

## Technical Details

- **Window Size**: 800x600 pixels (480,000 total pixels)
- **Color Depth**: 32-bit BGRA
- **Rendering Method**: Image buffer + single transfer
- **X11 Calls**: ~10 vs ~480,000 in the direct approach

Perfect for games, animations, and any application requiring high-performance graphics!
