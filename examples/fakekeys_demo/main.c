/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:45:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/24 15:45:00 by copilot           ###   ########.fr */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file main.c
 * @brief Fake key event demonstration using XTest extension
 *
 * This demo shows how to use the XTest extension to generate fake keyboard
 * events, based on the approach described at:
 * https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/
 *
 * Build with: gcc main.c -o fakekeys_demo -lX11 -lXtst -lXext
 * Run with: ./fakekeys_demo
 *
 * Note: This requires the XTest extension to be available on your X11 server.
 */

#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Send fake key event using XTest extension
 * @param display X11 display connection
 * @param keysym Key symbol to send
 * @param modsym Modifier key symbol (or NoSymbol for none)
 * @param is_press True for key press, false for key release
 */
static void send_key(Display *display, KeySym keysym, KeySym modsym,
                     int is_press) {
  KeyCode keycode = 0, modcode = 0;

  keycode = XKeysymToKeycode(display, keysym);
  if (keycode == 0) return;

  if (modsym != NoSymbol) {
    modcode = XKeysymToKeycode(display, modsym);
    if (modcode == 0) return;

    if (is_press) {
      XTestFakeKeyEvent(display, modcode, True, 0);
    }
  }

  XTestFakeKeyEvent(display, keycode, is_press ? True : False, 0);

  if (modsym != NoSymbol && !is_press) {
    XTestFakeKeyEvent(display, modcode, False, 0);
  }

  XSync(display, False);
}

/**
 * @brief Send a complete key press and release sequence
 * @param display X11 display connection
 * @param keysym Key symbol to send
 * @param modsym Modifier key symbol (or NoSymbol for none)
 */
static void send_key_sequence(Display *display, KeySym keysym, KeySym modsym) {
  send_key(display, keysym, modsym, 1);  // Press
  usleep(50000);                         // 50ms delay
  send_key(display, keysym, modsym, 0);  // Release
  usleep(50000);                         // 50ms delay
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

int main(void) {
  Display *display;

  printf("=== Fake Key Events Demo using XTest Extension ===\n\n");

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

  printf(
      "\nNote: This demo will send fake key events to the focused window.\n");
  printf("Make sure you have a text editor or terminal focused!\n");
  printf("Starting in 3 seconds...\n\n");

  sleep(3);

  // Demo 1: Send simple text "Hello"
  printf("Demo 1: Sending 'Hello'...\n");
  KeySym hello_keys[] = {XK_H, XK_e, XK_l, XK_l, XK_o};
  for (int i = 0; i < 5; i++) {
    send_key_sequence(display, hello_keys[i], NoSymbol);
  }

  sleep(1);

  // Demo 2: Send space and "World!"
  printf("Demo 2: Sending ' World!'...\n");
  send_key_sequence(display, XK_space, NoSymbol);
  KeySym world_keys[] = {XK_W, XK_o, XK_r, XK_l, XK_d, XK_exclam};
  for (int i = 0; i < 6; i++) {
    send_key_sequence(display, world_keys[i], NoSymbol);
  }

  sleep(1);

  // Demo 3: Send Enter key
  printf("Demo 3: Sending Enter key...\n");
  send_key_sequence(display, XK_Return, NoSymbol);

  sleep(1);

  // Demo 4: Send text with Shift modifier (uppercase)
  printf("Demo 4: Sending 'CAPS' with Shift modifier...\n");
  KeySym caps_keys[] = {XK_c, XK_a, XK_p, XK_s};
  for (int i = 0; i < 4; i++) {
    send_key_sequence(display, caps_keys[i], XK_Shift_L);
  }

  sleep(1);

  // Demo 5: Send Ctrl+A (select all)
  printf("Demo 5: Sending Ctrl+A (select all)...\n");
  send_key_sequence(display, XK_a, XK_Control_L);

  sleep(1);

  // Demo 6: Send Ctrl+C (copy)
  printf("Demo 6: Sending Ctrl+C (copy)...\n");
  send_key_sequence(display, XK_c, XK_Control_L);

  sleep(1);

  // Demo 7: Send Alt+Tab (this was problematic with XSendEvent)
  printf("Demo 7: Sending Alt+Tab (window switch)...\n");
  send_key_sequence(display, XK_Tab, XK_Alt_L);

  sleep(1);

  // Demo 8: Send function key F1
  printf("Demo 8: Sending F1 key...\n");
  send_key_sequence(display, XK_F1, NoSymbol);

  sleep(1);

  // Demo 9: Send arrow keys
  printf("Demo 9: Sending arrow key sequence (Up, Down, Left, Right)...\n");
  KeySym arrow_keys[] = {XK_Up, XK_Down, XK_Left, XK_Right};
  for (int i = 0; i < 4; i++) {
    send_key_sequence(display, arrow_keys[i], NoSymbol);
    usleep(200000);  // 200ms delay between arrows
  }

  printf("\nDemo completed! All fake key events have been sent.\n");

  // Clean up
  XCloseDisplay(display);
  return EXIT_SUCCESS;
}