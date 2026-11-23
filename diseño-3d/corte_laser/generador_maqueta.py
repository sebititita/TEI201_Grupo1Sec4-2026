"""Maqueta MDF - estacionamiento inteligente (1 espacio).
Genera SVG para corte láser + ensamble 3D (STEP/STL) a partir del espesor T.
Uso: python3 maqueta.py <espesor_mm> <carpeta_salida>
"""
import sys, os, math
from shapely.geometry import box as rect, Point, Polygon
from shapely.ops import unary_union

T = float(sys.argv[1]); OUT = sys.argv[2]; os.makedirs(OUT, exist_ok=True)
FINGER = 10 if T < 4.5 else 15

def nseg(L):
    n = max(3, int(round(L / FINGER)))
    return n if n % 2 else n + 1

def notches(L, keep, axis, at, inward):
    """Rectángulos de muesca a lo largo de un borde.
    L: largo del borde; keep: 'even'/'odd'; axis 'u' o 'v' (dirección del borde);
    at: coordenada fija del borde; inward: +1/-1 hacia dentro."""
    n = nseg(L); s = L / n; out = []
    for i in range(n):
        if (keep == 'even' and i % 2 == 1) or (keep == 'odd' and i % 2 == 0):
            a, b = i * s, (i + 1) * s
            c0, c1 = sorted([at, at + inward * T])
            out.append(rect(a, c0, b, c1) if axis == 'u' else rect(c0, a, c1, b))
    return out

def panel(a, b, bottom=None, top=None, left=None, right=None):
    """Panel a x b (u,v). Cada borde: None o 'even'/'odd'."""
    g = rect(0, 0, a, b); cuts = []
    if bottom: cuts += notches(a, bottom, 'u', 0, +1)
    if top:    cuts += notches(a, top, 'u', b, -1)
    if left:   cuts += notches(b, left, 'v', 0, +1)
    if right:  cuts += notches(b, right, 'v', a, -1)
    return g.difference(unary_union(cuts)) if cuts else g

def circ(u, v, d): return Point(u, v).buffer(d / 2, 64)

parts = []   # (nombre, geom2D, plano, origen(x,y,z), grabado(list geoms))
def add(name, g, plane, org, engr=()):
    parts.append((name, g, plane, org, list(engr)))

def closed_box(name, W, D, H, org, holes=None, lid=True, lid_extra=None):
    holes = holes or {}
    x0, y0, z0 = org
    bot = panel(W, D, 'odd', 'odd', 'odd', 'odd')
    fr = panel(W, H, 'even', 'even' if lid else None, 'even', 'even')
    sd = panel(D, H, 'even', 'even' if lid else None, 'odd', 'odd')
    def h(k, g): return g.difference(unary_union(holes[k])) if k in holes else g
    add(f'{name}_base', h('bottom', bot), 'XY', (x0, y0, z0))
    add(f'{name}_frente', h('front', fr), 'XZ', (x0, y0, z0))
    add(f'{name}_atras', h('back', fr), 'XZ', (x0, y0 + D - T, z0))
    add(f'{name}_lado_izq', h('left', sd), 'YZ', (x0, y0, z0))
    add(f'{name}_lado_der', h('right', sd), 'YZ', (x0 + W - T, y0, z0))
    if lid:
        top = panel(W, D, 'odd', 'odd', 'odd', 'odd')
        if lid_extra:   # visera hacia el frente (v negativo)
            V = lid_extra
            front_notch = unary_union(notches(W, 'odd', 'u', 0, +1))
            top = unary_union([top, rect(0, -V, W, 0)]).difference(front_notch)
            top = Polygon(top.exterior.coords, [i.coords for i in getattr(top, 'interiors', [])]) if top.geom_type == 'Polygon' else top
            add(f'{name}_tapa_visera', top, 'XY', (x0, y0, z0 + H - T))
        else:
            add(f'{name}_tapa', top, 'XY', (x0, y0, z0 + H - T))

# ---------------- dimensiones ----------------
SP_W, SP_L = 120, 175            # espacio de estacionamiento
CW, CH = 40, 140                 # columna (poste) 40x40, alto 160
TAB = 16                         # lengüetas columna
BW, BD = 240, 380                # base
EW, ED, EH = 145, 135, 50        # caja electrónica (exterior)
SW, SD, SH = 50 + 2 * T, 40, 52 + 2 * T   # caja sensor (exterior)
HOLE_Z = 25                      # altura paso de cable
cx0, cy0 = (BW - CW) / 2, 20 + SP_L + 5       # columna
ex0, ey0 = (BW - EW) / 2, cy0 + CW            # caja electrónica (pegada atrás de la columna)
sx0, sy0, sz0 = cx0 + CW / 2 - SW / 2, cy0, CH   # caja sensor sobre la columna

