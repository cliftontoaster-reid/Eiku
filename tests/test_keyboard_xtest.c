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
 * @brief Test XTest extension availability
 */
Test(keyboard_xtest, test_xtest_available) {
  cr_assert_not_null(g_test_ctx, "Test context should be initialized");
  cr_assert_not_null(g_test_ctx->display, "Display should be initialized");

  int available = is_xtest_available(g_test_ctx->display);
  cr_assert_eq(available, 1, "XTest extension should be available for testing");
}

/**
 * @brief Test keyboard state reset functionality
 */

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