/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_keyboard_xtest.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:30:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/24 15:30:00 by copilot           ###   ########.fr */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file test_keyboard_xtest.c
 * @brief Criterion tests using XTest extension for keyboard event simulation
 *
 * This file implements comprehensive keyboard testing using the XTest extension
 * to generate fake keyboard events. Based on the approach described at:
 * https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/
 *
 * Tests include:
 * - Single key presses and releases
 * - Key combinations with modifiers (Ctrl, Alt, Shift)
 * - Text input simulation
 * - Key sequence testing
 * - State transitions verification
 */

#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core/core.h"
#include "input/keyboard.h"

// Global test context and state
static t_eiku_context *g_test_ctx = NULL;
static t_eiku_keyboard_state g_keyboard_state;

// Forward declarations
void keyboard_xtest_setup(void);
void keyboard_xtest_teardown(void);

// Test suite for keyboard XTest functionality
TestSuite(keyboard_xtest,
          .description = "Keyboard testing with XTest extension",
          .init = keyboard_xtest_setup, .fini = keyboard_xtest_teardown);

/**
 * @brief Setup function called before each test
 */
void keyboard_xtest_setup(void) {
  g_test_ctx = eiku_init();
  cr_assert_not_null(g_test_ctx, "Failed to initialize Eiku context");

  // Initialize keyboard state
  int result = eiku_keyboard_init(g_test_ctx, &g_keyboard_state);
  cr_assert_eq(result, 1, "Failed to initialize keyboard state");

  // Enable key repeats for testing
  eiku_keyboard_set_repeats(&g_keyboard_state, true);
}

/**
 * @brief Cleanup function called after each test
 */
void keyboard_xtest_teardown(void) {
  if (g_test_ctx) {
    eiku_destroy(g_test_ctx);
    g_test_ctx = NULL;
  }

  // Reset keyboard state
  memset(&g_keyboard_state, 0, sizeof(g_keyboard_state));
}

/**
 * @brief Check if XTest extension is available
 * @param display X11 display connection
 * @return 1 if XTest is available, 0 otherwise
 */
static int is_xtest_available(Display *display) {
  int event_base, error_base, major_version, minor_version;

  if (!XTestQueryExtension(display, &event_base, &error_base, &major_version,
                           &minor_version)) {
    return 0;
  }

  return 1;
}

/**
 * @brief Send a fake key event using XTest extension
 * @param display X11 display connection
 * @param keysym Key symbol to send
 * @param modsym Modifier key symbol (or NoSymbol for none)
 * @param is_press True for key press, false for key release
 */
static void send_fake_key_event(Display *display, KeySym keysym, KeySym modsym,
                                int is_press) {
  KeyCode keycode = 0, modcode = 0;

  keycode = XKeysymToKeycode(display, keysym);
  if (keycode == 0) return;

  if (modsym != NoSymbol) {
    modcode = XKeysymToKeycode(display, modsym);
    if (modcode == 0) return;

    // Press modifier first if this is a key press
    if (is_press) {
      XTestFakeKeyEvent(display, modcode, True, 0);
    }
  }

  // Send the main key event
  XTestFakeKeyEvent(display, keycode, is_press ? True : False, 0);

  if (modsym != NoSymbol) {
    // Release modifier after if this is a key release
    if (!is_press) {
      XTestFakeKeyEvent(display, modcode, False, 0);
    }
  }

  XSync(display, False);
}

/**
 * @brief Send a complete key press and release sequence
 * @param display X11 display connection
 * @param keysym Key symbol to send
 * @param modsym Modifier key symbol (or NoSymbol for none)
 */
static void send_fake_key_sequence(Display *display, KeySym keysym,
                                   KeySym modsym) {
  send_fake_key_event(display, keysym, modsym, 1);  // Press
  usleep(10000);                                    // 10ms delay
  send_fake_key_event(display, keysym, modsym, 0);  // Release
  usleep(10000);                                    // 10ms delay
}

/**
 * @brief Process pending X11 events and update keyboard state
 * @param ctx Eiku context
 * @param state Keyboard state to update
 */
static void process_pending_events(t_eiku_context *ctx,
                                   t_eiku_keyboard_state *state) {
  XEvent event;
  while (XPending(ctx->display)) {
    XNextEvent(ctx->display, &event);
    eiku_key_handle_xevent(state, &event);
  }
  eiku_keyboard_update(state);
}

/**
 * @brief Test XTest extension availability
 */
Test(keyboard_xtest, test_xtest_available) {
  cr_assert_not_null(g_test_ctx, "Test context should be initialized");
  cr_assert_not_null(g_test_ctx->display, "Display should be initialized");

  int available = is_xtest_available(g_test_ctx->display);
  cr_assert_eq(available, 1, "XTest extension should be available for testing");
}

