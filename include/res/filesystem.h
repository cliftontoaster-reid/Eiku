/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:30 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:46:42 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct s_file {
  char *path;       /* owned by the caller/open: freed by eiku_file_close */
  int fd;           /* native file descriptor */
  off_t size;       /* file size (use off_t to match stat.st_size) */
  uint8_t *content; /* if non-NULL contains loaded content; freed by
                       eiku_file_close */
  struct stat info; /* embedded stat to avoid additional allocation; valid after
                       eiku_file_load_info */
} t_file;

typedef enum e_file_mode {
  EIKU_FS_READ = 0,
  EIKU_FS_WRITE = 1,
} t_file_mode;

typedef struct s_dir {
  char *path;     /* owned by the caller/open: freed by eiku_dir_close */
  char **entries; /* array of newly-allocated strings; all freed by
                     eiku_dir_close */
  size_t count;   /* number of entries in `entries` (not NULL-terminated) */
} t_dir;

/// @brief Returns true of the string resolves to a file or directory.
/// @param path The potential file or directory
/// @return True if the file exists, false if not.
bool eiku_file_exists(const char *path);
/// @brief Opens the file, returning a t_file structure.
/// @param path The path to the file.
/// @param mode The mode to use when opening the file.
/// @return Returns \ref t_file or \ref NULL if the operation was unsuccessful.
t_file *eiku_file_open(const char *path, t_file_mode mode);
/// @brief Closes the file and frees any owned resources.
/// @details Calling this closes the underlying file descriptor (if open),
/// frees `file->path`, `file->content` and any other allocations, then frees
/// the `t_file` structure itself. After this call the pointer must not be
/// used.
/// @param file The file to close (may be NULL).
void eiku_file_close(t_file *file);

/// @brief Checks if the given path is a directory.
/// @param path The path to check.
/// @return True if the path is a directory, false otherwise.
bool eiku_is_directory(const char *path);
/// @brief Checks if the given path is a regular file.
/// @param path The path to check.
/// @return True if the path is a file, false otherwise.
bool eiku_is_file(const char *path);
/// @brief Checks if the given path is a symbolic link.
/// @param path The path to check.
/// @return True if the path is a symlink, false otherwise.
bool eiku_is_symlink(const char *path);
/// @brief Checks if the given path is readable.
/// @param path The path to check.
/// @return True if the path is readable, false otherwise.
bool eiku_is_readable(const char *path);
/// @brief Checks if the given path is writable.
/// @param path The path to check.
/// @return True if the path is writable, false otherwise.
bool eiku_is_writable(const char *path);
/// @brief Checks if the given path is executable.
/// @param path The path to check.
/// @return True if the path is executable, false otherwise.
bool eiku_is_executable(const char *path);
/// @brief Reads the full content of the file to \ref file->content
/// @param file The file to read from.
/// @warning Large files may cause problems, it is advertised to check the file
/// size beforehand.
/// @return True if the operation was a success, false otherwise.
bool eiku_file_load_content(t_file *file);
/// @brief Loads the stats of the file using stat(2) into \ref file->info and
/// fills in \ref file->size.
/// @param file The file to stat.
/// @return True if the operation was a success, false otherwise.
bool eiku_file_load_info(t_file *file);
/// @brief Opens a directory to get its children.
/// @param path The path to check.
/// @return Returns \ref t_dir or \ref NULL if the operation was unsuccessful.
t_dir *eiku_dir_open(const char *path);
/// @brief Closes the directory and frees associated resources.
/// @param dir The directory to close.
void eiku_dir_close(t_dir *dir);
/// @brief Checks if the given directory is empty.
/// @param path The path to the directory.
/// @return True if the directory is empty, false otherwise.
bool eiku_dir_is_empty(const char *path);
/// @brief Extracts the filename from a given path.
/// @param path The full path.
/// @return A newly allocated string containing the filename, or NULL on
/// failure.
char *eiku_get_filename(const char *path);
/// @brief Extracts the directory name from a given path.
/// @param path The full path.
/// @return A newly allocated string containing the directory name, or NULL on
/// failure.
char *eiku_get_dirname(const char *path);
/// @brief Extracts the file extension from a filename.
/// @param filename The filename.
/// @return A newly allocated string containing the extension (without the dot),
/// or NULL if no extension.
char *eiku_get_file_extension(const char *filename);
/// @brief Joins two paths into a single path.
/// @param path1 The first path.
/// @param path2 The second path.
/// @details If `path2` is absolute, a copy of `path2` is returned. The
/// returned string is newly allocated and must be freed by the caller.
/// @return A newly allocated string containing the joined path, or NULL on
/// failure.
char *eiku_join_paths(const char *path1, const char *path2);
/// @brief Normalizes a path by resolving '.' and '..' components.
/// @param path The path to normalize.
/// @return A newly allocated string containing the normalized path, or NULL on
/// failure.
char *eiku_normalize_path(const char *path);
/// @brief Converts a relative path to an absolute path.
/// @param path The path to convert.
/// @return A newly allocated string containing the absolute path, or NULL on
/// failure.
char *eiku_get_absolute_path(const char *path);
/// @brief Creates a new directory at the specified path.
/// @param path The path where to create the directory.
/// @return True if the operation was successful, false otherwise.
bool eiku_create_directory(const char *path);
/// @brief Removes the file at the specified path.
/// @param path The path to the file to remove.
/// @return True if the operation was successful, false otherwise.
bool eiku_remove_file(const char *path);
/// @brief Removes the directory at the specified path (must be empty).
/// @param path The path to the directory to remove.
/// @return True if the operation was successful, false otherwise.
bool eiku_remove_directory(const char *path);

/// @brief Reads up to n bytes from the file into the buffer.
/// @param file The file to read from.
/// @param buffer The buffer to store the read data.
/// @param n The number of bytes to read.
/// @return The number of bytes read, or -1 on error.
ssize_t eiku_file_read(t_file *file, uint8_t *buffer, size_t n);
/// @brief Writes up to n bytes from the buffer to the file.
/// @param file The file to write to.
/// @param buffer The buffer containing the data to write.
/// @param n The number of bytes to write.
/// @return The number of bytes written, or -1 on error.
ssize_t eiku_file_write(t_file *file, const uint8_t *buffer, size_t n);
/// @brief Repositions the file offset for `file` similarly to lseek(2).
/// @param file The file whose offset to change.
/// @param offset The offset value (meaning depends on `whence`).
/// @param whence One of SEEK_SET, SEEK_CUR or SEEK_END.
/// @return The resulting offset location as an off_t, or (off_t)-1 on error
/// (errno will be set).
off_t eiku_file_seek(t_file *file, off_t offset, int whence);