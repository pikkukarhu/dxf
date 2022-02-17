# dxf
Dxf file handling

16.2. 2022 TODO refactoring before continue:

Change Document to File (Because current "Document" makes low level dxf-file handling and change Entities to new Document. 
It represents the Document abstraction of drawing.

Move Group out from  class File (previous Document), because ENtities need vector of Group as parameter, no string!