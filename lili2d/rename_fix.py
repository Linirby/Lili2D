import re

files = [
    "src/render/pipelines/world_2d_pipeline.cpp",
    "src/render/pipelines/ui_pipeline.cpp"
]

for filepath in files:
    with open(filepath, 'r') as f:
        content = f.read()
    
    # fix the struct member name
    content = content.replace('.vertex_a =', '.vertex_attributes =')

    with open(filepath, 'w') as f:
        f.write(content)

print("Fixing struct member name completed.")
