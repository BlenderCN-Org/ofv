
import bpy, bmesh
from mathutils import Vector

def createObject(name):
    global bm
    global mesh
    mesh = bpy.data.meshes.new("mesh")  # add a new mesh
    object = bpy.data.objects.new(name, mesh)  # add a new object using the mesh
    scene = bpy.context.scene
    scene.objects.link(object)  # put the object into the scene (link)
    scene.objects.active = object  # set as the active object in the scene
    object.select = True  # select object
    mesh = bpy.context.object.data
    bm = bmesh.new()
    return

filepath = "/home/santaclos/Desktop/ofv/models/output.ofv"

f = open(filepath, 'r', encoding='utf-8')
data = f.read()
f.close()

verticalIsY = True
objectCreated = False
verticesDic = {} # name, bm.vert
lines = data.split('\n')

for line in lines:

    if len(line) == 0:
        continue

    if line[0] == 'o':
        if objectCreated:
            bm.to_mesh(mesh)
            bm.free()
        name = line[line.index('[') + 1 : line.index(']')]
        createObject(name)
        objectCreated = True

    elif line[0] == 'v':
        split = line.split('[')
        inBrackets = split[1][0 : split[1].index(']')]
        ss = inBrackets.split(',')
        if verticalIsY:
            verticesDic[split[0]] = bm.verts.new(Vector((float(ss[0]),-float(ss[2]),float(ss[1]))))
        else:
            verticesDic[split[0]] = bm.verts.new(Vector((float(ss[0]),float(ss[1]),float(ss[2]))))

    elif line[0] == 'f':
        string = line[line.index('[') + 1 : line.index(']')]
        split = string.split(',')
        verts = []
        for a in split:
            verts.append(verticesDic[a])
        bm.faces.new(verts)

bm.to_mesh(mesh) # make the bmesh the object's mesh
bm.free()  # always do this when finished