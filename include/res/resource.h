/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:28 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 15:49:07 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/core.h"
#include "core/platform.h"

// Resource ID type - use 0 as invalid ID
typedef uint32_t eiku_res_id_t;
#define EIKU_INVALID_RES_ID 0

// Forward declarations
typedef struct s_res t_res;
typedef struct s_res_type t_res_type;

typedef struct s_res {
  eiku_res_id_t id;  // Unique auto-incremented identifier
  size_t size;
  void* data;
  int type;

  char* path;
  bool modified;
  bool safe;
  int ref_count;
  bool writeable;
  bool
      temporary;  // True if this is a copy created by eiku_res_acquire_writable

  t_res* prev;
  t_res* next;
} t_res;

typedef struct s_res_type {
  int id;
  const char* name;
  void (*del)(t_res*);
  void (*copy)(t_res*, const t_res*);

  struct s_res_type* next;
} t_res_type;

// Linked list of resources
typedef struct s_res_manager {
  t_res* resources;
  t_res_type* types;
  eiku_res_id_t next_id;  // Auto-increment counter for unique IDs
} t_res_manager;

// Resource registration and management

/// @brief Initialize a new resource manager
///
/// Creates and initializes a resource manager that tracks all resources
/// and their reference counts for automatic memory management.
///
/// @return Pointer to the initialized resource manager, or NULL on failure
EIKU_API t_res_manager* eiku_res_manager_init(void);

/// @brief Free a resource manager and all its resources
///
/// Cleans up the resource manager and forcibly frees all remaining resources.
/// This should typically be called during application shutdown.
///
/// @param manager The resource manager to free
EIKU_API void eiku_res_manager_free(t_res_manager* manager);

/// @brief Register a new resource type with the manager
///
/// Registers a resource type with custom deletion and copy functions.
/// This allows the manager to handle different types of resources
/// appropriately.
///
/// @param manager The resource manager
/// @param id Unique identifier for the resource type
/// @param name Human-readable name for the resource type
/// @param del Function to delete/cleanup a resource of this type
/// @param copy Function to copy a resource of this type
/// @return 0 on success, negative value on failure
EIKU_API int eiku_res_register_type(t_res_manager* manager, int id,
                                    const char* name, void (*del)(t_res*),
                                    void (*copy)(t_res*, const t_res*));

// Resource creation and loading

/// @brief Create a new resource of a specific type
///
/// Allocates memory for a new resource and registers it with the manager.
/// Each resource is assigned a unique auto-incremented ID for safe access.
/// Prefer using the ID-based API functions for safer resource management.
///
/// @param manager The resource manager
/// @param type The registered resource type ID
/// @param size Size of the resource data in bytes
/// @param safe If true, the resource is marked as safe (immutable)
/// @return Pointer to the new resource, or NULL on failure
EIKU_API t_res* eiku_res_create(t_res_manager* manager, int type, size_t size,
                                bool safe);

/// @brief Load a resource from a file
///
/// Loads a resource from the specified file path and registers it with the
/// manager. The resource type is determined automatically based on file
/// extension or content. Each loaded resource is assigned a unique ID.
/// Consider using the ID-based API for safer resource access.
///
/// @param manager The resource manager
/// @param path Path to the file to load
/// @return Pointer to the loaded resource, or NULL on failure
EIKU_API t_res* eiku_res_readFile(t_res_manager* manager, const char* path);

// Resource acquisition and reference management

/// @brief Acquire a resource by path (read-only access)
///
/// Gets an existing resource or loads a new one if it doesn't exist.
/// Increments the reference count. Multiple calls with the same path
/// return the same resource instance.
///
/// @deprecated Consider using ID-based functions for safer resource management.
/// Use resource->id after acquisition and switch to eiku_res_acquire_by_id().
///
/// @param manager The resource manager
/// @param res The resource to acquire
/// @return Pointer to the resource, or NULL on failure
EIKU_API t_res* eiku_res_acquire(t_res_manager* manager, t_res* res);

/// @brief Acquire a resource with writable access
///
/// Gets a resource with write permissions. If the resource is currently
/// shared (ref_count > 1), a copy is created automatically (copy-on-write).
///
/// If the resource is marked as safe, it will always create a copy to ensure
/// that the original resource remains unchanged.
///
/// @warning The resource is considered temporary in the case it is a copy.
/// If left unchanged, it will be automatically released when relinquished
/// therefore, deleting its content.
///
/// @param manager The resource manager
/// @param res The resource to acquire with writable access
/// @return Pointer to the writable resource, or NULL on failure
EIKU_API t_res* eiku_res_acquire_writable(t_res_manager* manager, t_res* res);

/// @brief Try to acquire writable access without copying
///
/// Attempts to get writable access to a resource. If the resource is
/// currently shared, returns NULL instead of creating a copy.
/// Use this when you prefer to wait rather than duplicate the resource.
///
/// @param manager The resource manager
/// @param res The resource to try to acquire
/// @return Pointer to the resource if available for exclusive write, NULL
/// otherwise
EIKU_API t_res* eiku_res_try_acquire_writable(t_res_manager* manager,
                                              t_res* res);

