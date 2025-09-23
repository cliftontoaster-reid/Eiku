/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_window_creation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:45:00 by copilot           #+#    #+#             */
/*   Updated: 2025/09/22 23:25:04 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file test_window_creation.c
 * @brief Criterion tests for window creation and management functions
 *
 * Tests for:
 * - eiku_new_window()
 * - eiku_window_set_title()
 *
 * These tests verify proper window creation, property setting,
 * and integration with the context window list.
 */

#include "core/core.h"
#include "core/platform.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

// Test suite for window creation functions
TestSuite(window_creation,
	.description = "Window creation and management tests");

// Helper function to create a valid context for tests
static t_eiku_context	*create_test_context(void)
{
	t_eiku_context	*ctx;

	ctx = eiku_init();
	cr_assert_not_null(ctx, "Test context creation should succeed");
	return (ctx);
}

/**
 * @brief Test successful window creation
 */
Test(window_creation, test_eiku_new_window_success)
{
	t_eiku_context	*ctx;
	const char		*title = "Test Window";
	int				width;
	int				height;
	t_eiku_window	*window;

	ctx = create_test_context();
	// Create a window with valid parameters
	width = 800;
	height = 600;
	window = eiku_new_window(ctx, width, height, title);
	// Verify window was created successfully
	cr_assert_not_null(window,
		"eiku_new_window() should return (a valid window)");
	// Verify window properties
	cr_assert_eq(window->width, width,
		"Window width should match requested width");
	cr_assert_eq(window->height, height,
		"Window height should match requested height");
	cr_assert_not_null(window->title, "Window title should be set");
	cr_assert_str_eq(window->title, title,
		"Window title should match requested title");
#ifdef EIKU_PLATFORM_LINUX
	// Verify X11-specific properties
	cr_assert_neq(window->window, 0, "X11 Window should be created");
	cr_assert_not_null(window->gc, "Graphics context should be created");
#endif
	// Verify window is added to context's window list
	cr_assert_eq(ctx->win_list, window,
		"Window should be added to context's window list");
	cr_assert_null(window->next, "Single window should have no next window");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window creation with invalid context
 */
Test(window_creation, test_eiku_new_window_null_context)
{
	t_eiku_window	*window;

	// Attempt to create window with null context
	window = eiku_new_window(NULL, 800, 600, "Test");
	cr_assert_null(window,
		"eiku_new_window() with NULL context should return (NULL)");
}

/**
 * @brief Test window creation with invalid dimensions
 */
Test(window_creation, test_eiku_new_window_invalid_dimensions)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window1;
	t_eiku_window	*window2;
	t_eiku_window	*window3;
	t_eiku_window	*window4;

	ctx = create_test_context();
	// Test zero width
	window1 = eiku_new_window(ctx, 0, 600, "Test");
	cr_assert_null(window1,
		"eiku_new_window() with zero width should return (NULL)");
	// Test negative width
	window2 = eiku_new_window(ctx, -100, 600, "Test");
	cr_assert_null(window2,
		"eiku_new_window() with negative width should return (NULL)");
	// Test zero height
	window3 = eiku_new_window(ctx, 800, 0, "Test");
	cr_assert_null(window3,
		"eiku_new_window() with zero height should return (NULL)");
	// Test negative height
	window4 = eiku_new_window(ctx, 800, -100, "Test");
	cr_assert_null(window4,
		"eiku_new_window() with negative height should return (NULL)");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window creation with null title
 */
Test(window_creation, test_eiku_new_window_null_title)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;

	ctx = create_test_context();
	// Attempt to create window with null title
	window = eiku_new_window(ctx, 800, 600, NULL);
	cr_assert_null(window,
		"eiku_new_window() with NULL title should return (NULL)");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test creating multiple windows
 */
Test(window_creation, test_eiku_new_window_multiple)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window1;
	t_eiku_window	*window2;

	ctx = create_test_context();
	// Create first window
	window1 = eiku_new_window(ctx, 800, 600, "Window 1");
	cr_assert_not_null(window1, "First window should be created successfully");
	// Create second window
	window2 = eiku_new_window(ctx, 640, 480, "Window 2");
	cr_assert_not_null(window2, "Second window should be created successfully");
	// Verify both windows exist and are different
	cr_assert_neq(window1, window2, "Windows should be different objects");
	// Verify window list structure (newest window should be first)
	cr_assert_eq(ctx->win_list, window2,
		"Newest window should be first in list");
	cr_assert_eq(window2->next, window1, "Second window should point to first");
	cr_assert_null(window1->next, "First window should be last in list");
	// Verify properties of both windows
	cr_assert_str_eq(window1->title, "Window 1",
		"First window title should be correct");
	cr_assert_str_eq(window2->title, "Window 2",
		"Second window title should be correct");
	cr_assert_eq(window1->width, 800, "First window width should be correct");
	cr_assert_eq(window2->width, 640, "Second window width should be correct");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window title setting with valid parameters
 */
Test(window_creation, test_eiku_window_set_title_success)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;
	const char		*new_title = "New Window Title";
	bool			result;

	ctx = create_test_context();
	window = eiku_new_window(ctx, 800, 600, "Original Title");
	cr_assert_not_null(window, "Window creation should succeed for title test");
	// Set a new title
	result = eiku_window_set_title(ctx, window, new_title);
	cr_assert_eq(result, true,
		"eiku_window_set_title() should return (true on success)");
	cr_assert_not_null(window->title,
		"Window title should not be NULL after setting");
	cr_assert_str_eq(window->title, new_title,
		"Window title should match new title");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window title setting with null parameters
 */
Test(window_creation, test_eiku_window_set_title_null_params)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;
	bool			result1;
	bool			result2;
	bool			result3;

	ctx = create_test_context();
	window = eiku_new_window(ctx, 800, 600, "Test Window");
	cr_assert_not_null(window, "Window creation should succeed for title test");
	// Test with null context
	result1 = eiku_window_set_title(NULL, window, "New Title");
	cr_assert_eq(result1, false,
		"eiku_window_set_title() with NULL context should return (false)");
	// Test with null window
	result2 = eiku_window_set_title(ctx, NULL, "New Title");
	cr_assert_eq(result2, false,
		"eiku_window_set_title() with NULL window should return (false)");
	// Test with null title
	result3 = eiku_window_set_title(ctx, window, NULL);
	cr_assert_eq(result3, false,
		"eiku_window_set_title() with NULL title should return (false)");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window title setting multiple times
 */
Test(window_creation, test_eiku_window_set_title_multiple)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window;
	char			*original_title;
	bool			result1;
	bool			result2;
	const char		*long_title = "This is a very long window title that might test memory allocation and string handling capabilities of the window title setting function";
	bool			result3;

	ctx = create_test_context();
	window = eiku_new_window(ctx, 800, 600, "Original Title");
	cr_assert_not_null(window, "Window creation should succeed for title test");
	// Store original title for comparison
	original_title = strdup(window->title);
	cr_assert_not_null(original_title, "Original title should be valid");
	// Set first new title
	result1 = eiku_window_set_title(ctx, window, "First New Title");
	cr_assert_eq(result1, true, "First title change should succeed");
	cr_assert_str_eq(window->title, "First New Title",
		"Title should be updated to first new title");
	// Set second new title
	result2 = eiku_window_set_title(ctx, window, "Second New Title");
	cr_assert_eq(result2, true, "Second title change should succeed");
	cr_assert_str_eq(window->title, "Second New Title",
		"Title should be updated to second new title");
	// Set very long title
	result3 = eiku_window_set_title(ctx, window, long_title);
	cr_assert_eq(result3, true, "Long title change should succeed");
	cr_assert_str_eq(window->title, long_title,
		"Long title should be set correctly");
	// Clean up
	free(original_title);
	eiku_destroy(ctx);
}

