/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2008 Blender Foundation.
 * All rights reserved.
 *
 * 
 * Contributor(s): Blender Foundation
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/editors/space_mat_livedb/space_mat_livedb.c
 *  \ingroup spmat_livedb
 */

#include "mat_livedb_intern.h"

#include <string.h>

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"
#include "BKE_context.h"
#include "BKE_screen.h"
#include "ED_screen.h"
#include "WM_api.h"
#include "WM_types.h"
#include "BIF_gl.h"
#include "UI_resources.h"
#include "UI_view2d.h"


/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Init main area */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_main_area_init(wmWindowManager *wm, ARegion *ar)
{
    wmKeyMap *keymap;
    
    /* make sure we keep the hide flags */
    ar->v2d.scroll |= (V2D_SCROLL_RIGHT | V2D_SCROLL_BOTTOM);
    ar->v2d.scroll &= ~(V2D_SCROLL_LEFT | V2D_SCROLL_TOP);	/* prevent any noise of past */
    ar->v2d.scroll |= V2D_SCROLL_HORIZONTAL_HIDE;
    ar->v2d.scroll |= V2D_SCROLL_VERTICAL_HIDE;

    ar->v2d.align = (V2D_ALIGN_NO_NEG_X | V2D_ALIGN_NO_POS_Y);
    ar->v2d.keepzoom = (V2D_LOCKZOOM_X | V2D_LOCKZOOM_Y | V2D_LIMITZOOM | V2D_KEEPASPECT);
    ar->v2d.keeptot = V2D_KEEPTOT_STRICT;
    ar->v2d.minzoom = ar->v2d.maxzoom = 1.0f;

    UI_view2d_region_reinit(&ar->v2d, V2D_COMMONVIEW_LIST, ar->winx, ar->winy);
    
    /* own keymap */
    keymap = WM_keymap_find(wm->defaultconf, "MatLiveDB", SPACE_MAT_LIVEDB, 0);
    /* don't pass on view2d mask, it's always set with scrollbar space, hide fails */
    WM_event_add_keymap_handler_bb(&ar->handlers, keymap, NULL, &ar->winrct);
} /* mat_livedb_main_area_init() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Free main area */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_main_area_free(ARegion *UNUSED(ar))
{
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Draw main area */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_main_area_draw(const bContext *C, ARegion *ar)
{
    View2D          *v2d = &ar->v2d;
    View2DScrollers *scrollers;
    
    /* clear */
    UI_ThemeClearColor(TH_BACK);
    glClear(GL_COLOR_BUFFER_BIT);
    
    draw_mat_livedb(C);
    
    /* reset view matrix */
    UI_view2d_view_restore(C);
    
    /* scrollers */
    scrollers = UI_view2d_scrollers_calc(C, v2d, V2D_ARG_DUMMY, V2D_ARG_DUMMY, V2D_ARG_DUMMY, V2D_ARG_DUMMY);
    UI_view2d_scrollers_draw(C, v2d, scrollers);
    UI_view2d_scrollers_free(scrollers);
} /* mat_livedb_main_area_draw() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Main area listener */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_main_area_listener(bScreen *UNUSED(sc), ScrArea *UNUSED(sa), ARegion *ar, wmNotifier *wmn)
{
    switch (wmn->category) {
		case NC_SPACE:
			if (wmn->data == ND_SPACE_MAT_LIVEDB)
				ED_region_tag_redraw(ar);
			break;
        default:
            break;
    }
} /* mat_livedb_main_area_listener() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Init LiveDB header area region */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_header_area_init(wmWindowManager *UNUSED(wm), ARegion *ar)
{
    ED_region_header_init(ar);
} /* mat_livedb_header_area_init() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Free LiveDB header area region */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_header_area_free(ARegion *UNUSED(ar))
{
} /* mat_livedb_header_area_free() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Draw LiveDB header area region */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_header_area_draw(const bContext *C, ARegion *ar)
{
    ED_region_header(C, ar);
} /* mat_livedb_header_area_draw() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* LiveDB header area listener */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_header_area_listener(bScreen *UNUSED(sc), ScrArea *UNUSED(sa), ARegion *ar, wmNotifier *wmn)
{
    switch (wmn->category) {
        case NC_SCENE:
            if (wmn->data == ND_OB_ACTIVE)
                ED_region_tag_redraw(ar);
            break;
		case NC_SPACE:
			if (wmn->data == ND_SPACE_MAT_LIVEDB)
				ED_region_tag_redraw(ar);
			break;
        default:
            break;
    }
} /* mat_livedb_header_area_listener() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* New LiveDB space */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static SpaceLink *mat_livedb_new(const bContext *UNUSED(C))
{
    ARegion  *ar;
    SpaceLDB *smat_livedb;

    smat_livedb = MEM_callocN(sizeof(SpaceLDB), "initmat_livedb");
    smat_livedb->spacetype = SPACE_MAT_LIVEDB;
    strcpy(smat_livedb->server_address, "127.0.0.1");
    
    /* header */
    ar = MEM_callocN(sizeof(ARegion), "header for mat_livedb");
    BLI_addtail(&smat_livedb->regionbase, ar);
    ar->regiontype = RGN_TYPE_HEADER;
    ar->alignment = RGN_ALIGN_BOTTOM;
    
    /* main area */
    ar = MEM_callocN(sizeof(ARegion), "main area for mat_livedb");
    BLI_addtail(&smat_livedb->regionbase, ar);
    ar->regiontype = RGN_TYPE_WINDOW;
    
    return (SpaceLink *)smat_livedb;
} /* mat_livedb_new() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Free LiveDB space */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_free(SpaceLink *sl)
{
    SpaceLDB *slivedb = (SpaceLDB *)sl;
    
    mat_livedb_cleanup_tree(slivedb);
} /* mat_livedb_free() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Init LiveDB space */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static void mat_livedb_init(wmWindowManager *UNUSED(wm), ScrArea *UNUSED(sa))
{	
} /* mat_livedb_init() */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* Init LiveDB space */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
static SpaceLink *mat_livedb_duplicate(SpaceLink *sl)
{
    SpaceLDB *smat_livedb  = (SpaceLDB *)sl;
    SpaceLDB *smat_livedbn = MEM_dupallocN(smat_livedb);

    smat_livedbn->tree.first = smat_livedbn->tree.last = 0;
    smat_livedbn->treestore  = 0;
    
    return (SpaceLink *)smat_livedbn;
} /* mat_livedb_duplicate */

/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
/* LiveDB space. Only called once, from space_api/spacetypes.c */
/* /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */
void ED_spacetype_mat_livedb(void)
{
    SpaceType   *st = MEM_callocN(sizeof(SpaceType), "spacetype time");
    ARegionType *art;
    
    st->spaceid = SPACE_MAT_LIVEDB;
    strncpy(st->name, "Materials LiveDB", BKE_ST_MAXNAME);
    
    st->new             = mat_livedb_new;
    st->free            = mat_livedb_free;
    st->init            = mat_livedb_init;
    st->duplicate       = mat_livedb_duplicate;
    st->operatortypes   = mat_livedb_operatortypes;
    st->keymap          = mat_livedb_keymap;
    st->dropboxes       = 0;//mat_livedb_dropboxes;
    
    /* regions: main window */
    art = MEM_callocN(sizeof(ARegionType), "spacetype time region");
    art->regionid   = RGN_TYPE_WINDOW;
    art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D;
    art->init       = mat_livedb_main_area_init;
    art->draw       = mat_livedb_main_area_draw;
    art->free       = mat_livedb_main_area_free;
    art->listener   = mat_livedb_main_area_listener;
    BLI_addhead(&st->regiontypes, art);
    
    /* regions: header */
    art = MEM_callocN(sizeof(ARegionType), "spacetype time header region");
    art->regionid   = RGN_TYPE_HEADER;
    art->prefsizey  = HEADERY;
    art->keymapflag = ED_KEYMAP_UI | ED_KEYMAP_VIEW2D | ED_KEYMAP_FRAMES | ED_KEYMAP_HEADER;
    art->init       = mat_livedb_header_area_init;
    art->draw       = mat_livedb_header_area_draw;
    art->free       = mat_livedb_header_area_free;
    art->listener   = mat_livedb_header_area_listener;
    BLI_addhead(&st->regiontypes, art);
    
    BKE_spacetype_register(st);
} /* ED_spacetype_mat_livedb() */
