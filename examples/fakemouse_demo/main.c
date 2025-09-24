/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:15:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/24 16:15:00 by copilot           ###   ########.fr */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Fake mouse event demonstration using XTest extension
 *
 * This demo shows how to use the XTest extension to generate fake mouse
 * events, based on the approach described at:
 * https://bharathisubramanian.wordpress.com/2010/04/01/x11-fake-mouse-events-generation-using-xtest/
 *
 * Build with: gcc main.c -o fakemouse_demo -lX11 -lXtst -lXext
 * Run with: ./fakemouse_demo
 *
 * Note: This requires the XTest extension to be available on your X11 server.
 */

#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Send fake mouse motion event using XTest extension
 * @param display X11 display connection
 * @param x X coordinate (absolute or relative)
 * @param y Y coordinate (absolute or relative)
 * @param relative True for relative movement, false for absolute
 */
static void send_mouse_motion(Display *display, int x, int y, int relative) {
  if (relative) {
    XTestFakeRelativeMotionEvent(display, x, y, 0);
  } else {
    XTestFakeMotionEvent(display, -1, x, y, 0);
  }
  XSync(display, False);
}

/**
 * @brief Send fake mouse button event using XTest extension
 * @param display X11 display connection
 * @param button Button number (1=left, 2=middle, 3=right, 4-7=scroll, 8+=extra)
 * @param is_press True for button press, false for button release
 */
static void send_mouse_button(Display *display, unsigned int button,
                              int is_press) {
  XTestFakeButtonEvent(display, button, is_press ? True : False, 0);
  XSync(display, False);
}

/**
 * @brief Send a complete mouse button click sequence (press + release)
 * @param display X11 display connection
 * @param button Button number to click
 */
static void send_mouse_click(Display *display, unsigned int button) {
  send_mouse_button(display, button, 1);  // Press
  usleep(50000);                          // 50ms delay
  send_mouse_button(display, button, 0);  // Release
  usleep(50000);                          // 50ms delay
}

/**
 * @brief Send scroll wheel event
 * @param display X11 display connection
 * @param direction 1 for up/right, -1 for down/left
 * @param axis 0 for vertical, 1 for horizontal
 */
static void send_scroll(Display *display, int direction, int axis) {
  unsigned int button;

  if (axis == 0) {                     // Vertical scrolling
    button = (direction > 0) ? 4 : 5;  // Button 4 = up, Button 5 = down
  } else {                             // Horizontal scrolling
    button = (direction > 0) ? 7 : 6;  // Button 7 = right, Button 6 = left
  }

  send_mouse_button(display, button, 1);  // Press
  send_mouse_button(display, button, 0);  // Release (immediate for scroll)
  usleep(100000);                         // 100ms delay between scrolls
}

/**
 * @brief Check if XTest extension is available
 * @param display X11 display connection
 * @return 1 if available, 0 otherwise
 */
static int check_xtest_extension(Display *display) {
  int event_base, error_base, major_version, minor_version;

  if (!XTestQueryExtension(display, &event_base, &error_base, &major_version,
                           &minor_version)) {
    printf("XTest extension not available!\n");
    return 0;
  }

  printf("XTest extension available: version %d.%d\n", major_version,
         minor_version);
  return 1;
}

/**
 * @brief Draw a circle with mouse movement
 * @param display X11 display connection
 * @param center_x Center X coordinate
 * @param center_y Center Y coordinate
 * @param radius Circle radius
 */
static void draw_circle_with_mouse(Display *display, int center_x, int center_y,
                                   int radius) {
  const int steps = 36;  // 10-degree increments
  const double angle_step = 2.0 * M_PI / steps;

  printf("Drawing a circle with mouse movement...\n");

  for (int i = 0; i <= steps; i++) {
    double angle = i * angle_step;
    int x = center_x + (int)(radius * cos(angle));
    int y = center_y + (int)(radius * sin(angle));

    send_mouse_motion(display, x, y, 0);  // Absolute positioning
    usleep(50000);                        // 50ms delay between points
  }
}

