import time
from enum import Enum

import numpy
import numpy as np


class MinorType(Enum):
    LEFT = 1
    RIGHT = 0


def convert_check_matrix_to_generate(check_matrix):
    dim_identity_matrix = np.shape(check_matrix)[1] - np.shape(check_matrix)[0]
    identity_matrix = np.eye(dim_identity_matrix)
    check_submatrix = extract_submatrix_from_check(check_matrix)
    generate_matrix = np.concatenate((identity_matrix, np.transpose(check_submatrix)), axis=1)
    return generate_matrix.astype(float)


def convert_generate_matrix_to_check(generate_matrix):
    dim_identity_matrix = np.shape(generate_matrix)[1] - np.shape(generate_matrix)[0]
    identity_matrix = np.eye(dim_identity_matrix)
    generate_submatrix = extract_submatrix_from_generate(generate_matrix)
    check_matrix = np.concatenate((np.transpose(generate_submatrix), identity_matrix), axis=1)
    return check_matrix.astype(float)


def extract_submatrix_from_generate(generate_matrix):
    k = np.shape(generate_matrix)[0]
    submatrix = generate_matrix[:, k:]
    return submatrix.astype(float)


def extract_submatrix_from_check(check_matrix):
    k = np.shape(check_matrix)[1] - np.shape(check_matrix)[0]
    submatrix = check_matrix[:, :k]
    return submatrix.astype(float)


def upper_triangular(M):
    # move all zeros to buttom of matrix
    M = np.concatenate((M[np.any(M != 0, axis=1)], M[np.all(M == 0, axis=1)]), axis=0)

    # iterate over matrix rows
    for i in range(0, M.shape[0]):

        # initialize row-swap iterator
        j = 1

        # select pivot value
        pivot = M[i][i]

        # find next non-zero leading coefficient
        while pivot == 0 and i + j < M.shape[0]:
            # perform row swap operation
            M[[i, i + j]] = M[[i + j, i]]

            # incrememnt row-swap iterator
            j += 1

            # get new pivot
            pivot = M[i][i]

        # if pivot is zero, remaining rows are all zeros
        if pivot == 0:
            # return upper triangular matrix
            return M

        # extract row
        row = M[i]

        # get 1 along the diagonal
        M[i] = row / pivot

        # iterate over remaining rows
        for j in range(i + 1, M.shape[0]):
            # subtract current row from remaining rows
            M[j] = (M[j] - M[i] * M[j][i]) % 2

    # return upper triangular matrix
    return M


def extract_nondegenerate_minor(matrix):
    pos = 0
    permutation = []
    for i in range(len(matrix[0])):
        for j in range(len(matrix)):
            if matrix[j][i] == 1 and j not in permutation:
                permutation.insert(j, i)
                pos += 1
                break
        if pos == len(matrix):
            break

    for i in range(0, len(matrix[0])):
        if i not in permutation:
            permutation.append(i)

    return permutation


def convert_minor_to_identity_matrix(n, matrix, direction):
    if direction == MinorType.LEFT:
        for i in range(n - 2, -1, -1):
            for j in range(len(matrix) - 1, i, -1):
                if matrix[i][j] == 1:
                    for k in range(len(matrix[0])):
                        matrix[i][k] = (matrix[i][k] + matrix[j][k]) % 2
    elif direction == MinorType.RIGHT:
        width, height = len(matrix[0]), len(matrix)
        for i in range(n - 2, -1, -1):
            for j in range(width - 1, width - height + i, -1):
                if matrix[i][j] == 1:
                    for k in range(width):
                        matrix[i][k] = (matrix[i][k] + matrix[j - height][k]) % 2

    return matrix


def apply_left_permutation(matrix, per):
    m = np.zeros((len(matrix), len(matrix[0])))

    for p in range(0, len(per)):
        for i in range(0, len(matrix)):
            m[i][p] = matrix[i][per[p]]
    return m


def apply_right_permutation(matrix, per):
    matrix_width = len(matrix[0])
    matrix_height = len(matrix)
    m = np.zeros((matrix_height, matrix_width))

    for p in range(0, len(per)):
        for i in range(0, len(matrix)):
            m[i][(matrix_width - matrix_height + p) % matrix_width] = matrix[i][per[p]]
    return m


def reverse_permutation(matrix, per):
    m = np.zeros((len(matrix), len(matrix[0])))

    for p in range(0, len(per)):
        for i in range(0, len(matrix)):
            m[i][per[p]] = matrix[i][p]
    return m


def get_check_matrix_by_generate(generate_matrix):
    # Приведение матрицы к ступенчатому виду преобразованиями строк
    triangular_matrix = upper_triangular(generate_matrix)

    # Выделение невырожденного минора из полученной матрицы в ступенчатом виде.
    permutation = extract_nondegenerate_minor(triangular_matrix)

    # Переход к коду π(C): перестановка столбцов так чтобы невырожденный минор в матрице был слева;
    # затем приведение минора слева к единичной матрице преобразованиями строк
    left_minor = apply_left_permutation(triangular_matrix, permutation)
    n, k = len(left_minor[0]), len(left_minor)
    canonical_generate = convert_minor_to_identity_matrix(k, left_minor, MinorType.LEFT)

    check_matrix = convert_generate_matrix_to_check(canonical_generate)
    return reverse_permutation(check_matrix, permutation)


