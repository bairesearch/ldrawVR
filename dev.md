### Development

#### Unreal Engine LDR import instructions

1. Normalise LDraw parts dat file extension and file format
	* Ensure all parts files are named using latest LDraw format .dat (not .DAT)
	* Copy all .dat files in D:\Program Files (x86)\LDraw\parts folder to E:\Files\design\ldraw\fbxConversion\data\
	* Download and open Far (https://sourceforge.net/projects/findandreplace)
		* far - settings - remove 'treat unknown file types as binary'
		* far - settings - remove 'exclude binary files from replace operation'
		* far - untick "include subdirectories"
		* replace all;
			```
			
			[ ]*([134]) 16 
			```
			with;
			```
			
			\1 0x2CCCCCC 
			```
		* {NB [ ]* is to take into account miscellaneous space at start of line}
		* {NB to far find and replace new line characters, use real new line character instead of \n}
	* References;
		* http://www.ldraw.org/article/292
		* http://www.ldraw.org/article/218.html
		* http://wiki.ldraw.org/index.php?title=User:Owen_Burgoyne/LDraw_Colour_Comparison_Chart
		* http://www.ldraw.org/article/547.html

2. Install blender import LDR addon
	* https://wiki.blender.org/index.php/Extensions:2.6/Py/Scripts/Import-Export/LDRAW_Importer
	* download LDR-Importer binary; https://github.com/le717/LDR-Importer/releases/download/v1.3.2/LDR-Importer-v1.3.2.zip (https://github.com/le717/LDR-Importer/releases/)
	* Blender > File > User Preferences > Addons > Install from File...'
	* Browse for the Zip archive you downloaded and select it.
	* Find LDR Importer in the list of Addons (search for "LDR" if needed), and tick the check mark to activate it.
	* Click the Save User Settings button
	* Click File > Import > LDraw Model (.ldr/.dat)
	* Set the LDraw Folder Path (in the bottom left corner of your screen, under the Import Options: header, there is a small field with the label LDraw Path)
	* Modify the field with the proper folder path to your LDraw Parts Library installation path (D:\Program Files (x86)\LDraw)

3. Execute convertLDRtoFBX.py conversion script on entire LDraw parts library
	* Blender
	* Open a Text Editor view in Blender (Shift F11).
	* If necessary create a new text block (so that it can be edited)
	* Copy and paste this text (convertLDRtoFBX.py):
		```
		import bpy
		import os
		from os import listdir
		from os.path import isfile, join

		path = 'E:/Files/design/ldraw/fbxConversion/data/'  # set this path

		onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
		for g in onlyfiles:
			if g.endswith('.dat') :
			in_file = os.path.join(path, g)
			out_file = os.path.splitext(in_file)[0] + ".fbx"

			bpy.ops.object.select_all(action='SELECT')
			bpy.ops.object.delete()

			bpy.ops.import_scene.ldraw(filepath=in_file) # change this line

			bpy.ops.object.select_all(action='SELECT')

			bpy.ops.export_scene.fbx(filepath=out_file)
		```
	* Run script
	* Error log for future reference;
		* DEBUG 2902/2903/87606.dat/[57895p01.dat/87079p01.dat/] fail to convert:
			* blender codecs.py UnicodeDecodeError: 'utf-8' codec can't decode
			* byte 1454, E:\Files\design\ldraw\fbxConversion\dataFailedToConvert\2902.dat [try to manually regenerate the file]
			* SOLUTION? (manually recreate all text in these files and subparts?)
		* DEBUG python for files in folder showing files from other folders;
			* http://stackoverflow.com/questions/3207219/how-to-list-all-files-of-a-directory-in-python
			* SOLUTION: use alternate method (not walk)
		* NB may have to run the script in multiple intervals because of a performance drop issue over time

4. Download and open Far (https://sourceforge.net/projects/findandreplace)
	* far - settings - set 'treat unknown file types as binary'
	* far - settings - remove 'exclude binary files from replace operation'
	* far - untick "include subdirectories"
	* replace all;
		`Mat_([0-9]*)\.[0-9][0-9][0-9][0-9]`
		with;
		`Mat_\1.0000`
	* replace all;
		`Mat_([0-9]*)\.[0-9][0-9][0-9]`
		with;
		`Mat_\1.000`
	* replace all;
		`Mat_0x2CCCCCC\.[0-9][0-9][0-9][0-9]`
		with;
		`Mat_0x2CCCCCC.0000`
	* replace all;
		`Mat_0x2CCCCCC\.[0-9][0-9][0-9]`
		with;
		`Mat_0x2CCCCCC.000`
	* CURRENT ISSUE: There is some kind of error with auto conversion because manual conversion works fine

5. Modify Unreal Editor source (same version as UE; eg 4.) to set all materials to "two sided" when created by fbx import

	* edit D:\Files\design\UEsrc\UnrealEngine-4.14.0-release\Engine\Source\Editor\UnrealEd\Private\Fbx\FbxMaterialImport.cpp
		line 672: +
		```
		//#ifdef RBB_MOD
		UnrealMaterial->TwoSided = 1;
		//#endif	
		```
	* edit D:\Files\design\UEsrc\UnrealEngine-4.13.2-release\Engine\Source\Editor\UnrealEd\Private\Fbx\FbxMaterialImport.cpp
		line 474: +
		```
		//#ifdef RBB_MOD
		UnrealMaterial->TwoSided = 1;
		//#endif
		```
	* Build UE

6. Import fbx files into Unreal
	* Create a new Unreal project
	* Import
	* select multiple fbx files
	* FBX import options
		* deselect skeletal Mesh
		* select import Materials*
	* import all
		* error log for future reference;
			```
			Failed to import '../../../../../story/unrealengine/assetImport/v2/dataSuccessfullyConverted/3811p05.fbx'. Failed to create asset '/Game/parts/colX/3811p05'.
			Failed to import '../../../../../story/unrealengine/assetImport/v2/dataSuccessfullyConverted/light.fbx'. Failed to create asset '/Game/parts/colX/light'.
			```

7. Define and automatically add materials to meshes [NOT REQUIRED]
	* create the following materials (in E:\Files\design\story\unrealengine\projects\ldrawVRv0\Content\FirstPerson\colours\);
		LDmaterialOpaque.uasset
	* create the following material instances (in E:\Files\design\story\unrealengine\projects\ldrawVRv0\Content\FirstPerson\colours\);
		* Mat_0 / Mat_0.000 - LDblack.uasset
		* Mat_1 / Mat_1.000 - LDblue.uasset
		* Mat_6 / Mat_6.000 - LDbrown.uasset
		* Mat_11 / Mat_11.000 - LDcyan.uasset
		* Mat_8 / Mat_8.000 - LDdarkGrey.uasset
		* Mat_2 / Mat_2.000 - LDgreen.uasset
		* Mat_7 / Mat_7.000 - LDlightGrey.uasset
		* Mat_5 / Mat_5.000 - LDmagenta.uasset
		* Mat_25 / Mat_25.000 - LDorange.uasset
		* Mat_22 / Mat_22.000 - LDpurple.uasset
		* Mat_4 / Mat_4.000 - LDred.uasset
		* Mat_15 / Mat_15.000 - LDwhite.uasset
		* Mat_14 / Mat_14.000 - LDyellow.uasset
		* Mat_0x2CCCCCC.000 / Mat_0x2CCCCCC.0000 - LDdefault.asset
		based on LDreferenceClass.h;
			```
			#define TAL_FILE_COLOUR_BLACK_RGB "000000"
			#define TAL_FILE_COLOUR_BLUE_RGB "0000F0"
			#define TAL_FILE_COLOUR_GREEN_RGB "00C000"
			#define TAL_FILE_COLOUR_CYAN_RGB "00F0F0"
			#define TAL_FILE_COLOUR_RED_RGB "F00000"
			#define TAL_FILE_COLOUR_MAGENTA_RGB "FF00FF"
			#define TAL_FILE_COLOUR_BROWN_RGB "C06000"
			#define TAL_FILE_COLOUR_LIGHTGREY_RGB "C0C0C0"
			#define TAL_FILE_COLOUR_DARKGREY_RGB "606060"
			#define TAL_FILE_COLOUR_YELLOW_RGB "FFFF00"
			#define TAL_FILE_COLOUR_WHITE_RGB "FFFFFF"
			#define TAL_FILE_COLOUR_ORANGE_RGB "FF0600"
			#define TAL_FILE_COLOUR_PURPLE_RGB "A000FF"
			#define TAL_FILE_COLOUR_DARKBLUE_RGB "000070"
			#define TAL_FILE_COLOUR_DARKGREEN_RGB "007000"
			#define TAL_FILE_COLOUR_DARKRED_RGB "700000"
			#define TAL_FILE_COLOUR_AQUA_RGB "007070"
			/
			#define DAT_FILE_COLOUR_UNDEFINED (0)
			#define DAT_FILE_COLOUR_BLACK (0)
			#define DAT_FILE_COLOUR_BLUE (1)
			#define DAT_FILE_COLOUR_GREEN (2)
			#define DAT_FILE_COLOUR_CYAN (11)
			#define DAT_FILE_COLOUR_RED (4)
			#define DAT_FILE_COLOUR_MAGENTA (5)
			#define DAT_FILE_COLOUR_BROWN (6)
			#define DAT_FILE_COLOUR_LIGHTGREY (7)
			#define DAT_FILE_COLOUR_DARKGREY (8)
			#define DAT_FILE_COLOUR_YELLOW (14)
			#define DAT_FILE_COLOUR_WHITE (15)
			#define DAT_FILE_COLOUR_ORANGE (25)
			#define DAT_FILE_COLOUR_PURPLE (22)
			#define DAT_FILE_COLOUR_DARKBLUE (272)
			#define DAT_FILE_COLOUR_DARKGREEN (288)
			#define DAT_FILE_COLOUR_DARKRED (320)
			#define DAT_FILE_COLOUR_AQUA (3)
			#define DAT_FILE_COLOUR_TRANPARENT_MOD (32)
			#define DAT_FILE_DEFAULT_COLOUR (16)
			#define DAT_FILE_DEFAULT_COLOUR_EDGELINE (24)
			```
	* Open LDmaterialOpaque.uasset in material editor - material - two sided (texturing) - tick
			(see https://docs.unrealengine.com/latest/INT/Engine/Rendering/Materials/Functions/Reference/Texturing/index.html#twosidedtexturing)

8. Create parts list
	* command prompt
	* d:
	* cd D:\Program Files (x86)\LDraw\PARTS\parts
	* dir > partsListStaticMeshes.txt
	* mv partsListStaticMeshes.txt E:\Files\design\ldrawVR\projects\ldrawVRv0\Source\ldrawVRv0\Private\sets\
	* open partsListStaticMeshes.txt with openoffice calc and remove all but last column (parts names / dat files)
	

#### Unreal Engine VR Template import procedure

1. Copy Content/VirtualReality and Content/VirtualRealityBP to existing project

2. Copy Config/DefaultInput.ini to existing project
	* DONE find out where the motion controller teleport buttons are defined in VR template blueprint (and remove A/X control; use this for something else eg colour change)
	* remove these;
		```
		-ActionMappings=(ActionName="TeleportLeft",Key=MotionController_Left_FaceButton1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="TeleportLeft",Key=MotionController_Left_FaceButton1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		-ActionMappings=(ActionName="TeleportRight",Key=MotionController_Right_FaceButton1 {None},bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="TeleportRight",Key=MotionController_Right_FaceButton1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		```
	* change these;
		```
		+ActionMappings=(ActionName="GrabLeft",Key=MotionController_Left_Grip1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="GrabRight",Key=MotionController_Right_Grip1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)	
		```
	* record of all mappings;
		```
		+ActionMappings=(ActionName="TeleportRight",Key=MotionController_Right_Thumbstick,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="GrabLeft",Key=MotionController_Left_Grip1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="GrabRight",Key=MotionController_Right_Grip1,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="TeleportLeft",Key=MotionController_Left_Thumbstick,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="HMDTeleport",Key=Gamepad_FaceButton_Bottom,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+ActionMappings=(ActionName="HMDTeleport",Key=SpaceBar,bShift=False,bCtrl=False,bAlt=False,bCmd=False)
		+AxisMappings=(AxisName="TeleportDirectionUp",Key=Gamepad_LeftY,Scale=1.000000)
		+AxisMappings=(AxisName="TeleportDirectionRight",Key=Gamepad_LeftX,Scale=1.000000)
		+AxisMappings=(AxisName="MotionControllerThumbLeft_Y",Key=MotionController_Left_Thumbstick_Y,Scale=-1.000000)
		+AxisMappings=(AxisName="MotionControllerThumbLeft_X",Key=MotionController_Left_Thumbstick_X,Scale=1.000000)
		+AxisMappings=(AxisName="MotionControllerThumbRight_Y",Key=MotionController_Right_Thumbstick_Y,Scale=-1.000000)
		+AxisMappings=(AxisName="MotionControllerThumbRight_X",Key=MotionController_Right_Thumbstick_X,Scale=1.000000)
		```

3. Replace PickupActorInterface blueprint with PickupActorInterface2 C++ class
	* replace PickupActorInterface (blueprint interface) references with PickupActorInterface2 (C++ interface) in;
		* BP_pickupCube - event pickup From PickupActorInterface + event drop From PickupActorInterface (x2)
		* BP_MotionController - Get Actor Near Hand function - Does implement interface node
		* BP_MotionController - Grab Actor function - Pickup using interface PickupActorInterface (Pickup message) node
		* BP_MotionController - Release Actor function - Drop using interface PickupActorInterface (Pickup message) node
		* BP_pickupCube (blueprint Actor) - Class Settings - Implemented interfaces

4. Add all relevant actors to Motion Contoller Map [NB motion controller map requires "NavMeshBoundsVolume" to work] 

5. Expand area of Motion Contoller Map

6. Add WidgetInteraction component to BP_MotionController - MotionController - Handmesh
	* https://www.youtube.com/watch?v=tgWWwiHQmzY
		* widget interaction with vr template
		* unreal engine "widget interaction" with "vr template"
		* unreal vr widget menu options
	* BP_MotionController - widgetInteraction - Interaction - Interaction source - World
		* NO: Interaction Source -> centre screen (HMD)
	* BP_MotionController - widgetInteraction - Debugging - show debug
	* BP_MotionController - class defaults - Input - auto receive input - player 0
	* BP_MotionController - event graph - MotionController Trigger L+R [NO: Facebutton2 L+R] -> Press/Release Pointer [left mouse button]

7. Modify ldrawVRpartActor.h/.cpp - add IPickupActorInterface2 interface
	* https://wiki.unrealengine.com/index.php?title=Interfaces_in_C%2B%2B
	* https://www.tomlooman.com/vrtemplate/comment-page-1/	
		* For anyone trying to make new objects interactive and having trouble with implementing the Pickup Actor Interface, the documentation is a little confusing– it makes it seem like you need to implement the actor interface for the level blueprint, which as far as I can tell, you don’t. Here’s what I did that worked:
			1. Select the actor you want to make interactive and choose ‘Blueprint/Add Script’
			2. Follow the documentation instructions to implement PickUpActor interface (ONLY FOR THIS NEW BLUEPRINT, NOT THE LEVEL)
			3. Then you can open up the BP_PickupCube actor and literally copy and paste that entire event graph over to your new one. If you see any errors, then you haven’t implemented the Pickup Actor Interface
			4. Lastly, if you want everything to move around and whatnot (like the example cubes) be sure to set the actor as ‘Moveable’ and check the box for ‘Simulate Physics.’
	* NB step 2 is the same as adding the ", public IPickupActorInterface2" c++ code
	* NB step 3 is the same as adding Pickup_Implementation/Drop_Implementation c++ code below:
	* ldrawVRpartActor.h;
		```
		#include "../VRtemplate/PickupActorInterface2.h"
		...
		class AldrawVRpartActor : public AStaticMeshActor, public IPickupActorInterface2
		...
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		void Pickup(USceneComponent* AttachTo);
		virtual void Pickup_Implementation(USceneComponent* AttachTo) override;
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		void Drop();
		virtual void Drop_Implementation() override;
		void setPhysicsOfMeshComponents(bool simulatePhysics);

		ldrawVRpartActor.cpp;
		void AldrawVRpartActor::Pickup_Implementation(USceneComponent* AttachTo)
		{
			USceneComponent* sceneComponent = K2_GetRootComponent();
			sceneComponent->K2_AttachToComponent(AttachTo, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);


		}
		void AldrawVRpartActor::Drop_Implementation()
		{
			K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		}
		```
		
8. Add to BP_MotionController event graph (or MotionControllerPawn?);
	* BP_MotionController - event graph - MotionController Facebutton1 L - increment selected actor colour BP + colour selected actor BP
	* BP_MotionController - event graph - MotionController Facebutton1 R - toggle lock selected actor to grid BP
	* BP_MotionController - event graph - MotionController Facebutton2 L - duplicate selected actor BP
	* BP_MotionController - event graph - MotionController Facebutton2 R - delete selected actor BP
	* BP_MotionController - event graph - Gamepad Special R [Oculus left menu button] - show/hide main menu
	* BP_MotionController - event graph - MotionController Facebutton1 L + MotionController Facebutton1 R (held simultaneously) - toggle LD grid resolution BP
	* If button right face button1  is pressed on motion controller, then call toggleLockSelectedActorToGridBP
	* Reassign change grid resolution to a new key (eg space bar, or holding down both left and right face button 1s)

9. Remove little red crosshair (originates from firstPersonTemplate)
	open AldrawVRv0HUD.cpp
	```
	..
	void AldrawVRv0HUD::DrawHUD()
	..
	/*
	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );	
	*/
	```

#### Oculus VR publish notes

If your project is using the Unreal Engine VR Template (4.13/4.14), you may receive the following error when uploading a build:

"The package contains DLL libraries that are known to cause issues when running on the Oculus platform. These files must be removed prior to uploading: .../Engine/Binaries/ThirdParty/OpenVR/OpenVRv1_0_2/Win64/openvr_api.dll"

To workaround this issue, remove the SteamVR plugin (which relies on openvr_api.dll) along with all references to the SteamVR chaperone:

1. Backup your entire project.
2. Delete the function calls to SetupRoomScaleOutline and UpdateRoomScaleOutline (following Event BeginPlay and Event Tick respectively) in the VirtualRealityBP/Blueprints/BP_MotionController Event Graph.
3. Either;
	a) delete the reference to SteamVR Chaperone in the BP_MotionController/SetupRoomScaleOutline function blueprint, or;
	b) delete the functions SetupRoomScaleOutline/UpdateRoomScaleOutline from the "Room-scale" functions in the Functions panel of the BP_MotionController blueprint editor.
4. Edit - Plugins - Virtual Reality - untick SteamVR enabled.

(See https://forums.oculusvr.com/t5/Store-Submissions/Cannot-upload-new-build-openvr-api-dll-not-accepted-any-more/m-p/455373/highlight/true#M27).

#### Change notes

* update ldrawVRprojectFolder\Content\UMG\mainMenuWB.uasset - Graph - add call to "exit Pressed MainMenu" after "OnPressed (Button 5)" - "Execute Console Command:quit"

	Fixed:
	```
	Error /Game/UMG/mainMenuWBActor : Failed import for Function /Game/UMG/mainMenuWB.mainMenuWB_C:exitPressedMainMenu__DelegateSignature
	Error /Game/UMG/mainMenuWBActor : Failed import for MulticastDelegateProperty /Game/UMG/mainMenuWB.mainMenuWB_C:exitPressedMainMenu
	```

