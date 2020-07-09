import numpy as np


def forward(A, B, pi, O):
    """
    :param A: state transition matrix (NxN)
    :param B: observation probability matrix (NxM)
    :param pi: initial state probabilities (N)
    :param O: observation sequence (T)

            2 <= N <= 10
            2 <= M <= 10
            1 <= T <= 30

            N is the number of states
            M is the number of possible observations
            T is the length of the observation sequence

            A[i][j] is the transition probability from state i to state j
            B[i][j] is the probability of observing observation j in state i
            pi[i] is the probability of initial state is being state i
            O[k] is the k-th observation, which is an index between 0 and M-1 (inclusive)

    :return: given the model(A, B, pi), probability of the observation sequence
    """
    T = O.shape[0]
    N = A.shape[0]
    forward = np.zeros((N, T))
    forward[:, 0] = pi*B[:, O[0]]
    for t in range(1, T):
        for j in range(N):
            forward[j, t] = np.sum(forward[:, t - 1]*(A[:, j]) * B[j, O[t]])
    res = np.sum(forward[:, T-1])
    return res


def viterbi(A, B, pi, O):
    """
    :param A: state transition matrix (NxN)
    :param B: observation probability matrix (NxM)
    :param pi: initial state probabilities (N)
    :param O: observation sequence (T)

            2 <= N <= 10
            2 <= M <= 10
            1 <= T <= 30

            N is the number of states
            M is the number of possible observations
            T is the length of the observation sequence

            A[i][j] is the transition probability from state i to state j
            B[i][j] is the probability of observing observation j in state i
            pi[i] is the probability of initial state is being state i
            O[k] is the k-th observation, which is an index between 0 and M-1 (inclusive)

    :return: given the model(A, B, pi) and observation sequence O, the most likely state sequence.
             It should be a NumPy array with size T. It includes state indices according to A's indices.
             For example: [1, 2, 1, 1, 0, 4]
    """
    T = O.shape[0]
    N = A.shape[0]
    viterbi = np.zeros((N, T))
    viterbi[:, 0] = pi*B[:, O[0]]
    backpointer = np.zeros((N, T))
    res = np.zeros(T)
    for t in range(1, T):
        for j in range(N):
            probs = viterbi[:, t - 1] * (A[:, j]) * (B[j, O[t]])
            viterbi[j, t] = np.max(probs)
            backpointer[j, t] = np.argmax(probs)

    best_path_ptr = np.argmax(viterbi[:, T-1])
    res[0] = int(best_path_ptr)

    b_index = 1
    for i in range(T - 1, 0, -1):
        res[b_index] = backpointer[int(best_path_ptr), i]
        best_path_ptr = backpointer[int(best_path_ptr), i]
        b_index += 1
    return np.flip(res)