# ---- base (maqueta) ----
slot_c = []
for xs in (cx0, cx0 + CW - T):
    slot_c.append(rect(xs, cy0 + CW / 2 - TAB / 2, xs + T, cy0 + CW / 2 + TAB / 2))
base = rect(0, 0, BW, BD).difference(unary_union(slot_c))
lx0 = (BW - SP_W) / 2
engr = [rect(lx0, 20, lx0 + 4, 20 + SP_L), rect(lx0 + SP_W - 4, 20, lx0 + SP_W, 20 + SP_L),
        rect(lx0, 20 + SP_L - 4, lx0 + SP_W, 20 + SP_L),
        rect(ex0, ey0, ex0 + EW, ey0 + ED).exterior]
add('maqueta_base', base, 'XY', (0, 0, -T), engr)

# ---- columna (poste) ----
cfront = panel(CW, CH, None, None, 'even', 'even')
cback = cfront.difference(circ(CW / 2, HOLE_Z, 12))
cside = panel(CW, CH, None, None, 'odd', 'odd')
cside = unary_union([cside, rect(CW / 2 - TAB / 2, -T, CW / 2 + TAB / 2, 0),
                     rect(CW / 2 - TAB / 2, CH, CW / 2 + TAB / 2, CH + T)])
add('poste_frente', cfront, 'XZ', (cx0, cy0, 0))
add('poste_atras', cback, 'XZ', (cx0, cy0 + CW - T, 0))
add('poste_lado_izq', cside, 'YZ', (cx0, cy0, 0))
add('poste_lado_der', cside, 'YZ', (cx0 + CW - T, cy0, 0))

# ---- caja sensor ----
tz = T + 52 - 2 - 10   # centro transductores (desde abajo de la caja)
sens_holes = {
    'front': [circ(SW / 2 - 13, tz, 16.4), circ(SW / 2 + 13, tz, 16.4)],
    'bottom': [circ(cx0 + CW / 2 - sx0, CW / 2, 12)] +
              [rect(xs - sx0, CW / 2 - TAB / 2, xs - sx0 + T, CW / 2 + TAB / 2) for xs in (cx0, cx0 + CW - T)],
}
closed_box('sensor', SW, SD, SH, (sx0, sy0, sz0), sens_holes, lid=True, lid_extra=20)

# ---- caja electrónica (caja central) ----
el_holes = {
    'front': [circ(cx0 + CW / 2 - ex0, HOLE_Z, 12)],
    'right': [rect(25 + i * 12, 12, 25 + i * 12 + 4, 36) for i in range(5)],   # ventilación
    'left': [rect(ED - T - 33.5 - 8, T + 2.5, ED - T - 33.5 + 8, T + 12.5)],  # puerto power bank (carga)
}
closed_box('caja_central', EW, ED, EH, (ex0, ey0, 0), el_holes, lid=True)

# ---- pieza de prueba de encaje ----
tst_a = panel(40, 30, 'even', None, None, None)
tst_b = panel(40, 30, 'odd', None, None, None).difference(circ(20, 18, 16.4))

# ================= SVG (empaquetado en planchas) =================
SHEET_W, SHEET_H, GAP = 600, 400, 6
def bounds(g): return g.bounds
items = []
for name, g, plane, org, en in parts:
    minx, miny, maxx, maxy = g.bounds
    items.append([name, g, en, maxx - minx, maxy - miny, minx, miny])
# rotar piezas más altas que anchas si no caben
items.sort(key=lambda it: -it[4])
sheets = [[]]; x = y = GAP; rowh = 0
for it in items:
    name, g, en, w, h, mx, my = it
    if x + w + GAP > SHEET_W:
        x = GAP; y += rowh + GAP; rowh = 0
    if y + h + GAP > SHEET_H and sheets[-1]:
        sheets.append([]); x = y = GAP; rowh = 0
    sheets[-1].append((name, g, en, x - mx, y - my, w, h))
    x += w + GAP; rowh = max(rowh, h)

def path(poly, dx, dy, H):
    def ring(c): return 'M ' + ' L '.join(f'{px+dx:.3f},{H-(py+dy):.3f}' for px, py in c) + ' Z'
    polys = [poly] if poly.geom_type == 'Polygon' else list(poly.geoms)
    return ' '.join(ring(p.exterior.coords) + ' ' + ' '.join(ring(i.coords) for i in p.interiors) for p in polys)

