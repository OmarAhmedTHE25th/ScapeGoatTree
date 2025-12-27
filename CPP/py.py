import tkinter as tk
from tkinter import messagebox
import sys

# --- CONFIGURATION ---
# Update this to your cmake-build-debug path
BUILD_PATH = r'C:\Users\DELL\CLionProjects\ScapeGoatTree\CPP\cmake-build-debug'

if BUILD_PATH not in sys.path:
    sys.path.append(BUILD_PATH)

try:
    import scapegoat_tree_py
except ImportError as e:
    print(f"CRITICAL ERROR: Could not load module.\n{e}")
    sys.exit(1)

class ScapeGoatGUI:
    def __init__(self, master):
        self.tree = scapegoat_tree_py.ScapeGoatTree()
        self.master = master
        self.master.title("ScapeGoat Tree Visualizer")
        self.master.geometry("1100x850") # Made slightly taller
        self.highlight_val = None

        # 1. Top Bar (Search & Info)
        self.top_frame = tk.Frame(master, bg="#ddd", pady=10)
        self.top_frame.pack(side=tk.TOP, fill=tk.X)

        tk.Label(self.top_frame, text="Search:", bg="#ddd").pack(side=tk.LEFT, padx=10)
        self.search_entry = tk.Entry(self.top_frame, width=10)
        self.search_entry.pack(side=tk.LEFT, padx=5)
        tk.Button(self.top_frame, text="Go", command=self.do_search, bg="gold").pack(side=tk.LEFT)
        tk.Button(self.top_frame, text="Stats Report", command=self.show_report, bg="lightblue").pack(side=tk.RIGHT, padx=20)

        # 2. Main Canvas
        self.canvas = tk.Canvas(master, bg="white")
        self.canvas.pack(fill=tk.BOTH, expand=True)

        # 3. Traversal Buttons (NEW SECTION)
        self.trav_frame = tk.Frame(master, bg="#f0f0f0", pady=5)
        self.trav_frame.pack(side=tk.BOTTOM, fill=tk.X)

        tk.Label(self.trav_frame, text="Displays:", bg="#f0f0f0", font=("Arial", 10, "bold")).pack(side=tk.LEFT, padx=10)

        # Add the 4 display buttons
        tk.Button(self.trav_frame, text="In-Order", command=self.show_inorder).pack(side=tk.LEFT, padx=5)
        tk.Button(self.trav_frame, text="Pre-Order", command=self.show_preorder).pack(side=tk.LEFT, padx=5)
        tk.Button(self.trav_frame, text="Post-Order", command=self.show_postorder).pack(side=tk.LEFT, padx=5)
        tk.Button(self.trav_frame, text="Level-Order", command=self.show_levels).pack(side=tk.LEFT, padx=5)

        # 4. Edit Controls
        self.controls = tk.Frame(master, bg="#eee", pady=10)
        self.controls.pack(side=tk.BOTTOM, fill=tk.X)

        tk.Label(self.controls, text="Edit Value:", bg="#eee").pack(side=tk.LEFT, padx=10)
        self.main_entry = tk.Entry(self.controls, width=10, font=("Arial", 12))
        self.main_entry.pack(side=tk.LEFT, padx=5)
        tk.Button(self.controls, text="Insert", command=self.do_insert, bg="lightgreen").pack(side=tk.LEFT, padx=5)
        tk.Button(self.controls, text="Delete", command=self.do_delete, bg="lightcoral").pack(side=tk.LEFT, padx=5)
        tk.Button(self.controls, text="Clear", command=self.do_clear, bg="silver").pack(side=tk.LEFT, padx=20)

    # --- NEW DISPLAY FUNCTIONS ---
    def show_inorder(self):
        result = self.tree.get_inorder()
        messagebox.showinfo("In-Order Traversal", result)

    def show_preorder(self):
        result = self.tree.get_preorder()
        messagebox.showinfo("Pre-Order Traversal", result)

    def show_postorder(self):
        result = self.tree.get_postorder()
        messagebox.showinfo("Post-Order Traversal", result)

    def show_levels(self):
        result = self.tree.get_levelorder()
        messagebox.showinfo("Level-Order Traversal", result)

    # --- EXISTING FUNCTIONS (Keep these as they were) ---
    def do_insert(self):
        val = self.main_entry.get()
        if val.lstrip('-').isdigit():
            self.tree.insert(int(val))
            self.highlight_val = None
            self.refresh()
        self.main_entry.delete(0, tk.END)

    def do_delete(self):
        val = self.main_entry.get()
        if val.lstrip('-').isdigit():
            self.tree.delete_value(int(val))
            self.highlight_val = None
            self.refresh()
        self.main_entry.delete(0, tk.END)

    def do_search(self):
        val = self.search_entry.get()
        if val.lstrip('-').isdigit():
            target = int(val)
            if self.tree.search(target):
                self.highlight_val = target
                messagebox.showinfo("Found", f"{target} is in the tree!")
            else:
                self.highlight_val = None
                messagebox.showwarning("Missing", f"{target} not found.")
            self.refresh()
        self.search_entry.delete(0, tk.END)

    def show_report(self):
        messagebox.showinfo("Stats", self.tree.get_balance_report())

    def do_clear(self):
        self.tree = scapegoat_tree_py.ScapeGoatTree()
        self.refresh()

    def refresh(self):
        self.canvas.delete("all")
        root = self.tree.get_root()
        if root:
            self.draw_node(root, 550, 50, 250)
        else:
            self.canvas.create_text(550, 300, text="Tree is Empty", font=("Arial", 20), fill="gray")

    def draw_node(self, node, x, y, x_offset):
        if not node: return
        if node.left:
            self.canvas.create_line(x, y, x - x_offset, y + 60, width=2)
            self.draw_node(node.left, x - x_offset, y + 60, x_offset / 2)
        if node.right:
            self.canvas.create_line(x, y, x + x_offset, y + 60, width=2)
            self.draw_node(node.right, x + x_offset, y + 60, x_offset / 2)

        fill_col = "gold" if (self.highlight_val is not None and node.value == self.highlight_val) else "skyblue"
        self.canvas.create_oval(x-20, y-20, x+20, y+20, fill=fill_col, outline="black")
        self.canvas.create_text(x, y, text=str(node.value), font=("Arial", 10, "bold"))

if __name__ == "__main__":
    root = tk.Tk()
    app = ScapeGoatGUI(root)
    root.mainloop()