fout = open('entry.txt', 'w')
for i in range(1000000):
    fout.write(f'{i}\n')
