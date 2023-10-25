# Шифр Цезаря — это вид шифра подстановки, в котором каждый символ
# в открытом тексте заменяется символом, находящимся на некотором
# постоянном числе позиций левее или правее него в алфавите.
# Напишите программу, которая реализует шифрование Цезаря.
# Входные данные: путь до изначального файла с текстом,
# требуемый сдвиг и язык текста(на выбор английский либо русский).
# Результат работы - новый файл с зашифрованным текстом.

def input_count(message: str) -> int:
    n = None

    while n is None:
        try:
            print(message)
            n = int(input())

            if n <= 0:
                n = None
                raise ValueError

        except ValueError:
            print("Неверный формат ввода.")

    return n

def get_user_input():
    print("Путь до изначального файла с текстом:")
    in_file_name = input()
    offset = input_count("Требуемый сдвиг:")
    print("Язык текста: английский или русский")
    language = input()

    return in_file_name, offset, language

def cesar_code_str(line: str, offset: int, alphabet: list) -> str:
    result_line = ""

    size = len(alphabet)
    for symbol in line:

        if symbol.lower() in alphabet:
            position_in_alphabet = alphabet.index(symbol.lower())

            symbol_with_offset = alphabet[(position_in_alphabet + offset) % size]

            if symbol.isupper():
                symbol_with_offset = symbol_with_offset.upper()

            result_line += symbol_with_offset
        else:
            result_line += symbol

    return result_line


def cesar_code_file(in_file_name: str, offset: int, alphabet: list, out_file_name: str = "out.txt"):
    try:
        with open(in_file_name, "r", encoding="utf-8") as f_in, open(out_file_name, "w", encoding="utf-8") as f_out:
            for line in f_in:
                f_out.write(cesar_code_str(line, offset, alphabet))
    except FileNotFoundError:
        print("Был введён неверный путь до файла")
    except OSError:
        print("Ошибка ОС при открытии файла")
    except Exception:
        print("Непредвиденная ошибка")


in_file_name, offset, language = get_user_input()

if language == "русский":
    alphabet = 'абвгдежзийклмнопрстуфхцчшщъыьэюя'
    cesar_code_file(in_file_name, offset, list(alphabet))
elif language ==  "английский":
    alphabet = 'abcdefghijklmnopqrstuvwxyz'
    cesar_code_file(in_file_name, offset, list(alphabet))
else :
    print("Неподдерживаемый язык")