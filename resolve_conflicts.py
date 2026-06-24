import os
import re

def resolve_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except UnicodeDecodeError:
        return

    # Regex to match 7 or more <, =, and >
    pattern = re.compile(r'<{7,} HEAD.*?\n(.*?)\n?={7,}\n(.*?)\n?>{7,} dev.*?\n', re.DOTALL)
    
    def replacer(match):
        head_content = match.group(1)
        dev_content = match.group(2)
        if 'AABB2' in head_content or 'AABB3' in head_content:
            return head_content + '\n'
        else:
            return dev_content + '\n'

    new_content, count = pattern.subn(replacer, content)
    if count > 0:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"Resolved {count} conflicts in {filepath}")

for root, dirs, files in os.walk('.'):
    if '.git' in root: continue
    for file in files:
        filepath = os.path.join(root, file)
        resolve_file(filepath)
