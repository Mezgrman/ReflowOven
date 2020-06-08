import matplotlib.pyplot as plt


TIME_0 = 0
TEMP_0 = 25

TIME_1 = 120
TEMP_1 = 100

TIME_2 = 270
TEMP_2 = 140

TIME_3 = 300
TEMP_3 = 170

TIME_4 = 330
TEMP_4 = 170

TIME_5 = 391
TEMP_5 = 0


def get_temperature(time):
    if time <= TIME_0:
        return TEMP_0
    if time <= TIME_1:
        m = (TEMP_1 - TEMP_0) / (TIME_1 - TIME_0)
        b = TEMP_1 - m * TIME_1
        return m * time + b
    if time <= TIME_2:
        m = (TEMP_2 - TEMP_1) / (TIME_2 - TIME_1)
        b = TEMP_2 - m * TIME_2
        return m * time + b
    if time <= TIME_3:
        m = (TEMP_3 - TEMP_2) / (TIME_3 - TIME_2)
        b = TEMP_3 - m * TIME_3
        return m * time + b
    if time <= TIME_4:
        m = (TEMP_4 - TEMP_3) / (TIME_4 - TIME_3)
        b = TEMP_4 - m * TIME_4
        return m * time + b
    if time <= TIME_5:
        m = (TEMP_5 - TEMP_4) / (TIME_5 - TIME_4)
        b = TEMP_5 - m * TIME_5
        return m * time + b
    if time > TIME_5:
        return TEMP_5


def main():
    x = list(range(600))
    plt.plot(x, list(map(get_temperature, x)))
    plt.xlabel("Time [s]")
    plt.ylabel("Temp. setpoint [°C]")
    plt.xlim(0, 600)
    plt.ylim(0, 300)
    plt.grid()
    plt.show()


if __name__ == "__main__":
    main()
