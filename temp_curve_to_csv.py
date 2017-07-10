import serial
import sys

s = serial.Serial(sys.argv[1])

with open("temp_curve.csv", 'w') as f:
    f.write("Time,Temperature,On Time\n")
    input() # Wait until start
    s.read(s.inWaiting()) # Discard previous output
    first = True
    time_dif = 0.0
    while True:
        try:
            line = s.readline().decode('ascii').rstrip("\n")
            time, set_temp, cur_temp, on_time = line.split("\t")
            time = int(time)
            set_temp = int(float(set_temp))
            cur_temp = int(float(cur_temp))
            on_time = float(on_time)
            if first:
                time_dif = time
            #time -= time_dif
            f.write("{},{},{},{}\n".format(time, set_temp, cur_temp, on_time))
            print(time, set_temp, cur_temp, on_time)
            first = False
        except ValueError:
            pass