/// @brief Relinquish writable access to a resource
///
/// Signals that you're done writing to a resource. This allows other
/// threads/processes to potentially acquire writable access.
///
/// @warning If the resource is a temporary copy created by
/// eiku_res_acquire_writable(), it will be automatically released (cleaned up)
/// to prevent memory leaks. Otherwise, the resource remains in memory until all
/// references are released.
///
/// @param manager The resource manager
/// @param res The resource to relinquish
EIKU_API void eiku_res_relinquish(t_res_manager* manager, t_res* res);

/// @brief Release a reference to a resource
///
/// Decrements the reference count of a resource. When the count reaches 0,
/// the resource is automatically cleaned up and freed.
///
/// @param manager The resource manager
/// @param res The resource to release
EIKU_API void eiku_res_release(t_res_manager* manager, t_res* res);

// Resource discovery and utilities

/// @brief Find a resource by path without acquiring it
///
/// Searches for an existing resource by path without incrementing
/// the reference count. Useful for checking if a resource exists
/// or getting metadata without taking ownership.
///
/// @param manager The resource manager
/// @param path Path to search for
/// @return Pointer to the resource if found, NULL otherwise
EIKU_API t_res* eiku_res_find_by_path(t_res_manager* manager, const char* path);

/// @brief Find a resource by ID without acquiring it
///
/// Searches for an existing resource by its unique ID without incrementing
/// the reference count. Useful for checking if a resource exists
/// or getting metadata without taking ownership.
///
/// @param manager The resource manager
/// @param id Resource ID to search for
/// @return Pointer to the resource if found, NULL otherwise
EIKU_API t_res* eiku_res_find_by_id(t_res_manager* manager, eiku_res_id_t id);

// ID-based resource management functions (recommended over pointer-based)

/// @brief Acquire a resource by ID (read-only access)
///
/// Gets an existing resource by ID and increments its reference count.
/// This is the recommended way to access resources safely.
///
/// @param manager The resource manager
/// @param id Resource ID to acquire
/// @return Resource ID of the acquired resource, or EIKU_INVALID_RES_ID on
/// failure
EIKU_API eiku_res_id_t eiku_res_acquire_by_id(t_res_manager* manager,
                                              eiku_res_id_t id);

/// @brief Acquire a resource with writable access by ID
///
/// Gets a resource with write permissions by ID. If the resource is currently
/// shared (ref_count > 1), a copy is created automatically (copy-on-write).
///
/// @param manager The resource manager
/// @param id Resource ID to acquire for writing
/// @return Resource ID of the writable resource (may be a new copy), or
/// EIKU_INVALID_RES_ID on failure
EIKU_API eiku_res_id_t eiku_res_acquire_writable_by_id(t_res_manager* manager,
                                                       eiku_res_id_t id);

/// @brief Try to acquire writable access by ID without copying
///
/// Attempts to get writable access to a resource by ID. If the resource is
/// currently shared, returns EIKU_INVALID_RES_ID instead of creating a copy.
///
/// @param manager The resource manager
/// @param id Resource ID to try to acquire
/// @return Resource ID if available for exclusive write, EIKU_INVALID_RES_ID
/// otherwise
EIKU_API eiku_res_id_t
eiku_res_try_acquire_writable_by_id(t_res_manager* manager, eiku_res_id_t id);

/// @brief Relinquish writable access to a resource by ID
///
/// Signals that you're done writing to a resource identified by ID.
/// If the resource is a temporary copy, it will be automatically released.
///
/// @param manager The resource manager
/// @param id Resource ID to relinquish
EIKU_API void eiku_res_relinquish_by_id(t_res_manager* manager,
                                        eiku_res_id_t id);

/// @brief Release a reference to a resource by ID
///
/// Decrements the reference count of a resource identified by ID.
/// When the count reaches 0, the resource is automatically cleaned up.
///
/// @param manager The resource manager
/// @param id Resource ID to release
EIKU_API void eiku_res_release_by_id(t_res_manager* manager, eiku_res_id_t id);

/// @brief Get resource data pointer by ID (use with caution)
///
/// Returns the data pointer for a resource. The resource must be acquired
/// first. This function should be used sparingly - prefer keeping resource
/// access through the ID-based API when possible.
///
/// @param manager The resource manager
/// @param id Resource ID
/// @return Pointer to resource data, or NULL if not found or not acquired
EIKU_API void* eiku_res_get_data_by_id(t_res_manager* manager,
                                       eiku_res_id_t id);

/// @brief Force delete a resource
///
/// Immediately deletes a resource regardless of reference count.
/// Use with caution as this can cause dangling pointers.
/// Typically used for cleanup or when you know no other references exist.
///
/// @param manager The resource manager
/// @param res The resource to delete
EIKU_INT void eiku_res_delete(t_res_manager* manager, t_res* res);
EIKU_INT t_res_type* find_res_type(t_res_manager* manager, int type_id);