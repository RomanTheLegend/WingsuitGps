import tkinter as tk
from tkinter import filedialog, font
import csv
import json
from DataPoint import DataPoint
from datetime import datetime
import Freefall
import geopy.distance


SLOPE_LEN = 4

first_ts = 0
timeseries = [None] * SLOPE_LEN
ff_flag, score = None, None
exit_dp, start_dp, finish_dp = None, None, None

def create_window(width, height, background_color):
    window = tk.Tk()
    window.title("FlyScope")
    window.geometry(f"{width}x{height}")
    window.configure(bg=background_color)
    window.resizable(False, False)

    menu_bar = tk.Menu(window)
    window.config(menu=menu_bar)

    # Add a "File" menu
    # file_menu = tk.Menu(menu_bar, tearoff=0)
    # menu_bar.add_cascade(label="File", menu=file_menu)
    #
    # # Add an "Open" option to the "File" menu
    # file_menu.add_command(label="Open", command=open_file)

    return window

def open_file():
    file_path = filedialog.askopenfilename(title="Open FlySight track", filetypes=[("CSV files", "*.csv")])
    if file_path:
        # Process the selected file (replace the existing code with your file processing logic)
        print(f"Opened file: {file_path}")

def place_text(window, text, text_color, position, font_size, font_weight, text_bg, label_var):
    custom_font = font.Font(family="Inconsolata", size=font_size, weight=font_weight)
    label = tk.Label(window, textvariable=label_var, fg=text_color, bg=text_bg, font=custom_font)
    label.place(x=position["x"], y=position["y"])

def convert_to_unix_epoch(raw_string):
    time_string = raw_string[:23]  # Extract timestamp string (excluding 'Z')

    # Parse the timestamp string
    # Example: 2023-08-24T12:59:14.400
    year = int(time_string[0:4])
    month = int(time_string[5:7])
    day = int(time_string[8:10])
    hour = int(time_string[11:13])
    minute = int(time_string[14:16])
    second = int(time_string[17:19])
    milis = int(time_string[20:23])

    dt = datetime(year, month, day, hour, minute, second)
    unix_epoch = int(dt.timestamp()) * 1000 + milis

    return unix_epoch

def add_element(value):
    global timeseries
    for i in range(SLOPE_LEN - 1):
        timeseries[i] = timeseries[i + 1]
    timeseries[SLOPE_LEN - 1] = value

def read_csv_line(csv_reader):
    global first_ts
    try:
        row = next(csv_reader)
        point = DataPoint()
        point.ts = convert_to_unix_epoch(row[0])
        if first_ts == 0:
            first_ts = point.ts
        point.t = (point.ts - first_ts) / 1000.0
        point.lat = float(row[1])
        point.lon = float(row[2])
        point.hMSL = float(row[3])
        point.velN = float(row[4])
        point.velE = float(row[5])
        point.velD = float(row[6])
        point.hAcc = float(row[7])
        point.vAcc = float(row[8])
        point.sAcc = float(row[9])
        point.heading = float(row[10])
        point.cAcc = float(row[11])
        point.gpsFix = row[12]
        point.numSV = int(row[13])
        return point
    except StopIteration:
        return None
    except Exception as e:
        print(e)
        return None

def skip_lines(csv_reader, num_lines):
    for _ in range(num_lines):
        next(csv_reader, None)

