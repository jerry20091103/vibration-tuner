import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import serial
import json

class ChordEntryApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Chord Entry UI")

        # 樂譜設定
        self.bpm_label = tk.Label(root, text="BPM:")
        self.bpm_label.grid(row=0, column=0)
        self.bpm_entry = tk.Entry(root)
        self.bpm_entry.grid(row=0, column=1)

        self.beats_label = tk.Label(root, text="Beats per Measure:")
        self.beats_label.grid(row=1, column=0)
        self.beats_entry = tk.Entry(root)
        self.beats_entry.grid(row=1, column=1)

        # 增加小節的按鈕
        self.add_measure_button = tk.Button(root, text="Add Measure", command=self.add_measure)
        self.add_measure_button.grid(row=2, column=0, columnspan=2)

        # 和弦輸入區域
        self.chords_frame = tk.Frame(root)
        self.chords_frame.grid(row=3, column=0, columnspan=2)

        self.measures = []
        self.add_measure()

        # 儲存與載入按鈕
        self.save_button = tk.Button(root, text="Save", command=self.save_file)
        self.save_button.grid(row=4, column=0)

        self.load_button = tk.Button(root, text="Load", command=self.load_file)
        self.load_button.grid(row=4, column=1)

        # 傳送按鈕
        self.send_button = tk.Button(root, text="Send to MCU", command=self.send_to_mcu)
        self.send_button.grid(row=5, column=0, columnspan=2)

        # 串口設置
        self.port_label = tk.Label(root, text="Serial Port:")
        self.port_label.grid(row=6, column=0)
        self.port_entry = tk.Entry(root)
        self.port_entry.grid(row=6, column=1)

        self.baudrate_label = tk.Label(root, text="Baudrate:")
        self.baudrate_label.grid(row=7, column=0)
        self.baudrate_entry = tk.Entry(root)
        self.baudrate_entry.grid(row=7, column=1)
        self.baudrate_entry.insert(0, "9600")

    def add_measure(self):
        """新增一個小節"""
        measure_frame = tk.Frame(self.chords_frame)
        measure_frame.pack(pady=10)

        measure_label = tk.Label(measure_frame, text=f"Measure {len(self.measures) + 1}")
        measure_label.pack(side=tk.LEFT)

        measure_entries = []
        for i in range(int(self.beats_entry.get() or 4)):
            entry = tk.Entry(measure_frame)
            entry.pack(side=tk.LEFT, padx=5)
            measure_entries.append(entry)

        self.measures.append(measure_entries)

    def get_chord_data(self):
        """獲取和弦數據"""
        chords = []
        for measure_num, measure_entries in enumerate(self.measures):
            for i, entry in enumerate(measure_entries):
                chord_name = entry.get()
                if chord_name:
                    chords.append({
                        "name": chord_name,
                        "startBeat": i,
                        "endBeat": i + 1,
                        "measure": measure_num + 1
                    })
        return chords

    def send_to_mcu(self):
        """將數據發送到 MCU"""
        bpm = int(self.bpm_entry.get())
        beats_per_measure = int(self.beats_entry.get())
        chords = self.get_chord_data()

        chord_data = {
            "BPM": bpm,
            "beatsPerMeasure": beats_per_measure,
            "chords": chords
        }

        json_data = json.dumps(chord_data)
        print("Sending JSON data:", json_data)

        port = self.port_entry.get()
        baudrate = int(self.baudrate_entry.get())

        try:
            ser = serial.Serial(port, baudrate, timeout=1)
            ser.write(json_data.encode('utf-8'))
            ser.close()
            print("Data sent to MCU successfully.")
        except serial.SerialException as e:
            print(f"Failed to send data: {e}")

    def save_file(self):
        """保存數據到文件"""
        chord_data = {
            "BPM": int(self.bpm_entry.get()),
            "beatsPerMeasure": int(self.beats_entry.get()),
            "chords": self.get_chord_data()
        }
        file_path = filedialog.asksaveasfilename(defaultextension=".json",
                                                 filetypes=[("JSON files", "*.json")])
        if file_path:
            with open(file_path, 'w') as file:
                json.dump(chord_data, file)
            print(f"Data saved to {file_path}")

    def load_file(self):
        """從文件中加載數據"""
        file_path = filedialog.askopenfilename(defaultextension=".json",
                                               filetypes=[("JSON files", "*.json")])
        if file_path:
            with open(file_path, 'r') as file:
                chord_data = json.load(file)

            # 更新 UI 來反映載入的數據
            self.bpm_entry.delete(0, tk.END)
            self.bpm_entry.insert(0, chord_data.get("BPM", 120))
            self.beats_entry.delete(0, tk.END)
            self.beats_entry.insert(0, chord_data.get("beatsPerMeasure", 4))

            # 清空現有的小節
            for widget in self.chords_frame.winfo_children():
                widget.destroy()
            self.measures = []

            # 加載新的小節和和弦
            measure_num = 1
            while True:
                measure_chords = [c for c in chord_data["chords"] if c["measure"] == measure_num]
                if not measure_chords:
                    break
                self.add_measure()
                for i, chord in enumerate(measure_chords):
                    self.measures[-1][i].insert(0, chord["name"])
                measure_num += 1

            print(f"Data loaded from {file_path}")

if __name__ == "__main__":
    root = tk.Tk()
    app = ChordEntryApp(root)
    root.mainloop()
