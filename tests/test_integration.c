/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_integration.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/22 23:26:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file test_integration.c
 * @brief Criterion integration tests for Eiku library
 *
 * These tests verify that multiple Eiku functions work correctly together
 * and test real-world usage scenarios.
 */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <unistd.h>

#include "core/core.h"
#include "core/platform.h"

// Test suite for integration tests
TestSuite(integration,
          .description = "Integration tests for combined functionality");

/**
 * @brief Test complete window lifecycle
 */
Test(integration, test_complete_window_lifecycle) {
  t_eiku_context *ctx;
  t_eiku_window *window;
  bool title_result;
  int destroy_result;

  // Initialize context
  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");
  // Create window
  window = eiku_new_window(ctx, 800, 600, "Lifecycle Test Window");
  cr_assert_not_null(window, "Window should be created for string tests");
  // Verify initial state
  cr_assert_eq(ctx->win_list, window, "Window should be in context list");
  cr_assert_str_eq(window->title, "Lifecycle Test Window",
                   "Initial title should be correct");
  // Change window title
  title_result = eiku_window_set_title(ctx, window, "Updated Title");
  cr_assert_eq(title_result, true, "Title update should succeed");
  cr_assert_str_eq(window->title, "Updated Title", "Title should be updated");
  // Window should still be in context list
  cr_assert_eq(ctx->win_list, window, "Window should still be in context list");
#ifdef EIKU_PLATFORM_LINUX
  // Verify X11 window is still valid
  cr_assert_neq(window->window, 0, "X11 window should still be valid");
  cr_assert_not_null(window->gc, "Graphics context should still be valid");
  // Perform some X11 operations to ensure everything is working
  XSync(ctx->display, False);
  XFlush(ctx->display);
#endif
  // Clean up (this should destroy the window as well)
  destroy_result = eiku_destroy(ctx);
  cr_assert_eq(destroy_result, EIKU_SUCCESS,
               "Context destruction should succeed");
}

/**
 * @brief Test multiple windows with title changes
 */
Test(integration, test_multiple_windows_with_titles) {
  t_eiku_context *ctx;
  t_eiku_window *window1;
  t_eiku_window *window2;
  t_eiku_window *window3;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");
  // Create multiple windows
  window1 = eiku_new_window(ctx, 400, 300, "Window 1");
  window2 = eiku_new_window(ctx, 500, 400, "Window 2");
  window3 = eiku_new_window(ctx, 600, 500, "Window 3");
  cr_assert_not_null(window1, "First window should be created");
  cr_assert_not_null(window2, "Second window should be created");
  cr_assert_not_null(window3, "Third window should be created");
  // Verify window list order (newest first)
  cr_assert_eq(ctx->win_list, window3, "Third window should be first in list");
  cr_assert_eq(window3->next, window2, "Third window should point to second");
  cr_assert_eq(window2->next, window1, "Second window should point to first");
  cr_assert_null(window1->next, "First window should be last");
  // Change titles of all windows
  cr_assert_eq(eiku_window_set_title(ctx, window1, "Updated Window 1"), true,
               "First window title update should succeed");
  cr_assert_eq(eiku_window_set_title(ctx, window2, "Updated Window 2"), true,
               "Second window title update should succeed");
  cr_assert_eq(eiku_window_set_title(ctx, window3, "Updated Window 3"), true,
               "Third window title update should succeed");
  // Verify all titles were updated
  cr_assert_str_eq(window1->title, "Updated Window 1",
                   "First window title should be updated");
  cr_assert_str_eq(window2->title, "Updated Window 2",
                   "Second window title should be updated");
  cr_assert_str_eq(window3->title, "Updated Window 3",
                   "Third window title should be updated");
  // Verify window list structure is still intact
  cr_assert_eq(ctx->win_list, window3, "Window list order should be preserved");
#ifdef EIKU_PLATFORM_LINUX
  // Verify all X11 windows are different
  cr_assert_neq(window1->window, window2->window,
                "X11 windows should be different");
  cr_assert_neq(window2->window, window3->window,
                "X11 windows should be different");
  cr_assert_neq(window1->window, window3->window,
                "X11 windows should be different");
#endif
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context destruction should succeed");
}

/**
 * @brief Test stress scenario with many window operations
 */
Test(integration, test_window_stress) {
  t_eiku_context *ctx;
  const int num_windows = 5;
  t_eiku_window *windows[num_windows];
  char title[64];
  t_eiku_window *current;
  int count;
  char new_title[64];
  bool result;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");
  // Create multiple windows
  for (int i = 0; i < num_windows; i++) {
    snprintf(title, sizeof(title), "Stress Test Window %d", i + 1);
    windows[i] = eiku_new_window(ctx, 300 + i * 50, 200 + i * 30, title);
    cr_assert_not_null(windows[i], "Window %d should be created", i + 1);
    cr_assert_str_eq(windows[i]->title, title,
                     "Window"
                     "%d title should be correct",
                     i + 1);
  }
  // Verify all windows are in the list
  current = ctx->win_list;
  count = 0;
  while (current) {
    count++;
    current = current->next;
  }
  cr_assert_eq(count, num_windows, "All windows should be in the context list");
  // Change titles multiple times
  for (int round = 0; round < 3; round++) {
    for (int i = 0; i < num_windows; i++) {
      snprintf(new_title, sizeof(new_title), "Round %d - Window %d", round + 1,
               i + 1);
      result = eiku_window_set_title(ctx, windows[i], new_title);
      cr_assert_eq(result, true,
                   "Title update round %d window"
                   "%d should succeed",
                   round + 1, i + 1);
      cr_assert_str_eq(windows[i]->title, new_title,
                       "Title should be updated correctly");
    }
  }
#ifdef EIKU_PLATFORM_LINUX
  // Perform X11 operations on all windows
  for (int i = 0; i < num_windows; i++) {
    cr_assert_neq(windows[i]->window, 0, "X11 window %d should be valid",
                  i + 1);
    cr_assert_not_null(windows[i]->gc,
                       "Graphics context"
                       "%d should be valid",
                       i + 1);
  }
  // Sync all operations
  XSync(ctx->display, False);
#endif
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context destruction should succeed");
}

