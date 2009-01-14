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

/* xdr_int8_t, xdr_uint8_t, xdr_int16_t, xdr_uint16_t, xdr_int32_t,
 * xdr_uint32_t, xdr_int64_t and xdr_uint64_t functions:
 *
 * Copyright (c) 1998, 1999, 2000, 2004, 2005 Free Software Foundation, Inc.
 * This file is part of the GNU C Library.
 * Contributed by Thorsten Kukuk <kukuk@vt.uni-paderborn.de>, 1998.
 *
 * (Under the same license as above).
 */

#ifndef PORTABLEXDR_XDR_H
#define PORTABLEXDR_XDR_H

#include <rpc/types.h>
#include <rpc/xdr_internal.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Please read the documentation that comes with PortableXDR,
 * otherwise you won't understand how to use the xdr_* conversion
 * functions.
 */
typedef bool_t (*xdrproc_t) ();
#define NULL_xdrproc_t NULL

/* XDR procedures for the basic types. */
static inline bool_t
xdr_void (XDR *xdrs ATTRIBUTE_UNUSED, void *vp ATTRIBUTE_UNUSED)
{
  return TRUE;
}

static inline bool_t
xdr_bool (XDR *xdrs, bool_t *p)
{
  int32_t i;
  switch (xdrs->x_op) {
  case XDR_ENCODE:
    i = *p ? TRUE : FALSE;
    return xdr_putlong (xdrs, &i);
  case XDR_DECODE:
    if (!xdr_getlong (xdrs, &i)) return FALSE;
    *p = i ? TRUE : FALSE;
    return TRUE;
  case XDR_FREE:
    return TRUE;
  default:
    return FALSE;
  }
}

