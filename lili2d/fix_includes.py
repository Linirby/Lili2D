import os
import re

src_dir = os.path.abspath('src')

def get_relative_include(current_file_path, include_path):
    # current_file_path is absolute path to the file doing the including
    # include_path is the string inside the quotes, e.g., "geometry/vec2.hpp"
    # We know include_path is relative to 'src'
    target_abs_path = os.path.join(src_dir, include_path)
    current_dir = os.path.dirname(current_file_path)
    
    # Calculate relative path from current_dir to target_abs_path
    rel_path = os.path.relpath(target_abs_path, current_dir)
    return rel_path

# Get all files in src
for root, dirs, files in os.walk(src_dir):
    for file in files:
        if file.endswith('.hpp') or file.endswith('.cpp'):
            file_path = os.path.join(root, file)
            with open(file_path, 'r') as f:
                content = f.read()
            
            # Find all #include "..."
            # We only replace if the included file exists in src/
            
            def repl(match):
                inc_str = match.group(1)
                # Check if it's one of our internal paths
                if os.path.exists(os.path.join(src_dir, inc_str)):
                    rel = get_relative_include(file_path, inc_str)
                    # Use forward slashes
                    rel = rel.replace(os.sep, '/')
                    return f'#include "{rel}"'
                return match.group(0)
            
            new_content = re.sub(r'#include\s+"([^"]+)"', repl, content)
            
            if new_content != content:
                with open(file_path, 'w') as f:
                    f.write(new_content)
                print(f"Updated {file_path}")

