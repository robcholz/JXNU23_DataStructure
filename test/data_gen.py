import os.path
import random
import sys


def clear_file(filename):
    with open(filename, 'w') as file:
        file.write('')
        file.flush()


def generate_data(filename: str, size_: int, generator):
    file_name = os.path.join(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'data'),
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


if len(sys.argv) < 2:
    print("Please provide the size argument using '-size'")
    sys.exit(1)
size_arg = sys.argv[2]
if not size_arg.isdigit():
    print("Invalid size argument. Please provide a positive integer.")
    sys.exit(1)
size = int(size_arg)
generate_data('disordered_{}m.txt', size, lambda file, count: disorder_gen(file, count))
generate_data('ordered_{}m.txt', size, lambda file, count: ordered_gen(file, count))
print(f"Data of size {size} generated")