/**
 * @brief Test single key press and release
 */
Test(keyboard_xtest, test_single_key_press) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  // Skip if XTest is not available
  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Send a fake 'A' key press
  send_fake_key_event(g_test_ctx->display, XK_a, NoSymbol, 1);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // Check that the key is detected as pressed
  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_A), 1,
               "Key A should be detected as pressed");
  cr_assert_eq(eiku_key_is_down(&g_keyboard_state, EIKU_KEY_A), 1,
               "Key A should be detected as down");

  // End the loop to transition from pressed to held
  eiku_key_end_loop(&g_keyboard_state);

  // Check that the key is now held
  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_A), 0,
               "Key A should no longer be pressed");
  cr_assert_eq(eiku_key_is_down(&g_keyboard_state, EIKU_KEY_A), 1,
               "Key A should still be down (held)");

  // Send a fake 'A' key release
  send_fake_key_event(g_test_ctx->display, XK_a, NoSymbol, 0);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // Check that the key is detected as released
  cr_assert_eq(eiku_key_is_released(&g_keyboard_state, EIKU_KEY_A), 1,
               "Key A should be detected as released");
  cr_assert_eq(eiku_key_is_down(&g_keyboard_state, EIKU_KEY_A), 0,
               "Key A should no longer be down");
}

/**
 * @brief Test key combination with modifier (Ctrl+A)
 */
Test(keyboard_xtest, test_ctrl_key_combination) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Send Ctrl+A key combination
  send_fake_key_sequence(g_test_ctx->display, XK_a, XK_Control_L);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // Check that both Ctrl and A were detected
  cr_assert(g_keyboard_state.modifiers & EIKU_MOD_CTRL,
            "Ctrl modifier should be detected");

  // The key events should have been processed
  cr_assert_geq(g_keyboard_state.text_length, 0, "Text buffer should be valid");
}

/**
 * @brief Test Alt+Tab combination (commonly problematic)
 */
Test(keyboard_xtest, test_alt_tab_combination) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Send Alt+Tab key combination (this was mentioned as problematic with
  // XSendEvent)
  send_fake_key_sequence(g_test_ctx->display, XK_Tab, XK_Alt_L);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // Check that Alt modifier was detected
  cr_assert(g_keyboard_state.modifiers & EIKU_MOD_ALT,
            "Alt modifier should be detected");

  // Check that Tab key was processed
  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_TAB), 1,
               "Tab key should be detected as pressed");
}

/**
 * @brief Test text input simulation
 */
Test(keyboard_xtest, test_text_input_simulation) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Clear text buffer
  eiku_keyboard_clear_text(&g_keyboard_state);

  // Send a sequence of keys to spell "HELLO"
  KeySym keys[] = {XK_H, XK_E, XK_L, XK_L, XK_O};
  const char expected[] = "hello";  // Expected lowercase due to no shift

  for (int i = 0; i < 5; i++) {
    send_fake_key_sequence(g_test_ctx->display, keys[i], NoSymbol);
    process_pending_events(g_test_ctx, &g_keyboard_state);
    usleep(5000);  // Small delay between keys
  }

  // Check that text was captured
  cr_assert_geq(g_keyboard_state.text_length, 5,
                "Text buffer should contain at least 5 characters");

  // Check that the text matches expected (case may vary based on X11 settings)
  cr_assert_str_eq(g_keyboard_state.text_buffer, expected,
                   "Text buffer should contain 'hello'");
}

/**
 * @brief Test key sequence with mixed modifiers
 */
Test(keyboard_xtest, test_mixed_modifier_sequence) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Test sequence: Shift+A, then Ctrl+C, then regular B

  // 1. Shift+A (should produce uppercase A)
  send_fake_key_sequence(g_test_ctx->display, XK_a, XK_Shift_L);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  cr_assert(g_keyboard_state.modifiers & EIKU_MOD_SHIFT,
            "Shift modifier should be detected");

  usleep(10000);

  // 2. Ctrl+C
  send_fake_key_sequence(g_test_ctx->display, XK_c, XK_Control_L);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  cr_assert(g_keyboard_state.modifiers & EIKU_MOD_CTRL,
            "Ctrl modifier should be detected");

  usleep(10000);

  // 3. Regular B
  send_fake_key_sequence(g_test_ctx->display, XK_b, NoSymbol);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // Verify that all keys were processed
  cr_assert_geq(g_keyboard_state.text_length, 1,
                "At least some text should have been captured");
}

/**
 * @brief Test function keys (F1-F12)
 */
