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
	{SOCK_FLOAT,     1,  N_("Efficiency"),      0.025f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{SOCK_FLOAT,     1,  N_("Power"),           100.0f, 0.0f, 0.0f, 0.0f, 0.001f, 100000.0f},
	{SOCK_FLOAT,     1,  N_("Distribution"),    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{SOCK_VECTOR,    1,  N_("Orientation"),     0.0f, 0.0f, 0.0f, 0.0f, -180.0f, 180.0f, PROP_DIRECTION},
	{SOCK_FLOAT,     1,  N_("Sampling Rate"),   1.0f, 0.0f, 0.0f, 0.0f, 0.001f, 10000.0f},
	{-1, 0, ""}
};

static bNodeSocketTemplate sh_node_out[] = {
	{SOCK_SHADER, 0, N_("OutTex")},
	{-1, 0, ""}
};

void register_node_type_emission_oct_texture(void) {
	static bNodeType ntype;

	if(ntype.type != SH_NODE_OCT_TEXTURE_EMI) node_type_base(&ntype, SH_NODE_OCT_TEXTURE_EMI, "Octane Texture Emission", NODE_CLASS_OCT_EMISSION, NODE_OPTIONS);
    node_type_compatibility(&ntype, NODE_NEW_SHADING);
	node_type_socket_templates(&ntype, sh_node_in, sh_node_out);
	node_type_size(&ntype, 100, 60, 150);
	node_type_init(&ntype, 0);
	node_type_exec(&ntype, 0, 0, 0);
	
	nodeRegisterType(&ntype);
} /* register_node_type_emission_oct_texture() */