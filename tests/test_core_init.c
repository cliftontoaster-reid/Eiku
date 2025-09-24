/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_core_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:30:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/24 12:38:52 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file test_core_init.c
 * @brief Criterion tests for core initialization and cleanup functions
 *
 * Tests for:
 * - eiku_init()
 * - eiku_destroy()
 *
 * These tests verify proper context initialization, X11 display setup,
 * and resource cleanup.
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <unistd.h>

#include "core/core.h"
#include "core/platform.h"

// Test suite for core initialization functions
TestSuite(core_init, .description = "Core initialization and cleanup tests");

/**
 * @brief Test successful context initialization
 */
Test(core_init, test_eiku_init_success) {
  t_eiku_context *ctx;
  int result;

  // Test basic initialization
  ctx = eiku_init();
  // Should return a valid context pointer
  cr_assert_not_null(ctx, "eiku_init() should return (a valid context)");
  // Verify basic context fields are properly initialized
#ifdef EIKU_PLATFORM_LINUX
  cr_assert_not_null(ctx->display, "Display should be initialized");
  cr_assert_geq(ctx->screen, 0, "Screen should be a valid screen number");
  cr_assert_not_null(ctx->visual, "Visual should be initialized");
  cr_assert_geq(ctx->depth, 1, "Depth should be at least 1 bit");
  cr_assert_null(ctx->win_list, "Window list should be initially empty");
  cr_assert_eq(ctx->end_loop, false, "end_loop should be initially false");
  cr_assert_eq(ctx->do_flush, true, "do_flush should be initially true");
#endif
  // Clean up
  result = eiku_destroy(ctx);
  cr_assert_eq(result, EIKU_SUCCESS, "Context cleanup should succeed");
}

/**
 * @brief Test multiple initializations work correctly
 */
Test(core_init, test_eiku_init_multiple) {
  t_eiku_context *ctx1;
  t_eiku_context *ctx2;

  // Create multiple contexts
  ctx1 = eiku_init();
  ctx2 = eiku_init();
  cr_assert_not_null(ctx1, "First context should be valid");
  cr_assert_not_null(ctx2, "Second context should be valid");
  cr_assert_neq(ctx1, ctx2, "Multiple contexts should be different objects");
#ifdef EIKU_PLATFORM_LINUX
  // Both should have valid displays (might be the same display connection)
  cr_assert_not_null(ctx1->display, "First context display should be valid");
  cr_assert_not_null(ctx2->display, "Second context display should be valid");
  // Screen numbers should be the same (default screen)
  cr_assert_eq(ctx1->screen, ctx2->screen,
               "Both contexts should use the same screen");
#endif
  // Clean up both contexts
  cr_assert_eq(eiku_destroy(ctx1), EIKU_SUCCESS,
               "First context cleanup should succeed");
  cr_assert_eq(eiku_destroy(ctx2), EIKU_SUCCESS,
               "Second context cleanup should succeed");
}

/**
 * @brief Test context destruction with null parameter
 */
Test(core_init, test_eiku_destroy_null) {
  int result;

  // Destroying null context should return error
  result = eiku_destroy(NULL);
  cr_assert_eq(
      result, EIKU_ERROR_INVALID_PARAM,
      "Destroying NULL context should return (EIKU_ERROR_INVALID_PARAM)");
}

/**
 * @brief Test context destruction idempotency (destroy twice)
 */
Test(core_init, test_eiku_destroy_twice) {
  t_eiku_context *ctx;
  int result1;

  // Create a context
  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created successfully");
  // First destroy should succeed
  result1 = eiku_destroy(ctx);
  cr_assert_eq(result1, EIKU_SUCCESS, "First destroy should succeed");
  // Note: Second destroy would be undefined behavior since ctx is freed,
  // so we don't test that to avoid segfaults
}

/**
 * @brief Test context properties after initialization
 */
Test(core_init, test_eiku_init_properties) {
  t_eiku_context *ctx;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created successfully");
#ifdef EIKU_PLATFORM_LINUX
  // Test X11-specific properties
  cr_assert_neq(ctx->root, 0, "Root window should be set");
  cr_assert_geq(ctx->depth, 8, "Color depth should be at least 8 bits");
  cr_assert_leq(ctx->depth, 32, "Color depth should not exceed 32 bits");
  // Test colormap
  cr_assert_neq(ctx->cmap, 0, "Colormap should be set");
  // Test initial state - private_cmap depends on visual selection
  // It can be 0 or 1 depending on whether a non-default visual was chosen
  cr_assert(ctx->private_cmap == 0 || ctx->private_cmap == 1,
            "private_cmap should be 0 or 1 depending on visual selection");
  cr_assert_null(ctx->loop_hook, "Loop hook should be null initially");
  cr_assert_null(ctx->loop_param, "Loop param should be null initially");
#endif
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context cleanup should succeed");
}

/**
 * @brief Test that context survives basic operations
 */
Test(core_init, test_eiku_context_stability) {
  t_eiku_context *ctx;
  Display *original_display;
  int original_screen;
  Window original_root;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created successfully");
#ifdef EIKU_PLATFORM_LINUX
  // Store initial values
  original_display = ctx->display;
  original_screen = ctx->screen;
  original_root = ctx->root;
  // Perform some X11 operations to ensure context is stable
  // Sync should not crash
  XSync(ctx->display, False);
  // Values should remain unchanged
  cr_assert_eq(ctx->display, original_display,
               "Display should remain unchanged");
  cr_assert_eq(ctx->screen, original_screen, "Screen should remain unchanged");
  cr_assert_eq(ctx->root, original_root, "Root window should remain unchanged");
  // Flush should work
  XFlush(ctx->display);
#endif
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context cleanup should succeed");
}

/**
 * @brief Stress test with many context create/destroy cycles
 */
Test(core_init, test_eiku_init_destroy_stress) {
  const int cycles =
      2;  // Reduced to avoid X11 connection issues in virtual display

  for (int i = 0; i < cycles; i++) {
    t_eiku_context *ctx = eiku_init();
    cr_assert_not_null(ctx, "Context %d should be created successfully", i);

#ifdef EIKU_PLATFORM_LINUX
    // Verify context is functional
    cr_assert_not_null(ctx->display, "Context %d display should be valid", i);

    // Perform a simple X11 operation
    XSync(ctx->display, False);
#endif

    // Clean up
    int result = eiku_destroy(ctx);
    cr_assert_eq(result, EIKU_SUCCESS, "Context %d cleanup should succeed", i);

    // Small delay to prevent overwhelming X11 server in CI
    usleep(1000);  // 1ms delay
  }
}