import os
import json
from PIL import Image

class TexturePacker:
    def __init__(self, max_width=4096, max_height=4096, padding=2):
        self.max_width = max_width
        self.max_height = max_height
        self.padding = padding
        self.root = None
        self.images = []  # List of (name, Image object)

    def add_image(self, name, image):
        self.images.append((name, image))

    def pack(self):
        # Sort by max side length (descending) to optimize packing
        self.images.sort(key=lambda x: max(x[1].width, x[1].height), reverse=True)

        # Calculate total area to estimate starting size
        total_area = sum(img.width * img.height for _, img in self.images)
        
        # Start with a power of 2 size that fits the area roughly
        size = 128
        while size * size < total_area:
            size *= 2
        
        # Ensure at least big enough for the largest image
        max_img_w = max((img.width for _, img in self.images), default=0)
        max_img_h = max((img.height for _, img in self.images), default=0)
        
        while size < max_img_w or size < max_img_h:
            size *= 2

        # Try packing with increasing sizes
        curr_w, curr_h = size, size
        
        while curr_w <= self.max_width and curr_h <= self.max_height:
            # Reset packing for this size
            self.root = {'x': 0, 'y': 0, 'w': curr_w, 'h': curr_h, 'used': False}
            packed_data = []
            success = True
            
            for name, img in self.images:
                # Calculate needed space including padding
                # Note: padding is applied to the right and bottom of the image slot
                w = img.width + self.padding
                h = img.height + self.padding
                
                node = self.find_node(self.root, w, h)
                if node:
                    packed_data.append({
                        'name': name,
                        'x': node['x'],
                        'y': node['y'],
                        'width': img.width,
                        'height': img.height
                    })
                    self.split_node(node, w, h)
                else:
                    success = False
                    break
            
            if success:
                return self.create_atlas(curr_w, curr_h, packed_data)
            
            # Grow atlas size
            if curr_w <= curr_h:
                curr_w *= 2
            else:
                curr_h *= 2
        
        raise Exception("Failed to pack images: Atlas size exceeded limit.")

    def find_node(self, node, w, h):
        if node['used']:
            return self.find_node(node['right'], w, h) or self.find_node(node['down'], w, h)
        elif w <= node['w'] and h <= node['h']:
            return node
        return None

    def split_node(self, node, w, h):
        node['used'] = True
        node['down'] = {'x': node['x'], 'y': node['y'] + h, 'w': node['w'], 'h': node['h'] - h, 'used': False, 'right': None, 'down': None}
        node['right'] = {'x': node['x'] + w, 'y': node['y'], 'w': node['w'] - w, 'h': h, 'used': False, 'right': None, 'down': None}

    def create_atlas(self, w, h, packed_data):
        atlas = Image.new('RGBA', (w, h), (0, 0, 0, 0))
        meta = {}
        
        for item in packed_data:
            name = item['name']
            # Find image
            img = next(i for n, i in self.images if n == name)
            atlas.paste(img, (item['x'], item['y']))
            
            meta[name] = {
                'frame': {
                    'x': item['x'],
                    'y': item['y'],
                    'w': item['width'],
                    'h': item['height']
                },
                'rotated': False,
                'trimmed': False,
                'spriteSourceSize': {
                    'x': 0,
                    'y': 0,
                    'w': item['width'],
                    'h': item['height']
                },
                'sourceSize': {
                    'w': item['width'],
                    'h': item['height']
                }
            }
            
        return atlas, meta

def process_category(base_path, category, output_dir):
    source_dir = os.path.join(base_path, category)
    if not os.path.exists(source_dir):
        print(f"Skipping {category}: Directory not found at {source_dir}")
        return

    print(f"Processing {category}...")
    packer = TexturePacker()
    count = 0
    
    # Walk through directory
    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.lower().endswith(('.png', '.jpg', '.jpeg')):
                file_path = os.path.join(root, file)
                # Rel path for name
                rel_path = os.path.relpath(file_path, source_dir)
                # Normalize separators to forward slash
                name = rel_path.replace(os.sep, '/')
                
                try:
                    img = Image.open(file_path)
                    packer.add_image(name, img)
                    count += 1
                except Exception as e:
                    print(f"Error loading {file_path}: {e}")

    if count == 0:
        print(f"No images found in {category}.")
        return

    try:
        atlas, meta = packer.pack()
        
        # Ensure output directory exists
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
            
        # Save Atlas Image
        atlas_filename = f"{category}_atlas.png"
        atlas_path = os.path.join(output_dir, atlas_filename)
        atlas.save(atlas_path)
        print(f"Saved atlas to {atlas_path}")
        
        # Save JSON
        json_filename = f"{category}_atlas.json"
        json_path = os.path.join(output_dir, json_filename)
        
        json_output = {
            'frames': meta,
            'meta': {
                'image': atlas_filename,
                'size': {'w': atlas.width, 'h': atlas.height},
                'scale': 1
            }
        }
        
        with open(json_path, 'w') as f:
            json.dump(json_output, f, indent=4)
        print(f"Saved meta to {json_path}")
        
    except Exception as e:
        print(f"Failed to pack {category}: {e}")

def main():
    # Determine directories
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)
    textures_dir = os.path.join(project_root, 'res', 'textures')
    atlas_dir = os.path.join(textures_dir, 'atlas')
    
    categories = ['tilesets', 'sprites', 'ui']
    
    if not os.path.exists(textures_dir):
        print(f"Textures directory not found at {textures_dir}")
        # If it doesn't exist, we can't pack.
        os.makedirs(textures_dir)
        print(f"Created textures directory at {textures_dir}")
        return

    for category in categories:
        process_category(textures_dir, category, atlas_dir)
        
    print("Done.")

if __name__ == '__main__':
    main()
