#!/usr/bin/python

import sys
import os
import glob
import re


DriverPath = ''
InsertPath = '/../../../'
if (len(sys.argv) == 2):
    DriverPath = sys.argv[1] + '/'
    sys.path.insert(0, os.path.abspath(os.getcwd()))
    import apply_relpath
    IncludePath = apply_relpath.get_topsrcdir_asrelativepathto_objdirsfnxsource()[1]


def pts(category, pyfile):
    print 'Auto-documenting %s file %s' % (category, pyfile)


def extract_xyz(pyfile):
    text1line = ''
    textMline = ''
    b2a = 0.52917720859
    efpAtomSymbol = re.compile(r"^\s*A\d*([A-Z]{1,2})\d*", re.IGNORECASE)

    ffrag = open(pyfile, 'r')
    contents = ffrag.readlines()
    ffrag.close()

    atoms = []
    ii = 0
    while (ii < len(contents)):
        line = contents[ii]

        if 'MONOPOLES' in line:
            break

        if efpAtomSymbol.search(line):
            sline = line.split()
            atoms.append([efpAtomSymbol.search(line).group(1), 
                float(sline[1]) * b2a, float(sline[2]) * b2a, float(sline[3]) * b2a])

        ii += 1

    text1line += str(len(atoms)) + r"""\ncomment\n"""
    for atom in atoms:
        text1line += r"""%-6s %12.6f %12.6f %12.6f\n""" % (atom[0], atom[1], atom[2], atom[3])
        textMline += """   %-6s %12.6f %12.6f %12.6f\n""" % (atom[0], atom[1], atom[2], atom[3])

    return text1line, textMline


chemdoodle = r"""
.. raw:: html

    <meta http-equiv="X-UA-Compatible" content="chrome=1">
    <link rel="stylesheet" href="http://hub.chemdoodle.com/cwc/latest/ChemDoodleWeb.css" type="text/css">
    <script type="text/javascript" src="http://hub.chemdoodle.com/cwc/latest/ChemDoodleWeb-libs.js"></script>
    <script type="text/javascript" src="http://hub.chemdoodle.com/cwc/latest/ChemDoodleWeb.js"></script>
    
    <script>
      ChemDoodle.default_atoms_useJMOLColors = true;
      ChemDoodle.default_atoms_circles_2D = true;
      ChemDoodle.default_atoms_circleDiameter_2D = 0.7;
      ChemDoodle.default_atoms_circleBorderWidth_2D = 0.05;
      ChemDoodle.default_bonds_width_2D = 0.10;
      ChemDoodle.default_shapes_lineWidth_2D = 0.1;
      ChemDoodle.default_shapes_arrowLength_2D = 0.07;
    </script>
"""


def canvas(fragname, molxyz):
    text = r"""
.. raw:: html

    <center>
    <script>
    
      // Molecule from XYZ file
      var mol = ChemDoodle.readXYZ('%s')

      // the Canvas
      var qq = new ChemDoodle.TransformCanvas('%s_l', 400, 300, true);
      qq.loadContent([mol]);
      qq.specs.scale = 25;
      qq.repaint();
    
    </script>

    <center>
    <div style="font-size:12px;">
    <strong>rotate</strong>: click+drag<br>
    <strong>translate</strong>: alt+click+drag<br>
    <strong>zoom</strong>: scroll
    </div>
    Visualization by <a href="http://web.chemdoodle.com" target="_blank">ChemDoodle Web</a>
    </center>

""" % (molxyz, fragname)

    return text


# Available fragments in psi4/libefp/libefp/fraglib
fdriver = open('source/autodoc_available_efpfragments.rst', 'w')
fdriver.write('\n\n')
fdriver.write(chemdoodle)
fdriver.write('\n\n')

for pyfile in glob.glob(DriverPath + '../../libefp/libefp/fraglib/*.efp'):
    filename = os.path.split(pyfile)[1]
    basename = os.path.splitext(filename)[0]
    div = '=' * len(basename)

    if basename not in []:

        pts('efp fragment', basename)
    
        fdriver.write(':srcefpfrag:`%s`\n%s\n\n' % (basename, '"' * (14 + len(basename))))
        molstr, molMstr = extract_xyz(pyfile)
        fdriver.write(canvas(basename, molstr))
        fdriver.write('\n\nFull Geometry in Angstroms ::\n\n%s\n\n' % (molMstr))
        fdriver.write('----\n')

    fdriver.write('\n')
fdriver.close()


      #// create a synthetic arrow
      #//var origin = mol.getCenter()
      #//var ptX = new ChemDoodle.structures.Point(0.0, 0.1);
      #//var axisX = new ChemDoodle.structures.d2.Line(origin, ptX);
      #//axisX.arrowType = ChemDoodle.structures.d2.Line.ARROW_SYNTHETIC;
