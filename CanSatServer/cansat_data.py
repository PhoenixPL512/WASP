import sys

class CanSatDataset:
    def __init__(self, timestamp, magnet, accel, gyro, temperature, pressure, altitude, rawgps):
        self.timestamp = timestamp
        self.magnet = magnet
        self.accel = accel
        self.gyro = gyro
        self.temperature = temperature
        self.pressure = pressure
        self.altitude = altitude
        self.rawgps = rawgps

    def __str__(self):
        return """
Read time: {0}ms
Magnetometer:
    X: {1}
    Y: {2}
    Z: {3}
Accelerometer:
    X: {4}
    Y: {5}
    Z: {6}
Gyroscope:
    X: {7}
    Y: {8}
    Z: {9}
Temperature: {10}
Pressure: {11}
Altitude: {12}
Raw GPS data: {13}""".format(
            self.timestamp,
            self.magnet[0], self.magnet[1], self.magnet[2],
            self.accel[0], self.accel[1], self.accel[2],
            self.gyro[0], self.gyro[1], self.gyro[2],
            self.temperature, self.pressure, self.altitude, self.rawgps)


class CanSatData:
    def __init__(self):
        self.timestamps = []
        self.magnets = []
        self.accels = []
        self.gyros = []
        self.temperatures = []
        self.pressures = []
        self.altitudes = []
        self.rawgps_data = []

    def add_data(self, timestamp, magnet, accel, gyro, temperature, altitude, rawgps):
        self.timestamps.append(timestamp)
        self.magnets.append(magnet)
        self.accels.append(accel)
        self.gyros.append(gyro)
        self.temperatures.append(temperature)
        self.altitudes.append(altitude)
        self.rawgps_data.append(rawgps)

    def get_single_dataset(self, index=-1):
        return CanSatDataset(self.timestamps[index], self.magnets[index], self.accels[index], self.gyros[index],
                             self.temperatures[index], self.pressures[index], self.altitudes[index],
                             self.rawgps_data[index])

    def add_dataset(self, dataset):
        self.timestamps.append(dataset.timestamp)
        self.magnets.append(dataset.magnet)
        self.accels.append(dataset.accel)
        self.gyros.append(dataset.gyro)
        self.temperatures.append(dataset.temperature)
        self.pressures.append(dataset.pressure)
        self.altitudes.append(dataset.altitude)
        self.rawgps_data.append(dataset.rawgps)

    def print(self):
        for i in range(0, len(self.timestamps)):
            print(self.get_single_dataset(i))

    def dump_to_file(self):
        with open("CanSatData.txt", "w") as file:
            file.write("Timestamp\tMagnetX\tMagnetY\tMagnetZ\tAccelX\tAccelY\tAccelZ\t"
                       "GyroX\tGyroY\tGyroZ\tTemperature\tPressure\tAltitude\tGPS raw data\n")
            for i in range(0, len(self.timestamps)):
                file.write("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\t{9}\t{10}\t{11}\t{12}\t{13}\n".format(
                    self.timestamps[i],
                    self.magnets[i][0], self.magnets[1][1], self.magnets[i][2],
                    self.accels[i][0], self.accels[i][1], self.accels[i][2],
                    self.gyros[i][0], self.gyros[i][1], self.gyros[i][2],
                    self.temperatures[i], self.pressures[i], self.altitudes[i], self.rawgps_data[i]
                ))
            file.close()