int main(void) {
  Display *display;

  printf("=== Fake Mouse Events Demo using XTest Extension ===\n\n");

  // Open X11 display
  display = XOpenDisplay(NULL);
  if (!display) {
    printf("Error: Could not open X11 display\n");
    return EXIT_FAILURE;
  }

  printf("Connected to X11 display: %s\n", DisplayString(display));

  // Check if XTest extension is available
  if (!check_xtest_extension(display)) {
    XCloseDisplay(display);
    return EXIT_FAILURE;
  }

  printf("\nNote: This demo will send fake mouse events to the desktop.\n");
  printf("Make sure your cursor is visible and watch for movement!\n");
  printf("Starting in 3 seconds...\n\n");

  sleep(3);

  // Demo 1: Basic mouse movement
  printf("Demo 1: Moving mouse to center of screen...\n");
  send_mouse_motion(display, 400, 300, 0);  // Move to center-ish position
  sleep(1);

  // Demo 2: Relative mouse movements (square pattern)
  printf("Demo 2: Drawing a square with relative movements...\n");
  send_mouse_motion(display, 100, 0, 1);  // Right
  usleep(500000);
  send_mouse_motion(display, 0, 100, 1);  // Down
  usleep(500000);
  send_mouse_motion(display, -100, 0, 1);  // Left
  usleep(500000);
  send_mouse_motion(display, 0, -100, 1);  // Up
  usleep(500000);

  // Demo 3: Mouse button clicks
  printf("Demo 3: Demonstrating mouse button clicks...\n");
  printf("  - Left click\n");
  send_mouse_click(display, 1);  // Left click
  sleep(1);

  printf("  - Right click\n");
  send_mouse_click(display, 3);  // Right click
  sleep(1);

  printf("  - Middle click\n");
  send_mouse_click(display, 2);  // Middle click
  sleep(1);

  // Demo 4: Double-click
  printf("Demo 4: Double-click...\n");
  send_mouse_click(display, 1);  // First click
  usleep(100000);                // Short delay for double-click timing
  send_mouse_click(display, 1);  // Second click
  sleep(1);

  // Demo 5: Drag operation (press, move, release)
  printf("Demo 5: Drag operation...\n");
  send_mouse_button(display, 1, 1);  // Press left button
  usleep(100000);

  send_mouse_motion(display, 50, 50, 1);  // Drag while holding
  usleep(200000);
  send_mouse_motion(display, -25, 25, 1);  // Continue dragging
  usleep(200000);

  send_mouse_button(display, 1, 0);  // Release left button
  sleep(1);

  // Demo 6: Scroll wheel events
  printf("Demo 6: Scroll wheel events...\n");
  printf("  - Scroll up\n");
  send_scroll(display, 1, 0);  // Scroll up
  send_scroll(display, 1, 0);  // Scroll up again

  printf("  - Scroll down\n");
  send_scroll(display, -1, 0);  // Scroll down
  send_scroll(display, -1, 0);  // Scroll down again

  printf("  - Horizontal scroll (if supported)\n");
  send_scroll(display, 1, 1);   // Scroll right
  send_scroll(display, -1, 1);  // Scroll left

  // Demo 7: Circle drawing with mouse
  printf("Demo 7: Drawing a circle...\n");
  draw_circle_with_mouse(display, 400, 300, 80);
  sleep(1);

  // Demo 8: Multiple button combinations
  printf("Demo 8: Multiple button press combinations...\n");
  printf("  - Left + Right button combination\n");
  send_mouse_button(display, 1, 1);  // Press left
  send_mouse_button(display, 3, 1);  // Press right
  usleep(500000);
  send_mouse_button(display, 1, 0);  // Release left
  send_mouse_button(display, 3, 0);  // Release right
  sleep(1);

  // Demo 9: Rapid clicking (simulating fast clicking)
  printf("Demo 9: Rapid clicking sequence...\n");
  for (int i = 0; i < 5; i++) {
    send_mouse_click(display, 1);
    usleep(100000);  // 100ms between clicks
  }
  sleep(1);

  // Demo 10: Movement with clicking (trace a pattern)
  printf("Demo 10: Tracing pattern with clicks...\n");
  int positions[][2] = {{300, 250}, {350, 250}, {400, 250},
                        {400, 300}, {400, 350}, {350, 350},
                        {300, 350}, {300, 300}, {300, 250}};

  for (int i = 0; i < 9; i++) {
    send_mouse_motion(display, positions[i][0], positions[i][1], 0);
    usleep(200000);
    send_mouse_click(display, 1);  // Click at each position
  }

  printf("\nDemo completed! All fake mouse events have been sent.\n");
  printf("You should have seen:\n");
  printf("- Mouse cursor movements in various patterns\n");
  printf("- Button clicks (may have interacted with desktop elements)\n");
  printf("- Scroll wheel events (may have scrolled windows)\n");
  printf("- Complex movement patterns including circles and traces\n");

  // Clean up
  XCloseDisplay(display);
  return EXIT_SUCCESS;
}