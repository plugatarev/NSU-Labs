# Напишите программу, которая принимает на вход(из файла либо из консоли)
# скобочную последовательность, а результатом работы которой является ответ,
# является ли данная скобочная последовательность правильной.
# Пример: “(()())()” - Правильная последовательность
# “(()))” - Неправильная последовательность
# “)())(” - Неправильная последовательность

def is_correct_brackets_sequence(brackets_sequence: str) -> bool:
    count = 0

    for bracket in brackets_sequence:
        if bracket == '(':
            count += 1
        elif bracket == ')':
            count -= 1
            if count == 0:
                return False
        else :
            return False

    return count == 0


print("Введите скобочную последовательность: ")

if is_correct_brackets_sequence(input()):
    print("Правильная последовательность")
else:
    print("Неверная последовательность")
