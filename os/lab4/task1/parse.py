def diff_time(time1:str, time2:str) -> str:
    return str(int(time1) - int(time2))


matlabfile = open("plott.m", "w")

matlabfile.write("cr = [0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.8, 0.8];\n")
matlabfile.write("cg = [0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 0.2, 0.2];\n")
matlabfile.write("cb = [0.2, 0.5, 0.8, 0.2, 0.5, 0.8, 0.2, 0.5, 0.8, 0.2, 0.5, 0.8, 0.2, 0.5, 0.8, 0.2, 0.5, 0.8, 0.2, 0.5];\n")
matlabfile.write("hold on;")

start_time = input("input start time: ")
count = int(input("input number of processes: "))

for i in range(1, count+1):
    matlabfile.write("y = " + str(i) + ";\n")
    file = open("writelog_" + str(i) + ".txt", "r")
    for j in range(8):
        time1, _ = file.readline().split("\n")
        time2, n = file.readline().split(" ")
        _ = file.readline()
        n, _ = n.split("\n")
        matlabfile.write("c = " + str(n) + ";\n")
        matlabfile.write("rectangle('Position', [" + diff_time(time1, start_time)  + ",y," + diff_time(time2,time1) + ",1], 'FaceColor', [cr(c), cg(c), cb(c)], 'EdgeColor', [1, 0.2, 0.2]);\n")

for i in range(1, count+1):
    matlabfile.write("y = " + str(count+i) + ";\n")
    file = open("readlog_" + str(i) + ".txt", "r")
    for j in range(8):
        time1, _ = file.readline().split("\n")
        time2, n = file.readline().split(" ")
        _ = file.readline()
        n, _ = n.split("\n")
        matlabfile.write("c = " + str(n) + ";\n")
        matlabfile.write("rectangle('Position', [" + diff_time(time1, start_time) + ",y," + diff_time(time2,time1) + ",1], 'FaceColor', [cr(c), cg(c), cb(c)], 'EdgeColor', [1, 0.2, 0.2]);\n")

matlabfile.write("grid on;\n")
matlabfile.write("hold off;\n")
