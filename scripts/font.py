from PIL import Image
import numpy as np

def image_to_u8x8_font(image_path, threshold=128, invert=False, output_file="font.txt"):
    # Загрузка изображения и бинаризация
    img = Image.open(image_path).convert("L")
    img = img.point(lambda x: 0 if x < threshold else 255, "1")  # Пороговая обработка

    if invert:
        img = img.point(lambda x: 0 if x == 255 else 255)  # Инверсия цветов

    pixels = np.array(img)
    height, width = pixels.shape

    # Проверка кратности 8
    if width % 8 != 0 or height % 8 != 0:
        raise ValueError("Изображение должно быть кратно 8 пикселям!")

    chars_x = width // 8
    chars_y = height // 8
    font_data = []

    for cy in range(chars_y):
        for cx in range(chars_x):
            char_data = []
            # Проходим по столбцам (X), а не по строкам
            for x in range(8):
                byte = 0
                # Собираем биты для текущего столбца
                for y in range(8):
                    px = pixels[cy * 8 + y, cx * 8 + x]
                    # Черный пиксель = 1, белый = 0
                    bit = 1 if px == 0 else 0
                    # Младший бит - верхний пиксель, старший - нижний
                    byte |= (bit << y)
                char_data.append(byte)
            font_data.append(char_data)

    # Сохранение в формате \ddd (восьмеричные числа)
    with open(output_file, "w") as f:
        for char in font_data:
            # Форматируем в восьмеричную систему счисления
            line = "".join(f"\\{b:03o}" for b in char)
            f.write(line + "\n")

    print(f"Файл шрифта сохранён в {output_file}")
    return font_data

# Пример использования
image_to_u8x8_font("scripts/lichess.png", threshold=1, invert=False)