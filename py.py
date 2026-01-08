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
# GUI
# ============================================
class AnimationManager:
    """Handles smooth animations for tree operations"""

    def __init__(self, gui):
        self.gui = gui
        self.is_animating = False
        self.animation_speed = 500  # milliseconds per frame

    def animate_search_path(self, value):
        """Animate the search path to a value"""
        if self.is_animating:
            return

        self.is_animating = True
        path = self._get_search_path(value)

        def highlight_step(index):
            if index >= len(path):
                # Final highlight in pink if found
                found = self.gui.get_active_tree().search(value)
                if found:
                    self.gui.draw_tree(highlight_val=value, highlight_color="#ff1493")
                self.is_animating = False
                return

            # Highlight current node in yellow
            node_val = path[index].value
            self.gui.draw_tree(highlight_val=node_val, highlight_color="#ffd700")
            self.gui.log(f"Searching... checking node {node_val}")

            # Schedule next step
            self.gui.root.after(self.animation_speed, lambda: highlight_step(index + 1))

        highlight_step(0)

    def animate_insertion(self, value, callback=None):
        """Animate the insertion of a value"""
        if self.is_animating:
            return

        self.is_animating = True
        path = self._get_insertion_path(value)

        def insert_step(index):
            if index >= len(path):
                # Actually insert the value
                self.gui.get_active_tree().insert(value)
                self.gui.draw_tree(highlight_val=value, highlight_color="#00ff00")
                self.gui.log(f"✓ Inserted {value}")
                self.is_animating = False
                if callback:
                    callback()
                return

            # Highlight current node being compared
            node_val = path[index].value
            self.gui.draw_tree(highlight_val=node_val, highlight_color="#ffff00")

            if value < node_val:
                self.gui.log(f"{value} < {node_val}, go left")
            else:
                self.gui.log(f"{value} > {node_val}, go right")

            self.gui.root.after(self.animation_speed, lambda: insert_step(index + 1))

        insert_step(0)

    def animate_deletion(self, value, callback=None):
        """Animate the deletion of a value"""
        if self.is_animating:
            return

        self.is_animating = True
        path = self._get_search_path(value)

        def delete_step(index):
            if index >= len(path):
                # Highlight node to delete in red, then delete it
                self.gui.draw_tree(highlight_val=value, highlight_color="#ff0000")
                self.gui.log(f"Deleting {value}...")

                def perform_delete():
                    found = self.gui.get_active_tree().delete_value(value)
                    if found:
                        self.gui.draw_tree()
                        self.gui.log(f"✓ Deleted {value}")
                    else:
                        self.gui.log(f"✗ Value {value} not found")
                    self.is_animating = False
                    if callback:
                        callback()

                self.gui.root.after(self.animation_speed, perform_delete)
                return

            # Highlight search path
            node_val = path[index].value
            self.gui.draw_tree(highlight_val=node_val, highlight_color="#ffaa00")
            self.gui.log(f"Searching for {value}... at {node_val}")

            self.gui.root.after(self.animation_speed, lambda: delete_step(index + 1))

        delete_step(0)

    def animate_batch_operation(self, values, operation="insert"):
        """Animate a batch of insertions or deletions"""
        if self.is_animating or not values:
            return

        self.is_animating = True
        current_index = [0]  # Use list to modify in nested function

        def process_next():
            if current_index[0] >= len(values):
                self.is_animating = False
                self.gui.log(f"✓ Batch {operation} complete!")
                return

            value = values[current_index[0]]
            current_index[0] += 1

            if operation == "insert":
                self.gui.get_active_tree().insert(value)
                self.gui.draw_tree(highlight_val=value, highlight_color="#00ff00")
                self.gui.log(f"Inserted {value} ({current_index[0]}/{len(values)})")
            else:
                self.gui.get_active_tree().delete_value(value)
                self.gui.draw_tree()
                self.gui.log(f"Deleted {value} ({current_index[0]}/{len(values)})")

            # Schedule next value
            self.gui.root.after(self.animation_speed // 2, process_next)

        process_next()

    def _get_search_path(self, value):
        """Get the path to search for a value"""
        path = []
        current = self.gui.get_active_tree().get_root()

        while current:
            path.append(current)
            if value == current.value:
                break
            elif value < current.value:
                current = current.left
            else:
                current = current.right

        return path

    def _get_insertion_path(self, value):
        """Get the path where a value would be inserted"""
        path = []
        current = self.gui.get_active_tree().get_root()

        if not current:
            return path

        while current:
            path.append(current)
            if value < current.value:
                if not current.left:
                    break
                current = current.left
            elif value > current.value:
                if not current.right:
                    break
                current = current.right
            else:
                break  # Value already exists

        return path
    def animate_sum_in_range(self, low, high):
        """Animate finding and summing nodes within a range"""
        if self.is_animating:
            return

        self.is_animating = True
        nodes_in_range = []

        # Helper to perform in-order traversal to find nodes in range
        def find_nodes(node):
            if not node:
                return
            if node.value > low:
                find_nodes(node.left)
            if low <= node.value <= high:
                nodes_in_range.append(node.value)
            if node.value < high:
                find_nodes(node.right)

        find_nodes(self.gui.get_active_tree().get_root())

        if not nodes_in_range:
            self.gui.log(f"No nodes found in range [{low}, {high}]")
            self.is_animating = False
            return

        current_sum = [0]

        def sum_step(index):
            if index >= len(nodes_in_range):
                self.gui.log(f"Total Sum in [{low}, {high}]: {current_sum[0]}")
                self.is_animating = False
                return

            val = nodes_in_range[index]
            current_sum[0] += val

            # Highlight current node in range with a distinct color (e.g., Purple/Orchid)
            self.gui.draw_tree(highlight_val=val, highlight_color="#da70d6")
            self.gui.log(f"Adding {val} to sum... (Current: {current_sum[0]})")

            self.gui.root.after(self.animation_speed, lambda: sum_step(index + 1))

        sum_step(0)
    def animate_values_in_range(self, low, high):
        """Animate finding and listing nodes within a range"""
        if self.is_animating:
            return

        self.is_animating = True
        nodes_in_range = []

        def find_nodes(node):
            if not node:
                return
            if node.value > low:
                find_nodes(node.left)
            if low <= node.value <= high:
                nodes_in_range.append(node.value)
            if node.value < high:
                find_nodes(node.right)

        find_nodes(self.gui.get_active_tree().get_root())

        if not nodes_in_range:
            self.gui.log(f"No nodes found in range [{low}, {high}]")
            self.is_animating = False
            return

        def highlight_step(index):
            if index >= len(nodes_in_range):
                self.gui.log(f"✓ All values in [{low}, {high}]: {nodes_in_range}")
                self.is_animating = False
                return

            val = nodes_in_range[index]
            self.gui.draw_tree(highlight_val=val, highlight_color="#da70d6")
            self.gui.log(f"Found node in range: {val}")

            self.gui.root.after(self.animation_speed, lambda: highlight_step(index + 1))

        highlight_step(0)

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
        self.root.title("🎬 Animated ScapeGoat Tree Visualizer")
        self.root.geometry("1200x800")
        self.root.state('zoomed')

        # Data structures
        self.treeA = scapegoat_tree_py.ScapeGoatTree()
        self.treeB = scapegoat_tree_py.ScapeGoatTree()
        self.selected_tree_var = tk.StringVar(value="A")

        # Animation manager
        self.animator = AnimationManager(self)
        self.animation_enabled = tk.BooleanVar(value=True)

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

        tk.Label(self.input_frame, text="Range :").grid(row=0, column=2, sticky="w")
        self.entry_high = tk.Entry(self.input_frame, width=10, font=("Arial", 12))
        self.entry_high.grid(row=0, column=3, padx=5)

        tk.Label(self.input_frame, text="Value:").grid(row=0, column=0, sticky="w")
        self.entry_val = tk.Entry(self.input_frame, width=10, font=("Arial", 12))
        self.entry_val.grid(row=0, column=1, padx=5)

        tk.Label(self.input_frame, text="Active Tree:").grid(row=1, column=0, sticky="w", pady=10)
        tk.Radiobutton(self.input_frame, text="Tree A", variable=self.selected_tree_var, value="A",
                       command=self.refresh_ui).grid(row=1, column=1)
        tk.Radiobutton(self.input_frame, text="Tree B", variable=self.selected_tree_var, value="B",
                       command=self.refresh_ui).grid(row=1, column=2)

        # Animation toggle
        tk.Checkbutton(self.input_frame, text="🎬 Animate", variable=self.animation_enabled,
                       font=("Arial", 10, "bold")).grid(row=2, column=0, columnspan=2, pady=5)

        # Section B: Operations (with animation variants)
        self.ops_frame = tk.LabelFrame(self.control_frame, text="Operations", padx=10, pady=10)
        self.ops_frame.grid(row=0, column=1, sticky="nsw", padx=10, pady=10)

        btn_opts = {'width': 13, 'pady': 2}

        # Row 0: Basic ops
        tk.Button(self.ops_frame, text="Insert", command=self.cmd_insert, bg="lightgreen", **btn_opts).grid(row=0, column=0, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Insert Batch", command=self.cmd_insertbatch, bg="lightgreen", **btn_opts).grid(row=0, column=1, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Delete", command=self.cmd_delete, bg="lightcoral", **btn_opts).grid(row=0, column=2, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Delete Batch", command=self.cmd_deletebatch, bg="lightcoral", **btn_opts).grid(row=0, column=3, padx=3, pady=2)
        tk.Button(self.ops_frame, text="🔍 Search", command=self.cmd_search, bg="gold", **btn_opts).grid(row=0, column=4, padx=3, pady=2)
        # Row 1: Displays
        tk.Button(self.ops_frame, text="Show In-Order", command=lambda: self.cmd_show("in"), **btn_opts).grid(row=1, column=0, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Show Pre-Order", command=lambda: self.cmd_show("pre"), **btn_opts).grid(row=1, column=1, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Show Post-Order", command=lambda: self.cmd_show("post"), **btn_opts).grid(row=1, column=2, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Show Levels", command=lambda: self.cmd_show("levels"), **btn_opts).grid(row=1, column=3, padx=3, pady=2)

        # Row 2: Maintenance
        tk.Button(self.ops_frame, text="Check Balance", command=self.cmd_balance, **btn_opts).grid(row=2, column=0, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Clear Tree", command=self.cmd_clear, **btn_opts).grid(row=2, column=1, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Undo", command=self.cmd_undo, bg="orange", **btn_opts).grid(row=2, column=2, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Redo", command=self.cmd_redo, bg="orange", **btn_opts).grid(row=2, column=3, padx=3, pady=2)
        tk.Button(self.ops_frame, text="Σ Range Sum", command=self.cmd_suminrange, bg="orchid", **btn_opts).grid(row=2, column=4, padx=3, pady=2)
        # Row 3: Specialized Queries
        colors = ["orchid", "plum", "violet", "mediumorchid", "fuchsia"]
        tk.Button(self.ops_frame, text="📊 Range Values", command=self.cmd_valuesinrange, bg=colors[0], **btn_opts).grid(row=3, column=0, padx=3, pady=2)
        tk.Button(self.ops_frame, text="🔢 K-th Smallest", command=self.cmd_kthsmallest, bg=colors[1], **btn_opts).grid(row=3, column=1, padx=3, pady=2)
        tk.Button(self.ops_frame, text="⏭️ Successor", command=self.cmd_successor, bg=colors[2], **btn_opts).grid(row=3, column=2, padx=3, pady=2)
        tk.Button(self.ops_frame, text="📉 Get Min", command=self.cmd_min, bg=colors[3], **btn_opts).grid(row=3, column=3, padx=3, pady=2)
        tk.Button(self.ops_frame, text="📈 Get Max", command=self.cmd_max, bg=colors[4], **btn_opts).grid(row=3, column=4, padx=3, pady=2)

# Section C: Advanced
        self.adv_frame = tk.LabelFrame(self.control_frame, text="Advanced / Demo", padx=10, pady=10)
        self.adv_frame.grid(row=0, column=2, sticky="nsw", padx=10, pady=10)

        tk.Button(self.adv_frame, text="Merge A+B -> A", command=self.cmd_merge, width=15, bg="lightblue").pack(pady=2)
        tk.Button(self.adv_frame, text="Compare A == B", command=self.cmd_compare, width=15).pack(pady=2)
        tk.Button(self.adv_frame, text="Is Empty?", command=self.cmd_isempty, width=15).pack(pady=2)
        tk.Button(self.adv_frame, text="Show Logs/Console", command=lambda: self.log_win.show(), width=15).pack(pady=2)

        # Speed control
        speed_frame = tk.Frame(self.adv_frame)
        speed_frame.pack(pady=5)
        tk.Label(speed_frame, text="Speed:").pack(side=tk.LEFT)
        self.speed_scale = tk.Scale(speed_frame, from_=100, to=1000, orient=tk.HORIZONTAL,
                                    command=self.update_speed, length=100)
        self.speed_scale.set(500)
        self.speed_scale.pack(side=tk.LEFT)

        self.log_win = LogWindow(self.root)
        self.log_win.log("Welcome! 🎬 Animation-enabled ScapeGoat Tree")

        # Initial draw
        self.draw_tree()

    def update_speed(self, val):
        """Update animation speed"""
        self.animator.animation_speed = int(val)

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

    # ============================================
    # ANIMATED COMMAND HANDLERS
    # ============================================

    def cmd_insert(self):
        val = self.get_val()
        if val is None:
            return

        if self.animation_enabled.get() and not self.animator.is_animating:
            self.animator.animate_insertion(val, lambda: self.entry_val.delete(0, tk.END))
        else:
            self.get_active_tree().insert(val)
            self.log(f"Inserted {val} into Tree {self.selected_tree_var.get()}")
            self.entry_val.delete(0, tk.END)
            self.draw_tree()

    def cmd_insertbatch(self):
        raw_val = self.entry_val.get()
        try:
            batch_values = [int(x) for x in raw_val.split()]
            if not batch_values:
                self.log("Error: Batch input is empty")
                return

            if self.animation_enabled.get() and not self.animator.is_animating:
                self.animator.animate_batch_operation(batch_values, "insert")
                self.entry_val.delete(0, tk.END)
            else:
                self.get_active_tree().insert_batch(batch_values)
                self.log(f"Inserted batch {batch_values}")
                self.entry_val.delete(0, tk.END)
                self.draw_tree()
        except ValueError:
            self.log("Error: Batch must be numbers separated by spaces")

    def cmd_delete(self):
        val = self.get_val()
        if val is None:
            return

        if self.animation_enabled.get() and not self.animator.is_animating:
            self.animator.animate_deletion(val, lambda: self.entry_val.delete(0, tk.END))
        else:
            found = self.get_active_tree().delete_value(val)
            msg = "Deleted" if found else "Not Found"
            self.log(f"Delete {val}: {msg}")
            self.entry_val.delete(0, tk.END)
            self.draw_tree()

    def cmd_deletebatch(self):
        raw_val = self.entry_val.get()
        try:
            batch_values = [int(x) for x in raw_val.split()]
            if not batch_values:
                self.log("Error: Batch input is empty")
                return

            if self.animation_enabled.get() and not self.animator.is_animating:
                self.animator.animate_batch_operation(batch_values, "delete")
                self.entry_val.delete(0, tk.END)
            else:
                self.get_active_tree().delete_batch(batch_values)
                self.log(f"Deleted batch {batch_values}")
                self.entry_val.delete(0, tk.END)
                self.draw_tree()
        except ValueError:
            self.log("Error: Batch must be numbers separated by spaces")

    def cmd_search(self):
        val = self.get_val()
        if val is None:
            return

        if self.animation_enabled.get() and not self.animator.is_animating:
            self.animator.animate_search_path(val)
        else:
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
    def cmd_suminrange(self):
        # If the user typed "10 - 40" in the Range High box specifically:
        raw_input = self.entry_high.get().strip()

        # Fallback: if Range High is empty, check the Value box (entry_val)
        if not raw_input:
            raw_input = self.entry_val.get().strip()

        self.log(f"Processing range input: '{raw_input}'")

        try:
            if "-" in raw_input:

                parts = [p.strip() for p in raw_input.split("-") if p.strip()]

                if len(parts) == 2:
                    low = int(parts[0])
                    high = int(parts[1])
                else:
                    raise ValueError("Please use format '10 - 40'")
            else:
                # If no hyphen, assume Value box is LOW and High box is HIGH
                low = int(self.entry_val.get().strip())
                high = int(self.entry_high.get().strip())

            if low > high:
                low, high = high, low

            if self.animation_enabled.get() and not self.animator.is_animating:
                self.animator.animate_sum_in_range(low, high)
            else:
                result = self.get_active_tree().SuminRange(low, high)
                self.log(f"✓ Sum in [{low}, {high}]: {result}")

        except Exception as e:
            self.log(f"❌ Range Error: {e}")
            self.log("Usage: Type '10 - 40' in either box OR fill both boxes.")
    def cmd_valuesinrange(self):
        # If the user typed "10 - 40" in the Range High box specifically:
        raw_input = self.entry_high.get().strip()

        # Fallback: if Range High is empty, check the Value box (entry_val)
        if not raw_input:
            raw_input = self.entry_val.get().strip()

        self.log(f"Processing range input: '{raw_input}'")

        try:
            if "-" in raw_input:

                parts = [p.strip() for p in raw_input.split("-") if p.strip()]

                if len(parts) == 2:
                    low = int(parts[0])
                    high = int(parts[1])
                else:
                    raise ValueError("Please use format '10 - 40'")
            else:
                # If no hyphen, assume Value box is LOW and High box is HIGH
                low = int(self.entry_val.get().strip())
                high = int(self.entry_high.get().strip())

            if low > high:
                low, high = high, low

            if self.animation_enabled.get() and not self.animator.is_animating:
                self.animator.animate_values_in_range(low, high)
            else:
                result = self.get_active_tree().ValuesinRange(low, high)
                self.log(f"✓ Values in [{low}, {high}]: {result}")

        except Exception as e:
            self.log(f"❌ Range Error: {e}")
            self.log("Usage: Type '10 - 40' in either box OR fill both boxes.")

    def cmd_kthsmallest(self):
        k = self.get_val()
        if k is None: return
        try:
            res = self.get_active_tree().KthSmallest(k)
            self.log(f"Rank {k} element is: {res}")
            self.draw_tree(highlight_val=res, highlight_color="#da70d6")
        except Exception as e:
            self.log(f"❌ Error: {e}")

    def cmd_successor(self):
        val = self.get_val()
        if val is None: return
        try:
            res = self.get_active_tree().GetSuccessor(val)
            self.log(f"Successor of {val} is: {res}")
            self.draw_tree(highlight_val=res, highlight_color="#da70d6")
        except Exception as e:
            self.log(f"❌ Error: {e}")

    def cmd_min(self):
        try:
            res = self.get_active_tree().GetMin()
            self.log(f"Minimum value: {res}")
            self.draw_tree(highlight_val=res, highlight_color="#da70d6")
        except Exception as e:
            self.log(f"❌ Error: {e}")

    def cmd_max(self):
        try:
            res = self.get_active_tree().GetMax()
            self.log(f"Maximum value: {res}")
            self.draw_tree(highlight_val=res, highlight_color="#da70d6")
        except Exception as e:
            self.log(f"❌ Error: {e}")
    # ============================================
        # DRAWING ENGINE
    # ============================================

    def draw_tree(self, highlight_val=None, highlight_color="#ff69b4"):
        self.canvas.delete("all")
        tree = self.get_active_tree()
        root = tree.get_root()

        if not root:
            self.canvas.create_text(600, 300,
                                    text=f"Tree {self.selected_tree_var.get()} is Empty",
                                    font=("Arial", 20), fill="gray")
            return

        h = self.get_height_py(root)
        canvas_w = self.canvas.winfo_width() or 1200
        self.draw_node_recursive(root, canvas_w // 2, 50, canvas_w // 4, highlight_val, highlight_color)

    def draw_node_recursive(self, node, x, y, dx, highlight, highlight_color):
        if not node:
            return

        # Draw edges first (so they appear behind nodes)
        if node.left:
            self.canvas.create_line(x, y, x - dx, y + 60, width=2, fill="#666")
            self.draw_node_recursive(node.left, x - dx, y + 60, dx / 2, highlight, highlight_color)
        if node.right:
            self.canvas.create_line(x, y, x + dx, y + 60, width=2, fill="#666")
            self.draw_node_recursive(node.right, x + dx, y + 60, dx / 2, highlight, highlight_color)

        # Draw node
        r = 20
        color = "#add8e6"  # Default sky blue

        if highlight is not None and node.value == highlight:
            color = highlight_color
            r = 24  # Make highlighted node bigger

        self.canvas.create_oval(x-r, y-r, x+r, y+r, fill=color, outline="black", width=2)
        self.canvas.create_text(x, y, text=str(node.value), font=("Arial", 11, "bold"))

    def get_height_py(self, node):
        if not node:
            return -1
        return 1 + max(self.get_height_py(node.left), self.get_height_py(node.right))

if __name__ == "__main__":
    root = tk.Tk()
    app = ScapeGoatGUI(root)
    root.mainloop()