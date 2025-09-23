/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiku example <example@eiku.dev>             +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 by example                   #+#    #+#             */
/*   Updated: 2025/09/23 by example                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Fast Colorscreen Example - High-Performance Gradient Window Demo
 *
 * This example demonstrates how to create a window and fill it with a color
 * gradient using the fast image buffer approach. Instead of drawing pixels
 * directly to the window (slow), we draw to an image buffer and then transfer
 * the entire image to the window at once (fast).
 *
 * Features demonstrated:
 * - High-performance image buffer rendering
 * - eiku_image_pixel_put() for fast pixel writing
 * - eiku_put_image_to_window() for efficient display
 * - Same beautiful gradient as the original, but much faster
 */

#include <core/core.h>
#include <res/image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Eiku Fast Color Gradient Demo"

/**
 * @brief Calculate a gradient color based on position
 *
 * Creates a smooth color transition across the window:
 * - Red component increases from left to right
 * - Green component increases from top to bottom
 * - Blue component creates a radial pattern from center
 *
 * @param x X coordinate (0 to width-1)
 * @param y Y coordinate (0 to height-1)
 * @param width Window width
 * @param height Window height
 * @return RGB color value (0xRRGGBB format)
 */
static int calculate_gradient_color(int x, int y, int width, int height) {
  float norm_x;
  float norm_y;
  float center_x;
  float center_y;
  float dx;
  float dy;
  float max_distance_sq;
  int blue;

  // Normalize coordinates to 0.0 - 1.0 range
  // Use (width-1) and (height-1) to ensure we reach full color range
  // but add a small offset to prevent division by zero
  norm_x = (width > 1) ? (float)x / (float)(width - 1) : 0.0f;
  norm_y = (height > 1) ? (float)y / (float)(height - 1) : 0.0f;
  // Calculate color components with better distribution
  int red = (int)(norm_x * 255.0f);    // Increases left to right (0-255)
  int green = (int)(norm_y * 255.0f);  // Increases top to bottom (0-255)
  // Blue component: radial gradient from center
  center_x = 0.5f;
  center_y = 0.5f;
  dx = norm_x - center_x;
  dy = norm_y - center_y;
  float distance = dx * dx + dy * dy;  // Distance squared from center
  // Convert distance to blue intensity (brightest at center)
  // Maximum distance from center to corner is sqrt(0.5) ≈ 0.707
  // So distance squared max is about 0.5
  max_distance_sq = 0.5f;
  distance = distance > max_distance_sq ? max_distance_sq : distance;
  blue = (int)((max_distance_sq - distance) / max_distance_sq * 255.0f);
  // Ensure components are in valid range [0, 255]
  red = red > 255 ? 255 : (red < 0 ? 0 : red);
  green = green > 255 ? 255 : (green < 0 ? 0 : green);
  blue = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
  // Combine RGB components
  return ((red << 16) | (green << 8) | blue);
}

/**
 * @brief Draw the gradient pattern to the image buffer (fast!)
 *
 * @param image Target image buffer
 */
static void draw_gradient_fast(t_eiku_image *image) {
  int color;
  int total_pixels;
  int pixels_drawn;

  total_pixels = WINDOW_WIDTH * WINDOW_HEIGHT;
  pixels_drawn = 0;
  printf("Drawing gradient pattern to image buffer (%dx%d pixels)...\n",
         WINDOW_WIDTH, WINDOW_HEIGHT);
  // Draw gradient pixel by pixel to image buffer
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      color = calculate_gradient_color(x, y, WINDOW_WIDTH, WINDOW_HEIGHT);
      eiku_image_pixel_put(image, x, y, color);
      pixels_drawn++;
    }
    // Show progress every 100 lines to reduce output
    if (y % 100 == 0) {
      printf("  Progress: %d/%d lines (%.1f%%) - %d pixels drawn\n", y,
             WINDOW_HEIGHT, (float)y / WINDOW_HEIGHT * 100.0f, pixels_drawn);
    }
  }
  printf(
      "✓ Gradient drawing complete!"
      "%d total pixels drawn to image buffer\n",
      total_pixels);
}

