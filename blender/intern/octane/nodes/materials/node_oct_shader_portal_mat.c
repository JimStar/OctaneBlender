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
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "../../../../source/blender/nodes/shader/node_shader_util.h"

static bNodeSocketTemplate sh_node_in[] = {
	{	SOCK_BOOLEAN,   1,  N_("Enabled"),	        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, PROP_NONE, SOCK_NO_INTERNAL_LINK},
	{	-1, 0, ""	}
};

static bNodeSocketTemplate sh_node_out[] = {
	{	SOCK_SHADER, 0, N_("OutMat")},
	{	-1, 0, ""	}
};

static int node_shader_gpu_oct_portal_mat(struct GPUMaterial *mat, struct bNode *UNUSED(node), struct bNodeExecData *UNUSED(execdata), struct GPUNodeStack *UNUSED(in), struct GPUNodeStack *out) {
    float rgb[4] = {0.1f, 0.1f, 0.1f, 0.5f};
    float roughness[4] = {0};
	return GPU_stack_link(mat, "node_bsdf_diffuse", 0, out, GPU_uniform(rgb), GPU_uniform(roughness), GPU_builtin(GPU_VIEW_NORMAL));
}

void register_node_type_sh_oct_portal_mat(void) {
	static bNodeType ntype;

    if(ntype.type != SH_NODE_OCT_PORTAL_MAT) node_type_base(&ntype, SH_NODE_OCT_PORTAL_MAT, "Octane Portal Material", NODE_CLASS_OCT_SHADER, 0);
	node_type_compatibility(&ntype, NODE_NEW_SHADING);
	node_type_socket_templates(&ntype, sh_node_in, sh_node_out);
	node_type_size(&ntype, 100, 160, 200);
	node_type_init(&ntype, 0);
	node_type_storage(&ntype, "", NULL, NULL);
	node_type_gpu(&ntype, node_shader_gpu_oct_portal_mat);
    ntype.update_internal_links = node_update_internal_links_default;

	nodeRegisterType(&ntype);
} /* register_node_type_sh_oct_portal_mat() */
