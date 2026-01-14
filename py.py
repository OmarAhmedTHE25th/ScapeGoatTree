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
    def animate_split(self, value, callback=None):
        """Animate the splitting of a tree at a value"""
        if self.is_animating:
            return

        self.is_animating = True
        # Highlight the 'pivot' node that will be sacrificed for the split
        self.gui.draw_tree(highlight_val=value, highlight_color="#ee82ee")
        self.gui.log(f"Splitting tree at value {value}...")

        def perform_split():
            # This callback will handle the actual C++ call and GUI refresh
            if callback:
                callback()
            self.is_animating = False

        # Short delay so the user sees the 'pivot' before it splits
        self.gui.root.after(1000, perform_split)
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
class CollapsibleFrame(tk.Frame):
    """A Frame that can be expanded/collapsed and updates the parent scrollbar"""
    def __init__(self, parent, title="", expanded=True):
        super().__init__(parent, bg="#e1e1e1") # darker bg for header distinction
        self.expanded = expanded
        self.title = title
        self.parent_canvas = None # Will store reference to sidebar canvas

        # The Clickable Header
        self.header_frame = tk.Frame(self, bg="#d0d0d0", relief=tk.RAISED, bd=1)
        self.header_frame.pack(fill=tk.X)

        # Toggle Button (Arrow)
        self.toggle_btn = tk.Label(self.header_frame, text="▼", width=3, bg="#d0d0d0", cursor="hand2")
        self.toggle_btn.pack(side=tk.LEFT)
        self.toggle_btn.bind("<Button-1>", lambda e: self.toggle())

        # Title Label
        self.lbl_title = tk.Label(self.header_frame, text=title, bg="#d0d0d0", font=("Arial", 10, "bold"), cursor="hand2")
        self.lbl_title.pack(side=tk.LEFT, fill=tk.X, expand=True)
        self.lbl_title.bind("<Button-1>", lambda e: self.toggle())

        # The Content Container
        self.content_frame = tk.Frame(self, padx=5, pady=5, bg="#f0f0f0")

        if expanded:
            self.content_frame.pack(fill=tk.X, expand=True)
            self.toggle_btn.config(text="▼")
        else:
            self.toggle_btn.config(text="▶")

    def toggle(self):
        if self.expanded:
            self.content_frame.pack_forget()
            self.toggle_btn.config(text="▶")
        else:
            self.content_frame.pack(fill=tk.X, expand=True)
            self.toggle_btn.config(text="▼")

        self.expanded = not self.expanded

        # CRITICAL: Force the sidebar scrollbar to update size
        if self.parent_canvas:
            self.parent_canvas.update_idletasks()
            self.parent_canvas.configure(scrollregion=self.parent_canvas.bbox("all"))
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
        self.root.geometry("1400x900")
        self.root.state('zoomed')

        # --- Data & Logic ---
        self.treeA = scapegoat_tree_py.ScapeGoatTree()
        self.treeB = scapegoat_tree_py.ScapeGoatTree()
        self.selected_tree_var = tk.StringVar(value="A")
        self.animator = AnimationManager(self)
        self.animation_enabled = tk.BooleanVar(value=True)

        # Initialize LogWindow FIRST so it's ready for any startup messages
        self.log_win = LogWindow(self.root)

        # --- MAIN CONTAINER (Holds Sidebar + Canvas) ---
        self.main_container = tk.Frame(root, bg="#f0f0f0")
        self.main_container.pack(fill=tk.BOTH, expand=True)

        # === 1. RIGHT SIDEBAR (The Control Panel) ===
        self.sidebar_visible = True
        self.sidebar_frame = tk.Frame(self.main_container, bg="#e0e0e0", width=340) # Slightly wider for all buttons
        self.sidebar_frame.pack(side=tk.RIGHT, fill=tk.Y)
        self.sidebar_frame.pack_propagate(False)

        # Toggle Button for Sidebar (Vertical strip)
        self.toggle_sidebar_btn = tk.Button(self.main_container, text="▶", command=self.toggle_sidebar, 
                                            bg="#d0d0d0", relief=tk.FLAT, width=2)
        self.toggle_sidebar_btn.pack(side=tk.RIGHT, fill=tk.Y)

        # Scrollbar for Sidebar
        sb_scroll = tk.Scrollbar(self.sidebar_frame, orient="vertical")
        sb_scroll.pack(side=tk.RIGHT, fill=tk.Y)

        # Canvas for Sidebar (Enables scrolling)
        self.sidebar_canvas = tk.Canvas(self.sidebar_frame, bg="#f0f0f0", yscrollcommand=sb_scroll.set)
        self.sidebar_canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        sb_scroll.config(command=self.sidebar_canvas.yview)

        # The Actual Frame inside the Canvas
        self.control_frame = tk.Frame(self.sidebar_canvas, bg="#f0f0f0")
        canvas_window = self.sidebar_canvas.create_window((0, 0), window=self.control_frame, anchor="nw")

        # Dynamic Scroll Region Update
        def update_scroll_region(event):
            self.sidebar_canvas.configure(scrollregion=self.sidebar_canvas.bbox("all"))
            self.sidebar_canvas.itemconfig(canvas_window, width=event.width)

        self.control_frame.bind("<Configure>", update_scroll_region)
        self.sidebar_canvas.bind("<Configure>", lambda e: self.sidebar_canvas.itemconfig(canvas_window, width=e.width))

        # === 2. LEFT TREE VIEW (The Main Event) ===
        self.canvas = tk.Canvas(self.main_container, bg="white")
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # ==========================================
        # CONTROL SECTIONS (Restored Features!)
        # ==========================================

        # --- SECTION A: INPUTS ---
        self.pane_input = CollapsibleFrame(self.control_frame, "Inputs & Selection", expanded=True)
        self.pane_input.parent_canvas = self.sidebar_canvas
        self.pane_input.pack(fill=tk.X, pady=2)
        f_in = self.pane_input.content_frame

        tk.Label(f_in, text="Value:").grid(row=0, column=0, sticky="w")
        self.entry_val = tk.Entry(f_in, width=15, font=("Arial", 12))
        self.entry_val.grid(row=0, column=1, padx=5, pady=2)

        tk.Label(f_in, text="Range :").grid(row=1, column=0, sticky="w")
        self.entry_high = tk.Entry(f_in, width=15, font=("Arial", 12))
        self.entry_high.grid(row=1, column=1, padx=5, pady=2)

        tk.Label(f_in, text="Active Tree:").grid(row=2, column=0, sticky="w", pady=5)
        tree_frame = tk.Frame(f_in)
        tree_frame.grid(row=2, column=1, sticky="w")
        tk.Radiobutton(tree_frame, text="Tree A", variable=self.selected_tree_var, value="A", command=self.refresh_ui).pack(side=tk.LEFT)
        tk.Radiobutton(tree_frame, text="Tree B", variable=self.selected_tree_var, value="B", command=self.refresh_ui).pack(side=tk.LEFT)

        tk.Checkbutton(f_in, text="🎬 Animate", variable=self.animation_enabled, font=("Arial", 10, "bold")).grid(row=3, column=0, columnspan=2, pady=5)

        # --- SECTION B: OPERATIONS ---
        self.pane_ops = CollapsibleFrame(self.control_frame, "Operations", expanded=True)
        self.pane_ops.parent_canvas = self.sidebar_canvas
        self.pane_ops.pack(fill=tk.X, pady=2)
        f_ops = self.pane_ops.content_frame

        btn_opts = {'width': 13, 'pady': 2}
        
        # Basic ops
        tk.Button(f_ops, text="Insert", command=self.cmd_insert, bg="lightgreen", **btn_opts).grid(row=0, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Insert Batch", command=self.cmd_insertbatch, bg="lightgreen", **btn_opts).grid(row=0, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="Delete", command=self.cmd_delete, bg="lightcoral", **btn_opts).grid(row=1, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Delete Batch", command=self.cmd_deletebatch, bg="lightcoral", **btn_opts).grid(row=1, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="🔍 Search", command=self.cmd_search, bg="gold", **btn_opts).grid(row=2, column=0, padx=3, pady=2)
        
        # Displays
        tk.Button(f_ops, text="Show In-Order", command=lambda: self.cmd_show("in"), **btn_opts).grid(row=3, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Show Pre-Order", command=lambda: self.cmd_show("pre"), **btn_opts).grid(row=3, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="Show Post-Order", command=lambda: self.cmd_show("post"), **btn_opts).grid(row=4, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Show Levels", command=lambda: self.cmd_show("levels"), **btn_opts).grid(row=4, column=1, padx=3, pady=2)

        # Maintenance
        tk.Button(f_ops, text="Check Balance", command=self.cmd_balance, **btn_opts).grid(row=5, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Clear Tree", command=self.cmd_clear, **btn_opts).grid(row=5, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="Undo", command=self.cmd_undo, bg="orange", **btn_opts).grid(row=6, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="Redo", command=self.cmd_redo, bg="orange", **btn_opts).grid(row=6, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="Σ Range Sum", command=self.cmd_suminrange, bg="orchid", **btn_opts).grid(row=7, column=0, padx=3, pady=2)

        # Specialized Queries
        colors = ["orchid", "plum", "violet", "mediumorchid", "fuchsia"]
        tk.Button(f_ops, text="📊 Range Values", command=self.cmd_valuesinrange, bg=colors[0], **btn_opts).grid(row=7, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="🔢 K-th Smallest", command=self.cmd_kthsmallest, bg=colors[1], **btn_opts).grid(row=8, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="⏭️ Successor", command=self.cmd_successor, bg=colors[2], **btn_opts).grid(row=8, column=1, padx=3, pady=2)
        tk.Button(f_ops, text="📉 Get Min", command=self.cmd_min, bg=colors[3], **btn_opts).grid(row=9, column=0, padx=3, pady=2)
        tk.Button(f_ops, text="📈 Get Max", command=self.cmd_max, bg=colors[4], **btn_opts).grid(row=9, column=1, padx=3, pady=2)

        # --- SECTION C: ADVANCED ---
        self.pane_adv = CollapsibleFrame(self.control_frame, "Advanced / Demo", expanded=False)
        self.pane_adv.parent_canvas = self.sidebar_canvas
        self.pane_adv.pack(fill=tk.X, pady=2)
        f_adv = self.pane_adv.content_frame

        tk.Button(f_adv, text="Merge A+B -> A", command=self.cmd_merge, width=30, bg="lightblue").pack(pady=2)
        tk.Button(f_adv, text="✂️ Split Active", command=self.cmd_split, width=30, bg="plum").pack(pady=2)
        tk.Button(f_adv, text="Compare A == B", command=self.cmd_compare, width=30).pack(pady=2)
        tk.Button(f_adv, text="Is Empty?", command=self.cmd_isempty, width=30).pack(pady=2)
        tk.Button(f_adv, text="Show Logs/Console", command=lambda: self.log_win.show(), width=30).pack(pady=2)

        # Speed control
        speed_frame = tk.Frame(f_adv)
        speed_frame.pack(pady=5)
        tk.Label(speed_frame, text="Speed:").pack(side=tk.LEFT)
        self.speed_scale = tk.Scale(speed_frame, from_=100, to=1000, orient=tk.HORIZONTAL, command=self.update_speed, length=100)
        self.speed_scale.set(500)
        self.speed_scale.pack(side=tk.LEFT)

        # --- SECTION D: ALPHA CONTROL ---
        self.pane_alpha = CollapsibleFrame(self.control_frame, "Alpha Control", expanded=False)
        self.pane_alpha.parent_canvas = self.sidebar_canvas
        self.pane_alpha.pack(fill=tk.X, pady=2)
        f_alpha = self.pane_alpha.content_frame

        tk.Label(f_alpha, text="Alpha (α) Strictness:").pack()
        self.alpha_scale = tk.Scale(f_alpha, from_=0.5, to=0.9, resolution=0.01, orient=tk.HORIZONTAL, command=self.update_alpha)
        self.alpha_scale.set(0.66) # Default 2/3
        self.alpha_scale.pack()

        self.draw_tree()

    def toggle_sidebar(self):
        """Toggle the visibility of the right sidebar"""
        if self.sidebar_visible:
            self.sidebar_frame.pack_forget()
            self.toggle_sidebar_btn.config(text="◀") # Arrow pointing left to reopen
        else:
            # Re-pack the sidebar. Note: we need to ensure the order is maintained.
            # Tree canvas is packed LEFT, so sidebar should be packed RIGHT.
            # The toggle button is packed RIGHT.
            self.canvas.pack_forget()
            self.toggle_sidebar_btn.pack_forget()
            
            self.sidebar_frame.pack(side=tk.RIGHT, fill=tk.Y)
            self.toggle_sidebar_btn.pack(side=tk.RIGHT, fill=tk.Y)
            self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            self.toggle_sidebar_btn.config(text="▶") # Arrow pointing right to close
            
        self.sidebar_visible = not self.sidebar_visible
        
        # Force a redraw so the tree centers correctly in the new space
        self.root.update_idletasks()
        self.draw_tree()

    def update_speed(self, val):
        """Update animation speed"""
        self.animator.animation_speed = 1100 - int(val)

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
    def cmd_split(self):
        val = self.get_val()
        if val is None:
            self.log("❌ Error: Enter a value to split at.")
            return

        def handle_split_logic():
            try:
                # Assuming your C++ binding returns a tuple (leftTree, rightTree)
                # and you are splitting the currently active tree
                active_tree = self.get_active_tree()
                tree_left, tree_right = active_tree.Split(val)

                # Update our Python references
                self.treeA = tree_left
                self.treeB = tree_right

                self.log(f"✂️ Split complete! Tree A = elements < {val}, Tree B = elements > {val}")
                self.draw_tree()
            except Exception as e:
                self.log(f"❌ Split Error: {e}")

        if self.animation_enabled.get():
            self.animator.animate_split(val, handle_split_logic)
        else:
            handle_split_logic()
    def update_alpha(self, val):
            self.get_active_tree().SetAlpha(float(val))
            self.log(f"Alpha updated to {val}. The goat is now {'stricter' if float(val) < 0.7 else 'lazier'}! 🐐")
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