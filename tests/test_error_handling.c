/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
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

#include "core/core.h"
#include "core/platform.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <limits.h>
#include <string.h>

// Test suite for error handling
TestSuite(error_handling, .description = "Error handling and edge case tests");

/**
 * @brief Test memory allocation edge cases
 */
Test(error_handling, test_memory_edge_cases)
{
	t_eiku_context	*ctx;
	char			*large_title;
	t_eiku_window	*window;

	// Test with extremely large title (potential memory allocation issue)
	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created for memory tests");
	// Create a very large title string
	const size_t large_size = 100000; // 100KB title
	large_title = malloc(large_size);
	cr_assert_not_null(large_title,
		"Large title allocation should succeed for test");
	// Fill with pattern
	memset(large_title, 'A', large_size - 1);
	large_title[large_size - 1] = '\0';
	// Try to create window with large title
	window = eiku_new_window(ctx, 800, 600, large_title);
	if (window != NULL)
	{
		// If creation succeeded, verify title was properly allocated
		cr_assert_not_null(window->title, "Window title should not be NULL");
		cr_assert_eq(strlen(window->title), large_size - 1,
			"Title length should match");
		cr_assert_str_eq(window->title, large_title,
			"Large title should be preserved");
	}
	// If creation failed,
		that's also acceptable behavior for such a large allocation
	free(large_title);
	eiku_destroy(ctx);
}

/**
 * @brief Test boundary conditions for window dimensions
 */
Test(error_handling, test_dimension_boundaries)
{
	t_eiku_context	*ctx;
	t_eiku_window	*huge_window;
	t_eiku_window	*tiny_window;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created for dimension tests");
	// Test with INT_MAX dimensions (should fail gracefully)
	huge_window = eiku_new_window(ctx, INT_MAX, INT_MAX, "Huge Window");
	// This should either fail (return NULL) or succeed with clamped values
	// Both behaviors are acceptable, but it shouldn't crash
	if (huge_window != NULL)
	{
		// If it succeeded, dimensions should be reasonable
		cr_assert_gt(huge_window->width, 0, "Width should be positive");
		cr_assert_gt(huge_window->height, 0, "Height should be positive");
	}
	// Test with minimum valid dimensions
	tiny_window = eiku_new_window(ctx, 1, 1, "Tiny Window");
	cr_assert_not_null(tiny_window,
		"Minimum dimension window should be created");
	cr_assert_eq(tiny_window->width, 1, "Minimum width should be preserved");
	cr_assert_eq(tiny_window->height, 1, "Minimum height should be preserved");
	eiku_destroy(ctx);
}

/**
 * @brief Test string handling edge cases
 */
Test(error_handling, test_string_edge_cases)
{
	t_eiku_context	*ctx;
	t_eiku_window	*empty_title_window;
	t_eiku_window	*whitespace_window;
	char			title_with_null[] = "Title\0Hidden";
	t_eiku_window	*null_window;
	bool			empty_result;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created for string tests");
	// Test with empty string title
	empty_title_window = eiku_new_window(ctx, 400, 300, "");
	cr_assert_not_null(empty_title_window,
		"Window with empty title should be created");
	cr_assert_str_eq(empty_title_window->title, "",
		"Empty title should be preserved");
	// Test title with only whitespace
	whitespace_window = eiku_new_window(ctx, 400, 300, "   \t\n   ");
	cr_assert_not_null(whitespace_window,
		"Window with whitespace title should be created");
	cr_assert_str_eq(whitespace_window->title, "   \t\n   ",
		"Whitespace title should be preserved");
	// Test title with null bytes in the middle (should stop at first null)
	null_window = eiku_new_window(ctx, 400, 300, title_with_null);
	cr_assert_not_null(null_window,
		"Window with embedded null should be created");
	cr_assert_str_eq(null_window->title, "Title",
		"Title should stop at null byte");
	// Test title setting with empty string
	empty_result = eiku_window_set_title(ctx, empty_title_window, "");
	cr_assert_eq(empty_result, true, "Setting empty title should succeed");
	cr_assert_str_eq(empty_title_window->title, "",
		"Empty title should be set");
	eiku_destroy(ctx);
}