/**
 * @brief Main application entry point
 */
int main(void) {
  t_eiku_context *ctx;
  t_eiku_window *window;
  t_eiku_image *image;
  clock_t start_time, end_time;
  double cpu_time_used;

  printf("=== Eiku FAST Color Gradient Demo ===\n");
  printf("Window size: %dx%d pixels\n", WINDOW_WIDTH, WINDOW_HEIGHT);
  printf("Gradient type: Radial RGB transition\n");
  printf("Method: Image buffer + single transfer (FAST!)\n\n");

  // Initialize Eiku
  printf("1. Initializing Eiku...\n");
  ctx = eiku_init();
  if (!ctx) {
    fprintf(stderr, "Error: Failed to initialize Eiku context\n");
    return (1);
  }
  printf("   ✓ Eiku context created successfully\n");

  // Create window
  printf("2. Creating window...\n");
  window = eiku_new_window(ctx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  if (!window) {
    fprintf(stderr, "Error: Failed to create window\n");
    eiku_destroy(ctx);
    return (1);
  }
  printf("   ✓ Window created: %s\n", WINDOW_TITLE);

  // Create image buffer
  printf("3. Creating image buffer...\n");
  image = eiku_new_image(ctx, WINDOW_WIDTH, WINDOW_HEIGHT);
  if (!image) {
    fprintf(stderr, "Error: Failed to create image buffer\n");
    eiku_destroy(ctx);
    return (1);
  }
  printf("   ✓ Image buffer created (%dx%d, %d bpp)\n", image->width,
         image->height, image->bpp);

  // Wait for window to be ready
  printf("4. Preparing to draw...\n");
  sleep(1);

  // Draw the gradient to image buffer and measure time
  printf("5. Drawing gradient pattern...\n");
  start_time = clock();
  draw_gradient_fast(image);
  end_time = clock();
  cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
  printf("   ✓ Image buffer rendering took %.3f seconds\n", cpu_time_used);

  // Transfer image to window (this is the magic!)
  printf("6. Transferring image to window...\n");
  start_time = clock();
  int result = eiku_put_image_to_window(ctx, window, image, 0, 0);
  end_time = clock();
  double transfer_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  if (result != 0) {
    fprintf(stderr, "Error: Failed to transfer image to window: %d\n", result);
    eiku_destroy_image(ctx, image);
    eiku_destroy(ctx);
    return (1);
  }
  printf("   ✓ Image transfer took %.6f seconds\n", transfer_time);
  printf("   ✓ Total rendering time: %.3f seconds\n",
         cpu_time_used + transfer_time);

  // Display completion message
  printf("\n=== Demo Complete ===\n");
  printf("✓ Fast gradient pattern displayed successfully!\n");
  printf("✓ Performance benefits:\n");
  printf("  - Image buffer rendering: Fast pixel access\n");
  printf("  - Single transfer: Minimal X11 calls\n");
  printf("  - No window synchronization per pixel\n");
  printf("✓ Window shows RGB gradient:\n");
  printf("  - Red: increases left to right\n");
  printf("  - Green: increases top to bottom\n");
  printf("  - Blue: radial pattern from center\n");
  printf("\nCompare this speed to the original colourscreen example!\n");
  printf("The window will remain open for 30 seconds...\n");
  printf("Press Ctrl+C to exit early.\n");

  // Keep window open for viewing
  sleep(30);

  // Cleanup
  printf("\n7. Cleaning up...\n");
  eiku_destroy_image(ctx, image);
  eiku_destroy(ctx);
  printf("   ✓ Resources freed\n");

  printf("\nThank you for trying the Eiku FAST Color Gradient Demo!\n");
  printf("Image buffer + eiku_put_image_to_window = HIGH PERFORMANCE! 🚀\n");
  return (0);
}