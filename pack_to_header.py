import os
import urllib.request

files = {
    'index_html': 'index.html',
    'style_css': 'style.css',
    'main_js': 'main.js',
    'nipplejs_min_js': 'nipplejs.min.js'
}

# Auto-download nipplejs for offline use
if not os.path.exists('nipplejs.min.js'):
    print("Downloading nipplejs.min.js from CDN...")
    try:
        urllib.request.urlretrieve("https://cdnjs.cloudflare.com/ajax/libs/nipplejs/0.10.1/nipplejs.min.js", "nipplejs.min.js")
    except Exception as e:
        print(f"Failed to download nipplejs: {e}")

output_file = 'WebUI.h'

print(f"Generating {output_file}...")

with open(output_file, 'w', encoding='utf-8') as out:
    out.write('// Auto-generated file containing Web UI\n')
    out.write('#ifndef WEBUI_H\n')
    out.write('#define WEBUI_H\n\n')
    
    for var_name, filename in files.items():
        if os.path.exists(filename):
            with open(filename, 'r', encoding='utf-8') as f:
                content = f.read()
            # Use C++ raw string literal syntax
            out.write(f'const char {var_name}[] PROGMEM = R"=====(\n')
            out.write(content)
            out.write('\n)=====";\n\n')
            print(f" -> Added {filename}")
        else:
            print(f" -> Warning: {filename} not found!")
            
    out.write('#endif // WEBUI_H\n')

print(f"Successfully generated {output_file}!")
print("You can now copy WebUI.h to your Arduino sketch folder.")