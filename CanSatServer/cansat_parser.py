import os.path
import sys
from cansat_data import CanSatData, CanSatDataset
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt


def find_between(s, first, last):
    try:
        start = s.index(first) + len(first)
        end = s.index(last, start)
        return s[start:end]
    except ValueError:
        return ""


class CanSatParser:
    def __init__(self):
        self.file = None
        self.log_file = None
        self.data = CanSatData()

    def start(self):
        if self.file is None:
            self.start_from_nrf()
        else:
            self.start_from_file()

    def start_from_nrf(self):
        print("Not implemented yet")

    def start_from_file(self):
        if not os.path.isfile(self.file):
            print("File {0} not found!".format(self.file))
            sys.exit(1)

        self.log_file = open(self.file, "r")
        self.read_datasets_from_file()

    def read_datasets_from_file(self):
        for line in self.log_file:
            dataset = self.parse_dataset(line)
            if dataset is not None:
                self.data.add_dataset(dataset)

    def parse_dataset(self, rawdata):
        if rawdata[0] != '@':
            return None
        timestamp = find_between(rawdata, '@', '[')
        magnet = find_between(rawdata, "[MAG]:#", '#').split('|')
        accel = find_between(rawdata, "[ACC]:#", '#').split('|')
        gyro = find_between(rawdata, "[GYR]:#", '#').split('|')
        tpa = find_between(rawdata, "[TPA]:#", '#').split('|')
        gps = None
        try:
            gps = rawdata[rawdata.index("[GPS]")+5:]
        except ValueError:
            pass
        return CanSatDataset(float(timestamp)/1000, [int(magnet[0]), int(magnet[1]), int(magnet[2])],
                             [int(accel[0]), int(accel[1]), int(accel[2])],
                             [int(gyro[0]), int(gyro[1]), int(gyro[2])],
                             float(tpa[0]), float(tpa[1]), float(tpa[2]), gps)

    def show_plot(self, plot):
        if plot == "temperature":
            plt.plot(self.data.timestamps, self.data.temperatures)
            plt.xlabel("Time (in seconds)")
            plt.ylabel("Temperature (in celcius)")
            plt.show()
        elif plot == "altitude":
            plt.plot(self.data.timestamps, self.data.altitudes)
            plt.xlabel("Time (in seconds)")
            plt.ylabel("Altitude (approx, in meters)")
            plt.show()
        elif plot == "pressure":
            plt.plot(self.data.timestamps, self.data.pressures)
            plt.xlabel("Time (in seconds)")
            plt.ylabel("Pressure (in millibars)")
            plt.show()
        else:
            print("Unknown plot type")
