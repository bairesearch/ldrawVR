# LDrawVR

### Author

Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)

### License

* All ldrawVR repository source code/assets are licensed under AGPLv3.
* Unreal Engine 4.14.x source code/assets are available under their own proprietary license and are not distributed with the ldrawVR repository.
* AGPLv3 is not compatible with the Unreal Engine proprietary license - a compatible license should be requested if required.

### Description

ldrawVR is a virtual reality editor for LDraw.

ldrawVR currently supports Oculus Rift with Oculus Touch (although it could be easily upgraded to support Vive, considering that it is built using
the Unreal Engine VR Template).

The C++ source is currently licensed under AGPLv3, which means that compiled/built versions of the project can't be distributed (due to Unreal
Engine licensing restrictions).

Custom parts are currently experimental (which use UE procedurally generated mesh objects), but it contains a full snapshot of the ldraw.org
parts library (stored as UE static meshes). dat->fbx->uasset conversion of the parts library was achieved using the Blender Ldraw Importer
add-on (along with some other tools to hack the fbx binaries).

(See http://forums.ldraw.org/thread-22002.html for first release announcement).

### Installation

#### Compilation

1. Install Unreal Engine 4.14.x [4.14.3]: open Epic Games Launcher - Unreal Engine - Library - Engine Versions + - add 4.14.x

2. Copy Unreal Engine 4.14.x Templates content to ldrawVRprojectFolder\Content folder (do not overwrite existing ldrawVR content files)
	* copy all files from Epic Games\UE_4.14\Templates\TemplateResources\Standard\FirstPerson\Content\ to;
		* ldrawVRprojectFolder\Content\FirstPerson
	* copy all files from Epic Games\UE_4.14\Templates\TP_VirtualRealityBP\Content\VirtualReality\ to;
		* ldrawVRprojectFolder\Content\VirtualReality
	* copy all files from Epic Games\UE_4.14\Templates\TP_VirtualRealityBP\Content\VirtualRealityBP\ to;
		* ldrawVRprojectFolder\Content\VirtualRealityBP
	* copy all files from Epic Games\UE_4.14\Templates\TemplateResources\Standard\Geometry\Content\Meshes\ to;
		* ldrawVRprojectFolder\Content\Geometry\Meshes
	
3. Open Epic Games Launcher - Unreal Engine - Library - Open Unreal Engine 4.14.x - Projects - Browse - open ldrawVR project folder

4. Right click on ldrawVRv0.uproject - generate visual studio project files

5. Unreal Editor - Launch - Desktop/Windows [test]

6. Unreal Editor - File - Package Project - Windows - Windows (64-bit)

#### Configuration

1. Copy Source\ldrawVRv0\Private\contentBackup\contentBackup\Documents\ldrawVR to My Documents (X:\Users\yourusername\Documents\ldrawVR)
2. Ensure that the ldraw.org PARTS folder is installed here; X:\Program Files (x86)\LDraw\PARTS (required for custom part parsing)

### Execution

To run ldrawVR, perform one of the following;
1. execute the "ldrawVR" Windows shortcut
2. execute ldrawVRv0.exe -vr
3. execute ldrawVRv0.exe, press tilda (~) to open the command console, then enter "stereo on"

### Controls

#### Oculus Touch controls:

* Facebutton1 L - increment selected actor colour BP + colour selected actor BP
* Facebutton1 R - toggle lock selected actor to grid BP
* Facebutton2 L - duplicate selected actor BP
* Facebutton2 R - delete selected actor BP
* Oculus left menu button - show/hide main menu
* Facebutton1 L + MotionController Facebutton1 R (held simultaneously) - toggle LD grid resolution BP

### Demonstration

Demonstration videos were created December 2016, when the software was first tested (after around 4-5 weeks of development), a few days after the touch system arrived;

* ldrawVRfirstTrial2b-RBB17Dec2016-partInsertionAndManipulation.mp4 - https://vimeo.com/196078980
* ldrawVRfirstTrial3a-RBB17Dec2016-submodelManipulation.mp4 - https://vimeo.com/196079184
* ldrawVRfirstTrial3b-RBB17Dec2016-pageAndColourAndSetSelection.mp4 - https://vimeo.com/196079305
* ldrawVRfirstTrial3c-RBB17Dec2016-partRecolour.mp4 - https://vimeo.com/196079486
* ldrawVRfirstTrial3d-RBB17Dec2016-partDuplicationAndDeletion.mp4 - https://vimeo.com/196079522
* ldrawVRfirstTrial3e-RBB17Dec2016-categorySelection.mp4 - https://vimeo.com/196079609
* ldrawVRfirstTrial4a-RBB20Dec2016-physicsObliterate.mp4 - https://vimeo.com/196450270
* ldrawVRfirstTrial4b-RBB20Dec2016-physicsPile - https://vimeo.com/196450376
* ldrawVRfirstTrial4c-RBB20Dec2016-collisionAndAlignmentDetection - https://vimeo.com/196450456
