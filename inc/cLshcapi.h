/*
 * cLshcapi.h
 * Authors: Gokhan Poyraz <gokhan@kylone.com>
 *
 * Kylone Endpoint API for C implementation
 * Copyright (c) 2018, Kylone Technology International Ltd.
 * API Version 2.0.81
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
*/

#ifndef CLKYLONE_ENDPOINT_API_H_
#define CLKYLONE_ENDPOINT_API_H_

#define SHC_OK                   0     // calling function successful
#define SHC_FAIL                 -1    // calling function get failed
#define SHC_UNSUPPORTED_PLATFORM 1     // device or platform is not supported
#define SHC_L3_DENIED            2     // access through routed networks not allowed
#define SHC_DEVICE_DISABLED      3     // device status is disabled on server side

#define SHC_OPT_DOCACHE          1     // Store remote content as cache
#define SHC_OPT_DONOTCACHE       2     // Disable caching

#define CLSHC_INVOKE_CONFIGREADY    0
#define CLSHC_INVOKE_CONTENTREADY   1
#define CLSHC_INVOKE_CONNECTFAILED  2
#define CLSHC_INVOKE_PROGRESS       3
#define CLSHC_INVOKE_REMOTEMESSAGE  4

#define CLSHC_MESSAGE_BANNERTEXT    "import" // Update Message
#define CLSHC_MESSAGE_RESTART       "commit" // Restart Application
#define CLSHC_MESSAGE_SUSPEND       "kill"   // Suspend
#define CLSHC_MESSAGE_RESUME        "resume" // Resume
#define CLSHC_MESSAGE_REBOOTSYSTEM  "reboot" // Reboot System
#define CLSHC_MESSAGE_FWUPDATE      "clean"  // Update Firmware

typedef void (*fcLshcapi_configreadyCB)(const char *, void *);
typedef void (*fcLshcapi_contentreadyCB)(const char *, void *);
typedef void (*fcLshcapi_connectfailedCB)(int, void *);
typedef void (*fcLshcapi_progressCB)(int, void *);
typedef void (*fcLshcapi_remotemessageCB)(const char *, void *);

struct shcapi_mediaitem {
   const char *url;
   const unsigned char *data;
   unsigned long size;
};

#ifdef __cplusplus
extern "C" {
#endif
   extern const char *cLshcapi_getapiversion_alloc();
   extern int cLshcapi_create(int argc, char **argv, const char *cachedir, const char *iface);
   extern int cLshcapi_delete();
   extern int cLshcapi_setcallback(void *uptr, int cbid, ...);
   extern int cLshcapi_connect(const char *targethost, int cache);
   extern int cLshcapi_isconnecting();
   extern int cLshcapi_serverisready(const char *targethost, int timeout);
   extern const char *cLshcapi_getbannertext();
   extern const char *cLshcapi_getwoemodel_alloc();
   extern const char *cLshcapi_getconfigval(const char *tag);
   extern struct shcapi_mediaitem *cLshcapi_media_newitem();
   extern int cLshcapi_media_releaseitem(struct shcapi_mediaitem *item);
   extern int cLshcapi_media_loaditem(struct shcapi_mediaitem *item);
   extern void *cLshcapi_content_rootitem(const char *rootid);
   extern void *cLshcapi_content_categories(const char *rootid);
   extern void *cLshcapi_content_items(const char *rootid, const char *category);
   extern void *cLshcapi_content_item(const char *rootid, const char *category, const char *tag, const char *tagval);
   extern void *cLshcapi_content_root();
   extern int cLshcapi_node_childcount(void *item);
   extern void *cLshcapi_node_childfirst(void *item);
   extern void *cLshcapi_node_next(void *item);
   extern const char *cLshcapi_node_attribute(void *node, const char *tag);
   extern const char *cLshcapi_node_value(void *node);
   extern const char *cLshcapi_item_attribute(void *item, const char *tag);
   extern void cLshcapi_util_delaymillisecond(long ms);
   extern struct shcapi_mediaitem *cLshcapi_getcoverart(const char *targethost, const char *mediabasepath);
   extern const char *cLshcapi_drmquery_alloc(const char *qsys, const char *qtyp, const char *qmeth, const char *qpmid, const char *quuid, const char *qcode);
   extern const char *cLshcapi_drmquerydescr();
   extern const char *cLshcapi_drmtokenize_alloc(const char *src, const char *uuid);
#ifdef __cplusplus
}
#endif

#endif /*CLKYLONE_ENDPOINT_API_H_*/
