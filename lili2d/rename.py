import os
import glob

replacements = {
    "color_target_info": "color_ti",
    "vertex_create_info": "vertex_ci",
    "fragment_create_info": "fragment_ci",
    "vertices_buffer_create_info": "vertices_bci",
    "indices_buffer_create_info": "indices_bci",
    "transfer_buffer_info": "transfer_bi",
    "sampler_info": "sampler_i",
    "vertex_binding": "vertex_b",
    "index_binding": "index_b",
    "texture_sampler_binding": "texture_sb",
    "vertex_buffer_desc": "vertex_bd",
    "color_target_desc": "color_td",
    "create_info": "ci"
}

# files to update
files_to_update = [
    "src/render/renderer.cpp",
    "src/render/core/shader.cpp",
    "src/render/core/gpu_mesh.cpp",
    "src/render/core/texture.cpp",
    "src/render/passes/world_2d_pass.cpp",
    "src/render/passes/ui_pass.cpp",
    "src/render/pipelines/world_2d_pipeline.cpp",
    "src/render/pipelines/ui_pipeline.cpp"
]

for filepath in files_to_update:
    with open(filepath, 'r') as f:
        content = f.read()
    
    for old, new in replacements.items():
        # A simple replacement is mostly safe here because these are unique variable names
        # But to be safer, we can enforce word boundaries.
        import re
        content = re.sub(r'\b' + old + r'\b', new, content)

    with open(filepath, 'w') as f:
        f.write(content)

print("Renaming completed.")
