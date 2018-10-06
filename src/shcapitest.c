/*
 * shcapitest.c
 * Authors: Gokhan Poyraz <gokhan@kylone.com>
 *
 * Kylone Endpoint API for C implementation
 * Copyright (c) 2018, Kylone Technology International Ltd.
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

#include <stdio.h>  //printf
#include <stdlib.h> //free
#include <string.h> //strcmp
#include <signal.h>

#include <cLshcapi.h>

static int app_signal_issued = 0;

struct shcapi_mediaitem *app_new_media_item(const char *url)
{
   struct shcapi_mediaitem *item = cLshcapi_media_newitem();
   item->url = url;
   if (cLshcapi_media_loaditem(item) == SHC_OK) {
      printf("  item loaded:\n    url: %s\n    sze: %lu\n", item->url, item->size);
   } else {
      printf("! unable to load media from url(%s)\n", url);
   }
   return item;
}

void app_set_main_items()
{
   const char *url = (const char *) 0;
   struct shcapi_mediaitem *item = (struct shcapi_mediaitem *) 0;

   printf("~ setting-up: logo\n");
   url = cLshcapi_getconfigval("logo");
   item = app_new_media_item(url);
   if (item->size) {
      // consume item->data
      printf("  main logo set\n");
   }
   cLshcapi_media_releaseitem(item);

   printf("~ setting-up: background\n");
   url = cLshcapi_getconfigval("bgnd");
   item = app_new_media_item(url);
   if (item->size) {
      // consume item->data
      printf("  main background set\n");
   }
   cLshcapi_media_releaseitem(item);
}

static void app_configreadyCB(const char *xml, void *userptr)
{
   printf("~ callback: config is ready\n");
   app_set_main_items();
}

static void app_contentreadyCB(const char *xml, void *userptr)
{
   void *rootnode, *rootitem, *category, *item, *attr;
   int itemnum;

   printf("~ callback: content is ready\n");

   printf("> walking on content;\n");
   rootnode = cLshcapi_content_root();
   printf("  number of item: %d\n", (rootnode != (void *) 0) ? cLshcapi_node_childcount(rootnode) : 0);
   rootitem = cLshcapi_node_childfirst(cLshcapi_content_root());
   while (rootitem != (void *) 0) {
      const char *rootid = cLshcapi_node_attribute(rootitem, "id");
      printf("    [%s], number of category: %d\n", (rootid ? rootid : "<>"), cLshcapi_node_childcount(rootitem));
      category = cLshcapi_node_childfirst(rootitem);
      while (category != (void *) 0) {
         const char *catid = cLshcapi_node_attribute(category, "id");
         const char *catrank = cLshcapi_node_attribute(category, "rank");
         printf("      category: [%s], rank: %s, number of items: %d\n", (catid ? catid : "<>"), (catrank ? catrank : "<>"), cLshcapi_node_childcount(category));
         item = cLshcapi_node_childfirst(category);
         itemnum = 0;
         while (item != (void *) 0) {
            printf("         [%d], number of attributes: %d\n", itemnum, cLshcapi_node_childcount(item));
            attr = cLshcapi_node_childfirst(item);
            while (attr != (void *) 0) {
               const char *attrid = cLshcapi_node_attribute(attr, "id");
               const char *attrval = cLshcapi_node_value(attr);
               printf("            %s \t: %s\n", (attrid ? attrid : "<>"), (attrval ? attrval : "<>"));
               attr = cLshcapi_node_next(attr);
            }
            item = cLshcapi_node_next(item);
            ++itemnum;
         }
         category = cLshcapi_node_next(category);
      }
      rootitem = cLshcapi_node_next(rootitem);
   }
   printf("- done\n");
}

static void app_connectfailedCB(int errcode, void *userptr)
{
   printf("~ callback: connect failed with reason(%d)\n", errcode);
}

static void app_progressCB(int progress, void *userptr)
{
   printf("~ callback: progress: %d%%\n", progress);
}

static void app_remotemessageCB(const char *msg, void *userptr)
{
   printf("~ got remote message: %s\n", msg);
   if (strcmp(msg, CLSHC_MESSAGE_BANNERTEXT) == 0) {
      printf("  update header text: '%s'\n", cLshcapi_getbannertext());
   } else
   if (strcmp(msg, CLSHC_MESSAGE_RESTART) == 0) {
      printf("  app restart\n");
   } else
   if (strcmp(msg, CLSHC_MESSAGE_SUSPEND) == 0) {
      printf("  app suspend\n");
   } else
   if (strcmp(msg, CLSHC_MESSAGE_RESUME) == 0) {
      printf("  app resume\n");
   } else
   if (strcmp(msg, CLSHC_MESSAGE_REBOOTSYSTEM) == 0) {
      printf("  sys reboot\n");
   } else
   if (strcmp(msg, CLSHC_MESSAGE_FWUPDATE) == 0) {
      printf("  sys firmware or app update\n");
   } else {
      printf("! unknown message\n");
   }
}

void app_example_usage_of_content(const char *targethost)
{
   void *cat, *item, *attr;
   const char *pstr;
   struct shcapi_mediaitem *coverart;

   printf("> listing root items;\n");
   item = cLshcapi_node_childfirst(cLshcapi_content_root());
   while (item != (void *) 0) {
      const char *rootid = cLshcapi_node_attribute(item, "id");
      printf("    %s\n", (rootid ? rootid : "<>"));
      item = cLshcapi_node_next(item);
   }
   printf("- done\n");

   printf("> listing categories in main-menu;\n");
   item = cLshcapi_content_categories("item");
   while (item != (void *) 0) {
      const char *catid = cLshcapi_node_attribute(item, "id");
      printf("    %s\n", (catid ? catid : "<>"));
      item = cLshcapi_node_next(item);
   }
   printf("- done\n");

   printf("> listing main-menu items in category '0';\n");
   item = cLshcapi_content_items("item", "0");
   while (item != (void *) 0) {
      const char *menuid = cLshcapi_item_attribute(item, "src");
      const char *menutext = cLshcapi_item_attribute(item, "txt");
      printf("    %s \t: %s\n", (menuid ? menuid : "<>"), (menutext ? menutext : "<>"));
      item = cLshcapi_node_next(item);
   }
   printf("- done\n");

   printf("> listing main-menu items in category 'info';\n");
   item = cLshcapi_content_items("item", "info");
   while (item != (void *) 0) {
      const char *menuid = cLshcapi_item_attribute(item, "src");
      const char *menutext = cLshcapi_item_attribute(item, "txt");
      printf("    %s \t: %s\n", (menuid ? menuid : "<>"), (menutext ? menutext : "<>"));
      item = cLshcapi_node_next(item);
   }
   printf("- done\n");

   printf("> listing attributes of menu-item 'tv';\n");
   item = cLshcapi_content_item("item", "0", "src", "tv");
   attr = cLshcapi_node_childfirst(item);
   while (attr != (void *) 0) {
      const char *attrid = cLshcapi_node_attribute(attr, "id");
      const char *attrval = cLshcapi_node_value(attr);
      printf("    %s \t: %s\n", (attrid ? attrid : "<>"), (attrval ? attrval : "<>"));
      attr = cLshcapi_node_next(attr);
   }
   printf("- done\n");

   printf("> getting only txt attribute of menu-item 'tv';\n");
   item = cLshcapi_content_item("item", "0", "src", "tv");
   pstr = cLshcapi_item_attribute(item, "txt");
   printf("    txt attribute of menu-item 'tv' is '%s'\n", (pstr ? pstr : "<>"));
   printf("- done\n");

   printf("> fetching cover-art image of first item in first music category;\n");
   cat = cLshcapi_content_categories("music");
   while (cat != (void *) 0) {
      item = cLshcapi_node_childfirst(cat);
      while (item != (void *) 0) {
         const char *mediabasepath = cLshcapi_item_attribute(item, "fbp");
         pstr = cLshcapi_item_attribute(item, "txt");
         coverart = cLshcapi_getcoverart(targethost, mediabasepath);
         if (coverart != (struct shcapi_mediaitem *) 0) {
            printf("    cover-art image of item '%s' : %lu byte(s)\n", pstr, coverart->size);
            cLshcapi_media_releaseitem(coverart);
            // only fist item is processed.
            break;
         } else {
            printf("  ! unable to fetch cover-art of item '%s'\n", pstr);
         }
         // continue to process next item
         item = cLshcapi_node_next(item);
      }
      // only fist category is processed.
      break;
      cat = cLshcapi_node_next(cat);
   }
   printf("- done\n");

   printf("> fetching tokenized URL of first item in first movie category;\n");
   cat = cLshcapi_content_categories("movie");
   while (cat != (void *) 0) {
      item = cLshcapi_node_childfirst(cat);
      while (item != (void *) 0) {
         const char *uuid = cLshcapi_item_attribute(item, "uuid");
         const char *src = cLshcapi_item_attribute(item, "src");
         const char *token = cLshcapi_drmtokenize_alloc(src, uuid);
         pstr = cLshcapi_item_attribute(item, "txt");
         printf("    tokenized URL of item '%s' is '%s'\n", pstr, token);
         free((void *)token);
         // only fist item is processed.
         break;
         // continue to process next item
         item = cLshcapi_node_next(item);
      }
      // only fist category is processed.
      break;
      cat = cLshcapi_node_next(cat);
   }
   printf("- done\n");

}

int app_set_callback_functions()
{
   if (cLshcapi_setcallback(0, CLSHC_INVOKE_CONFIGREADY, app_configreadyCB) != SHC_OK) {
      printf("! unable to set callback: app_configreadyCB\n\n");
      return 1;
   }
   if (cLshcapi_setcallback(0, CLSHC_INVOKE_CONTENTREADY, app_contentreadyCB) != SHC_OK) {
      printf("! unable to set callback: app_contentreadyCB\n\n");
      return 1;
   }
   if (cLshcapi_setcallback(0, CLSHC_INVOKE_CONNECTFAILED, app_connectfailedCB) != SHC_OK) {
      printf("! unable to set callback: app_connectfailedCB\n\n");
      return 1;
   }
   if (cLshcapi_setcallback(0, CLSHC_INVOKE_PROGRESS, app_progressCB) != SHC_OK) {
      printf("! unable to set callback: app_progressCB\n\n");
      return 1;
   }
   if (cLshcapi_setcallback(0, CLSHC_INVOKE_REMOTEMESSAGE, app_remotemessageCB) != SHC_OK) {
      printf("! unable to set callback: app_remotemessageCB\n\n");
      return 1;
   }
   return 0;
}

int app_wait_server_if_not_ready(const char *targethost, int timeout, int maxtry)
{
   int trycount = 0;
   while (cLshcapi_serverisready(targethost, timeout) != SHC_OK) {
      if (++trycount >= maxtry) {
         printf("! server is not ready, giving up\n\n");
         return 1;
      }
      printf("! waiting server to be ready(%d)\n", trycount);
      cLshcapi_util_delaymillisecond(1000);
   }
   printf("  server is ready\n");
   return 0;
}

int app_wait_connect_thread()
{
   while (cLshcapi_isconnecting() == SHC_OK)
      cLshcapi_util_delaymillisecond(1000);
   printf("> connect thread finished\n");
   return 0;
}

void app_print_api_version()
{
   const char *version = cLshcapi_getapiversion_alloc();
   printf("> Using API version %s\n", version);
   free((void *)version);
}

int app_terminate(int retcode)
{
   printf("> destroying API\n");
   cLshcapi_delete();
   printf("> done\n");
   return retcode;
}

static void breakhandler(int sig)
{
   printf(" signal\n");
   app_signal_issued = 1;
}

int main(int argc, char** argv)
{
   int docache = SHC_OPT_DOCACHE;
   const char *targethost = (const char *) 0;

   if (argc < 2) {
      printf("Usage: %s <targethost> [cache]\n", argv[0]);
      printf("cache can be 1 or 0, will do or wont do cache respectively (default 1)\n\n");
      return 1;
   } else {
      targethost = argv[1];
      if (argc > 2) {
         if (argv[2][0] == '0')
            docache = SHC_OPT_DONOTCACHE;
      }
   }

   app_print_api_version();
   printf("> target host: %s\n", targethost);
   printf("  caching is %s\n", docache == SHC_OPT_DOCACHE ? "enabled" : "disabled");
   /*
    * given cache folder should be exists, otherwise cLshcapi_create will return SHC_FAIL
    */
   printf("> creating API\n");
   if (cLshcapi_create(argc, argv, "/tmp", "eth0") != SHC_OK) {
      printf("! unable to create API\n\n");
      return app_terminate(1);
   }

   printf("> setting-up: callback functions\n");
   if (app_set_callback_functions() != 0)
      return app_terminate(1);

   printf("> checking server availability\n");
   if (app_wait_server_if_not_ready(targethost, 2, 10) != 0)
      return app_terminate(1);

   printf("> connecting...\n");
   if (cLshcapi_connect(targethost, SHC_OPT_DOCACHE) != SHC_OK) {
      printf("! could not connect to %s\n\n", targethost);
      return app_terminate(1);
   }

   printf("> connect thread launched, waiting to be finished\n");
   app_wait_connect_thread();

   app_example_usage_of_content(targethost);

   printf("> waiting command from the server\n  you may send manually to test it\n  hit ctrl+c when done\n");
   signal(SIGINT, breakhandler);
   while (!app_signal_issued)
      cLshcapi_util_delaymillisecond(500);

   return app_terminate(0);
}
