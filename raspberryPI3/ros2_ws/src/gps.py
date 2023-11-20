import fileinput
import subprocess
import sys
import csv
import os


def get_gps_data():
    data = subprocess.run(["ros2", "topic", "echo", "/gnss_data", "--once"], shell=True, capture_output=True, text=True)
    print(data)
    test_data = 'latitude: 1.4043e-10 \n \
                longitude: 0.0 \n \
                altitude: 0.0 \n \
                quality: 0 \n \
                hacc: 16777215.0 \n \
                vacc: 8690176.0 \n \
                ---'
    data = data.split("\n")
    data.pop()
    ret = {}
    for line in data:
        key, val = line.strip().split(":")
        ret[key] = val

    return ret
def main():
    # gps = get_gps_data()
    print(sys.argv)
    gps = {}
    with fileinput.input() as f_input:
        for line in f_input:
            if line[0] == "-":
                print("cont")
                continue
            print("line", line)
            key, val = line.strip().split(":")
            gps[key] = val
    print(gps)

    filename = "out.csv"
    print(f"The supplied filename is {filename}")

    # The fields for the CSV file
    fields = ['latitude', 'longitude', 'quality', 'hacc']

    # A new row to append to the CSV file
    row = [gps['latitude'], gps['longitude'], gps['quality'], gps['hacc']]

    # Check if the file exists
    file_exists = os.path.isfile(filename)

    # Open the file in append mode
    with open(filename, 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)

        # If the file did not exist, write the header
        if not file_exists:
            writer.writerow(fields)

        # Write the new row
        writer.writerow(row)


if __name__ == "__main__":
    main()