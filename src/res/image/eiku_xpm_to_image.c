/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_xpm_to_image.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+       +#+ */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 22:30:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/25 22:30:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/core.h"
#include "core/platform.h"
#include "res/image.h"

/* Minimal XPM parser: supports formats like
 * header: "width height ncolors chars_per_pixel"
 * color lines: "<key> c #RRGGBB" or "<key> c None"
 * pixel lines: each row is width * cpp characters
 */

static int parse_hex_color(const char *s) {
  // Expecting #RRGGBB (6 hex digits)
  if (!s) return -1;
  if (s[0] != '#') return -1;
  size_t len = strlen(s);
  if (len != 7) return -1;
  unsigned int v = 0;
  for (int i = 1; i < 7; ++i) {
    char c = s[i];
    unsigned int d;
    if (c >= '0' && c <= '9')
      d = c - '0';
    else if (c >= 'a' && c <= 'f')
      d = 10 + (c - 'a');
    else if (c >= 'A' && c <= 'F')
      d = 10 + (c - 'A');
    else
      return -1;
    v = (v << 4) | d;
  }
  return (int)v;
}

typedef struct s_xpm_color {
  char *key; /* cpp chars, null-terminated */
  int color; /* 0xRRGGBB or 0 for None */
} t_xpm_color;

/* cleanup helper: destroy image (if any), free color table and return NULL */
static t_eiku_image *xpm_cleanup(t_eiku_context *ctx, t_eiku_image *img,
                                 t_xpm_color *ct, int ncolors) {
  if (img) eiku_destroy_image(ctx, img);
  if (ct) {
    for (int i = 0; i < ncolors; ++i) {
      if (ct[i].key) free(ct[i].key);
    }
    free(ct);
  }
  return NULL;
}

EIKU_API t_eiku_image *eiku_xpm_to_image(t_eiku_context *ctx, char **xpm_data) {
  if (!ctx || !xpm_data) return NULL;

  /* Read header */
  const char *hdr = xpm_data[0];
  if (!hdr) return NULL;

  int w = 0, h = 0, ncolors = 0, cpp = 0;
  if (sscanf(hdr, "%d %d %d %d", &w, &h, &ncolors, &cpp) != 4) return NULL;
  if (w <= 0 || h <= 0 || ncolors <= 0 || cpp <= 0) return NULL;

  /* Allocate color table */
  t_xpm_color *ct = calloc((size_t)ncolors, sizeof(*ct));
  if (!ct) return NULL;

  /* Parse color definitions */
  for (int i = 0; i < ncolors; ++i) {
    const char *line = xpm_data[1 + i];
    if (!line) return xpm_cleanup(ctx, NULL, ct, ncolors);

    /* key is first cpp characters */
    size_t linelen = strlen(line);
    if ((size_t)cpp > linelen) return xpm_cleanup(ctx, NULL, ct, ncolors);
    ct[i].key = malloc((size_t)cpp + 1);
    if (!ct[i].key) return xpm_cleanup(ctx, NULL, ct, ncolors);
    memcpy(ct[i].key, line, (size_t)cpp);
    ct[i].key[cpp] = '\0';

    /* find the 'c' token that designates the color */
    const char *p = line + cpp;
    const char *cpos = NULL;
    while (*p) {
      /* skip whitespace */
      while (*p && isspace((unsigned char)*p)) ++p;
      if (!*p) break;
      if (*p == 'c' && (p[1] == '\0' || isspace((unsigned char)p[1]))) {
        cpos = p + 1;
        break;
      }
      /* skip token */
      while (*p && !isspace((unsigned char)*p)) ++p;
    }
    if (!cpos) {
      /* no explicit color; treat as black */
      ct[i].color = 0x000000;
      continue;
    }
    /* skip spaces after 'c' */
    while (*cpos && isspace((unsigned char)*cpos)) ++cpos;
    if (!*cpos) {
      ct[i].color = 0x000000;
      continue;
    }
    /* next token is color spec */
    char coltok[64];
    size_t k = 0;
    while (*cpos && !isspace((unsigned char)*cpos) && k + 1 < sizeof(coltok)) {
      coltok[k++] = *cpos++;
    }
    coltok[k] = '\0';
    if (k == 0) {
      ct[i].color = 0x000000;
    } else if (strcmp(coltok, "None") == 0) {
      ct[i].color =
          0x000000; /* treat transparent as black (no alpha support) */
    } else {
      int c = parse_hex_color(coltok);
      if (c < 0) {
        /* unsupported color spec (names) -> fail */
        return xpm_cleanup(ctx, NULL, ct, ncolors);
      }
      ct[i].color = c;
    }
  }

  /* Create image */
  t_eiku_image *img = eiku_new_image(ctx, w, h);
  if (!img) return xpm_cleanup(ctx, NULL, ct, ncolors);

  /* Parse pixel rows */
  int pixel_start = 1 + ncolors;
  for (int y = 0; y < h; ++y) {
    const char *row = xpm_data[pixel_start + y];
    if (!row) {
      return xpm_cleanup(ctx, img, ct, ncolors);
    }
    size_t rowlen = strlen(row);
    if (rowlen < (size_t)w * (size_t)cpp) {
      return xpm_cleanup(ctx, img, ct, ncolors);
    }
    for (int x = 0; x < w; ++x) {
      const char *kptr = row + ((size_t)x * (size_t)cpp);
      /* find color in table */
      int found = 0;
      int color = 0x000000;
      for (int ci = 0; ci < ncolors; ++ci) {
        if (strncmp(kptr, ct[ci].key, (size_t)cpp) == 0) {
          color = ct[ci].color;
          found = 1;
          break;
        }
      }
      if (!found) {
        /* unknown key -> use black */
        color = 0x000000;
      }
      if (eiku_image_pixel_put(img, x, y, color) != EIKU_SUCCESS) {
        return xpm_cleanup(ctx, img, ct, ncolors);
      }
    }
  }

  /* free color table keys */
  for (int i = 0; i < ncolors; ++i) free(ct[i].key);
  free(ct);

  return img;
}
