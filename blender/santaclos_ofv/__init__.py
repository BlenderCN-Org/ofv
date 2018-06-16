
bl_info = {
    "name": "OFV format",
    "author": "santaclos",
    "version": (0, 0, 0),
    "blender": (2, 78, 0),
    "location": "File > Import-Export",
    "description": "Import OFV",
    "warning": "",
    "wiki_url": "",
    "support": 'OFFICIAL',
    "category": "Import-Export"}

if "bpy" in locals():
    import importlib
    if "import_ofv" in locals():
        importlib.reload(import_obj)


import bpy
from bpy.props import (
        BoolProperty,
        FloatProperty,
        StringProperty,
        EnumProperty,
        )
from bpy_extras.io_utils import (
        ImportHelper,
        ExportHelper,
        orientation_helper_factory,
        path_reference_mode,
        axis_conversion,
        )


IOOBJOrientationHelper = orientation_helper_factory("IOOBJOrientationHelper", axis_forward='-Z', axis_up='Y')


class ImportOFV(bpy.types.Operator, ImportHelper, IOOBJOrientationHelper):
    bl_idname = "import_scene.ofv"
    bl_label = "Import OFV"
    bl_options = {'PRESET', 'UNDO'}

    filename_ext = ".ofv"
    filter_glob = StringProperty(
            default="*.ofv",
            options={'HIDDEN'},
            )

    y_is_vertical = BoolProperty(
            name="Y is vertical",
            description="Y is the vertical axis",
            default=True,
            )

    def execute(self, context):
        # print("Selected: " + context.active_object.name)
        from . import import_ofv

        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "filter_glob",
                                            "split_mode",
                                            ))

        global_matrix = axis_conversion(from_forward=self.axis_forward,
                                        from_up=self.axis_up,
                                        ).to_4x4()
        keywords["global_matrix"] = global_matrix
        keywords["use_cycles"] = (context.scene.render.engine == 'CYCLES')

        if bpy.data.is_saved and context.user_preferences.filepaths.use_relative_paths:
            import os
            keywords["relpath"] = os.path.dirname(bpy.data.filepath)

        return import_ofv.load(context, **keywords)

    def draw(self, context):
        layout = self.layout

        row = layout.row(align=True)
        row.prop(self, "y_is_vertical")

def menu_func_import(self, context):
    self.layout.operator(ImportOFV.bl_idname, text="OFV (.ofv)")

classes = (
    ImportOFV,
)


def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.INFO_MT_file_import.append(menu_func_import)
    #bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.types.INFO_MT_file_import.remove(menu_func_import)
    #bpy.types.INFO_MT_file_export.remove(menu_func_export)

    for cls in classes:
        bpy.utils.unregister_class(cls)


if __name__ == "__main__":
    register()
