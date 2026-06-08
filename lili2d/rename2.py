import os
import re

files = [
    "src/render/pipelines/world_2d_pipeline.cpp",
    "src/render/pipelines/ui_pipeline.cpp"
]

for filepath in files:
    with open(filepath, 'r') as f:
        content = f.read()
    
    content = re.sub(r'\bvertex_attributes\b', 'vertex_a', content)
    content = re.sub(r'\bvertex_attrs\b', 'vertex_a', content)

    with open(filepath, 'w') as f:
        f.write(content)

print("Renaming vertex_attributes to vertex_a completed.")
