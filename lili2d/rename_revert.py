import re

files_to_update = {
    "src/render/passes/world_2d_pass.cpp": [
        (r'\bvertex_b\b', 'vertex_binding'),
        (r'\bindex_b\b', 'index_binding')
    ],
    "src/render/passes/ui_pass.cpp": [
        (r'\bvertex_b\b', 'vertex_binding'),
        (r'\bindex_b\b', 'index_binding')
    ],
    "src/render/core/texture.cpp": [
        (r'\bsampler_i\b', 'sampler_info')
    ]
}

for filepath, replacements in files_to_update.items():
    with open(filepath, 'r') as f:
        content = f.read()
    
    for old, new in replacements:
        content = re.sub(old, new, content)

    with open(filepath, 'w') as f:
        f.write(content)

print("Reverted 1-letter abbreviations.")