def get_generate_matrix_by_check(generate_matrix):
    triangular_matrix = upper_triangular(generate_matrix)
    permutation = extract_nondegenerate_minor(triangular_matrix)
    right_minor = apply_right_permutation(triangular_matrix, permutation)
    n, k = len(generate_matrix[0]), len(generate_matrix)
    canonical_check = convert_minor_to_identity_matrix(k, right_minor, MinorType.RIGHT)

    generate_matrix = convert_check_matrix_to_generate(canonical_check)
    return reverse_permutation(generate_matrix, permutation)


start = time.time()
data = numpy.loadtxt("input.txt")

print("Задана матрица")
print(data)

check_m = get_check_matrix_by_generate(data)
gen_m = get_generate_matrix_by_check(data)
end = time.time()

print("Если была задана проверочная матрица, из нее получена порождающая")
print(gen_m)

print("Если была задана порождающая матрица, из нее получена проверочная")
print(check_m)

print("time: {}ms".format(end - start))

12:40 19.02.2023
Плугатарев Кирилл Андреевич Avatar

import time

import numpy
import numpy as np


def convert_canonical_matrix(matrix):
    dim_identity_matrix = np.shape(matrix)[1] - np.shape(matrix)[0]
    identity_matrix = np.eye(dim_identity_matrix)
    submatrix = extract_submatrix_from(matrix)
    concatenate_matrix = np.concatenate((np.transpose(submatrix), identity_matrix), axis=1)
    return concatenate_matrix.astype(float)


def extract_submatrix_from(matrix):
    k = np.shape(matrix)[0]
    submatrix = matrix[:, k:]
    return submatrix.astype(float)


def upper_triangular(M):
    # move all zeros to buttom of matrix
    M = np.concatenate((M[np.any(M != 0, axis=1)], M[np.all(M == 0, axis=1)]), axis=0)

    # iterate over matrix rows
    for i in range(0, M.shape[0]):

        # initialize row-swap iterator
        j = 1

        # select pivot value
        pivot = M[i][i]

        # find next non-zero leading coefficient
        while pivot == 0 and i + j < M.shape[0]:
            # perform row swap operation
            M[[i, i + j]] = M[[i + j, i]]

            # incrememnt row-swap iterator
            j += 1

            # get new pivot
            pivot = M[i][i]

        # if pivot is zero, remaining rows are all zeros
        if pivot == 0:
            # return upper triangular matrix
            return M

        # extract row
        row = M[i]

        # get 1 along the diagonal
        M[i] = row / pivot

        # iterate over remaining rows
        for j in range(i + 1, M.shape[0]):
            # subtract current row from remaining rows
            M[j] = (M[j] - M[i] * M[j][i]) % 2

    # return upper triangular matrix
    return M


def extract_nondegenerate_minor(matrix):
    pos = 0
    permutation = []
    for i in range(len(matrix[0])):
        for j in range(len(matrix)):
            if matrix[j][i] == 1 and j not in permutation:
                permutation.insert(j, i)
                pos += 1
                break
        if pos == len(matrix):
            break

    for i in range(0, len(matrix[0])):
        if i not in permutation:
            permutation.append(i)

    return permutation


def convert_minor_to_identity_matrix(n, matrix):
    for i in range(n - 2, -1, -1):
        for j in range(len(matrix) - 1, i, -1):
            if matrix[i][j] == 1:
                for k in range(len(matrix[0])):
                    matrix[i][k] = (matrix[i][k] + matrix[j][k]) % 2

    return matrix


def apply_permutation(matrix, per):
    m = np.zeros((len(matrix), len(matrix[0])))

    for p in range(0, len(per)):
        for i in range(0, len(matrix)):
            m[i][p] = matrix[i][per[p]]
    return m


def reverse_permutation(matrix, per):
    m = np.zeros((len(matrix), len(matrix[0])))

    for p in range(0, len(per)):
        for i in range(0, len(matrix)):
            m[i][per[p]] = matrix[i][p]
    return m


def get_convert_matrix(generate_matrix):
    # Приведение матрицы к ступенчатому виду преобразованиями строк
    triangular_matrix = upper_triangular(generate_matrix)

    # Выделение невырожденного минора из полученной матрицы в ступенчатом виде.
    permutation = extract_nondegenerate_minor(triangular_matrix)

    # Переход к коду π(C): перестановка столбцов так чтобы невырожденный минор в матрице был слева;
    # затем приведение минора слева к единичной матрице преобразованиями строк
    left_minor = apply_permutation(triangular_matrix, permutation)
    n, k = len(left_minor[0]), len(left_minor)
    canonical_matrix = convert_minor_to_identity_matrix(k, left_minor)

    result_matrix = convert_canonical_matrix(canonical_matrix)
    return reverse_permutation(result_matrix, permutation)


data = numpy.loadtxt("input.txt")

print("Задана матрица")
print(data)

start = time.time()
result = get_convert_matrix(data)
end = time.time()

print("Получили")
print(result)

print("time: {}ms".format(end - start))