/**
 * @brief Test context reuse after window operations
 */
Test(integration, test_context_reuse) {
  t_eiku_context *ctx;
  t_eiku_window *window1;
  t_eiku_window *window2;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");
  // Create and test first batch of windows
  window1 = eiku_new_window(ctx, 400, 300, "First Window");
  cr_assert_not_null(window1, "First window should be created");
  eiku_window_set_title(ctx, window1, "First Window Updated");
  cr_assert_str_eq(window1->title, "First Window Updated",
                   "First window title should be updated");
  // Create second batch of windows in the same context
  window2 = eiku_new_window(ctx, 500, 400, "Second Window");
  cr_assert_not_null(window2, "Second window should be created");
  // Verify both windows coexist
  cr_assert_eq(ctx->win_list, window2, "Second window should be first in list");
  cr_assert_eq(window2->next, window1, "Second window should point to first");
  // Modify both windows
  eiku_window_set_title(ctx, window1, "First Modified Again");
  eiku_window_set_title(ctx, window2, "Second Modified");
  cr_assert_str_eq(window1->title, "First Modified Again",
                   "First window should be modified again");
  cr_assert_str_eq(window2->title, "Second Modified",
                   "Second window should be modified");
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context destruction should succeed");
}

/**
 * @brief Test edge case: rapid window creation and title changes
 */
Test(integration, test_rapid_operations) {
  t_eiku_context *ctx;
  const int operations = 10;
  char title[32];
  t_eiku_window *window;
  char new_title[32];
  t_eiku_window *current;
  int count;

  ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");
  // Rapid window creation
  for (int i = 0; i < operations; i++) {
    snprintf(title, sizeof(title), "Rapid %d", i);
    window = eiku_new_window(ctx, 100 + i * 10, 100 + i * 10, title);
    cr_assert_not_null(window, "Rapid window %d should be created", i);
    // Immediately change title
    snprintf(new_title, sizeof(new_title), "Changed %d", i);
    cr_assert_eq(eiku_window_set_title(ctx, window, new_title), true,
                 "Rapid title change %d should succeed", i);
  }
  // Verify final state
  current = ctx->win_list;
  count = 0;
  while (current) {
    count++;
    // Verify each window has the expected "Changed" title format
    cr_assert(strstr(current->title, "Changed") != NULL,
              "Window should have 'Changed' in title");
    current = current->next;
  }
  cr_assert_eq(count, operations, "All windows should be in list");
  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context destruction should succeed");
}

/**
 * @brief Test window properties consistency after operations
 */
Test(integration, test_window_properties_consistency) {
  t_eiku_context *ctx = eiku_init();
  cr_assert_not_null(ctx, "Context initialization should succeed");

  // Create window with specific properties
  const int original_width = 640;
  const int original_height = 480;
  const char *original_title = "Property Test";

  t_eiku_window *window =
      eiku_new_window(ctx, original_width, original_height, original_title);
  cr_assert_not_null(window, "Window should be created");

  // Verify initial properties
  cr_assert_eq(window->width, original_width,
               "Initial width should be correct");
  cr_assert_eq(window->height, original_height,
               "Initial height should be correct");
  cr_assert_str_eq(window->title, original_title,
                   "Initial title should be correct");

  // Change title multiple times
  const char *titles[] = {"First Change", "Second Change", "Third Change",
                          "Final Title"};
  const int num_title_changes = sizeof(titles) / sizeof(titles[0]);

  for (int i = 0; i < num_title_changes; i++) {
    cr_assert_eq(eiku_window_set_title(ctx, window, titles[i]), true,
                 "Title change %d should succeed", i + 1);
    cr_assert_str_eq(window->title, titles[i],
                     "Title should be updated to '%s'", titles[i]);

    // Verify other properties remain unchanged
    cr_assert_eq(window->width, original_width,
                 "Width should remain unchanged after title change %d", i + 1);
    cr_assert_eq(window->height, original_height,
                 "Height should remain unchanged after title change %d", i + 1);

#ifdef EIKU_PLATFORM_LINUX
    // Verify X11 properties remain valid
    cr_assert_neq(window->window, 0,
                  "X11 window should remain valid after title change %d",
                  i + 1);
    cr_assert_not_null(
        window->gc,
        "Graphics context should remain valid after title change %d", i + 1);
#endif
  }

  // Clean up
  cr_assert_eq(eiku_destroy(ctx), EIKU_SUCCESS,
               "Context destruction should succeed");
}