/* XDR 64bit integers */
bool_t
xdr_int64_t (XDR *xdrs, int64_t *ip)
{
  int32_t t1, t2;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t1 = (int32_t) ((*ip) >> 32);
      t2 = (int32_t) (*ip);
      return (XDR_PUTINT32(xdrs, &t1) && XDR_PUTINT32(xdrs, &t2));
    case XDR_DECODE:
      if (!XDR_GETINT32(xdrs, &t1) || !XDR_GETINT32(xdrs, &t2))
        return FALSE;
      *ip = ((int64_t) t1) << 32;
      *ip |= (uint32_t) t2;	/* Avoid sign extension.  */
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 64bit unsigned integers */
bool_t
xdr_uint64_t (XDR *xdrs, uint64_t *uip)
{
  uint32_t t1;
  uint32_t t2;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t1 = (uint32_t) ((*uip) >> 32);
      t2 = (uint32_t) (*uip);
      return (XDR_PUTINT32 (xdrs, (int32_t *) &t1) &&
	      XDR_PUTINT32(xdrs, (int32_t *) &t2));
    case XDR_DECODE:
      if (!XDR_GETINT32(xdrs, (int32_t *) &t1) ||
	  !XDR_GETINT32(xdrs, (int32_t *) &t2))
        return FALSE;
      *uip = ((uint64_t) t1) << 32;
      *uip |= t2;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 32bit integers */
bool_t
xdr_int32_t (XDR *xdrs, int32_t *lp)
{
  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      return XDR_PUTINT32 (xdrs, lp);
    case XDR_DECODE:
      return XDR_GETINT32 (xdrs, lp);
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 32bit unsigned integers */
bool_t
xdr_uint32_t (XDR *xdrs, uint32_t *ulp)
{
  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      return XDR_PUTINT32 (xdrs, (int32_t *) ulp);
    case XDR_DECODE:
      return XDR_GETINT32 (xdrs, (int32_t *) ulp);
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 16bit integers */
bool_t
xdr_int16_t (XDR *xdrs, int16_t *ip)
{
  int32_t t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = (int32_t) *ip;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
	return FALSE;
      *ip = (int16_t) t;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 16bit unsigned integers */
bool_t
xdr_uint16_t (XDR *xdrs, uint16_t *uip)
{
  uint32_t ut;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      ut = (uint32_t) *uip;
      return XDR_PUTINT32 (xdrs, (int32_t *) &ut);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, (int32_t *) &ut))
	return FALSE;
      *uip = (uint16_t) ut;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 8bit integers */
bool_t
xdr_int8_t (XDR *xdrs, int8_t *ip)
{
  int32_t t;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      t = (int32_t) *ip;
      return XDR_PUTINT32 (xdrs, &t);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, &t))
	return FALSE;
      *ip = (int8_t) t;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

/* XDR 8bit unsigned integers */
bool_t
xdr_uint8_t (XDR *xdrs, uint8_t *uip)
{
  uint32_t ut;

  switch (xdrs->x_op)
    {
    case XDR_ENCODE:
      ut = (uint32_t) *uip;
      return XDR_PUTINT32 (xdrs, (int32_t *) &ut);
    case XDR_DECODE:
      if (!XDR_GETINT32 (xdrs, (int32_t *) &ut))
	return FALSE;
      *uip = (uint8_t) ut;
      return TRUE;
    case XDR_FREE:
      return TRUE;
    default:
      return FALSE;
    }
}

#if 0
static inline bool_t
xdr_float (XDR *xdrs, float *p)
{
  XXX;
}

static inline bool_t
xdr_double (XDR *xdrs, double *p)
{
  XXX;
}
#endif

/* Some very common aliases for the basic integer functions. */
#define xdr_int xdr_int32_t
#define xdr_u_int xdr_uint32_t
#define xdr_long xdr_int32_t
#define xdr_u_long xdr_uint32_t

#define xdr_short xdr_int16_t
#define xdr_u_short xdr_uint16_t

  /* NB: In PortableXDR, char is ALWAYS treated as signed octet. */
#define xdr_char xdr_int8_t
#define xdr_u_char xdr_uint8_t

/*
#define xdr_hyper xdr_int64_t
#define xdr_u_hyper xdr_uint64_t
#define xdr_quad xdr_int64_t
#define xdr_u_quad xdr_uint64_t
*/

/* Enumerations. */
static inline bool_t
xdr_enum (XDR *xdrs, enum_t *ep)
{
  return xdr_int32_t (xdrs, ep);
}

/* Union with discriminator. */
extern bool_t xdr_union (XDR *xdrs, enum_t *discrim, void *p, struct xdr_discrim *choices, xdrproc_t default_proc);

/* Variable-size array of arbitrary elements. */
extern bool_t xdr_array (XDR *xdrs, void *p, size_t num_elements, size_t max_elements, size_t element_size, xdrproc_t element_proc);

/* Fixed-size array of arbitrary elements. */
extern bool_t xdr_vector (XDR *xdrs, void *p, size_t num_elements, size_t element_size, xdrproc_t element_size);

/* Variable-size array of bytes. */
extern bool_t xdr_bytes (XDR *xdrs, char **bytes, size_t num_bytes, size_t max_bytes);

/* Fixed-size array of bytes. */
extern bool_t xdr_opaque (XDR *xdrs, void *p, size_t num_bytes);

/* Variable-size string (handled as normal C string in the program, but
 * serialized as length + bytes).
 */
extern bool_t xdr_string (XDR *xdrs, char **str, size_t max_bytes);

#define xdr_wrapstring(xdrs,str) (xdr_string ((xdrs),(str),~0))

#if 0
/* A reference is a pointer which cannot be NULL. */
static inline bool_t
xdr_reference (XDR *xdrs, void **p, size_t size, xdrproc_t proc)
{
  XXX
}

/* A pointer is a pointer to an object that can be NULL.  It is
 * serialized as a boolean (TRUE = not null, FALSE = null), followed
 * by the object.
 */
static inline bool_t
xdr_pointer (XDR *xdrs, void **p, size_t size, xdrproc_t proc)
{
  XXX
}
#endif

/* Free an XDR object (recursively). */
extern void xdr_free (xdrproc_t, void *);

/* Construct an XDR stream from an in-memory buffer. */
extern void xdrmem_create (XDR *xdrs, void *p, size_t size, enum xdr_op);

/* Construct an XDR stream from a FILE* or file descriptor.  Note
 * that neither of these will close the underlying file.
 */
extern void xdrstdio_create (XDR *xdrs, FILE *, enum xdr_op);
extern void xdrfd_create (XDR *xdrs, int fd, enum xdr_op);

/* PortableXDR variations which allow you to close the underlying
 * file after use.
 */
#define XDR_CLOSE_FILE 1

extern void xdrstdio_create2 (XDR *xdrs, FILE *, enum xdr_op, uint32_t flags);
extern void xdrfd_create2 (XDR *xdrs, int fd, enum xdr_op, uint32_t flags);

/*
  Does anyone ever use these?  Contributions welcome.
extern void xdrrec_create (...);
extern bool_t xdrrec_endofrecord (...);
extern bool_t xdrrec_skiprecord (...);
extern bool_t xdrrec_eof (...);
*/

#ifdef __cplusplus
}
#endif

#endif /* PORTABLEXDR_XDR_H */