def update_readings(window, csv_reader, labels, references):
    global in_freefall, exit_dp, ff_flag, score, start_dp, finish_dp

    point = read_csv_line(csv_reader)
    add_element(point)
    if exit_dp is None and timeseries[0] is not None:
        exit_dp = Freefall.detect_freefall(timeseries)
        if exit_dp is not None:
            exit_dp.t = (exit_dp.ts - first_ts) / 1000.0
            ff_flag.set("====== freefall ======  ")


    current_alt = point.hMSL - references['dz']['hmsl']
    if start_dp is None and current_alt <= 2500:
        start_dp = point

    if finish_dp is None and current_alt <= 1500:
        finish_dp = point

    for readings_key, readings_data in labels.items():
        if readings_key == 'gl':
            readings_data["ref"].set(f"{DataPoint.glideRatio(point):.1f}")
        elif readings_key == 'hspeed':
            readings_data["ref"].set(f"{DataPoint.horizontalSpeed(point)*3.6:.0f}")
        elif readings_key == 'vspeed':
            readings_data["ref"].set(f"{point.velD*3.6:.0f}")
        elif readings_key == 'hmsl':
            readings_data["ref"].set(f"{current_alt:.0f}")
        elif readings_key == 'totalSpeed':
            readings_data["ref"].set(f"{DataPoint.totalSpeed(point)*3.6:.0f}")

        if start_dp is not None and finish_dp is not None and current_alt <= 1400:
            time = finish_dp.t - start_dp.t;
            coords_1 = (start_dp.lat, start_dp.lon)
            coords_2 = (finish_dp.lat, finish_dp.lon)
            distance = geopy.distance.geodesic(coords_1, coords_2).km
            speed = distance / time
            score.set(f"Time:  {time:.4f}\n"
                      f"Dist:  {distance:.5f}\n"
                      f"Speed: {speed:.3f}")

        # Schedule the next update after 0.2 seconds
    window.after(200, update_readings, window, csv_reader, labels, references)
    # else:
    #     window.destroy()

def main():
    global ff_flag, score
    # Window properties
    window_width = 240
    window_height = 280
    window_background = "gray"

    # CSV file properties
    csv_filename = "data.csv"

    with open("references.json", "r") as file:
        references = json.load(file)
        references["dz"]["lat"] = float(references["dz"]["lat"])
        references["dz"]["lon"] = float(references["dz"]["lon"])
        references["dz"]["hmsl"] = int(references["dz"]["hmsl"])
        references["target"]["lat"] = float(references["dz"]["lat"])
        references["target"]["lon"] = float(references["dz"]["lon"])



    with open("elements.json", "r") as file:
        elements_data = json.load(file)

    # Create window
    window = create_window(window_width, window_height, window_background)
    # Variables for label text
    readings = {}
    for element_type, element_info in elements_data.items():
        if element_type != "labels":
            readings.update({element_type: {}})
            readings[element_type]["ref"] = tk.StringVar()

            position = {"x": element_info["x"], "y": element_info["y"]}
            font_size = element_info["font_size"]
            font_weight = "bold" if element_info["font_id"] == 1 else "normal"
            text_color = element_info["color"] if "color" in element_info.keys() else "yellow"
            readings[element_type]["precision"] = element_info["precision"] if "precision" in element_info.keys() else 0

            place_text(window, "", text_color, position, font_size, font_weight, window_background, readings[element_type]["ref"])
        else:
            for label_name, label_props in element_info.items():
                text_var = tk.StringVar()
                text_var.set(label_name)
                position = {"x": label_props["x"], "y": label_props["y"]}
                font_size = label_props["font_size"]
                font_weight = "bold" if label_props["font_id"] == 1 else "normal"
                text_color = label_props["color"] if "color" in label_props.keys() else "yellow"

                place_text(window, label_name, text_color, position, font_size, font_weight, window_background, text_var)

        ff_flag = tk.StringVar()
        ff_flag.set("====== airplane ======")
        place_text(window, "", "lightgray", {"x": 45, "y": 280}, 10, "normal", "gray", ff_flag)

        score = tk.StringVar()
        place_text(window, "", "blue", {"x": 45, "y": 130}, 15, "normal", "gray", score)
    # Open CSV file
    with open(csv_filename, newline='') as csvfile:
        csv_reader = csv.reader(csvfile)

        skip_lines(csv_reader, 2)
        # Schedule the first update after 0.2 seconds
        window.after(200, update_readings, window, csv_reader, readings, references)

        # Start the main event loop
        window.mainloop()

if __name__ == "__main__":
    main()
