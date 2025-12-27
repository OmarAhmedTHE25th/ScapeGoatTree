import tkinter as tk
import sys
from tkinter import messagebox

# Update this path if needed
sys.path.append(r'C:\Users\DELL\CLionProjects\ScapeGoatTree\CPP\cmake-build-debug')

try:
    import scapegoat_tree_py
    print("C++ Module loaded successfully!")
except ImportError as e:
    print(f"Error loading module: {e}")
    sys.exit(1)

class TreeGUI:
    def __init__(self, master):
        self.tree = scapegoat_tree_py.ScapeGoatTree()
        self.master = master
        self.master.title("ScapeGoat Tree Visualizer")
        self.master.state('zoomed') # This opens the window maximized automatically on Windows

        self.highlight_val = None
        # --- FIX 1: Create the Controls Frame FIRST ---
        self.controls = tk.Frame(master, bg="lightgray", pady=10)
        # Pack it at the BOTTOM so it sticks there
        self.controls.pack(side=tk.BOTTOM, fill=tk.X)

        # --- FIX 2: Create the Canvas SECOND ---
        self.canvas = tk.Canvas(master, bg="white")
        # Pack it to fill whatever space is left
        self.canvas.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        # Add Input & Buttons to the control frame
        tk.Label(self.controls, text="Value:", bg="lightgray").pack(side=tk.LEFT, padx=10)

        self.entry = tk.Entry(self.controls, font=("Arial", 14), width=10)
        self.entry.pack(side=tk.LEFT, padx=5)

        tk.Button(self.controls, text="Insert", command=self.add_node, bg="lightgreen", width=10).pack(side=tk.LEFT, padx=5)
        tk.Button(self.controls, text="Delete", command=self.remove_node, bg="lightcoral", width=10).pack(side=tk.LEFT, padx=5)

        # Initial message
        self.refresh_canvas()

    def add_node(self):
        val = self.entry.get()
        if val.lstrip('-').isdigit():
            self.tree.insert(int(val))
            self.refresh_canvas()
        self.entry.delete(0, tk.END)

    def remove_node(self):
        val = self.entry.get()
        if val.lstrip('-').isdigit():
            self.tree.delete_value(int(val))
            self.refresh_canvas()
        self.entry.delete(0, tk.END)

    def refresh_canvas(self):
        self.canvas.delete("all")
        root_node = self.tree.get_root()

        if root_node:
            self.draw_node(root_node, 500, 50, 250)
        else:
            # If you see this text, the Canvas is working!
            self.canvas.create_text(500, 300, text="Tree is empty (Try inserting 10)", font=("Arial", 20), fill="gray")

# Add 'highlight_val' to your class variables


def draw_node(self, node, x, y, x_offset):
    if not node: return

    # ... keep lines drawing code ...

    # Change color if it matches the search result
    node_color = "skyblue"
    if self.highlight_val is not None and node.value == self.highlight_val:
        node_color = "yellow"

    self.canvas.create_oval(x-20, y-20, x+20, y+20, fill=node_color, outline="black")
    self.canvas.create_text(x, y, text=str(node.value), font=("Arial", 10, "bold"))

def handle_search(self):
    val = self.entry.get()
    if val.isdigit():
        target = int(val)
        if self.tree.search(target): #
            self.highlight_val = target
        else:
            self.highlight_val = None
        self.refresh_canvas()
def show_balance_report(self):
    report = self.tree.is_balanced_report() # [cite: 100]
    messagebox.showinfo("Tree Balance Status", report)
def batch_insert(self, list_of_nums):
    for n in list_of_nums:
        self.tree.insert(n) # [cite: 15]

if __name__ == "__main__":
    root = tk.Tk()
    gui = TreeGUI(root)
    root.mainloop()