/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_res_readFile.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:55:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 14:55:00 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"
#include "res/resource.h"

EIKU_API t_res* eiku_res_readFile(t_res_manager* manager, const char* path) {
  if (!manager || !path) {
    return NULL;  // Invalid parameters
  }

  // Check if the resource already exists
  t_res* existing_res = eiku_res_find_by_path(manager, path);
  if (existing_res) {
    existing_res->ref_count++;
    return existing_res;  // Return existing resource with incremented ref count
  }

  // Load the file from disk
  t_file* file = eiku_file_open(path, EIKU_FS_READ);
  if (!file) {
    return NULL;  // Failed to open file
  }

  // We make it read the entire file after requesting it's information.
  if (!eiku_file_load_info(file) || !eiku_file_load_content(file)) {
    eiku_file_close(file);
    return NULL;  // Failed to load file info or content
  }

  // Type -1 is a simple free blob of data with no specific type
  t_res* new_res = (t_res*)malloc(sizeof(t_res));
  if (!new_res) {
    return NULL;  // Memory allocation failed
  }
  new_res->data = file->content;  // Take ownership of the content
  new_res->size = file->size;
  new_res->type = -1;            // Unknown type, can be set later
  new_res->path = strdup(path);  // Store the path
  new_res->modified = false;
  new_res->safe = true;        // Loaded from file, considered safe
  new_res->ref_count = 1;      // Initial reference count
  new_res->writeable = false;  // Will never be writeable
  new_res->prev = NULL;
  new_res->next = NULL;
  eiku_file_close(file);  // Close the file, content is now owned by res

  // Insert the new resource at the beginning of the manager's resource list
  if (manager->resources) {
    new_res->next = manager->resources;
    manager->resources->prev = new_res;
  }
  manager->resources = new_res;

  return new_res;  // Return the newly created resource
}
