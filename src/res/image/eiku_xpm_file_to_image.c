/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::  */
/*   eiku_xpm_file_to_image.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 22:40:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/25 22:40:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/core.h"
#include "core/platform.h"
#include "res/image.h"

#ifdef EIKU_PLATFORM_LINUX

/* Read an XPM file (text file containing "..." C-style string literals per
 * row). We extract the quoted strings into a NULL-terminated char** array
 * and call eiku_xpm_to_image. This keeps the parsing logic in the in-memory
 * loader.
 */

static char *extract_quoted(const char *line) {
  if (!line) return NULL;
  const char *s = strchr(line, '"');
  if (!s) return NULL;
  ++s;
  const char *e = strchr(s, '"');
  if (!e) return NULL;
  size_t len = (size_t)(e - s);
  char *out = malloc(len + 1);
  if (!out) return NULL;
  memcpy(out, s, len);
  out[len] = '\0';
  return out;
}

EIKU_API t_eiku_image *eiku_xpm_file_to_image(t_eiku_context *ctx,
                                              const char *filename) {
  if (!ctx || !filename) return NULL;

  FILE *f = fopen(filename, "r");
  if (!f) return NULL;

  /* We'll collect lines that contain quoted strings into a dynamic array. */
  char **lines = NULL;
  size_t alloc = 0;
  size_t used = 0;
  char buf[1024];
  while (fgets(buf, sizeof(buf), f)) {
    char *q = extract_quoted(buf);
    if (q) {
      if (used + 1 >= alloc) {
        size_t n = alloc ? alloc * 2 : 16;
        char **tmp = realloc(lines, n * sizeof(*lines));
        if (!tmp) {
          free(q);
          for (size_t i = 0; i < used; ++i) free(lines[i]);
          free(lines);
          fclose(f);
          return NULL;
        }
        lines = tmp;
        alloc = n;
      }
      lines[used++] = q;
    }
  }

  fclose(f);

  if (!lines) return NULL;

  /* NULL-terminate the array */
  lines = realloc(lines, (used + 1) * sizeof(*lines));
  if (!lines) return NULL;
  lines[used] = NULL;

  t_eiku_image *img = eiku_xpm_to_image(ctx, lines);

  /* free temporary strings */
  for (size_t i = 0; i < used; ++i) free(lines[i]);
  free(lines);

  return img;
}

#else

EIKU_API t_eiku_image *eiku_xpm_file_to_image(t_eiku_context *ctx,
                                              const char *filename) {
  (void)ctx;
  (void)filename;
  return NULL;
}

#endif
