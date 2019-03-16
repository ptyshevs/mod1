l = []
for i in range(-1, 2):
    for j in range(-1, 2):
        for k in range(-1, 2):
            print("{{{}, {}, {}}},".format(i, j, k))
            l.append((i, j, k))
print(len(l))