public class CTreeViewManager
{

    public void AddHierarchyToTreeView(TreeView treeView, string hierarchyMessage)
    {
        // Always start from the 'World' node.
        TreeNode rootNode = treeView.Nodes.Find("World", false).FirstOrDefault();

        // If 'World' node does not exist, create it.
        if (rootNode == null)
        {
            rootNode = new TreeNode("World") { Name = "World" };
            treeView.Nodes.Add(rootNode);
        }

        // Split the message by '/' and start from the 'World' node.
        string[] parts = hierarchyMessage.Split('/');
        TreeNode lastNode = rootNode;

        foreach (string part in parts)
        {
            if (string.IsNullOrEmpty(part)) continue; // Skip empty strings.

            // Find or create the child node.
            TreeNode[] childNodes = lastNode.Nodes.Find(part, false);
            if (childNodes.Length == 0)
            {
                // Child node does not exist, so create and add it.
                TreeNode newNode = new TreeNode(part) { Name = part };
                var found = treeView.Nodes.Find(part, true);
                if (found != null)
                {
                    for(int i = 0; i < found.Length; i++)
                    {
                        treeView.Nodes.Remove(found[i]);
                    }
                    
                }

                lastNode.Nodes.Add(newNode);
                lastNode = newNode; // Move to the new node for the next iteration.
            }
            else
            {
                // Child node exists, move to it for the next iteration.
                lastNode = childNodes[0];
            }
        }

        //treeView.CollapseAll(); // Expand all nodes to show the full hierarchy.
    }

    public void SetUpTreeViewForCustomDraw(TreeView treeView)
    {
        // Set the TreeView's DrawMode property to invoke custom drawing
        treeView.DrawMode = TreeViewDrawMode.OwnerDrawText;
        treeView.DrawNode += new DrawTreeNodeEventHandler(TreeView_DrawNode);
    }

    private void TreeView_DrawNode(object sender, DrawTreeNodeEventArgs e)
    {
        TreeView treeView = (TreeView)sender;
        if (treeView == null) return;

        // Determine the background color
        Color backColor = e.Node.IsSelected ? SystemColors.Highlight : e.Node.BackColor;

        // Foreground color does not change if the treeview is focused or not
        Color foreColor = e.Node.IsSelected ? SystemColors.HighlightText : e.Node.ForeColor;

        // Fill the background
        using (SolidBrush brush = new SolidBrush(backColor))
        {
            e.Graphics.FillRectangle(brush, e.Bounds);
        }

        // Draw the text
        TextRenderer.DrawText(e.Graphics,
                              e.Node.Text,
                              treeView.Font,
                              e.Bounds,
                              foreColor,
                              backColor,
                              TextFormatFlags.GlyphOverhangPadding);

        // If the node has focus, draw the focus rectangle (optional)
        if (e.Node.IsSelected)
        {
            ControlPaint.DrawFocusRectangle(e.Graphics, e.Bounds, foreColor, backColor);
        }
    }
}