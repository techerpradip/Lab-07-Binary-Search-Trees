#!/usr/bin/python

# This program takes as its sole argument the name of a text file containing
# visual representations of trees.  It produces a TeX file containing code
# which will generate diagrams of those trees.
#
# The input file is expected to be a text document.  Each line of the text
# document is one of the following:
#   * A line starting with spaces and a label marker ':'.  These lines are
#     translated into textual labels.  These markers separate trees from each
#     other.
#   * A line containing exclusively alphanumeric symbols and spaces.  These
#     lines are treated as tree levels, with each alphanumeric sequence
#     representing a single tree node.
#   * A line starting with spaces and a comment marker '#'.  These lines are
#     ignored, except that they interrupt sequences of the above lines.
#   * A line containing exclusively non-alphanumeric characters, such as '/' and
#     '\'.  These lines are ignored.
# Each sequence of level lines (separated by label lines) is grouped together
# and transformed into a tree.  Spacing in these lines is relevant.  The rules
# for generating the tree are as follows:
#
#   * Nodes on adjacent levels may not occupy the same columns.
#   * Nodes on adjacent levels have a horizontal distance metric equal to the
#     number of columns between their closest two characters.
#   * The left child of a parent node is the node on the next level to the left
#     of the parent; the right child of a parent node is the node on the next
#     level to the right of the parent.  It is an error for two parent nodes to
#     have equal distance to the same child.
#   * Every node on a positive (non-root) level must have a parent.
#
# The resulting TeX file is relatively self-contained but requires TikZ.

import sys

class InvariantError(Exception): pass

class TextPosition(object):
    def __init__(self, lineno, colno):
        self.lineno = lineno
        self.colno = colno
    def __repr__(self):
        return "TextPosition(lineno={},colno={})".format(self.lineno,self.colno)
    def __str__(self):
        return "line {}, column {}".format(self.lineno,self.colno)
    def __add__(self, n):
        return TextPosition(self.lineno, self.colno + n)
    def __radd__(self, n):
        return TextPosition(self.lineno, self.colno + n)
    def newline(self):
        return TextPosition(self.lineno+1, 1)

class Parseable(object):
    def __init__(self, textpos):
        self.textpos = textpos

class Node(Parseable):
    def __init__(self, textpos, text, left=None, right=None):
        super(Node, self).__init__(textpos)
        self.text = text
        self.left = left
        self.right = right
    def distance(self, other):
        """
        Measures the horizontal distance between two nodes.  If this node is to
        the left of the other node, returns a negative number; if this node is
        to the right, returns a positive number.  Returns None if the nodes
        overlap in any way.
        """
        if self.textpos.colno > other.textpos.colno:
            d = other.distance(self)
            return None if d is None else -d
        # We know we're to the left of the other
        self_right = self.textpos.colno+len(self.text)-1
        other_left = other.textpos.colno
        if self_right >= other_left:
            return None
        else:
            return self_right - other_left
    def write_tikz(self, file, level, pos_index_in_level):
        # The number of slots on a given level L is 2^L.  We're going to make
        # sure that every node has at least the padding amount between it and
        # other nodes by calculating the coordinates within the LaTeX document.
        # For each node in the tree, we want that node's center at the following
        # position, using TDW, MNS, and VS for the variables defined by Tree, L
        # for the level of the node, and I for the index of the position of this
        # node within its level:
        #   ( (TDW-(MNS*2^L))/(2^L+1)*(I+1) + MNS/2 + MNS*I, -L*VS )
        file.write("\\node[treeNode] (n-{level}-{index}) at "
                   "( {{({tdw}-({mns}*{levelExp2}))/({levelExp2})*({index}+0.5)"
                   " + {mns}/2 + {mns}*{index}}}"
                   ", {{-{level}*{vs}}}) {{{text}}};\n".format(
                       level=level, index=pos_index_in_level,
                       tdw="\\treeDiagramWidth", mns="\\minimumNodeSize",
                       vs="\\verticalSpacing", levelExp2=2**level,
                       text=self.text))
        if self.left is not None:
            self.left.write_tikz(file, level+1, pos_index_in_level*2)
            file.write("\\draw[treeEdge] (n-{level}-{index}) -- "
                       "(n-{clevel}-{cindex});\n".format(
                           level=level, index=pos_index_in_level,
                           clevel=level+1, cindex=pos_index_in_level*2
                       ))
        if self.right is not None:
            self.right.write_tikz(file, level+1, pos_index_in_level*2+1)
            file.write("\\draw[treeEdge] (n-{level}-{index}) -- "
                       "(n-{clevel}-{cindex});\n".format(
                           level=level, index=pos_index_in_level,
                           clevel=level+1, cindex=pos_index_in_level*2+1
                       ))

