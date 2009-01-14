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

#ifndef PORTABLEXDR_TYPES_H
#define PORTABLEXDR_TYPES_H

/* Note that unlike Sun's RPC, we don't define all sorts of 'unusual'
 * types like u_char, quad_t, etc.  Instead we always use the
 * C99 types from <stdint.h> such as uint8_t and int64_t.
 */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Define the boolean type in the wire protocol. */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef int32_t bool_t;

/* Define the enumerated type in the wire protocol. */
typedef int32_t enum_t;

#ifdef __cplusplus
}
#endif

#endif /* PORTABLEXDR_TYPES_H */
