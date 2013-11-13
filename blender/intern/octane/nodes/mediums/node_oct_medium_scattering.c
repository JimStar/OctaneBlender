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
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Robin Allen
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "../../../../source/blender/nodes/shader/node_shader_util.h"

static bNodeSocketTemplate sh_node_in[] = {
	{SOCK_FLOAT,     1,  N_("Absorption"),  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{SOCK_FLOAT,     1,  N_("Scattering"),  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{SOCK_FLOAT,     1,  N_("Phase"),       0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f},
	{SOCK_SHADER,    1,  N_("Emission")},
	{SOCK_FLOAT,     1,  N_("Scale"),       100.0f, 0.0f, 0.0f, 0.0f, 0.001f, 10000.0f},
	{-1, 0, ""}
};

static bNodeSocketTemplate sh_node_out[] = {
	{SOCK_SHADER, 0, N_("OutTex")},
	{-1, 0, ""}
};

void register_node_type_medium_oct_scattering(void) {
	static bNodeType ntype;

    if(ntype.type != SH_NODE_OCT_SCATTER_MED) node_type_base(&ntype, SH_NODE_OCT_SCATTER_MED, "Octane Scattering Medium", NODE_CLASS_OCT_MEDIUM, NODE_OPTIONS);
    node_type_compatibility(&ntype, NODE_NEW_SHADING);
	node_type_socket_templates(&ntype, sh_node_in, sh_node_out);
	node_type_size(&ntype, 100, 60, 150);
	node_type_init(&ntype, 0);
	node_type_exec(&ntype, 0, 0, 0);
	
	nodeRegisterType(&ntype);
} /* register_node_type_medium_oct_scattering() */