class Block(Parseable):
    def __init__(self, textpos):
        super(Block, self).__init__(textpos)

class NopBlock(Block):
    def __init__(self, textpos):
        super(NopBlock, self).__init__(textpos)
    def write_tex(self, file):
        pass

class Label(Block):
    def __init__(self, textpos, text):
        super(Label, self).__init__(textpos)
        self.text = text
    def write_tex(self, file):
        file.write("\n\n")
        file.write(self.text)
        file.write("\n\n")

class LevelGroup(Block):
    def __init__(self, textpos, levels):
        super(LevelGroup, self).__init__(textpos)
        self.levels = levels

class Tree(Block):
    def __init__(self, textpos, root):
        super(Tree, self).__init__(textpos)
        self.root = root
    def write_tex(self, file):
        # Open a group to isolate the diagram.
        file.write("\\par\\begingroup\n")
        # Provide constant sizes so the diagram can be adjusted.
        file.write("\\newcommand{\\treeDiagramWidth}{0.9\\textwidth}\n")
        file.write("\\newcommand{\\minimumNodeSize}{8mm}\n")
        file.write("\\newcommand{\\verticalSpacing}{10mm}\n")
        # Let's put this tree in a picture and then let the node draw it.
        file.write("\\begin{center}\\begin{tikzpicture}\n")
        file.write("\\tikzstyle{treeNode}=[minimum width=\\minimumNodeSize,"
                   "minimum height=\\minimumNodeSize,circle,draw,"
                   "inner sep=1mm]\n")
        file.write("\\tikzstyle{treeEdge}=[->]\n")
        file.write("\\node[minimum width=\\treeDiagramWidth,rectangle] "
                   "at (\\treeDiagramWidth/2,0) {};")
        self.root.write_tikz(file, level=0, pos_index_in_level=0)
        file.write("\\end{tikzpicture}\\end{center}\n")
        # Pack it up!
        file.write("\\endgroup\\par")

class ParserError(Exception): pass