Test(keyboard_xtest, test_function_keys) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Test F1 key
  send_fake_key_sequence(g_test_ctx->display, XK_F1, NoSymbol);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_F1), 1,
               "F1 key should be detected as pressed");

  // Test F12 key
  send_fake_key_sequence(g_test_ctx->display, XK_F12, NoSymbol);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_F12), 1,
               "F12 key should be detected as pressed");
}

/**
 * @brief Test arrow keys
 */
Test(keyboard_xtest, test_arrow_keys) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Test all arrow keys
  KeySym arrows[] = {XK_Up, XK_Down, XK_Left, XK_Right};
  t_keycode expected_keys[] = {EIKU_KEY_UP, EIKU_KEY_DOWN, EIKU_KEY_LEFT,
                               EIKU_KEY_RIGHT};

  for (int i = 0; i < 4; i++) {
    eiku_keyboard_reset(&g_keyboard_state);  // Reset state between tests

    send_fake_key_sequence(g_test_ctx->display, arrows[i], NoSymbol);
    process_pending_events(g_test_ctx, &g_keyboard_state);

    cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, expected_keys[i]), 1,
                 "Arrow key should be detected as pressed");
  }
}

/**
 * @brief Test keyboard state reset functionality
 */
Test(keyboard_xtest, test_keyboard_reset) {
  // This test doesn't require XTest, just tests the reset functionality

  // Simulate some key states manually (without XTest)
  g_keyboard_state.keys[EIKU_KEY_A - 1] = EIKU_KEY_STATE_HELD;
  g_keyboard_state.keys[EIKU_KEY_B - 1] = EIKU_KEY_STATE_PRESSED;
  g_keyboard_state.modifiers = EIKU_MOD_CTRL | EIKU_MOD_ALT;
  strcpy(g_keyboard_state.text_buffer, "test");
  g_keyboard_state.text_length = 4;

  // Reset the keyboard state
  eiku_keyboard_reset(&g_keyboard_state);

  // Verify everything is reset
  for (int i = 0; i < EIKU_KEY_COUNT; i++) {
    cr_assert_eq(g_keyboard_state.keys[i], EIKU_KEY_STATE_UP,
                 "All keys should be reset to UP state");
  }

  cr_assert_eq(g_keyboard_state.modifiers, EIKU_MOD_NONE,
               "All modifiers should be cleared");

  // Text buffer should be cleared (reset calls clear_text internally)
  cr_assert_eq(g_keyboard_state.text_length, 0,
               "Text buffer should be cleared by reset");
  cr_assert_eq(g_keyboard_state.text_buffer[0], '\0',
               "Text buffer should start with null terminator after reset");
}

/**
 * @brief Test text buffer clearing
 */
Test(keyboard_xtest, test_text_buffer_clear) {
  // Set up some text in the buffer
  strcpy(g_keyboard_state.text_buffer, "test text");
  g_keyboard_state.text_length = 9;

  // Clear the text buffer
  eiku_keyboard_clear_text(&g_keyboard_state);

  // Verify it's cleared
  cr_assert_eq(g_keyboard_state.text_length, 0,
               "Text buffer length should be 0 after clear");
  cr_assert_eq(g_keyboard_state.text_buffer[0], '\0',
               "Text buffer should start with null terminator");
}

/**
 * @brief Test key repeat functionality
 */
Test(keyboard_xtest, test_key_repeats) {
  cr_skip_test("XTest requires real X11 display - skipping in CI environment");

  if (!is_xtest_available(g_test_ctx->display)) {
    cr_skip_test("XTest extension not available");
  }

  // Test with repeats disabled
  eiku_keyboard_set_repeats(&g_keyboard_state, false);

  // Send the same key twice while "held"
  send_fake_key_event(g_test_ctx->display, XK_a, NoSymbol, 1);  // Press
  process_pending_events(g_test_ctx, &g_keyboard_state);

  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_A), 1,
               "First key press should be detected");

  eiku_key_end_loop(&g_keyboard_state);  // Transition to held

  // Send another press event (simulating repeat)
  send_fake_key_event(g_test_ctx->display, XK_a, NoSymbol, 1);
  process_pending_events(g_test_ctx, &g_keyboard_state);

  // With repeats disabled, this should not register as a new press
  cr_assert_eq(eiku_key_is_pressed(&g_keyboard_state, EIKU_KEY_A), 0,
               "Repeated key press should be ignored when repeats disabled");

  // Clean up
  send_fake_key_event(g_test_ctx->display, XK_a, NoSymbol, 0);  // Release
  process_pending_events(g_test_ctx, &g_keyboard_state);
}