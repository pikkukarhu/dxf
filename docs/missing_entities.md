# DXF Implementation Roadmap: Missing Entities

This document tracks the entities currently missing from the DXF parser and provide technical notes for their implementation.

## Currently Implemented
- `POINT`
- `LINE`
- `3DFACE`
- `CIRCLE`
- `ARC`
- `ELLIPSE`
- `LWPOLYLINE`
- `POLYLINE` (Old-style with `VERTEX` sequence)
- `TEXT`
- `MTEXT`
- `INSERT`
- `ATTDEF`
- `ATTRIB`

---

## Missing Entities

### 1. High Priority (Common in 2D Drawings)
- **`HATCH`**: Used for patterned or solid area fills. Complex parsing involving boundary paths.
- **`DIMENSION`**: All measurement types (Linear, Aligned, Radial, etc.). Often contains an internal anonymous block for rendering.
- **`SPLINE`**: Smooth mathematical curves (NURBS). Requires Knot vectors and Control points.
- **`SOLID`**: 2D filled triangles or quadrilaterals.
- **`LEADER`**: Arrowed lines pointing to annotations.

### 2. Geometric Primitives
- **`XLINE`**: An infinite construction line passing through two points.
- **`RAY`**: A semi-infinite line starting at a point.
- **`MLINE`**: Multi-lines consisting of multiple parallel lines.
- **`TRACE`**: Legacy solid-filled lines with thickness.

### 3. Modern & Complex Entities
- **`IMAGE`**: References to raster images (PNG, JPG, etc.).
- **`WIPEOUT`**: Polygonal masking areas.
- **`MESH`**: Multi-faceted 3D objects.
- **`3DSOLID`**: Complex 3D objects using ACIS data (see Technical Notes).
- **`REGION`**: 2D areas using ACIS data.
- **`TOLERANCE`**: GD&T symbols.

### 4. Specialized
- **`HELIX`**: 3D spirals.
- **`PDFUNDERLAY` / `DWFUNDERLAY`**: External document references.
- **`LIGHT` / `CAMERA`**: Scene rendering objects.
- **`OLEFRAME`**: Linked Windows objects (Excel, etc.).
- **`PROXY_ENTITY`**: Placeholders for specialized vertical-market objects.

---

## Technical Notes

### 3DSOLID / ACIS Extraction
To extract valid ACIS files from DXF for use in tools like Autodesk Inventor:

#### For DXF R2010 and older (SAT format):
Data is stored in Group Codes `1` and `3`. It is obfuscated using an XOR 95 (0x5F) operation.
**Decoding Algorithm:**
```cpp
decoded_char = (c == ' ') ? ' ' : (c ^ 0x5F);
```
Save the result with a `.sat` extension.

#### For DXF R2013 and newer (SAB format):
Data is stored as raw binary in the `ACDSDATA` section under Group Code `310`, referenced by the entity's handle. Save the result with a `.sab` extension.
