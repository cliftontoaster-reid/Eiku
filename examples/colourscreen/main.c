/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eiku example <example@eiku.dev>             +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 23:30:00 by example           #+#    #+#             */
/*   Updated: 2025/09/22 23:30:00 by example          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Colorscreen Example - Gradient Window Demo
 *
 * This example demonstrates how to create a window and fill it with a color
 * gradient based on pixel position. The gradient transitions smoothly across
 * the window, creating a beautiful color effect.
 *
 * Features demonstrated:
 * - Window creation and management
 * - Pixel-by-pixel drawing
 * - Color calculations and gradients
 * - Real-time rendering
 */

#include <core/core.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Eiku Color Gradient Demo"

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
static int	calculate_gradient_color(int x, int y, int width, int height)
{
	float	norm_x;
	float	norm_y;
	float	center_x;
	float	center_y;
	float	dx;
	float	dy;
	float	max_distance_sq;
	int		blue;

	// Normalize coordinates to 0.0 - 1.0 range
	// Use (width-1) and (height-1) to ensure we reach full color range
	// but add a small offset to prevent division by zero
	norm_x = (width > 1) ? (float)x / (float)(width - 1) : 0.0f;
	norm_y = (height > 1) ? (float)y / (float)(height - 1) : 0.0f;
	// Calculate color components with better distribution
	int red = (int)(norm_x * 255.0f);   // Increases left to right (0-255)
	int green = (int)(norm_y * 255.0f); // Increases top to bottom (0-255)
	// Blue component: radial gradient from center
	center_x = 0.5f;
	center_y = 0.5f;
	dx = norm_x - center_x;
	dy = norm_y - center_y;
	float distance = dx * dx + dy * dy; // Distance squared from center
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
 * @brief Draw the gradient pattern to the window
 *
 * @param ctx Eiku context
 * @param window Target window
 */
static void	draw_gradient(t_eiku_context *ctx, t_eiku_window *window)
{
	int	color;

	printf("Drawing gradient pattern (%dx%d pixels)...\n", WINDOW_WIDTH,
		WINDOW_HEIGHT);
	// Debug: Test drawing at specific coordinates to check for offset
	printf("Debug - Testing pixel placement:\n");
	printf("  Drawing red pixel at (0,0)\n");
	eiku_pixel_put(ctx, window, 0, 0, 0xFF0000); // Red at top-left
	printf("  Drawing green pixel at (1,0)\n");
	eiku_pixel_put(ctx, window, 1, 0, 0x00FF00); // Green next to it
	printf("  Drawing blue pixel at (0,1)\n");
	eiku_pixel_put(ctx, window, 0, 1, 0x0000FF); // Blue below red
	printf("  Drawing white pixel at (10,10)\n");
	eiku_pixel_put(ctx, window, 10, 10, 0xFFFFFF); // White marker
	printf("  Sleeping 2 seconds to check placement...\n");
	sleep(2);
	// Draw gradient pixel by pixel
	for (int y = 0; y < WINDOW_HEIGHT; y++)
	{
		for (int x = 0; x < WINDOW_WIDTH; x++)
		{
			color = calculate_gradient_color(x, y, WINDOW_WIDTH, WINDOW_HEIGHT);
			eiku_pixel_put(ctx, window, x, y, color);
		}
		// Show progress every 100 lines to reduce output
		if (y % 100 == 0)
		{
			printf("  Progress: %d/%d lines (%.1f%%)\n", y, WINDOW_HEIGHT,
				(float)y / WINDOW_HEIGHT * 100.0f);
		}
	}
	printf("✓ Gradient drawing complete!\n");
}

/**
 * @brief Main application entry point
 */
int	main(void)
{
	t_eiku_context *ctx;
	t_eiku_window *window;

	printf("=== Eiku Color Gradient Demo ===\n");
	printf("Window size: %dx%d pixels\n", WINDOW_WIDTH, WINDOW_HEIGHT);
	printf("Gradient type: Radial RGB transition\n\n");

	// Initialize Eiku
	printf("1. Initializing Eiku...\n");
	ctx = eiku_init();
	if (!ctx)
	{
		fprintf(stderr, "Error: Failed to initialize Eiku context\n");
		return (1);
	}
	printf("   ✓ Eiku context created successfully\n");

	// Create window
	printf("2. Creating window...\n");
	window = eiku_new_window(ctx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	if (!window)
	{
		fprintf(stderr, "Error: Failed to create window\n");
		eiku_destroy(ctx);
		return (1);
	}
	printf("   ✓ Window created: %s\n", WINDOW_TITLE);

	// Wait a moment for window to be ready
	printf("3. Preparing to draw...\n");
	sleep(1);

	// Draw the gradient
	printf("4. Drawing gradient pattern...\n");
	draw_gradient(ctx, window);

	// Display completion message
	printf("\n=== Demo Complete ===\n");
	printf("✓ Gradient pattern displayed successfully!\n");
	printf("✓ Window shows RGB gradient:\n");
	printf("  - Red: increases left to right\n");
	printf("  - Green: increases top to bottom\n");
	printf("  - Blue: radial pattern from center\n");
	printf("\nThe window will remain open for 30 seconds...\n");
	printf("Press Ctrl+C to exit early.\n");

	// Keep window open for viewing
	sleep(30);

	// Cleanup
	printf("\n5. Cleaning up...\n");
	eiku_destroy(ctx);
	printf("   ✓ Resources freed\n");

	printf("\nThank you for trying the Eiku Color Gradient Demo!\n");
	return (0);
}