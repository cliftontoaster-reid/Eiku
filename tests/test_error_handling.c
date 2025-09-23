/* ************************************************************************** */
/*                                                                            */
/*                      		cr_assert_eq(destroy_result,
   EIKU_SUCCESS, "Destruction %d should succeed", i); :::      ::::::::   */
/*   test_error_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:15:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/22 16:39:16 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file test_error_handling.c
 * @brief Criterion tests for error handling and edge cases
 *
 * These tests verify that the Eiku library handles error conditions
 * gracefully and returns appropriate error codes.
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <limits.h>
#include <string.h>

#include "core/core.h"
#include "core/platform.h"

// Test suite for error handling
TestSuite(error_handling, .description = "Error handling and edge case tests");

/**
 * @brief Test memory allocation edge cases
 */
Test(error_handling, test_memory_edge_cases) {
  t_eiku_context *ctx;
  char *large_title;
  t_eiku_window *window;

  // Test with extremely large title (potential memory allocation issue)
  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created for memory tests");
  // Create a very large title string
  const size_t large_size = 100000;  // 100KB title
  large_title = malloc(large_size);
  cr_assert_not_null(large_title,
                     "Large title allocation should succeed for test");
  // Fill with pattern
  memset(large_title, 'A', large_size - 1);
  large_title[large_size - 1] = '\0';
  // Try to create window with large title
  window = eiku_new_window(ctx, 800, 600, large_title);
  if (window != NULL) {
    // If creation succeeded, verify title was properly allocated
    cr_assert_not_null(window->title, "Window title should not be NULL");
    cr_assert_eq(strlen(window->title), large_size - 1,
                 "Title length should match");
    cr_assert_str_eq(window->title, large_title,
                     "Large title should be preserved");
  }
  // If creation failed,
  // that's also acceptable behavior for such a large allocation
  free(large_title);
  eiku_destroy(ctx);
}

/**
 * @brief Test null parameter handling
 */
Test(error_handling, test_null_parameters) {
  t_eiku_context *ctx;
  t_eiku_window *window;
  int result;

  // Test eiku_destroy with null
  result = eiku_destroy(NULL);
  cr_assert_eq(result, EIKU_ERROR_INVALID_PARAM,
               "Null context destroy should return (error)");
  // Test window creation with null context
  window = eiku_new_window(NULL, 400, 300, "Test");
  cr_assert_null(window, "Window creation with null context should fail");
  // Test with valid context
  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created for null tests");
  // Test window creation with null title
  window = eiku_new_window(ctx, 400, 300, NULL);
  cr_assert_null(window, "Window creation with null title should fail");
  // Clean up
  eiku_destroy(ctx);
}

/**
 * @brief Test boundary window dimensions
 */
Test(error_handling, test_boundary_dimensions) {
  t_eiku_context *ctx;
  t_eiku_window *window;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context should be created for boundary tests");

  // Test zero dimensions
  window = eiku_new_window(ctx, 0, 300, "Zero Width");
  cr_assert_null(window, "Zero width window should fail");

  window = eiku_new_window(ctx, 400, 0, "Zero Height");
  cr_assert_null(window, "Zero height window should fail");

  // Test very large dimensions (may succeed or fail depending on system)
  window = eiku_new_window(ctx, 32000, 24000, "Large Window");
  // Don't assert - behavior is system dependent

  if (window != NULL) {
    // If it succeeded, basic properties should be set
    cr_assert_geq(window->width, 32000,
                  "Large window width should be preserved");
    cr_assert_geq(window->height, 24000,
                  "Large window height should be preserved");
  }

  // Clean up
  eiku_destroy(ctx);
}