def parse_file(filename):
    """
    Parses a file into a list of blocks.  Then, converts level groups into
    trees.
    """
    with open(filename) as f:
        lines = f.readlines()
    blocks = []
    textpos = TextPosition(1,1)
    for line in lines:
        textpos += len(line) - len(line.lstrip())
        line = line.strip()
        if len(line) == 0:
            # Blank line.  Ignore it.
            pass
        elif line[0] == '#':
            # Comment line.  Ignore it.
            blocks.append(NopBlock(textpos))
        elif line[0] == ':':
            # Label line.  Retrieve and aggregate.
            if len(blocks) > 0 and isinstance(blocks[-1], Label):
                blocks[-1].text += line[1:] + '\n'
            else:
                blocks.append(Label(textpos, line[1:] + '\n'))
        elif all(map(lambda x: not x.isalnum(), line)):
            # Symbol line.  Ignore it.
            pass
        elif all(map(lambda x: x.isalnum() or x == ' ', line)):
            # Level line.  Parse and store.
            level = []
            startpos = textpos
            while len(line) > 0:
                # Consume any space we find.
                textpos += len(line) - len(line.lstrip())
                line = line.strip()
                # The next sequence of non-space characters is a node.
                idx = line.find(' ')
                if idx == -1:
                    # Then the node is the rest of the line.
                    nodename = line
                    line = ''
                else:
                    # Then we know where the node ends
                    nodename = line[:idx]
                    line = line[idx:]
                level.append(Node(textpos, nodename))
                textpos += len(nodename)
            # Now we have all the levels.
            if len(blocks) > 0 and isinstance(blocks[-1], LevelGroup):
                blocks[-1].levels.append(level)
            else:
                blocks.append(LevelGroup(startpos, [level]))
        else:
            raise ParserError("Line {} unrecognized: mix of symbols and "
                              "non-symbols?".format(textpos.lineno))
        textpos = textpos.newline()

    translated = []
    for block in blocks:
        if isinstance(block, LevelGroup):
            levels = block.levels
            if len(levels) == 0:
                raise InvariantError("Empty level group at {}".format(
                    block.textpos))
            if len(levels[0]) == 0:
                raise InvariantError(
                    "First level empty in level group at {}".format(
                        block.textpos))
            if len(levels[0]) > 1:
                raise ParserError(
                    "Top level has more than one node at {}".format(
                        block.textpos))
            root = levels[0][0]
            parent_level = 0
            # For each level, link its nodes to the level above it.
            for parent_level in range(0, len(levels)-1):
                if len(levels[parent_level+1]) == 0:
                    raise InvariantError("Level {} contains no nodes!".format(
                        parent_level+1))
                for child in levels[parent_level+1]:
                    favorite_parent = None
                    favorite_parent_distance = None
                    on_left_of_favorite_parent = None
                    for parent in levels[parent_level]:
                        parent_distance = parent.distance(child)
                        if parent_distance is None:
                            raise ParserError(
                                "Parent node (at {}) and child node (at {}) "
                                "overlap.".format(
                                    parent.textpos, child.textpos))
                        child_on_left = (parent_distance > 0)
                        parent_distance = abs(parent_distance)
                        if parent_distance == favorite_parent_distance:
                            raise ParserError(
                                "Child node (at {}) has equal distance from "
                                "parents (at {} and {})".format(
                                    child.textpos, favorite_parent.textpos,
                                    parent.textpos))
                        if favorite_parent_distance is None or \
                           favorite_parent_distance > parent_distance:
                            favorite_parent = parent
                            favorite_parent_distance = parent_distance
                            on_left_of_favorite_parent = child_on_left
                    if favorite_parent is None:
                        raise InvariantError("Child (at {}) somehow did not "
                                             "choose a parent.".format(
                                                 child.textpos))
                    if on_left_of_favorite_parent:
                        if favorite_parent.left is not None:
                            raise ParserError(
                                "Parent (at {}) has two children on its left: "
                                "one at {} and one at {}.".format(
                                    favorite_parent.textpos,
                                    favorite_parent.left.textpos,
                                    child.textpos))
                        else:
                            favorite_parent.left = child
                    else:
                        if favorite_parent.right is not None:
                            raise ParserError(
                                "Parent at {} has two children on its "
                                "right: one at {} and one at {}.".format(
                                    favorite_parent.textpos, child.textpos,
                                    favorite_parent.right.textpos))
                        else:
                            favorite_parent.right = child
            translated.append(Tree(block.textpos, root))
        else:
            translated.append(block)

    return translated

def write_blocks(filename, blocks):
    with open(filename, 'w') as f:
        for block in blocks:
            block.write_tex(f)
        f.write('\n')

def main():
    errors = 0
    if len(sys.argv) < 2:
        print "Expected arguments to be the names of files to convert."
        sys.exit(1)
    for filename in sys.argv[1:]:
        try:
            blocks = parse_file(filename)
        except IOError as e:
            print "Could not parse {}: {}".format(filename, str(e))
            errors += 1
            continue
        except ParserError as e:
            print "Could not parse {}: {}".format(filename, str(e))
            errors += 1
            continue
        if filename.rfind('.') == -1:
            newname = filename + '.tex'
        else:
            newname = filename[:filename.rfind('.')] + '.tex'
        try:
            write_blocks(newname, blocks)
        except IOError as e:
            print "Could not write {}: {}".format(newname, str(e))
            errors += 1
            continue
    if errors > 0:
        print "Errors occurred while processing trees."
        sys.exit(2)

main()
