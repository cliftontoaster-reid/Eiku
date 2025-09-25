/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_normalize_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:43:39 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 13:54:43 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "res/filesystem.h"

#ifndef EIKU_PLATFORM_WINDOWS

/* Simple lexical normalization resolving '.' and '..', does not resolve
   symlinks and does not require the path to exist. */
char *eiku_normalize_path(const char *path) {
  if (!path) return NULL;
  bool absolute = (path[0] == '/');
  char *tmp = strdup(path);
  if (!tmp) return NULL;
  char **stack = NULL;
  size_t stack_len = 0;

  char *tok = strtok(tmp, "/");
  while (tok) {
    if (strcmp(tok, "") == 0 || strcmp(tok, ".") == 0) {
      /* skip */
    } else if (strcmp(tok, "..") == 0) {
      if (stack_len > 0) {
        free(stack[stack_len - 1]);
        stack_len--;
      } else if (!absolute) {
        /* preserve leading .. for relative paths */
        char **n = realloc(stack, (stack_len + 1) * sizeof(*stack));
        if (!n) {
          for (size_t i = 0; i < stack_len; ++i) free(stack[i]);
          free(stack);
          free(tmp);
          return NULL;
        }
        stack = n;
        stack[stack_len++] = strdup("..");
      }
    } else {
      char **n = realloc(stack, (stack_len + 1) * sizeof(*stack));
      if (!n) {
        for (size_t i = 0; i < stack_len; ++i) free(stack[i]);
        free(stack);
        free(tmp);
        return NULL;
      }
      stack = n;
      stack[stack_len++] = strdup(tok);
    }
    tok = strtok(NULL, "/");
  }

  /* build result */
  size_t needed = 1;         /* for NUL */
  if (absolute) needed += 1; /* leading slash */
  for (size_t i = 0; i < stack_len; ++i) needed += strlen(stack[i]) + 1;
  char *out = malloc(needed);
  if (!out) {
    for (size_t i = 0; i < stack_len; ++i) free(stack[i]);
    free(stack);
    free(tmp);
    return NULL;
  }
  char *p = out;
  if (absolute) *p++ = '/';
  for (size_t i = 0; i < stack_len; ++i) {
    size_t l = strlen(stack[i]);
    memcpy(p, stack[i], l);
    p += l;
    if (i + 1 < stack_len) *p++ = '/';
    free(stack[i]);
  }
  *p = '\0';

  free(stack);
  free(tmp);
  return out;
}

#endif
