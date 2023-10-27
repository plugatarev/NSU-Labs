# Напишите программу, которая запрашивает у пользователя число n,
# а затем выводит n первых строк треугольника Паскаля. Обеспечьте
# отказоустойчивость при введении пользователем не валидного значения
# n (т.е. не целого положительного числа)

def generate_pascals_triangle(n):
    triangle = []
    for i in range(n):
        if i == 0:
            triangle.append([1])
        else:
            prev_row = triangle[i - 1]
            new_row = [1]
            for j in range(1, i):
                new_row.append(prev_row[j - 1] + prev_row[j])
            new_row.append(1)
            triangle.append(new_row)
    return triangle

def print_pascals_triangle(triangle):
    max_width = len(" ".join(map(str, triangle[-1])))
    for row in triangle:
        row_str = " ".join(map(str, row))
        print(row_str.center(max_width))


n = 0
while n <= 0: 
    try: 
        n = int(input("Введите число строк треугольника Паскаля: "))
        if n <= 0:
            print("Пожалуйста, введите положительное число.")
        else:
            pascals_triangle = generate_pascals_triangle(n)
            print("\nТреугольник Паскаля из первых", n, "строк:")
            print_pascals_triangle(pascals_triangle)
    except ValueError:
        print("Некорректный ввод. Пожалуйста, введите целое число.")
