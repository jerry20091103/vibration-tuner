import tkinter as tk
from tkinter import ttk, filedialog
import json

class ChordEntryApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Chord Entry UI")
        self.root.geometry("1920x1080")
        self.root.configure(bg="#2e2e2e")

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TLabel", background="#2e2e2e", foreground="white")
        style.configure("TButton", background="#4d4d4d", foreground="white")
        style.map("TButton", background=[("active", "#5a5a5a")], foreground=[("active", "black")])
        style.configure("TCombobox", background="#4d4d4d", foreground="white")
        style.map("TCombobox", fieldbackground=[("readonly", "#4d4d4d")], foreground=[("readonly", "white")])
        style.map("TCombobox", background=[("readonly", "#4d4d4d")], foreground=[("readonly", "white")])
        style.configure("TEntry", fieldbackground="#4d4d4d", foreground="white")

        # 樂譜設定
        self.bpm_label = ttk.Label(root, text="BPM:")
        self.bpm_label.grid(row=0, column=0, padx=10, pady=10)
        self.bpm_entry = ttk.Entry(root)
        self.bpm_entry.grid(row=0, column=1, padx=10, pady=10)

        self.beats_label = ttk.Label(root, text="Beats per Measure:")
        self.beats_label.grid(row=1, column=0, padx=10, pady=10)
        self.beats_entry = ttk.Entry(root)
        self.beats_entry.grid(row=1, column=1, padx=10, pady=10)
        self.beats_entry.bind("<KeyRelease>", self.update_measures)

        # 和弦選擇器
        self.chord_selector = ttk.Combobox(root, values=["C", "Cm", "C7", "Cm7", "Cmaj7", "Cdim", "Caug", "Csus2", "Csus4",
                                                         "D", "Dm", "D7", "Dm7", "Dmaj7", "Ddim", "Daug", "Dsus2", "Dsus4",
                                                         "E", "Em", "E7", "Em7", "Emaj7", "Edim", "Eaug", "Esus2", "Esus4",
                                                         "F", "Fm", "F7", "Fm7", "Fmaj7", "Fdim", "Faug", "Fsus2", "Fsus4",
                                                         "G", "Gm", "G7", "Gm7", "Gmaj7", "Gdim", "Gaug", "Gsus2", "Gsus4",
                                                         "A", "Am", "A7", "Am7", "Amaj7", "Adim", "Aaug", "Asus2", "Asus4",
                                                         "B", "Bm", "B7", "Bm7", "Bmaj7", "Bdim", "Baug", "Bsus2", "Bsus4"], style="TCombobox")
        self.chord_selector.grid(row=2, column=0, columnspan=2, padx=10, pady=10)
        self.chord_selector.set("C")

        # 增加小節的按鈕
        self.add_measure_button = ttk.Button(root, text="Add Measure", command=self.add_measure)
        self.add_measure_button.grid(row=3, column=0, columnspan=2, padx=10, pady=10)

        # 和弦輸入區域
        self.chords_frame = tk.Frame(root, bg="#2e2e2e")
        self.chords_frame.grid(row=4, column=0, columnspan=2, padx=10, pady=10)

        self.measures = []
        self.add_measure()

        # 儲存按鈕
        self.save_button = ttk.Button(root, text="Save", command=self.save_file)
        self.save_button.grid(row=5, column=0, padx=10, pady=10)

        # 傳送按鈕
        self.send_button = ttk.Button(root, text="Send to MCU", command=self.send_to_mcu)
        self.send_button.grid(row=5, column=1, padx=10, pady=10)

        # 讀取按鈕
        self.load_button = ttk.Button(root, text="Load", command=self.load_file)
        self.load_button.grid(row=6, column=0, columnspan=2, padx=10, pady=10)

    def add_measure(self):
        """新增一個小節"""
        measure_frame = tk.Frame(self.chords_frame, bg="#2e2e2e")
        measure_frame.pack(pady=10)

        measure_label = ttk.Label(measure_frame, text=f"Measure {len(self.measures) + 1}")
        measure_label.pack(side=tk.LEFT)

        measure_entries = []
        for i in range(int(self.beats_entry.get() or 4)):
            entry = ttk.Button(measure_frame, text="", width=5)
            entry.config(command=lambda e=entry: self.add_chord(e))
            entry.pack(side=tk.LEFT, padx=5)
            measure_entries.append(entry)

        self.measures.append(measure_entries)

    def update_measures(self, event=None):
        """更新所有小節的拍數"""
        beats_per_measure = int(self.beats_entry.get() or 4)
        for measure_entries in self.measures:
            current_beats = len(measure_entries)
            if current_beats < beats_per_measure:
                for _ in range(beats_per_measure - current_beats):
                    entry = ttk.Button(measure_entries[0].master, text="", width=5)
                    entry.config(command=lambda e=entry: self.add_chord(e))
                    entry.pack(side=tk.LEFT, padx=5)
                    measure_entries.append(entry)
            elif current_beats > beats_per_measure:
                for _ in range(current_beats - beats_per_measure):
                    entry = measure_entries.pop()
                    entry.destroy()

    def add_chord(self, entry):
        """在按鈕上放置和弦"""
        chord_name = self.chord_selector.get()
        entry.config(text=chord_name)

    def get_chord_data(self):
        """獲取和弦數據"""
        chords = []
        for measure_num, measure_entries in enumerate(self.measures):
            for i, entry in enumerate(measure_entries):
                chord_name = entry.cget("text")
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

        # 在此處添加串口通信代碼

    def save_file(self):
        """保存數據到文件"""
        bpm = self.bpm_entry.get()
        beats_per_measure = self.beats_entry.get()

        if not bpm.isdigit() or not beats_per_measure.isdigit():
            print("Please enter valid numbers for BPM and Beats per Measure.")
            return

        file_path = filedialog.asksaveasfilename(defaultextension=".json", filetypes=[("JSON files", "*.json")])
        print(f"File path: {file_path}")  # 添加這行來檢查文件路徑
        if file_path:
            chord_data = {
                "BPM": int(bpm),
                "beatsPerMeasure": int(beats_per_measure),
                "chords": self.get_chord_data()
            }
            with open(file_path, 'w') as file:
                json.dump(chord_data, file)
            print(f"Data saved to {file_path}")
        else:
            print("Save operation cancelled.")


    def load_file(self):
        """從文件讀取數據"""
        file_path = filedialog.askopenfilename(defaultextension=".json", filetypes=[("JSON files", "*.json")])
        if file_path:
            with open(file_path, 'r') as file:
                chord_data = json.load(file)
            self.bpm_entry.delete(0, tk.END)
            self.bpm_entry.insert(0, chord_data["BPM"])
            self.beats_entry.delete(0, tk.END)
            self.beats_entry.insert(0, chord_data["beatsPerMeasure"])
            self.measures = []
            for widget in self.chords_frame.winfo_children():
                widget.destroy()
            for measure_num in range((len(chord_data["chords"]) + int(self.beats_entry.get()) - 1) // int(self.beats_entry.get())):
                self.add_measure()
            for chord in chord_data["chords"]:
                self.measures[chord["measure"] - 1][chord["startBeat"]].config(text=chord["name"])

if __name__ == "__main__":
    root = tk.Tk()
    app = ChordEntryApp(root)
    root.mainloop()
