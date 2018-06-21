# Urho3D InGameEditor
-----------------------------------------------------------------------------------

Description
-----------------------------------------------------------------------------------
There is no editor code written in C++ but the sample demonstrates accessing the existing Editor.as script from C++ code.
Code to hook into acessing the editor is based on Urho3DPlayer.cpp. In this example, 19_VehicleDemo scene is created and you
play in it as well as edit it.

**Before you begin**  
Your build tree should contain the latest script files from the Urho3D/Data/Scripts folder from the master branch!

The script files found in the Data/InGameEditor folder are based on the latest from the head, as of July 06, 2017. 
The changes to the originals are minimal, however, you should diff and update these two script files whenever the 
Data/Scripts/Editor.as and Data/Scripts/Editor/EditorUI.as files are updated.


Known problem
-----------------------------------------------------------------------------------
~~I just noticed that the gizmo doesn't show up in the editor.~~ Fixed.

To Build
-----------------------------------------------------------------------------------
To build it, unzip/drop the repository into your Urho3D/ folder and build it the same way as you'd build the default Samples that come with Urho3D.

License
-----------------------------------------------------------------------------------
The MIT License (MIT)







