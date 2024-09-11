import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import json
import serial
import serial.tools.list_ports


class ChordEntryApp:
    def __init__(self, root):
        self.root = root
        self.root.title("ChordPulse X")
        self.root.geometry("720x720")
        self.root.configure(bg="#2e2e2e")

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TLabel", background="#2e2e2e", foreground="white")
        style.configure("TButton", background="#4d4d4d", foreground="white")
        style.map("TButton", background=[
                  ("active", "#5a5a5a")], foreground=[("active", "black")])
        style.configure("TCombobox", background="#4d4d4d",
                        foreground="white", width=20)
        style.map("TCombobox", fieldbackground=[
                  ("readonly", "#4d4d4d")], foreground=[("readonly", "white")])
        style.map("TCombobox", background=[
                  ("readonly", "#4d4d4d")], foreground=[("readonly", "white")])
        style.configure("TEntry", fieldbackground="#4d4d4d",
                        foreground="white")

        # 和弦選擇部分
        self.selector_frame = tk.Frame(self.root, bg="#2e2e2e")
        self.selector_frame.grid(
            row=2, column=0, padx=10, pady=10, sticky='ew')

        self.chord_selector_label = ttk.Label(
            self.selector_frame, text="Chord Selector:")
        self.chord_selector_label.pack(side=tk.LEFT, padx=5)

        self.chord_selector = tk.StringVar()
        self.chord_selector.set("Select Chord")
        self.chord_menu = tk.Menu(
            self.root, tearoff=0, bg="#4d4d4d", fg="white")

        chords = {
            "C": ["C", "Cm", "C7", "Cm7", "Cmaj7", "Cdim", "Caug", "Csus2", "Csus4"],
            "D": ["D", "Dm", "D7", "Dm7", "Dmaj7", "Ddim", "Daug", "Dsus2", "Dsus4"],
            "E": ["E", "Em", "E7", "Em7", "Emaj7", "Edim", "Eaug", "Esus2", "Esus4"],
            "F": ["F", "Fm", "F7", "Fm7", "Fmaj7", "Fdim", "Faug", "Fsus2", "Fsus4"],
            "G": ["G", "Gm", "G7", "Gm7", "Gmaj7", "Gdim", "Gaug", "Gsus2", "Gsus4"],
            "A": ["A", "Am", "A7", "Am7", "Amaj7", "Adim", "Aaug", "Asus2", "Asus4"],
            "B": ["B", "Bm", "B7", "Bm7", "Bmaj7", "Bdim", "Baug", "Bsus2", "Bsus4"]
        }

        for key in chords:
            submenu = tk.Menu(self.chord_menu, tearoff=0,
                              bg="#4d4d4d", fg="white")
            for chord in chords[key]:
                submenu.add_command(
                    label=chord, command=lambda c=chord: self.set_chord(c))
            self.chord_menu.add_cascade(label=key, menu=submenu)

        self.chord_button = tk.Button(
            self.selector_frame, textvariable=self.chord_selector, command=self.show_chord_menu)
        self.chord_button.pack(side=tk.LEFT)

        # 設定框架
        self.settings_frame = tk.Frame(self.root, bg="#2e2e2e")
        self.settings_frame.grid(
            row=0, column=0, padx=10, pady=10, sticky='ew')

        self.bpm_label = ttk.Label(self.settings_frame, text="BPM:")
        self.bpm_label.grid(row=0, column=0, padx=10, pady=10)
        self.bpm_entry = ttk.Entry(self.settings_frame)
        self.bpm_entry.grid(row=0, column=1, padx=10, pady=10)

        self.beats_label = ttk.Label(
            self.settings_frame, text="Beats per Measure:")
        self.beats_label.grid(row=1, column=0, padx=10, pady=10)
        self.beats_entry = ttk.Entry(self.settings_frame)
        self.beats_entry.grid(row=1, column=1, padx=10, pady=10)
        self.beats_entry.bind("<KeyRelease>", self.update_measures)

        # 按鈕框架
        self.button_frame = tk.Frame(self.root, bg="#2e2e2e")
        self.button_frame.grid(row=1, column=0, padx=10, pady=10, sticky='ew')

        self.load_button = ttk.Button(
            self.button_frame, text="Load", command=self.load_file)
        self.load_button.grid(row=0, column=0, padx=10, pady=10)

        self.save_button = ttk.Button(
            self.button_frame, text="Save", command=self.save_file)
        self.save_button.grid(row=0, column=1, padx=10, pady=10)

        self.send_button = ttk.Button(
            self.button_frame, text="Send to MCU", command=self.send_to_mcu)
        self.send_button.grid(row=0, column=2, padx=10, pady=10)

        # 和弦輸入區域
        self.chords_frame = tk.Frame(self.root, bg="#2e2e2e")
        self.chords_frame.grid(row=3, column=0, padx=10, pady=10, sticky='ew')

        self.measures = []
        self.add_measure()

    def set_chord(self, chord_name):
        """設置和弦選擇器的值"""
        self.chord_selector.set(chord_name)

    def show_chord_menu(self):
        """顯示和弦選擇菜單"""
        self.chord_menu.post(self.chord_button.winfo_rootx(
        ), self.chord_button.winfo_rooty() + self.chord_button.winfo_height())

    def add_measure(self, insert_after=None):
        """新增一個小節"""
        measure_frame = tk.Frame(self.chords_frame, bg="#2e2e2e")
        measure_frame.pack(pady=10, fill=tk.X, expand=True)

        # 小節編號標籤
        measure_label = ttk.Label(measure_frame, text=f"Measure {
                                  len(self.measures) + 1}")
        measure_label.pack(side=tk.LEFT)

        measure_entries = []
        for i in range(int(self.beats_entry.get() or 4)):
            entry = ttk.Button(measure_frame, text="", width=5)
            entry.config(command=lambda e=entry: self.add_chord(e))
            entry.pack(side=tk.LEFT, padx=5)
            measure_entries.append(entry)

        # 放在右邊的 "+" 按鈕來新增下一個小節
        button_frame = tk.Frame(measure_frame, bg="#2e2e2e")
        button_frame.pack(side=tk.RIGHT)

        add_button = ttk.Button(button_frame, text="+", command=lambda: self.add_measure(
            insert_after=insert_after + 1 if insert_after is not None else len(self.measures)))
        add_button.pack(side=tk.LEFT, padx=5)

        # 放在右邊的 "-" 按鈕來刪除該小節
        remove_button = ttk.Button(button_frame, text="-", command=lambda: self.remove_measure(
            measure_frame, insert_after if insert_after is not None else len(self.measures) - 1))
        remove_button.pack(side=tk.LEFT, padx=5)

        if insert_after is None or insert_after >= len(self.measures):
            # 添加到最後一個小節
            self.measures.append(measure_entries)
        else:
            # 插入到選定的小節之後
            self.measures.insert(insert_after + 1, measure_entries)
            measure_frame.pack_forget()
            measure_frame.pack(pady=10, fill=tk.X, expand=True,
                               before=self.chords_frame.winfo_children()[insert_after + 1])

        self.update_measure_labels()

    def remove_measure(self, measure_frame, index):
        """刪除指定的小節"""
        if len(self.measures) > 1:  # 至少要保留一個小節
            self.measures.pop(index)  # 刪除該小節的和弦按鈕
            measure_frame.destroy()  # 刪除該小節的整個框架
            self.update_measure_labels()

    def update_measure_labels(self):
        """更新所有小節的標籤編號"""
        for i, measure_entries in enumerate(self.measures):
            measure_frame = measure_entries[0].master
            measure_label = measure_frame.winfo_children()[0]  # 取得 Label
            measure_label.config(text=f"Measure {i + 1}")

    def update_measures(self, event=None):
        """更新所有小節的拍數"""
        beats_per_measure = int(self.beats_entry.get() or 4)
        # 更新每個小節的拍數
        for measure_entries in self.measures:
            current_beats = len(measure_entries)
            if current_beats < beats_per_measure:
                # 如果當前拍數小於需要的拍數，添加更多和弦按鈕
                for _ in range(beats_per_measure - current_beats):
                    entry = ttk.Button(
                        measure_entries[0].master, text="", width=5)
                    entry.config(command=lambda e=entry: self.add_chord(e))
                    entry.pack(side=tk.LEFT, padx=5)
                    measure_entries.append(entry)
            elif current_beats > beats_per_measure:
                # 如果當前拍數大於需要的拍數，移除多餘的和弦按鈕
                for _ in range(current_beats - beats_per_measure):
                    entry = measure_entries.pop()
                    entry.pack_forget()

    def add_chord(self, entry):
        """設定按鈕上的和弦"""
        selected_chord = self.chord_selector.get()
        if selected_chord != "Select Chord":
            entry.config(text=selected_chord)

    def load_file(self):
        """從JSON文件加載和弦"""
        file_path = filedialog.askopenfilename(
            filetypes=[("JSON files", "*.json")])
        if not file_path:
            return
        try:
            with open(file_path, 'r') as file:
                data = json.load(file)

                # 加載BPM和拍數
                self.bpm_entry.delete(0, tk.END)
                self.bpm_entry.insert(0, data.get('BPM', ''))
                self.beats_entry.delete(0, tk.END)
                self.beats_entry.insert(0, data.get('beatsPerMeasure', ''))

                beats_per_measure = int(self.beats_entry.get() or 4)

                # 清空當前的和弦小節
                for measure_frame in self.chords_frame.winfo_children():
                    measure_frame.destroy()
                self.measures.clear()

                # 總拍數需要重新初始化以追踪和弦的放置位置
                total_beats = 0
                current_measure = []
                measure_index = 0

                for chord_data in data.get('chords', []):
                    chord_name = chord_data["name"]
                    start_beat = chord_data["startBeat"]
                    end_beat = chord_data["endBeat"]

                    # 計算需要多少 measure，並補上空白的 measure
                    while total_beats < start_beat:
                        # 如果當前小節的拍數還沒滿，填充空拍
                        remaining_beats = beats_per_measure - \
                            len(current_measure)
                        if remaining_beats > 0:
                            for _ in range(min(remaining_beats, start_beat - total_beats)):
                                current_measure.append("")  # 空拍
                                total_beats += 1

                        # 如果該小節滿了，將它加到 measures 中，並新增下一個小節
                        if len(current_measure) >= beats_per_measure:
                            self.add_measure()
                            for i, entry in enumerate(self.measures[measure_index]):
                                entry.config(text=current_measure[i] if i < len(
                                    current_measure) else "")
                            measure_index += 1
                            current_measure = []

                    # 將和弦加到當前拍數
                    current_measure.append(chord_name)
                    total_beats += 1

                    # 當一個小節填滿時，加入到 measures，並開始下一個小節
                    if len(current_measure) >= beats_per_measure:
                        self.add_measure()
                        for i, entry in enumerate(self.measures[measure_index]):
                            entry.config(text=current_measure[i] if i < len(
                                current_measure) else "")
                        measure_index += 1
                        current_measure = []

                # 處理最後剩下未填充滿的小節
                if current_measure:
                    self.add_measure()
                    for i, entry in enumerate(self.measures[measure_index]):
                        entry.config(text=current_measure[i] if i < len(
                            current_measure) else "")

                messagebox.showinfo("Load", "Chord file loaded successfully!")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load file: {e}")

    def save_file(self):
        """將和弦保存到JSON文件"""
        file_path = filedialog.asksaveasfilename(
            defaultextension=".json", filetypes=[("JSON files", "*.json")])
        if not file_path:
            return
        try:
            chords_data = {
                'BPM': self.bpm_entry.get(),
                'beatsPerMeasure': self.beats_entry.get(),
                'chords': self.generate_chord_data()
            }
            with open(file_path, 'w') as file:
                json.dump(chords_data, file, indent=4)
            messagebox.showinfo("Save", "Chord file saved successfully!")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to save file: {e}")

    def send_to_mcu(self):
        """將和弦數據傳送到MCU"""
        try:
            chords_data = {
                'BPM': int(self.bpm_entry.get()),
                'beatsPerMeasure': int(self.beats_entry.get()),
                'chords': self.generate_chord_data()
            }
            json_data = json.dumps(chords_data)
            # 列出可用的 COM 埠
            ports = list(serial.tools.list_ports.comports())
            if not ports:
                messagebox.showerror("Error", "No COM ports found.")
                return

            # 建立選擇 COM 埠的對話框
            port_list = [port.device for port in ports]
            port_window = tk.Toplevel(self.root)
            port_window.title("Select COM Port")
            port_window.geometry("300x100")
            tk.Label(port_window, text="Select COM Port:").pack(pady=10)
            selected_port = tk.StringVar(port_window)
            selected_port.set(port_list[0])
            port_menu = ttk.Combobox(
                port_window, textvariable=selected_port, values=port_list, state='readonly')
            port_menu.pack()

            def send_data():
                port = selected_port.get()
                baud_rate = 9600
                try:
                    with serial.Serial(port, baud_rate, timeout=1) as ser:
                        ser.write(json_data.encode('utf-8'))
                        ser.flush()
                    messagebox.showinfo("Success", f"Data sent to {
                                        port} successfully!")
                    port_window.destroy()
                except Exception as e:
                    messagebox.showerror("Error", f"Failed to send data: {e}")
                    port_window.destroy()

            send_button = ttk.Button(
                port_window, text="Send", command=send_data)
            send_button.pack(pady=10)

        except Exception as e:
            messagebox.showerror("Error", f"Failed to send data: {e}")

    def generate_chord_data(self):
        """生成包含startBeat和endBeat的和弦數據"""
        chords = []
        beats_per_measure = int(self.beats_entry.get() or 4)
        total_beats = 0

        for measure_entries in self.measures:
            for i, entry in enumerate(measure_entries):
                chord_name = entry.cget('text')
                if chord_name:  # 如果和弦名稱不為空
                    start_beat = total_beats
                    end_beat = total_beats + 1  # 假設每個和弦佔用一拍
                    chords.append({
                        "name": chord_name,
                        "startBeat": start_beat,
                        "endBeat": end_beat
                    })
                total_beats += 1  # 每次加一拍

        return chords


if __name__ == "__main__":
    root = tk.Tk()
    app = ChordEntryApp(root)
    root.mainloop()