/**
 * @brief Test window creation with various title lengths and characters
 */
Test(window_creation, test_eiku_new_window_title_variations)
{
	t_eiku_context	*ctx;
	t_eiku_window	*window1;
	const char		*special_title = "Window with Special Characters: àáâãäåæçèéêë!@#$%^&*()";
	t_eiku_window	*window2;
	char			long_title[1000];
	t_eiku_window	*window3;

	ctx = create_test_context();
	// Test empty title
	window1 = eiku_new_window(ctx, 400, 300, "");
	cr_assert_not_null(window1, "Window with empty title should be created");
	cr_assert_str_eq(window1->title, "", "Empty title should be preserved");
	// Test title with special characters
	window2 = eiku_new_window(ctx, 400, 300, special_title);
	cr_assert_not_null(window2,
		"Window with special characters should be created");
	cr_assert_str_eq(window2->title, special_title,
		"Special character title should be preserved");
	// Test very long title
	memset(long_title, 'A', sizeof(long_title) - 1);
	long_title[sizeof(long_title) - 1] = '\0';
	window3 = eiku_new_window(ctx, 400, 300, long_title);
	cr_assert_not_null(window3, "Window with long title should be created");
	cr_assert_str_eq(window3->title, long_title,
		"Long title should be preserved");
	// Clean up
	eiku_destroy(ctx);
}

/**
 * @brief Test window creation with extreme dimensions
 */
Test(window_creation, test_eiku_new_window_extreme_dimensions)
{
	t_eiku_context *ctx = create_test_context();

	// Test very small window
	t_eiku_window *small_window = eiku_new_window(ctx, 1, 1, "Tiny Window");
	cr_assert_not_null(small_window, "Very small window should be created");
	cr_assert_eq(small_window->width, 1,
		"Small window width should be preserved");
	cr_assert_eq(small_window->height, 1,
		"Small window height should be preserved");

	// Test large window (within reasonable limits)
	t_eiku_window *large_window = eiku_new_window(ctx, 3840, 2160,
			"Large Window");
	cr_assert_not_null(large_window, "Large window should be created");
	cr_assert_eq(large_window->width, 3840,
		"Large window width should be preserved");
	cr_assert_eq(large_window->height, 2160,
		"Large window height should be preserved");

	// Clean up
	eiku_destroy(ctx);
}