for k, sh in enumerate(sheets, 1):
    L = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{SHEET_W}mm" height="{SHEET_H}mm" viewBox="0 0 {SHEET_W} {SHEET_H}">',
         f'<rect x="0" y="0" width="{SHEET_W}" height="{SHEET_H}" fill="none" stroke="#cccccc" stroke-width="0.2"/>']
    for name, g, en, dx, dy, w, h in sh:
        for e in en:
            if e.geom_type not in ('Polygon', 'MultiPolygon'): continue
            L.append(f'<path d="{path(e, dx, dy, SHEET_H)}" fill="#0000ff" stroke="none"/>')
        L.append(f'<path d="{path(g, dx, dy, SHEET_H)}" fill="none" stroke="#ff0000" stroke-width="0.1"/>')
    L.append('</svg>')
    open(f'{OUT}/corte_laser_plancha{k}.svg', 'w').write('\n'.join(L))
    print(f'plancha {k}:', ', '.join(s[0] for s in sh))

# ================= DXF =================
import ezdxf
def pack(its, maxw):
    its = sorted(its, key=lambda it: -it[4]); out=[]; x=y=GAP; rowh=0; W=0
    for name, g, en, w, h, mx, my in its:
        if x + w + GAP > maxw: x = GAP; y += rowh + GAP; rowh = 0
        out.append((name, g, en, x - mx, y - my)); x += w + GAP; rowh = max(rowh, h); W=max(W,x)
    return out, W, y + rowh + GAP
def dxf(placed, fn):
    doc = ezdxf.new('R2010', setup=True); doc.units = ezdxf.units.MM
    doc.layers.add('CORTE', color=1); doc.layers.add('GRABADO', color=5); doc.layers.add('DELINEADO', color=2)
    RGB = {'CORTE': (255, 0, 0), 'GRABADO': (0, 0, 255), 'DELINEADO': (255, 255, 0)}
    ACI = {'CORTE': 1, 'GRABADO': 5, 'DELINEADO': 2}
    def att(l): return {'layer': l, 'color': ACI[l], 'true_color': ezdxf.colors.rgb2int(RGB[l])}
    msp = doc.modelspace()
    def rings(g):
        ps = [g] if g.geom_type == 'Polygon' else list(g.geoms)
        for p in ps:
            yield p.exterior.coords
            for i in p.interiors: yield i.coords
    for name, g, en, dx, dy in placed:
        for r in rings(g):
            msp.add_lwpolyline([(a + dx, b + dy) for a, b in r], close=True, dxfattribs=att('CORTE'))
        for e in en:
            if e.geom_type in ('LineString', 'LinearRing'):
                msp.add_lwpolyline([(a + dx, b + dy) for a, b in e.coords], close=True, dxfattribs=att('DELINEADO')); continue
            for r in rings(e):
                msp.add_lwpolyline([(a + dx, b + dy) for a, b in r], close=True, dxfattribs=att('GRABADO'))
    doc.saveas(fn)
piezas = [it for it in items if it[0] != 'maqueta_base']
for mw in (400, 500, 600):
    pl, w, h = pack(piezas, mw); print(f'piezas ancho max {mw}: ocupa {w:.0f} x {h:.0f} mm')
import rectpack
from shapely import affinity
SW2, SH2, G = 340, 390, 5
best = None
for algo in (rectpack.MaxRectsBssf, rectpack.MaxRectsBaf, rectpack.MaxRectsBl, rectpack.SkylineMwf, rectpack.GuillotineBafSas, rectpack.SkylineBl, rectpack.GuillotineBssfSas):
    for so in (rectpack.SORT_AREA, rectpack.SORT_LSIDE, rectpack.SORT_SSIDE, rectpack.SORT_PERI, rectpack.SORT_DIFF, rectpack.SORT_RATIO):
        pk = rectpack.newPacker(rotation=True, pack_algo=algo, sort_algo=so)
        for i, it in enumerate(piezas): pk.add_rect(it[3] + G, it[4] + G, rid=i)
        pk.add_bin(SW2 + G, SH2 + G); pk.pack()
        r = pk.rect_list()
        if best is None or len(r) > len(best): best = r
print('caben', len(best), 'de', len(piezas))
pl = []
for b, x, y, w, h, rid in best:
    name, g, en, pw, ph, mx, my = piezas[rid]
    g2 = affinity.translate(g, -mx, -my)
    if (w - G > pw + 0.01) != (h - G > ph + 0.01) and abs(pw - ph) > 0.01 and abs((w - G) - pw) > 0.01:
        g2 = affinity.rotate(g2, 90, origin=(0, 0)); bx = g2.bounds; g2 = affinity.translate(g2, -bx[0], -bx[1])
    pl.append((name, g2, [], x + G / 2 + 2.5, y + G / 2 + 2.5))
