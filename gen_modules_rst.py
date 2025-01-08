import os

def generate_modules_rst(src_dir, output_file):
    with open(output_file, 'w') as f:
        f.write("Modules\n=======\n\n")
        for root, dirs, files in os.walk(src_dir):
            indent = "    " * (root.count(os.sep) - src_dir.count(os.sep))
            f.write(f"{indent}- {os.path.basename(root)}\n")
            for file in files:
                if file.endswith(('.h', '.cpp')):
                    f.write(f"{indent}    - {file}\n")

generate_modules_rst("src", "docs/source/modules.rst")