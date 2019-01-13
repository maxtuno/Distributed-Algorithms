if __name__ == '__main__':

    import sys

    n = int(sys.argv[1])

    k = 1
    sys.stdout.write('{}\n'.format(n))
    for i in range(n):
        for j in range(n):
            if i != j:
                sys.stdout.write('{} '.format(k))
                k += 1
            else:
                sys.stdout.write('{} '.format(0))
        sys.stdout.write('\n')
