import os.path
import random
import re
import sys


def clear_file(filename):
    with open(filename, 'w') as file:
        file.write('')
        file.flush()


def generate_data(filename: str, size_: int, generator):
    file_name = os.path.join(os.path.join(os.path.join(project_path, 'test'), 'data'),
                             filename.format(size_))
    clear_file(file_name)
    with open(file_name, 'a') as file:
        generator(file, size_ * 10000)


def disorder_gen(file, count: int):
    numbers = random.sample(range(1, count + 1), count)
    for number in numbers:
        file.write(str(number) + ' ')


def ordered_gen(file, count: int):
    for number in range(count):
        file.write(str(number) + ' ')


# generate data
if len(sys.argv) < 2:
    print("Please provide the size argument using '-size'")
    sys.exit(1)
size_arg = sys.argv[2]
project_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if not size_arg.isdigit():
    print("Invalid size argument. Please provide a positive integer.")
    sys.exit(1)
size = int(size_arg)
generate_data('disordered_{}m.txt', size, lambda file, count: disorder_gen(file, count))
generate_data('ordered_{}m.txt', size, lambda file, count: ordered_gen(file, count))
print(f"Data of size {size} generated")

# replace macros

header_file_path = os.path.join(os.path.join(project_path, 'src'), 'Path.h')
data_path = os.path.join(os.path.join(project_path, 'test'), 'data')
replacement_string_1 = os.path.join(data_path, 'disordered_50m.txt')
replacement_string_2 = os.path.join(data_path, 'ordered_50m.txt')

with open(header_file_path, 'r') as p_file:
    content = p_file.read()

pattern_1 = r'#define ORDERED_SAMPLE_PATH\s+""'
pattern_2 = r'#define DISORDERED_SAMPLE_PATH\s+""'
replacement_1 = f'#define ORDERED_SAMPLE_PATH "{replacement_string_1}"'
replacement_2 = f'#define DISORDERED_SAMPLE_PATH "{replacement_string_2}"'
content = re.sub(pattern_1, replacement_1, content)
updated_content = re.sub(pattern_2, replacement_2, content)

with open(header_file_path, 'w') as p_file:
    p_file.write(updated_content)
