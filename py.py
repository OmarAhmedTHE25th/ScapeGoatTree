import sys
import tkinter as tk
from pathlib import Path

# ============================================
# ROBUST PATH RESOLUTION
# ============================================

def find_cpp_module():
    """
    Searches for the compiled C++ module in common build locations.
    Returns the path if found, None otherwise.
    """
    script_dir = Path(__file__).parent.resolve()

    # Common build directory patterns
    search_paths = [
        script_dir / "cmake-build-debug",
        script_dir / "cmake-build-release",
        script_dir / "build",
        script_dir / "cmake-build-relwithdebinfo",
        script_dir / "out" / "build",
        script_dir / "build" / "Debug",
        script_dir / "build" / "Release",
        ]

    # Also check if we're already in a build directory
    if "build" in str(script_dir).lower():
        search_paths.insert(0, script_dir)

    for search_path in search_paths:
        if not search_path.exists():
            continue



        # Look for any file starting with scapegoat_tree_py
        for file in search_path.iterdir():
            if file.is_file() and file.name.startswith("scapegoat_tree_py"):
                # Check if it's a Python extension module
                if file.suffix in ['.pyd', '.so'] or '.so' in file.name:
                    return str(search_path)

    return None

# Try to find and add the module path
module_path = find_cpp_module()

if module_path:
    sys.path.insert(0, module_path)
else:
    print("=" * 60)
    print("ERROR: Could not find scapegoat_tree_py module!")
    print("=" * 60)
    print("\nSearched in:")
    script_dir = Path(__file__).parent.resolve()
    for p in [script_dir / "cmake-build-debug", script_dir / "cmake-build-release", script_dir / "build"]:
        print(f"  • {p}")
        if p.exists():
            print(f"    Contents: {[f.name for f in p.iterdir() if f.is_file()][:5]}")
    print("\nPlease build the project first:")
    print("  mkdir build && cd build")
    print("  cmake .. && cmake --build .")
    print("=" * 60)
    sys.exit(1)

# Now try to import
try:
    import scapegoat_tree_py
    print("✓ Successfully imported C++ module")
except ImportError as e:
    print(f"✗ CRITICAL ERROR: Could not load C++ module.\n{e}")
    print(f"\nModule search path: {module_path}")
    print("\nMake sure the module is built correctly:")
    print("  cd build && cmake --build . --config Release")
    sys.exit(1)

# ============================================
# REST OF YOUR GUI CODE (unchanged)
# ============================================

class LogWindow:
    def __init__(self, parent):
        self.win = tk.Toplevel(parent)
        self.win.title("Tree Logs & Console")
        self.win.geometry("700x400")
        self.win.withdraw()

        self.win.protocol("WM_DELETE_WINDOW", self.win.withdraw)

        self.log_text = tk.Text(self.win, bg="#1e1e1e", fg="#d4d4d4",
                                insertbackground="white", font=("Consolas", 11))
        self.log_text.pack(fill=tk.BOTH, expand=True)
        self.log_text.config(state=tk.DISABLED)

    def show(self):
        self.win.deiconify()

    def log(self, msg):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, "> " + msg + "\n")
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)

class ScapeGoatGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("ScapeGoat Tree Visualizer (C++ Backend)")
        self.root.geometry("1100x750")
        self.root.state('zoomed')

        self.treeA = scapegoat_tree_py.ScapeGoatTree()
        self.treeB = scapegoat_tree_py.ScapeGoatTree()
        self.selected_tree_var = tk.StringVar(value="A")

        # Top Canvas
        self.top_frame = tk.Frame(root, bg="white")
        self.top_frame.pack(fill=tk.BOTH, expand=True, side=tk.TOP)
        self.canvas = tk.Canvas(self.top_frame, bg="white")
        self.canvas.pack(fill=tk.BOTH, expand=True)

        # Bottom Controls
        self.control_frame = tk.Frame(root, bg="#f0f0f0", bd=2, relief=tk.RAISED)
        self.control_frame.pack(fill=tk.X, side=tk.BOTTOM)

        # Section A: Inputs
        self.input_frame = tk.LabelFrame(self.control_frame, text="Inputs & Selection", padx=10, pady=10)
        self.input_frame.grid(row=0, column=0, sticky="nsw", padx=10, pady=10)
        tk.Label(self.input_frame, text="Value:").grid(row=0, column=0, sticky="w")
        self.entry_val = tk.Entry(self.input_frame, width=10, font=("Arial", 12))
        self.entry_val.grid(row=0, column=1, padx=5)

        tk.Label(self.input_frame, text="Active Tree:").grid(row=1, column=0, sticky="w", pady=10)
        tk.Radiobutton(self.input_frame, text="Tree A", variable=self.selected_tree_var, value="A",
                       command=self.refresh_ui).grid(row=1, column=1)
        tk.Radiobutton(self.input_frame, text="Tree B", variable=self.selected_tree_var, value="B",
                       command=self.refresh_ui).grid(row=1, column=2)

        # Section B: Operations
        self.ops_frame = tk.LabelFrame(self.control_frame, text="Operations", padx=10, pady=10)
        self.ops_frame.grid(row=0, column=1, sticky="nsw", padx=10, pady=10)
        btn_opts = {'width': 12, 'pady': 2}
        tk.Button(self.ops_frame, text="Insert", command=self.cmd_insert, bg="lightgreen", **btn_opts).grid(row=0, column=0, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Insert Batch", command=self.cmd_insertbatch, bg="lightgreen", **btn_opts).grid(row=0, column=1, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Delete", command=self.cmd_delete, bg="lightcoral", **btn_opts).grid(row=0, column=2, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Delete Batch", command=self.cmd_deletebatch, bg="lightcoral", **btn_opts).grid(row=0, column=3, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Search", command=self.cmd_search, bg="gold", **btn_opts).grid(row=0, column=4, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Show In-Order", command=lambda: self.cmd_show("in"), **btn_opts).grid(row=1, column=0, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Show Pre-Order", command=lambda: self.cmd_show("pre"), **btn_opts).grid(row=1, column=1, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Show Post-Order", command=lambda: self.cmd_show("post"), **btn_opts).grid(row=1, column=3, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Show Levels", command=lambda: self.cmd_show("levels"), **btn_opts).grid(row=1, column=2, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Check Balance", command=self.cmd_balance, **btn_opts).grid(row=2, column=0, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Clear Tree", command=self.cmd_clear, **btn_opts).grid(row=2, column=1, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Undo", command=self.cmd_undo, bg="orange", **btn_opts).grid(row=3, column=0, padx=5, pady=2)
        tk.Button(self.ops_frame, text="Redo", command=self.cmd_redo, bg="orange", **btn_opts).grid(row=3, column=1, padx=5, pady=2)

        # Section C: Advanced
        self.adv_frame = tk.LabelFrame(self.control_frame, text="Advanced / Demo", padx=10, pady=10)
        self.adv_frame.grid(row=0, column=2, sticky="nsw", padx=10, pady=10)
        tk.Button(self.adv_frame, text="Merge A+B -> A", command=self.cmd_merge, width=15, bg="lightblue").pack(pady=2)
        tk.Button(self.adv_frame, text="Compare A == B", command=self.cmd_compare, width=15).pack(pady=2)
        tk.Button(self.adv_frame, text="Is Empty?", command=self.cmd_isempty, width=15).pack(pady=2)
        tk.Button(self.adv_frame, text="Show Logs/Console", command=lambda: self.log_win.show(), width=15).pack(pady=2)

        self.log_win = LogWindow(self.root)
        self.log_win.log("Welcome! C++ Backend Connected.")

    def get_active_tree(self):
        return self.treeA if self.selected_tree_var.get() == "A" else self.treeB

    def log(self, msg):
        self.log_win.log(msg)

    def refresh_ui(self):
        self.draw_tree()

    def get_val(self):
        val = self.entry_val.get()
        if val.lstrip('-').isdigit():
            return int(val)
        return None

    def cmd_insert(self):
        val = self.get_val()
        if val is not None:
            self.get_active_tree().insert(val)
            self.log(f"Inserted {val} into Tree {self.selected_tree_var.get()}")
            self.entry_val.delete(0, tk.END)
            self.draw_tree()

    def cmd_insertbatch(self):
        raw_val = self.entry_val.get()
        try:
            batch_values = [int(x) for x in raw_val.split()]
            if batch_values:
                self.get_active_tree().insert_batch(batch_values)
                self.log(f"Inserted batch {batch_values} into Tree {self.selected_tree_var.get()}")
                self.entry_val.delete(0, tk.END)
                self.draw_tree()
            else:
                self.log("Error: Batch input is empty")
        except ValueError:
            self.log("Error: Batch must be numbers separated by spaces")

    def cmd_deletebatch(self):
        raw_val = self.entry_val.get()
        try:
            batch_values = [int(x) for x in raw_val.split()]
            if batch_values:
                self.get_active_tree().delete_batch(batch_values)
                self.log(f"Deleted batch {batch_values} from Tree {self.selected_tree_var.get()}")
                self.entry_val.delete(0, tk.END)
                self.draw_tree()
            else:
                self.log("Error: Batch input is empty")
        except ValueError:
            self.log("Error: Batch must be numbers separated by spaces")

    def cmd_delete(self):
        val = self.get_val()
        if val is not None:
            found = self.get_active_tree().delete_value(val)
            msg = "Deleted" if found else "Not Found"
            self.log(f"Delete {val} from Tree {self.selected_tree_var.get()}: {msg}")
            self.entry_val.delete(0, tk.END)
            self.draw_tree()

    def cmd_search(self):
        val = self.get_val()
        if val is not None:
            found = self.get_active_tree().search(val)
            res = "FOUND" if found else "NOT FOUND"
            self.log(f"Search {val}: {res}")
            if found:
                self.draw_tree(highlight_val=val)

    def cmd_undo(self):
        self.get_active_tree().undo()
        self.log(f"Undo performed on Tree {self.selected_tree_var.get()}")
        self.draw_tree()

    def cmd_redo(self):
        self.get_active_tree().redo()
        self.log(f"Redo performed on Tree {self.selected_tree_var.get()}")
        self.draw_tree()

    def cmd_clear(self):
        if self.selected_tree_var.get() == "A":
            self.treeA = scapegoat_tree_py.ScapeGoatTree()
        else:
            self.treeB = scapegoat_tree_py.ScapeGoatTree()
        self.draw_tree()
        self.log(f"Tree {self.selected_tree_var.get()} Cleared.")

    def cmd_show(self, mode):
        tree = self.get_active_tree()
        name = self.selected_tree_var.get()
        res = ""
        if mode == "in": res = tree.get_inorder()
        elif mode == "pre": res = tree.get_preorder()
        elif mode == "post": res = tree.get_postorder()
        elif mode == "levels": res = tree.get_levels()
        self.log(f"--- {mode} ({name}) ---\n{res}")

    def cmd_balance(self):
        res = self.get_active_tree().get_balance_report()
        self.log(f"--- Balance ({self.selected_tree_var.get()}) ---\n{res}")

    def cmd_merge(self):
        self.log("Merging Tree B into Tree A...")
        self.treeA = self.treeA + self.treeB
        self.selected_tree_var.set("A")
        self.draw_tree()
        self.log("Merge Complete. Showing Tree A.")

    def cmd_compare(self):
        is_equal = (self.treeA == self.treeB)
        self.log(f"Tree A == Tree B: {is_equal}")

    def cmd_isempty(self):
        empty = self.get_active_tree().is_empty()
        self.log(f"Tree {self.selected_tree_var.get()} Is Empty: {empty}")

    def draw_tree(self, highlight_val=None):
        self.canvas.delete("all")
        tree = self.get_active_tree()
        root = tree.get_root()

        if not root:
            self.canvas.create_text(550, 200, text=f"Tree {self.selected_tree_var.get()} is Empty",
                                    font=("Arial", 20), fill="gray")
            return

        h = self.get_height_py(root)
        canvas_w = 1100
        initial_dx = max(300, 30 * (2 ** (h - 1))) if h > 0 else 300
        self.draw_node_recursive(root, canvas_w // 2, 50, canvas_w // 4, highlight_val)

    def draw_node_recursive(self, node, x, y, dx, highlight):
        if not node: return

        if node.left:
            self.canvas.create_line(x, y, x - dx, y + 60, width=2)
            self.draw_node_recursive(node.left, x - dx, y + 60, dx / 2, highlight)
        if node.right:
            self.canvas.create_line(x, y, x + dx, y + 60, width=2)
            self.draw_node_recursive(node.right, x + dx, y + 60, dx / 2, highlight)

        r = 18
        color = "#add8e6"
        if highlight is not None and node.value == highlight:
            color = "#ff69b4"

        self.canvas.create_oval(x-r, y-r, x+r, y+r, fill=color, outline="black", width=2)
        self.canvas.create_text(x, y, text=str(node.value), font=("Arial", 10, "bold"))

    def get_height_py(self, node):
        if not node: return -1
        return 1 + max(self.get_height_py(node.left), self.get_height_py(node.right))

if __name__ == "__main__":
    root = tk.Tk()
    app = ScapeGoatGUI(root)
    root.mainloop()