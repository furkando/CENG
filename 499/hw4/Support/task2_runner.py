import os
import pickle
from task2 import forward, viterbi


def test(data_folder):
    all_tests = True
    for filename in sorted(list(os.listdir(data_folder))):
        with open(os.path.join(data_folder, filename), 'rb') as pickle_file:
            data = pickle.load(pickle_file)
        A = data['A']
        B = data['B']
        pi = data['pi']
        O = data['O']

        forward_answer = data['forward_answer']
        viterbi_answer = data['viterbi_answer']

        forward_valid = abs(
            forward(A, B, pi, O)-forward_answer) <= forward_answer*10**-4
        viterbi_valid = (viterbi_answer == viterbi(A, B, pi, O)).astype(
            int).sum() == viterbi_answer.shape[0]

        all_tests = all_tests and forward_valid and viterbi_valid

        print(filename)
        print('Forward:', forward_valid)
        print('Viterbi:', viterbi_valid)
        print()
    if all_tests:
        print('PASSED all tests')
    else:
        print('Some of the tests FAILED')


test('task2_data')