allb = unary_union([affinity.translate(g, dx, dy) for _, g, _, dx, dy in pl]).bounds
print('ocupa', round(allb[2] - allb[0]), 'x', round(allb[3] - allb[1]), 'desde', round(allb[0]), round(allb[1]))
dxf(pl, f'{OUT}/piezas_sin_base_MDF{T:g}mm.dxf')
import json
json.dump([(n, list(affinity.translate(g, dx, dy).bounds)) for n, g, _, dx, dy in pl], open(f'{OUT}/placements.json', 'w'))
pb, w2, h2 = pack([it for it in items if it[0] == 'maqueta_base'], 1000)
dxf(pb, f'{OUT}/base_MDF{T:g}mm.dxf')

# ================= 3D =================
import cadquery as cq
def solid(g):
    polys = [g] if g.geom_type == 'Polygon' else list(g.geoms)
    res = None
    for p in polys:
        w = cq.Workplane('XY').polyline(list(p.exterior.coords)[:-1]).close().extrude(T)
        for i in p.interiors:
            w = w.cut(cq.Workplane('XY').polyline(list(i.coords)[:-1]).close().extrude(T))
        res = w if res is None else res.union(w)
    return res.val()
M = {'XY': [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0]],
     'XZ': [[1, 0, 0, 0], [0, 0, -1, T], [0, 1, 0, 0]],
     'YZ': [[0, 0, 1, 0], [1, 0, 0, 0], [0, 1, 0, 0]]}
asm = cq.Assembly(name='maqueta_estacionamiento')
colors = {'maqueta': (0.78, 0.66, 0.5), 'poste': (0.72, 0.6, 0.45), 'sensor': (0.8, 0.7, 0.55), 'caja': (0.75, 0.63, 0.48)}
solids = {}
for name, g, plane, org, en in parts:
    if plane is None: continue
    s = solid(g)
    if plane == 'XZ':
        s = s.rotate(cq.Vector(0,0,0), cq.Vector(1,0,0), 90).translate(cq.Vector(0, T, 0))
    elif plane == 'YZ':
        s = s.rotate(cq.Vector(0,0,0), cq.Vector(1,0,0), 90).rotate(cq.Vector(0,0,0), cq.Vector(0,0,1), 90)
    s = s.translate(cq.Vector(*org))
    solids[name] = s
    c = colors[[k for k in colors if name.startswith(k)][0]]
    asm.add(s, name=name, color=cq.Color(*c, 1))
# componentes (volúmenes referenciales)
comp = {}
pcb_y = sy0 + T + 3.5
comp['HC-SR04_pcb'] = (cq.Workplane('XY').box(45, 1.6, 20).translate((sx0 + SW / 2, pcb_y + 0.8, sz0 + tz)), (0.12, 0.3, 0.65))
tr = None
for dxs in (-13, 13):
    c = cq.Workplane('XZ').center(sx0 + SW / 2 + dxs, sz0 + tz).circle(8).extrude(12).translate((0, pcb_y, 0))
    tr = c if tr is None else tr.union(c)
comp['HC-SR04_transductores'] = (tr, (0.75, 0.77, 0.8))
comp['ESP32'] = (cq.Workplane('XY').box(30, 57, 13).translate((ex0 + T + 2 + 15, ey0 + T + 1 + 28.5, T + 6.5)), (0.1, 0.1, 0.12))
comp['protoboard'] = (cq.Workplane('XY').box(85, 54, 9).translate((ex0 + T + 2 + 30 + 4 + 42.5, ey0 + T + 1 + 27, T + 4.5)), (0.95, 0.95, 0.93))
comp['power_bank'] = (cq.Workplane('XY').box(100, 65, 15).translate((ex0 + EW - T - 1 - 50, ey0 + ED - T - 1 - 32.5, T + 7.5)), (0.2, 0.2, 0.22))
for k, (w, c) in comp.items():
    asm.add(w, name=k, color=cq.Color(*c, 1))
asm.save(f'{OUT}/maqueta_ensamble.step')
# chequeo interferencias entre piezas de MDF
names = list(solids); bad = []
for i in range(len(names)):
    for j in range(i + 1, len(names)):
        v = solids[names[i]].intersect(solids[names[j]]).Volume()
        if v > 0.5: bad.append((names[i], names[j], round(v, 1)))
for k,(w,c) in comp.items():
    for n2,s2 in solids.items():
        v=w.val().intersect(s2).Volume()
        if v>0.5: bad.append((k,n2,round(v,1)))
print('interferencias:', bad if bad else 'ninguna')
import pickle
allm = cq.Compound.makeCompound(list(solids.values()))
cq.exporters.export(cq.Workplane().add(allm), f'{OUT}/maqueta_mdf.stl', tolerance=0.1)
for k, (w, c) in comp.items():
    cq.exporters.export(w, f'{OUT}/_comp_{k}.stl', tolerance=0.1)
for n, s in solids.items():
    cq.exporters.export(cq.Workplane().add(s), f'{OUT}/_part_{n}.stl', tolerance=0.1)
print('sensor box', SW, SD, SH, 'sheets', len(sheets))