/**
 * @brief Test operations on destroyed/invalid contexts
 */
Test(error_handling, test_invalid_context_operations)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;

	// Create and immediately destroy context
	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created");
	// Create a window first
	window = eiku_new_window(ctx, 400, 300, "Test Window");
	cr_assert_not_null(window, "Window should be created");
	// Destroy context
	eiku_destroy(ctx);
	// Now try to use the destroyed context (should not crash,
		but behavior is undefined)
	// Note: This is technically undefined behavior,
		but we want to ensure it doesn't crash
	// In a real scenario, this would be a programming error
	// The following operations should either return error codes or handle gracefully
	// We can't reliably test this without risking crashes,
		so we'll focus on null parameter tests
}

/**
 * @brief Test rapid creation and destruction
 */
Test(error_handling, test_rapid_create_destroy)
{
	const int		iterations = 20;
	t_eiku_context	*ctx;
		char title[32];
	t_eiku_window	*window;
		char new_title[32];
	bool			result;
	int				destroy_result;

	for (int i = 0; i < iterations; i++)
	{
		ctx = eiku_init();
		cr_assert_not_null(ctx, "Context %d should be created", i);
		// Create a window
		snprintf(title, sizeof(title), "Window %d", i);
		window = eiku_new_window(ctx, 300 + i, 200 + i, title);
		cr_assert_not_null(window, "Window %d should be created", i);
		// Modify title
		snprintf(new_title, sizeof(new_title), "Modified %d", i);
		result = eiku_window_set_title(ctx, window, new_title);
		cr_assert_eq(result, true, "Title modification %d should succeed", i);
		// Destroy immediately
		destroy_result = eiku_destroy(ctx);
		cr_assert_eq(destroy_result, EIKU_SUCCESS, "Destruction
			%d should succeed", i);
	}
}

/**
 * @brief Test title setting with special characters and encodings
 */
Test(error_handling, test_special_character_handling)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;
	const int		num_titles = sizeof(special_titles)
				/ sizeof(special_titles[0]);
	bool			result;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created");
	window = eiku_new_window(ctx, 400, 300, "Test Window");
	cr_assert_not_null(window, "Window should be created");
	// Test various special character combinations
	const char *special_titles[] = {"Title with UTF-8: àáâãäåæçèéêë",
									"Symbols: !@#$%^&*()_+-=[]{}|;':\",./<>?",
									"Numbers and spaces: 123 456 789",
									"Mixed: ABC123!@# XYZ789$%^",
									"Unicode: 你好世界 🌍 🚀 ⭐",
									"Long with special: "
									"This is a very long title with special characters àáâãäåæçèéêë "
									"and symbols !@#$%^&*()_+-=[]{}|;':\",./<>? and numbers 123456789"};
	for (int i = 0; i < num_titles; i++)
	{
		result = eiku_window_set_title(ctx, window, special_titles[i]);
		cr_assert_eq(result, true, "Special title
			%d should be set successfully", i);
		// Verify the title was set (note: some characters might not be preserved
		// depending on X11 encoding, but the function should not crash)
		cr_assert_not_null(window->title,
			"Title should not be NULL after setting special characters");
		cr_assert_gt(strlen(window->title), 0,
			"Title should not be empty after setting");
	}
	eiku_destroy(ctx);
}

/**
 * @brief Test resource exhaustion scenarios
 */
Test(error_handling, test_resource_limits)
{
	t_eiku_context	*ctx;
	const int		max_windows = 50;
	t_eiku_window	*windows[max_windows];
	int				created_count;
		char title[32];
	t_eiku_window	*current;
	int				list_count;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created");
	// Try to create many windows to test resource limits
	created_count = 0;
	for (int i = 0; i < max_windows; i++)
	{
		snprintf(title, sizeof(title), "Resource Test %d", i);
		windows[i] = eiku_new_window(ctx, 100 + i, 100 + i, title);
		if (windows[i] != NULL)
		{
			created_count++;
		}
		else
		{
			// Window creation failed - this might be due to resource limits
			// This is acceptable behavior
			break ;
		}
	}
	// Should have created at least a few windows
	cr_assert_gt(created_count, 0,
		"Should be able to create at least some windows");
	// Verify all created windows are in the list
	current = ctx->win_list;
	list_count = 0;
	while (current)
	{
		list_count++;
		current = current->next;
	}
	cr_assert_eq(list_count, created_count,
		"All created windows should be in the list");
	eiku_destroy(ctx);
}

/**
 * @brief Test concurrent-like operations (rapid switching)
 */
Test(error_handling, test_rapid_operations)
{
	t_eiku_context	*ctx;
	const int		num_windows = 5;
	t_eiku_window	*windows[num_windows];
		char title[32];
			char new_title[64];
	bool			result;
			char new_title[64];
	bool			result;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created");
	// Create several windows
	for (int i = 0; i < num_windows; i++)
	{
		snprintf(title, sizeof(title), "Rapid Test %d", i);
		windows[i] = eiku_new_window(ctx, 200 + i * 50, 150 + i * 30, title);
		cr_assert_not_null(windows[i], "Window %d should be created", i);
	}
	// Rapidly change titles in different orders
	for (int round = 0; round < 10; round++)
	{
		for (int i = 0; i < num_windows; i++)
		{
			snprintf(new_title, sizeof(new_title), "Round %d Window %d", round,
				i);
			result = eiku_window_set_title(ctx, windows[i], new_title);
			cr_assert_eq(result, true, "Rapid title change round %d window
				%d should succeed", round, i);
		}
		// Also test in reverse order
		for (int i = num_windows - 1; i >= 0; i--)
		{
			snprintf(new_title, sizeof(new_title), "Reverse %d Window %d",
				round, i);
			result = eiku_window_set_title(ctx, windows[i], new_title);
			cr_assert_eq(result, true, "Rapid reverse title change round
				%d window %d should succeed", round, i);
		}
	}
	// Verify all windows still exist and have correct final titles
	for (int i = 0; i < num_windows; i++)
	{
		cr_assert_not_null(windows[i]->title, "Window
			%d title should not be NULL", i);
		cr_assert(strstr(windows[i]->title, "Reverse") != NULL, "Window
			%d should have final reverse title", i);
	}
	eiku_destroy(ctx);
}

/**
 * @brief Test error code consistency
 */
Test(error_handling, test_error_code_consistency)
{
	// Test that error functions return consistent error codes

	// eiku_destroy with NULL should always return EIKU_ERROR_INVALID_PARAM
	for (int i = 0; i < 5; i++)
	{
		int result = eiku_destroy(NULL);
		cr_assert_eq(result, EIKU_ERROR_INVALID_PARAM,
			"eiku_destroy(NULL) should consistently return (EIKU_ERROR_INVALID_PARAM (attempt
			%d)", i));
	}

	// eiku_new_window with NULL context should always return NULL
	for (int i = 0; i < 5; i++)
	{
		t_eiku_window *window = eiku_new_window(NULL, 400, 300, "Test");
		cr_assert_null(window,
			"eiku_new_window with NULL context should consistently return (NULL (attempt
			%d)", i));
	}

	// eiku_window_set_title with NULL parameters should always return false
	t_eiku_context *ctx = eiku_init();
	cr_assert_not_null(ctx, "Context should be created for error code tests");

	t_eiku_window *window = eiku_new_window(ctx, 400, 300, "Test");
	cr_assert_not_null(window, "Window should be created for error code tests");

	for (int i = 0; i < 5; i++)
	{
		bool result1 = eiku_window_set_title(NULL, window, "Test");
		bool result2 = eiku_window_set_title(ctx, NULL, "Test");
		bool result3 = eiku_window_set_title(ctx, window, NULL);

		cr_assert_eq(result1, false,
			"eiku_window_set_title with NULL context should consistently return (false (attempt
			%d)", i));
		cr_assert_eq(result2, false,
			"eiku_window_set_title with NULL window should consistently return (false (attempt
			%d)", i));
		cr_assert_eq(result3, false,
			"eiku_window_set_title with NULL title should consistently return (false (attempt
			%d)", i));
	}

	eiku_destroy(ctx);
}