/* PortableXDR - a free, portable XDR implementation.
 * Copyright (C) 2009 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#ifndef PORTABLEXDR_XDR_INTERNAL_H
#define PORTABLEXDR_XDR_INTERNAL_H

#include <rpc/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#ifndef __GNUC_PREREQ
#define __GNUC_PREREQ(maj,min) 0
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif
#else
#define ATTRIBUTE_UNUSED
#endif                          /* __GNUC__ */

/* Here we get into what could be termed the "internals" of XDR.  Most
 * callers can get away with only ever using the basic XDR types and
 * functions (see <rpc/xdr.h>.  Sometimes, particularly for
 * compatibility with Sun XDR code, you need to delve into the
 * internals.
 */

enum xdr_op {
  XDR_ENCODE,
  XDR_DECODE,
  XDR_FREE
};

/* Operations available on an XDR data stream (file, socket or memory
 * area).  Callers shouldn't normally use these, but Sun's XDR
 * implementation exposes these operations, so we do too.
 */
struct xdr_ops {
  /* Get/put "long" (ie. 32 bit quantity). */
  bool_t    (*x_getlong)  (XDR *, int32_t *);
  bool_t    (*x_putlong)  (XDR *, int32_t *);
  /* Get/put bytes. */
  bool_t    (*x_getbytes) (XDR *, void *, size_t);
  bool_t    (*x_putbytes) (XDR *, void *, size_t);
  /* Get or seek within the stream (offsets from beginning of stream). */
  off_t     (*x_getpostn) (XDR *);
  bool_t    (*x_setpostn) (XDR *, off_t);
  /* Returns a pointer to the next n bytes in the stream. */
  void *    (*x_inline)   (XDR *, size_t);
  /* Free the stream. */
  void      (*x_destroy)  (XDR *);
};

struct xdr {
  /* Calling code can read the operation field, but should not update it. */
  enum xdr_op x_op;		/* operation (encode/decode/free) */

  /* Calling code may use this pointer for any purpose, eg.
   * to point to their own data.
   */
  void *x_public;

  struct xdr_ops x_ops;

  /* The remaining fields are private and could change in any
   * future release.  Calling code should not use or modify them.
   */
  void *x__private;
};

/* Define wrapper functions around the x_ops. */
static inline bool_t
xdr_getlong (XDR *xdrs, int32_t *v)
{
  return xdrs->x_ops->x_getlong (xdrs, v);
}
static inline bool_t
xdr_putlong (XDR *xdrs, int32_t *v)
{
  return xdrs->x_ops->x_putlong (xdrs, v);
}
static inline bool_t
xdr_getbytes (XDR *xdrs, void *p, size_t len)
{
  return xdrs->x_ops->x_getbytes (xdrs, p, len);
}
static inline bool_t
xdr_putbytes (XDR *xdrs, void *p, size_t len)
{
  return xdrs->x_ops->x_putbytes (xdrs, p, len);
}
static inline off_t
xdr_getpos (XDR *xdrs)
{
  return xdrs->x_ops->x_getpostn (xdrs);
}
static inline bool_t
xdr_setpos (XDR *xdrs, off_t v)
{
  return xdrs->x_ops->x_setpostn (xdrs, v);
}
static inline void 
xdr_inline (XDR *xdrs, size_t len)
{
  return xdrs->x_ops->x_inline (xdrs, len);
}
static inline void
xdr_destroy (XDR *xdrs)
{
  return xdrs->x_ops->x_destroy (xdrs);
}

/* For compatibility with Sun XDR. */
#define XDR_GETLONG  xdr_getlong
#define XDR_PUTLONG  xdr_putlong
#define XDR_GETBYTES xdr_getbytes
#define XDR_PUTBYTES xdr_putbytes
#define XDR_GETPOS   xdr_getpos
#define XDR_SETPOS   xdr_setpos
#define XDR_INLINE   xdr_inline
#define XDR_DESTROY  xdr_destroy

/* Also seen in the wild ... */
#define XDR_GETINT32 xdr_getlong
#define XDR_PUTINT32 xdr_putlong

/* These are the "inline" versions of the macros.  These are used
 * in SunRPC for some primitive optimizations.  For example, suppose
 * you have to parse 4 integers from the stream.  You could
 * optimize using these macros by doing:
 *
 *   if (xdr->x_op == XDR_DECODE) {
 *     void *buf = xdr_inline (xdr, 4 * BYTES_PER_XDR_UNIT);
 *     if (buf) {
 *       i0 = IXDR_GET_LONG (buf); // NB. Macro autoincrements buf.
 *       i1 = IXDR_GET_LONG (buf);
 *       i2 = IXDR_GET_LONG (buf);
 *       i3 = IXDR_GET_LONG (buf);
 *       return TRUE;
 *     }
 *   }
 *   else if (xdr->x_op == XDR_ENCODE) {
 *     // Similar code required for encoding.
 *   }
 *   // Fallback code
 *   if (!xdr_int (xdr, &i0)) return FALSE;
 *   if (!xdr_int (xdr, &i1)) return FALSE;
 *   if (!xdr_int (xdr, &i2)) return FALSE;
 *   if (!xdr_int (xdr, &i3)) return FALSE;
 *   return TRUE;
 *
 * Note that you have to fallback in the case when xdr_inline
 * returns NULL.  This is NOT an error case.
 *
 * Whether this optimization is really worth it or not is left as
 * an exercise in benchmarking.  In any case, PortableXDR's rpcgen
 * does NOT perform this optimization.
 */

#define BYTES_PER_XDR_UNIT 4

#define IXDR_GET_LONG(buf) ((int32_t) ntohl (*((int32_t *)(buf))++))
#define IXDR_GET_BOOL(buf) ((bool_t) IXDR_GET_LONG ((buf)))
#define IXDR_GET_ENUM(buf,type) ((type) IXDR_GET_LONG ((buf)))
#define IXDR_GET_U_LONG(buf) ((uint32_t) IXDR_GET_LONG ((buf)))
#define IXDR_GET_SHORT(buf) ((int16_t) IXDR_GET_LONG ((buf)))
#define IXDR_GET_U_SHORT(buf) ((uint16_t) IXDR_GET_LONG ((buf)))
#define IXDR_GET_INT32 IXDR_GET_LONG

#define IXDR_PUT_LONG(buf,v) ((*((int32_t *)(buf))++) = htonl ((v)))
#define IXDR_PUT_BOOL(buf,v) IXDR_PUT_LONG((buf), (int32_t) (v))
#define IXDR_PUT_ENUM(buf,v) IXDR_PUT_LONG((buf), (int32_t) (v))
#define IXDR_PUT_U_LONG(buf,v) IXDR_PUT_LONG((buf), (int32_t) (v))
#define IXDR_PUT_SHORT(buf,v) IXDR_PUT_LONG((buf), (int32_t) (v))
#define IXDR_PUT_U_SHORT(buf,v) IXDR_PUT_LONG((buf), (int32_t) (v))
#define IXDR_PUT_INT32 IXDR_PUT_LONG

#ifdef __cplusplus
}
#endif

#endif /* PORTABLEXDR_XDR_INTERNAL_H */
