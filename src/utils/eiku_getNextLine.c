/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_getNextLine.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 21:58:42 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/25 22:03:40 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define EIKU_GNL_BUF 256

static char *append_bytes(char *buf, size_t buf_len, const char *add,
                          size_t add_len) {
  char *n = realloc(buf, buf_len + add_len + 1);
  if (!n) {
    free(buf);
    return NULL;
  }
  memcpy(n + buf_len, add, add_len);
  n[buf_len + add_len] = '\0';
  return n;
}

EIKU_API char *eiku_getNextLine(int fd) {
  if (fd < 0) return NULL;

  /* Static buffer that retains unread bytes between calls for a single fd. */
  static int s_fd = -1;
  static char s_buf[EIKU_GNL_BUF];
  static size_t s_start = 0; /* offset of first valid byte in s_buf */
  static size_t s_len = 0;   /* number of valid bytes starting at s_start */

  ssize_t r;
  char *out = NULL;
  size_t out_len = 0;

  /* If caller switched fd, reset static buffer state */
  if (s_fd != fd) {
    s_fd = fd;
    s_start = 0;
    s_len = 0;
  }

  for (;;) {
    /* First consume any bytes already in the static buffer */
    if (s_len > 0) {
      size_t i;
      for (i = 0; i < s_len; ++i) {
        if (s_buf[s_start + i] == '\n') {
          /* Found newline inside retained buffer */
          size_t take = i + 1;
          char *n = append_bytes(out, out_len, s_buf + s_start, take);
          if (!n) return NULL;
          out = n;
          out_len += take;
          s_start += take;
          s_len -= take;
          if (s_len == 0) s_start = 0;
          return out;
        }
      }
      /* no newline in buffer, append all and clear it */
      char *n = append_bytes(out, out_len, s_buf + s_start, s_len);
      if (!n) return NULL;
      out = n;
      out_len += s_len;
      s_start = 0;
      s_len = 0;
    }

    /* Read more data into static buffer */
    r = read(fd, s_buf, EIKU_GNL_BUF);
    if (r < 0) {
      if (errno == EINTR) continue;
      free(out);
      return NULL;
    }
    if (r == 0) break; /* EOF */
    s_start = 0;
    s_len = (size_t)r;
    /* Loop again to process freshly read bytes */
  }

  if (out_len == 0) {
    free(out);
    return NULL; /* EOF and nothing read */
  }

  return out;
}