FAQ and TESTIMONIALS
====================

You can find the latest FAQ at http://code.google.com/p/fpscreatorengine/wiki/FAQ

History
=======

V1.1.7
------

 * Adapted Project Blue's accuracy system to work with weapon wielding character's accuracy system
 * Replaced accuracy system with Project Blue's accuracy system
 * Added Weapon wielding characters now use the fire iteration specified in their weapons gunspec  
 * Fixed bug with accuracy values being ignored by weapon shooting characters 
 * Added new action AIMOVETOSOUND will tell the entity to move the most recent sound that has been played within it's hearing range (sounds include: footsteps,reloading,gunshots etc) 
 * Added new condition AIHEARDSOUND=x will return true if a sound was heard within the entities hearing range (sounds include: footsteps,reloading,gunshots etc)
 * Added flag to SETUP.INI to control whether the AI debug guidelines show or not
 * Fixed issue causing universe object culler to fail when camera one created
 * Added 'depth' technique to new set of PS_2_0 shaders and redirected to those (newblosser flag)
 * Added new FPI condition AIONPOINT=X x=is the points id number - this will check if the entity is on the point (named by x)
 * Added BLOOM post process shader with others in 'effectbank\ps_2_0'
 * Added POSTPROCESSING=1 flag to SETUP.INI to switch on/off the post process feature (performance)
 * Added new POST PROCESSING subroutines (U75+ only), typically depth of field, bloom, etc
 * Added new FPI action AIGOTOPOINT=X this will make the entity go from its current position to the point specified by x (to create a point use aiaddpoint=x) this path will avoid any obstacles
 * Added new FPI action AIADDPOINT=X x=is the points id number - this will create a point to be used with other ai point commands
 * Fixed bug from V1.16 that broke runfpi because of the runfpi scripts not being loaded
 * Added new FPI condition AITEAM=X 0=neutral(currently unsupported), 1=ally, 2=enemy
 * Added new FPI condition ISDARKAI=X which is true if the entity has been loaded into the Dark AI system 
 * Changed all data structure members from integer to float which fell under timer based calculations
 * Changed character weapon fire from counter to timer based logic
 * Changed flak movement and detonation from counter to timer based logic
 * Changed speed of gun animations to use timer based logic
 * Changed decal animation from counter to timer based logic
 * HUD Fader code now using timer based logic
 * Gun/Smoke/Flash/Brass counters now using timer based logic
 * Hover and Spin behaviour of collectable entities now using timer based logic
 * New improved physics system (timer based) allows sync rate zero without increasing physics speeds
 * Permanently sey SYNC RATE to ZERO for machine independent speeds (vsync ensures smooth 60-100fps)
 * Added code to allow Logitech Dual Action Controller to work by specifying XBOX=3 and XBOX=4 (inverted)
 * Modified the DarkAI player position command to include players Y coordinate (new version of DarkAI)
 * Added AIACTION=X condition (0 - if idle 1 - if attacking 2 - if in cover 3 - if retreating)
 * Added AIFOLLOWPLR=X if set to 1 this will allow AI ally entities to follow the player if set to 0 the AI ally will stop following
 * Added AIMOVERANDOM to allow AI entities to move about randomly (must be looped or entity will stop moving after a while)
 * Fixed bug causing machine gun's sounds to cut off
 * Added ETIMERSTART AND ETIMERGREATER=X to allow for personal entity timers
 * Added AICONDSOUNDFINISHED to allow sequence of WAV sounds to be played inside FPI scripts
 * Added new TALK FPI command (identical to SOUND command except this one triggers DarkVOICES for compatible characters
 * Extended ENTITYELEMENT data structure with new 'soundistalking' for DarkVOICES (FPI=TALK) support
 * Added addition to build game promot text to include (XXX MB) where XXX is memory used
 * Added new Information Section to Build Game Dialog to inform about memory consumption
 * Modified Lightmapping code to release DDS textures as they are saved (lower memory hit)
 * Added new FPI commands to control a new shader FX field called 'ShaderVariables'
 * Added new FPI condition called PLRFACING which is true when the player is facing the entity scripted
 * NOTE: Above commands are selectshadervariable,setshadervariable,incshadervariable,decshadervariable
 * NOTE: Above conditions are shadervariable,shadervariablegreater,shadervariableless
 * NOTE: Above features are not officially documented or supported and are for experimental use only
 * Fixed bug preventing serial codes from being confused between X9 and X10 when both installed on system
 * Fixed bug causing version numbers of format X.XX.XXXX not to correctly determine which value was bigger
 * Added script called APPEARANTIGRAVITY to allow good control of waypoint based moving platforms
 * Prevented the entity debug text to show at same time as RAWTEXT as the font size becomes an issue
 * Added NOTE to FPI Wizards warning that they overwrite any external FPI (and rely on internal templates)
 * Added LIGHTSOURCE data to scenery shaders to allow segment shaders to pickup local average light position
 * Added new NEWBLOSSERSHADERS flag to SETUP.INI, when set to one substitutes all D for D2 texture loading
 * Added for new shaders/textures in the PS2 effectbank folder (huge thanks to Mark Blosser for the hard work here)
 * Added blinking and pulsating corridor segment pieces to enrich scifi themed game levels
 * NOTE: New sample level added to mapbank called GS13-PS2ShadedScifi.fpm demonstrating the above improvements
 * NOTE: Remember to switch FULL SHADER EFFECTS on in order to view the improved scifi segments
 * Fixed bug causing entities that did not specify a TEXTURED field to wipe out texture when any shading applied
 * Added new HEADLIMBS field in FPE to control the DarkVOICES feature (specifies which limbs are the head)
 * Added new 1.6GB MEMORY cap on all build processes, to ensure excessive memory usage does not crash PC
 * Added new flag 'systemmemorycapoff=0' which controls whether above system mem cap if switched off
 * Introduced latest ODE Phyics DLL (replacing old ODE module from 2005)
 * Fixed world step issue created between old 2005 ODE and new 2010 ODE modules (timer based physics)
 * Fixed character rotation issue caused by old ODE/new ODE, now all characters orient forwards
 * Added new RAGDOLL FPI action and ragdollcorpse.fpi script, simply select to ragdoll your characters
 * Added extra support for controller within the title and in-game menus (no need for keyboard now)
 * NOTE: For the escape key, use the controllers start button (or similarly placed central buttons)
 * When building final game, all WAV files are assumed to come with a .LIPSYNC (so they get copied over)
 * Fixed issues relating to rag doll jitters, and limb handling good for non-specific models (legacy)
 * Now prevents weapons such as shotguns (blast) from knocking over ragdoll based characters until dead

FPGC VARIANT SPECIFIC
---------------------

 * Ensured pointer does not move irratically if no controller is plugged in
 * Added code to include PS2.0 and CONTROLLER HINT files to a final build
 * Added new level called CANDLE.FPM which demonstrates candle and alienglobe
 * Finished off DECAL BASED PARTICLE support inside DECALSPEC.TXT (alienglobe)
 * Added extra functionality to add DECAL MODE 7 (particle fragments), used for candle flame/fire/etc
 * Fixed several WW2 switches that where rotated 180 degrees (suspect new matrix calc with pivot!)
 * Converted all LIGHT entity media to use new entitylight type (geoemtry+light source)
 * Issue of some WW2 switches being double rotated on the pivot. Corrected X models.
 * Added LIGHTOFFSETUP and LIGHTOFFSETZ to control position of light relative to entity
 * Added new ISMARKER=5 for FPE to create a special EntityLight (Entity with lightsource)
 * New CONTROLLERHINT feature shows Logitech Dual Action controller which indicates what user can press
 * Added CONTROLLERHINT flag to SETUP.INI and a new controller folder in gamecore/huds/
 * NOTE: Do we need Button 4 (ENTER) now we have Button 2 (LEFT CLICK) on the controller for FPGC?
 * Added code which detects if USE can be pressed, and if so, treat LEFT as USE and prevent item drop
 * If in FPGC mode, mouse buttons reversed so LEFT=pickup/drop and also [ENTER], RIGHT=NOTHING
 * VRMODE=5 now detects of VR920 available and switches to VRMODE=4 if YES and VRMODE=0 if NO
 * Mouse Sensitivity flag in SETUP.INI (centralise SETUP.INI code)
 * Restore size of debug text if RAWTEXT changes it
 * Add entity name to equipment entity (so wrench only activates 'broken door' and nothing else) TEST
 * Stop wrench animation if did are not actually activating anything
 * When jump off very tall structures (cannot get hurt)
 * Wrench equipment and graphics complete
 * Integrated VR920 support (through SETUP.INI VRMODE=1)
 * Added store to unified interface (with ADULT RATED flag to remove unsuitable content)
 * Changed sounds so less violent bone crunching noises, now thuds and cute
 * Changed RED haze HUD to cyan so does not seem like blood
 * Removed any additional references to guns and ammo from the interface and arena
 * Selected EDU friendly Model Packs for eventual Add-On Packs
 * Floor segments in ART MODE, reduce highlighter to a thin waffer (better visual)
 * Cannot add "W" as an additional shortcut WALL PAINTING as used for waypoint mode
 * LOG by default so we can offer more help when bugs strike (loading levels missing media etc)
 * Changed default so shroud is set to full (faster PCs that in 2005 and it makes the editor look nicer)
 * Prevented 'missing media' prompt from showing when LOG mode is active
 * New 'performance hit' mode in editor to allow custom textures to be shown in map and game (for characters)
 * Renamed STORY ZONE to MEDIA ZONE (trigger video and sound)
 * Rediscovered DROP action for equipment so when you press the H KEY it will drop the equipment item you are carrying (all equipment)
 * Added VRMODEMAG field as a percentage magnifier for all VR tracking input data (100=default no magnification,200=twice as sensitive)
 * Characters can now be textured within the map editor and instantly previewed using those textures
 * Extended DROP functionality in FPI so can drop JUST THE ITEM BEING HELD (VISUALLY)
 * Raised highlighter (so Z clash does not happen when draw floor then drag a character in)
 * Brightened up map editor with ambience so it does not look VERY DARK when adding just characters!!
 * Fixed so game ALWAYS comes to front Z order position (previously hides behind FPSC sometimes)
 * Added new art from Peter to complete FPGC front end and ABOUT images
 * Added HASEQUIPMENT to mirror HASWEAPON so characters can hold equipment instead of weapons
 * Renamed SOUND/SOUNDSET fields to CUSTOM VALUES so we can add text and other data for scripts (prompts)
 * Properties Dialog detects non-shooting genre and removes references to weapons and ammo fields
 * Added new WEBLINK action as part of NEWMEDIAINZONE.FPI (used by MEDIA ZONE) to launch web links
 * Further aligned new interface to direct Auto Update to X9 server, fixed logo alignment and removed NEW ARENA
 * Added extra code to allow BUILD GAME to produce non-violent EXEs
 * Added warning bar when user is putting too many entities down (ten year old kid syndrome indicator) - goes RED!
 * Player Start Marker defaults lives and strength to zero (for non-confrontational default game state)

V1.1.6 BETA 18 - 11/11/09
-------------------------

 * Amended 'alt tab' fix to allow keyboard/mouse to regain focus after switchover

V1.1.6 BETA 17 - 10/11/09
-------------------------

 * Added code to help test game to appear above interface view

V1.1.6 BETA 16 - 06/11/09
-------------------------

 * Changed ASPECTRATIO flag to default to zero (legacy support)
 * Fixed bug causing some standalones to crash when ai_free is called
 * Removed 100kb of interface code duplicating the TGCStore (now a DLL)
 * Fixed interface bug, allowing HELP>VIEW LICENSES to read correct folder
 * Restored ability for save/load to keep AI indexes of non characters
 * NOTE: Above fix solves the issue of weaponglow script loss on reload
 * Now retains all HUD data at point of save, fixes hud reload for entity prompts
 * Increased default FPS cap from 34 to 38 (until timer-based FPS comes along)
 * Solved issue of HUD data retaining in-game menu visibility modes (reset to hidden)
 * To ensure that TEST GAME absolutely appears on top at all times, TOPMOST flag used
 * Added OK button back into TEST GAME dialog as many users requested its return
 * Universe no longer disappears when using 'full effects' on test game rebuild
 * NOTE: You cannot use Light Mapping with those 'Full Effects' that use a nonstandard FVF (bump.fx)

V1.1.6 BETA 15 - 15/10/09
-------------------------

 * Added thick black border to the RAWTEXT FPI feature
 * Interface now remembers which trees where expanded for entity and segment libraries

V1.1.6 BETA 14 - 13/10/09
-------------------------

 * Added PREVIEW button back into the FPI wizard for editing game screens (X9)
 * Added new ASPECTRATIO, defaults to one to fix long-running aspect issue
 * Interface : Entity and segment library trees collapsed now by default (neater)
 * Interface : Now shows ALL the guns, not just the first 17 in Weapon Slot dialog

V1.1.6 BETA 13 - 12/10/09
-------------------------

 * Added extra LOG entry at very top of each EXE to test if they run (then fail)
 * Changed FPSC-Game.exe to initialise at 640x480 then upscale to chosen (netbook)
 * If resolution specified in SETUP.INI not exist, uses desktop resolution
 * Save Load dialog now adjusts based on the resolution of the game (run-time)
 * Adjusted loading bar progress width based on chosen resolution
 * Amended the interface logging code to ensure the trace log appears with the EXE
 * NOTE: to trace the interface, set trace=1 in the FPSCI-050.ini file (langfile)

V1.1.6 BETA 12 - 09/10/09
-------------------------

 * Fixed bug causing switches to reverse and doors not to show open on reload
 * Added new showalluniquetextures=0 flag to show per-entity unique textures
 * Added the TRIALPAY offer to a secondary BUILD GAME dialog prompt (to help users who cannot afford FPSC)

V1.1.6 BETA 11 - 19/09/09
-------------------------

 * Fixed interface to allow both X9 and X10 serial codes, and re-activated free version code (BUILD GAME redirect)

V1.1.6 BETA 10 - 18/09/09
-------------------------

 * Replaced interface with new unified version (X9+X10) (slight layout change, new getting started dialog, etc)
 * Can set player start marker lives and health to zero, removing them from influence within the game
 * Updated SWITCH2.FPI and added SWITCH2KEY.fpi and SWITCH3KEY.fpi (later requires key to operate switch)
 * Added LIGHTCANDLE.FPI to simulate the luminance of fire and candle light in a room (for dynamic lights only)
 * Fixed bug causing close portals clip universe camera close to them (removed near plane check from frustrum zero)

V1.1.6 BETA 9 - 16/09/09
------------------------

 * Major re-work of the universe collision builder, now performs two passes (first wraps static entities in boundbox)
 * Fixed major bug causing all non-character, non-marker entities to restoring the state on a game re-load
 * Renamed CURSOR LAYER to just LAYER for better screen management and reduces clipping of text with some fonts
 * Entities that have been picked up and then the level re-loaded no longer re-appear in their original positions

V1.1.6 BETA 8 - 10/09/09
------------------------

 * Fixed throw scripts which now load normally (fixes characters not falling over)
 * Updated legacy FPSC interface to remove the OKAY button and triggers auto-test game now (fixes ALT_TAB issue)
 * Fixed bug causing loading page script on game re-load to wipe out script bank one with its own logic (light bug)

V1.1.6 BETA 7 - 09/09/09
------------------------

* Fixed ODE DLL bug causing collision exist to always return zero, fixes slow (to high) memory leak
* Changed default HSRMODE (Heavy Style Recursion) to 2 (performance hit but avoids culling issues)
* Additionally removed HSRMODE ONE which never made it to release (zero=speed/two=accuracy)
* Creates GUN folder if it is not present in the final EXE to designate the genre mode
* Prevented scripts from loading on mass in test and build game (faster builds, smaller EXEs)
* Reconfigured TEST GAME to remove OK button (speeds up test process and fixes key/mouse focus bug)
* Fixed issue of test game window not always appearing at front Z order (ALT+TAB bug removed)
* Updated 'missing media' system which now produces a report alongside the FPM file (.log text format)
* Removed memory gaph from test game debug window to speed up debug properties display

V1.1.6 BETA 6 - 04/09/09
------------------------

 * Fixed bug causing characters not to use their per-element texture (when fast bone was employed)
 * Characters can now be textured and viewed as textured in the map editor (slight performance hit)
 * Added new PLAYERDROPCURRENT action so only currently selected gun will be dropped (if used in weapon script)
 * Updated Game Creator Store to allow artists to upload segment based entitybank subfolders
 * Fixed bug causing doors to lose their rotation information on an initial re-load of a saved position
 * Refined saveload in-game screen grab to use entire screen not a cropped window in center (better)
 * Optimized loading of character sound effects (better sharing, lower footprint; 150 characters=600MB=fixed )
 * Toned down player footfalls slightly to remove the consistenty annoying clang of movement

V1.1.6 BETA 5 - 14/08/09
------------------------

 * Entities that had their collision switched off retain their state after a game is reloaded with LOAD GAME
 * Characters that are completely destroyed (ACTDESTROY) no longer reappear on a reload with LOAD GAME
 * Compiled BETA5 with SafeArrays OFF to reduce executable sizes and test the tighter version publicly
 * Prevented characters from sinking into the floor when reloading after selecting LOAD GAME
 * Fixed bug causing characters to sink into floor sometimes during reload when click LOAD GAME

V1.1.6 BETA 4 - 12/08/09
------------------------

 * Fixed crash from model pack 9 and 10 shotguns (capped sound slot usage at engine level)
 * Solved bug causing animations to carry on when in PAUSE MODE (will now freeze in place)
 * Updated saveload to include pitch of the camera and whether a gun is being held
 * Extended mouse sensitivity flag to the saveload menu dialog for universal response to the value
 * Prevented some characters from using spawn animation when reloading saved position (.animdo could be -1)

V1.1.6 BETA 3 - 11/08/09
------------------------

 * Fixed bug in DBP, now allows instances to use FIX OBJECT PIVOT (which fixes new map editor rotation bug)
 * Fixed reading of RAISEFACTOR value to allow >0.0 and <1.0 values to report as 1 (legacy center Y fix)

V1.1.6 BETA 2 - 08/08/09
------------------------

 * Replaced FVF274 convert with legacy convert mode which intercepts the extra data before it loads
 * Above solves the characters turning into floor spiders bug in BETA1

V1.1.6 BETA 1 - 07/08/09
------------------------

 * Source now compiles under U74 and above (and is combined with mapeditor code) for single code base
 * Updated text file to read 'creation' instead of 'FPSCREATOR', amogst other similar text shifts
 * Updated Game Creator Store with new sensitive rating flag features for future 'kid friendly' control
 * Added a convert to FVF 274 before dynamic entities are saved to fix crash when using newer DBOs (U74)
 * Fixed bug caused by large quantity of FPI files in scriptbank (crashed with over 200 files)

V1.1.5 BETA 4 - 10/04/09
------------------------

 * Updated GAME CREATOR STORE EXE and TXT to newer April 8th 2009 version

V1.1.5 BETA 3 - TAKE TWO - 18/03/09
-----------------------------------

 * Updated GAME CREATOR STORE EXE and TXT to March 2009 version (skybox and retire support)
 * SAVE/LOAD : Fixed non-character entities from animating after a reload
 * Fixed bug causing FLAK to reset after the end level (FLAK data no longer per-level)

V1.1.5 BETA 2 - 16/02/09
------------------------

 * Updated GAME CREATOR STORE EXE and TXT to December 20008 version (skybox support)
 * SAVE/LOAD : Fixed non-character entities from animating after a reload
 * Fixed bug causing FLAK to reset after the end level (FLAK data no longer per-level)

V1.1.5 BETA 1 - 10/02/09
------------------------

 * README updated with additional FAQ text
 * SAVE/LOAD : Added additional waypoint/spawn data to save data files
 * SAVE/LOAD : Stopped VISCOLMAP leaving garbage markers when entity reloaded at new position
 * SAVE/LOAD : Fixed problem of character direction being corrupted when reloading entity characters
 * SAVE/LOAD : Fixed problem of characters retaining their animation states after a reload
 * SAVE/LOAD : Fixed problem of spawned entities not appearing visible after a reload
 * SAVE/LOAD : Entity now retains their current target after a reload
 * New parameter value for XBOX=2 which will invert the Y VIEW control for comfort

V1.1.4 BETA 5 - 07/01/09
------------------------

 * Added waypoints_multilayer.fpm in MAPBANK\TUTORIALS to demonstrate using waypoints over multiple layers
 * Reduced lightmap quality value from 10 to 5 as the visual impact is slight and improves stability of more levels
 * Fixed problem of characters reversing rotation when saved and reloaded (only rotates for corpses now)

V1.1.4 BETA 4 - 11/12/08
------------------------

 * Fixed bug causing characters to reload from a saved position in their original positions
 * Above fix also allows corpses to remain in the scene and weapons to be collected after reload

V1.1.4 BETA 3 - 10/12/08
------------------------

 * Fixed bug causing multi-level FPSC games to crash if old ODE objects left in old scene
 * Fixed problem of users without XBOX controllers getting a camera spin effect, added XBOX flag
 * New flag in SETUP.INI where you can add XBOX=1 to allow the game to use a connected controller

V1.1.4 BETA 2 - 08/12/08
------------------------

 * Each new level and loaded game position now auto-triggers first weapon in slots
 * Added two new FPI conditions (LEVELEQUAL=X and LEVELNOTEQUAL=X) where X is level
 * Added support for XBOX360 controller with hard coded controls (customize to come in 2009)

V1.1.4 BETA 1 - 17/11/08
------------------------

 * Added extra notes to the README.TXT
 * BUILD GAME speed up and memory consumption reduced (clones sounds and image-load skip)
 * Fixed BUMPBONE to respect new increased bone count allowance of 60 per mesh
 * Updated Game Creator Store DLL to fix the segment upload issue under Vista
 * Renamed the MANIFEST files to make sense to the EXE files associated with them

V1.1.3 RELEASE - 30/09/08
-------------------------

 * Fixed bug in LOAD GAME position causing larger entity levels to crash

V1.1.3 BETA 5 - 08/09/08
------------------------

 * Updated Game Creator Store to allow writes into the languagebank folder (entity LOC files)
 * Updated FASTBONE.FX to support 60 bones (maximum for vertex shader)
 * Also updated FPSC-GAME.EXE to restrict bones submitted to shaders to 60
 * Note that models using more than 60 bones per mesh will not work!

V1.1.3 BETA 4 - 13/08/08
------------------------

 * Fixed bug in muzzle flash code caused by typing out an incorrect variable name during copy/paste from AIRSLIDE's mod :)
 * Fixed bug causing GLOBAL variables to be wiped out when saved level was reloaded
 * Added LOCAL VALUES from FPI scripting to SAVE/LOAD game position feature

V1.1.3 BETA 3 - 05/08/08
------------------------

 * Courtesy of Airslide, a number of small feature additions:
 * Added FIRERATE to the GUNSPEC.TXT parser, and reflected rate of fire in how the gun fires when set to a value other than 12 (FIRERATE=12)
 * Added SHOTGUN to the GUNSPEC.TXT parser, and reflected shot gun style reloading to flagged weapons (SHOTGUN=1)
 * Amended GUNSPEC for MOSSBERG and UZI to reflect above additions (shotgun reloads correctly and uzi fires quicker)

V1.1.3 BETA 2 - 25/07/08
------------------------

 * Updated the GameCreatorStore DLL to solve the problem editing previews in Store Seller Mode

V1.1.3 BETA 1 - 22/07/08
------------------------

 * Seperated Game Creator Store from main FPSC interface (to allow store to be updated independently of editor code)
 * Added PLRSETHEALTH and PLRSUBHEALTH to set the players health, and to subtract from the players health value
 * Added ADDHEALTH, SUBHEALTH and SETHEALTH to set, increase and decrease the entities health value
 * Added new HEALTHPRISON.FPI script to show use of above entity health commands

V1.1.2 RELEASE- 07/07/08
------------------------

 * Small tweak fixing a few music preview related issues

V1.1.1 BETA 8 - 02/07/08
------------------------

 * Fixed collision issues in some import models (scale in non-bone anim data)
 * Modified COWARD10.FPI so they will stop during a waypoint traversal if shot
 * Added the Official Community Guide to FPSC in the DOCS folder (thanks Nickydude)

V1.1.1 BETA 7 - 01/07/08
------------------------

 * Further modified object alignment and scaling to allow for more import types
 * Modified character Y alignment to negate pre-scaled object collision center
 * Modified entity start-up Y alignment to allow for entities with scale specified

V1.1.1 BETA 6 - 01/07/08
------------------------

 * Increased melee range so some models do not 'push' player around (50 to 60)
 * Above applies to all 'close based' MELEE scripts in update
 * Updated build to use October 2006 DirectX, not March 2008 DirectX

V1.1.1 BETA 5 - 30/06/08
------------------------

 * Fixed interface bug causing store download status not to be updated properly
 * Fixed alignment bug introduced in B3 and B4 (reset only if animdata present)

V1.1.1 BETA 4 - 24/06/08
------------------------

 * Fixed floating character bug introduced in BETA3 (part of bound box recode)
 * Changed boundbox calculation code to take scale from animdata, not frame comb
 * Above fix to MapEditor and Game means bound boxes / object sizes more accurate
 * Above fix also solves some entities have very large highlight shrouds
 * Overridden HSRMODE flag in SETUP.INI with new mode 2 (FULL PORTAL RECURSION)
 * Above addition restores portal traversal performance (set flag to 1 for full)

V1.1.1 BETA 3 - 23/06/08
------------------------

 * Fixed Large and Small Wall segments so they do not get clipped out of universe
 * Fixed PLAYERDROP command so the item is placed on the floor not floating
 * Added new pickupcandrop.fpi script which can drop the item by pressing H key
 * Updated Interface with new code for handling Music Preview in store

V1.1.1 BETA 2 - 16/06/08
------------------------

 * Fixed potential crash caused by JPGs being reduced for fast build game time
 * Modified Image code so JPG textures managed, rather than deposited in videomem
 * Added clean exit code if imageblock is too big for heap (64MB allowed initally)
 * Amended build portals so it detects tall static entities before extending higher
 * Modified PICKUPSLIPPY.FPI to add note (must set Physics Always Active ON)

V1.1.1 BETA 1 - 14/06/08
------------------------

 * Added manifest files to main executables (Vista assist)
 * Reduced universe clip height from 550 to 150 (speed up build portals)
 * Removed registry based serial code write (interferes with X10 installation)
 * Corrected interface title so it reports 'non free' when build game present
 * Corrects serial code if accidently manually entered a lower case letter (V)
 * Amended code so increased frame rate cap (38fps) does not de-sync weapons
 * Prevent entities from spreading out if entity is only unit to be spawned
 * Adds new portal culling code back in, but allowing for shared meshgroups
 * Above tweak hopes to increase FPS (V109 results) & retaining large area renders
 * Added version number to start-up splash from the main version.ini file

V1.1.0 RELEASE - 11/06/08
-------------------------

 * New FPSCreator.exe to handle 'instant upgrade' feature in Store

V1.1.0 BETA5 - 09/06/08
-----------------------

 * Added new FPE fields and code to support arbitary footfall keyframes;

   FOOTFALLMAX = 2
   FOOTFALL0   = 30
   FOOTFALL1   = 50

 * Above example triggers a footfall sound effect at frames 30 and 50.
 * Fixed bug causing entities with low SPEED values to stutter to a halt
 * Now when characters EXPLODABLE flag set to YES, they do explode (i.e. robots)
 * Updated DBPCOMPILER associated with executables so they report true linenumbers
 * Fixed bug causing mapeditor to lose cursor position after a TEST GAME

V1.1.0 BETA 4 - 07/06/08
------------------------

 * Added extra camera control keys when in entity zoom view (SHIFT+ARROW KEYS)
 * Doubled responsiveness of camera control rotation in entity zoom view
 * Extended clipping height of main universe a further 500 units for high entities
 * Above fix solves issue where very high static entities are used in the level
 * Now 'always active' items collected with PLAYERTAKE keep their scripts running
 * Above fix allows the PLAYERDROP FPI action to function properly
 * Added new script PICKUPSLIPPY.FPI which randomly drops what has been collected

V1.1.0 BETA 3 - 06/06/08
------------------------

 * Finally fixed strange issue causing guns to lose data when initially bought
 * Added code to support accumilation of MOUSEMOVE data in FPSC Interface

V1.1.0 BETA 2 - 05/06/08
------------------------

 * Removed UNDO and REDO from main toolbar menu to keep editor in 800x600 view
 * Reduced height of preferences dialog to keep within 800x600 view
 * Added new FPI freeze command PLRDISABLE which disables mouselook and gun
 * Recompiled MapEditor.exe to resolve mouselook smoothness within interface

V1.1.0 BETA 1 - 29/05/08
------------------------

 * Added version control within engine, and added version number to seg BIN files
 * Added version number in the test game debug view
 * Now reads the VERSION.INI file to determine the internal version number
 * Modified EDITOR and GAME to automatically use VSYNC (interval of one) = smooth
 * Fixed FPSCreator.exe bug causing HOMExxx to cause line under artist name
 * Fixed FPSCreator.exe so ABOUT box better aligned and No License message to show
 * Fixed bug causing gun not to reload when running
 * Added code to detect zero or invalid serial codes and disable some features
 * Above addition is part of the development of a new free version of FPSC X9
 * Reversed code which reduces portal recursion (it caused large interior bugs)
 * Modified 50 character FPE files fixing animation 63 (822 to 882) - anim issue
 * No longer crashes if model missing or model name incorrect, uses placeholder
 * Added code to support the free version of FPSC from V110 onwards
 * Amended Fake Duct Door so transparency not showing incorrect portal culling
 * Fixed bug causing FLAK TOBJ to crash with object number illegal error
 * Improved AI handling so priority entities only reduce at 25fps or lower
 * Improved AI handling so priority reduction happens slowly, not instant drop
 * Stopped a repeat sound loop bug when entity suddenly reduced while shooting
 * Added code to detect Vista and UAC, and exit interface is UAC is enabled

V1.0.9 RELEASE - 15/05/08
------------------------

 * Added new Game Creator store component to interface

V1.0.9 BETA11 - 13/05/08
------------------------

 * Fixed bug causing gun data not to be found when adding entity in level map
 * Fixed problem causing BETA10 not to use universe skip code
 * Removed NOGRAVITY code from FLOORLOGIC as it causes characters to float
 * Adjusted quick statistics so the reflect proportion of time spent (over 1000 ms)
 * Added code so floorlogic entity does not travel into static geometry
 * Added code so floorlogic entity does not travel into lowered and raised areas

V1.0.9 BETA10 - 12/05/08
------------------------

 * Spawned entities in FULL SHADER MODE that use alpha now become visible (fadein)
 * If you delete a static entity, universe now rebuilds properly
 * Moved material detect of segments after BIN load (solves silent floor issue)
 * Fixed bug causing muzzle flash to remain just after zooming in
 * Automatic weapons no longer have a repeat-fire bug, code added to prevent
 * Gun selection no longer locks when change weapon, then quickly reload
 * Added PICKUP.WAV to Items folder as some scripts relied on this file
 * Added FLAK USAGE FLAGS so only load in flak which is actually used

V1.0.9 BETA9 - 08/05/08
-----------------------

 * Fixed long standing issue causing segment overlays not to trigger rebuild

V1.0.9 BETA8 - 07/05/08
-----------------------

 * Replaced SHARDS decal which had been deleted during the update
 * Coded support for older legacy HUD models (added MP5 AK47)
 * Added code to detect SCOPE texture in gun folder even if GUNSPEC not specified
 * Added code to prevent an empty DESC field in FPI to cause HUD corruption
 * Fixed bug causing some images (usually JPG) not to load correctly (HRES missing)
 * Fixed bug causing sounds not to load for pre-built floors in test game
 * Modified updater to launch FPSCreatorCleaner.exe rather than FPSCreator.exe
 * Above new program will delete all legacy DBO/BIN files from your installation
 * Fixed IMAGEBLOCKMODE flag so it no longer stores intermediatry in FPG file

V1.0.9 BETA7 - 30/05/08 PM
--------------------------

 * Fixed bug causing decals to disappear in the final game executable
 * Fixed bug causing new game save/load code to crash, now works with global vars
 * Changed MapEditor.exe to get file extra detail from 1256 rather than 1768
 * Above fixes a bug causing ent/seg browse to lose items on category change
 * Fixed bug that caused some models not to load their internal textures (MP9/10)

V1.0.9 BETA6 - 30/04/08 AM
--------------------------

 * Optimized loading time further by converting more X to DBO during game builds
 * There are no longer any X files exported to the standalone final game
 * Above changes improved loading time by 1 second on a typical single level exe

V1.0.9 BETA5 - 25/04/08
-----------------------

 * Fixed lightmapping corruption, no longer adds lightmaps to imageblock
 * Limit global and local variables between 0 and 99
 * Globals are now reset at the title page of final executable games
 * Created material usage flags to only load material sounds actually used
 * Moved majority to material sound loads out of per-level loading
 * Added usage flags for fragments so only loads those used in game
 * Above load time reduction took typical load from 26 seconds to 22 seconds

V1.0.9 BETA4 - 24/04/08 
-----------------------

 * Compiled and updated FPSC-MAPEDITOR.EXE for possible zero damage bug fix
 * Added new PLRFREEZE action. Set to 1 to hold plr in place, zero to release
 * Added new IMAGEBLOCKMODE to SETUP.INI. Set to 1 use image block feature
 * Imageblock feature combines all texture files into a single BIN file
 * Created new script in PEOPLE called 'meleefrosthit10.fpi' to test above
 * Added GLOBAL VALUES from FPI scripting to SAVE/LOAD game position feature
 * Updated all character FPE files in default media to allow melee attack (anim8)
 * Added V109-example.fpm to mapbank\tutorials\ to help demonstrate new features
 * Added new FPI action MUSICOVERRIDE to replace in-game music in real-time
 * Added new script called 'appearchangemusic.fpi', add to Sound Zone's init AI
 * Added new script called 'musicchangeinzone.fpi' to trigger music change
 * To ensure soundset does not load as 3D sound, rename entity to 'MUSIC ZONE'

V1.0.9 BETA3 - 21/04/08 
-----------------------

 * Finished BLOODONFLOOR feature so blood splats on floor if flagged
 * Altered player gravity so falling is more realistic (removed Y velocity cap)
 * Added new FPI commands to add variables as follows:

   GLOBALVAR=X - set the current global variable index to write/read from to X
   LOCALVAR=X - set the current local variable index to wrire/read from to X
   SETVAR=X - set the value of the current variable to X
   INCVAR=X - increases the value of the current variable by X
   DECVAR=X - decreases the value of the current variable by X
   VAREQUAL=X - returns true if the current variable equals X
   VARNOTEQUAL=X - returns true if the current variable does not equal X
   VARGREATER=X - returns true if the current variable is greater than X
   VARLESS=X - returns true if the current variable is less than X

 * Added two new FPI scripts DOORLOCKPICK.FPI and PICKUPLOCKPICKS.FPI
 * Above scripts used to demonstrate variables in script (and add cool feature)
 * Added readout of first three global and local variables in entity debug view
 * Added new FPI commands to add changes to defaults:

   NEWJUMPHEIGHT=X - change the default jump height of the player to X (default 50)
   LIGHTINTENSITY=X - change the intensity of a dynamic light to new percentage

 * New script LIGHTSUPERGRAVITY.FPI demonstrates above FPI actions
 * When using dynamic lights and DBU skip build, the dynamic lighting corrupt fixed

V1.0.9 BETA2 - 07/04/08 
-----------------------

 * Fixed long standing BUILD GAME freeze caused by not saving temporary level
 * Fixed crash caused by specifying an invalid BRASS value for weapons
 * Added new MATERIALS to the audiobank folder (from model pack 3)
 * INCFRAME no longer allows animation sequence to be corrupted 
 * Fixed bug causing high poly static entities overflowing lightmap textures
 * Fixed bug causing some outer corner and wall segments to disappear (corridors)
 * ACTION 1 - Set LIGHTMAPOLD flag in SETUP.INI to 1, and it will switch of DarkLIGHTS
 * ACTION 2 - Added new flag BLOODONFLOOR in SETUP.INI, set to one to cause blood to hit floor

V1.0.9 BETA1 - 21/03/08 
-----------------------

 * Added 'RAISEFACTOR' to entity FPE fields to allow model to be raised to correct model alignments
 * Added new MELEE10.FPI script in PEOPLE folder to allow basic melee characters
 * Added new MELEEBITE10.FPI script for a close quarters version of above
 * Added new MELEECLOSE10.FPI script for a close quarters version of above
 * Added new MOVER10.FPI script in PEOPLE folder to allow characters to PUSH the player
 * Added MONSTER voice art into audiobank for new monster character type
 * Added new MISC\MELEE.WAV and BITE.WAV into AUDIOBANK for generic melee attack sound
 * New COLLISIONMODE value for FPE which can switch off all collision but one limb (1000+solidlimbindex)
 * WARNING: Do not use above feature to create large grass coverage with individual grass blades
 * Added extra code to TIMESTAMP log file generation so it does not reported failed objects as loaded
 * Added 'FRAME:current\destination' statistic to Entity Debug Info for dynamics
 * Fixed bug using throw FPI mid main animation sequence would mess current frame on return

V1.0.8 - 11/03/08
-----------------

 * Fixed G2.TGA and G.TGA so they read D2.TGA and D.TGA

V1.0.8 BETA6 - 08/03/08
-----------------------

 * Added Q&A about disappearing geometry when using deadend segments
 * Adjusted HEADSHOT FPI action so 1 means default height of 42, other value specifies own height for head detection
 * Added new HEADSHOTDAMAGE FPI action to specify own head shot damage value

V1.0.8 BETA5 - 29/02/08
-----------------------

 * Made substantial change to universe meshgroup management to solve edge mesh disappearances by allowing more shared mesh detection and limit interior detection

V1.0.8 BETA4 - 14/02/08
-----------------------

 * Corrected cursor assigned to FAKE segments, which now use new SOLID portal type

V1.0.8 BETA3 - 11/02/08
-----------------------

 * Removed a double call to 'sound, music and animation monitoring' causing a small increase in overall framerate

V1.0.8 BETA2 - 10/02/08
-----------------------

 * Reverted back to DirectX (OCTOBER 2006) D3DX_31.DLL for sustained compatibility with current FPSC users systems

V1.0.8 BETA1 - 09/02/08
-----------------------

 * Using slightly modified DLLs from previous X10 Map Editor development for MAPEDITOR and GAME (non-impacting)
 * Added code to SETUP DLL so passing an internal width of 799 will allow mapeditor to use desktop resolution instead
 * Amended mapeditor code to use new rendering larger backbuffer and calculate for widescreen aspect ratios
 * Toned down volume of metal walking material sound, less klangy
 * Changed SYNC RATE to 34, and reduced detection ranges of characters to 500-600 (as recommended by modder s4real)
 * Added code to skip areabox traversal if areabox empty of core meshes and visited before that cycle (10fps>20fps)
 * Increased epsilon to allow meshes in segment construction wider that node for use as blockers (corridor visibility)
 * Corrected all corridor segment values to ensure special mesh sizes are visible under new adjustments
 * New segment type 11 added to all FAKE segments so that CSG holes punched through no longer causes a portal
 * When a bullet hits a character in the head, 65500 damage is instantly dealt to that character (head shot)
 * Head shot is not active by default. use the new APPEARWITHHEADSHOT.FPI in the INIT field of a characters script
 * Added new HEADSHOT action for the FPI script which accepts zero for immune to headshots, and one to allow them
 * Painting full room segments now automatically adds an outside ceiling to seal the segment if the area above is free
 * Above addition ensures a quick painting of the level produces a sealed interior, thus producing a faster game
 * Added FLAG in FPSC-050.txt for field 406. When set to 1, it will disable the above auto ceiling feature

V1.0.7 RC4 - 07/08/07
---------------------

 * Model Packs can add additional files to the weapon folders. The build game now includes all non TGA files from the weapons folder where required

V1.0.6 RC4 - 22/06/07
---------------------

 * When used the universe construction skip, old entity data would be used, rather than the existing map element data
 * Added additional error reporting on all direct image loading, now returns CWD and Parameters Of LOAD IMAGE call

V1.0.6 RC3 - 21/06/07
---------------------

 * Removed the delay at the end of each level (sometimes 20 seconds), prior to loading
 * Added a small program to the TOOLS folder which allows you to test a friends machine for compatibility with FPSC standalone games
 * Weapon when placed with weapon.fpi into a level would not orient correctly, replaced rotate object 0,0,0 with RX,RY,RZ
 * Universe construction skip no longer interferes with build process or standalone executables
 * Saving entity bank (.ENT) during construction would shuffle items using the entity reduction code from V104 (wrong)
 * Added code to save the lighting data at the end of a universe construction to be retained for re-tests

V1.0.6 RC1 and RC2 - 18/06/07
-----------------------------

 * Increased speed of build process by skipping entitiy instance and attachment reports (which had an inbuilt display delay)
 * Added universe construction skip code to re-use old static geometry and lighting if no static level elements where changed (faster build time)
 * Ensured BUILD GAME can handle up to 50 levels, by ensuring internal arrays are large enough to handle extra level file information
 * Added code to disable the FPI script editing for Arena Mode games. The script editors only produce single player game layouts
 * Fixed problem of static entities using illumination shader in full shader mode did not account for lightmap data
 * Added the local IP Address to multiplayer game title screen to provide maximum information for LAN and WAN gaming
 * Further tweaked AI so that pistol and shutgun (forward attacker behaviours) do not run if not in line with player
 * Further tweaked AI so that throw.fpi used by grenaders will lob grenades downward if player below them
 * To avoid breaking AI behaviours of existing pre-V105 games, all modified scripts have a new 10.fpi extension (X10 designation)
 * Better support for runtime error line numbers, in that they actually point to the correct line in the engine now
 * Lightmapper now removes the RETRY prompt, and instead adds exception error handling for better bug reporting

V1.0.5 RC2 - 12/03/07
---------------------

 * Modified firewall detection so that a true IP Address is detected and used for info and HOSTING (whatismyip.php)
 * Restored ability for lightmapper to delete old lightmap files, which caused game sizes to swell over time
 * Modified BUMPBONE shader so full effect characters are not semi-transparent by default
 * Added additional thread and memory checks to the lightmapper, to catch unusual behaviour on some systems
 * Added parameter to USEWEAPON FPI action to allow the throwing of flak to provide strength (100=full/50=half)
 * Updated the core AI scripts for all default characters, giving them a more direct purpose (more predicatable behaviour)
 * Added collision data to all voids in a scene so characters cannot run off ledges - better behaviour overall
 * Added collision data radar to entity debug information mode so collision interactions can be visualised

V1.0.5 RC1 - 09/03/07
---------------------

 * Changed V103/V105 texts in all languages to V105
 * Fixed bug to restore ability to move the selection area splitter (making property area wider)
 * Fixed bug to restore the colour dialog on the object properties field
 * Selecting EDIT LOADING SCREEN without selecting a level no longer freezes the editor
 * Reverted to DirectX OCTOBER 2006 (d3dx9_31.dll) due to compatibility issues with 1.X Shaders
 * Replaced DirectX detector with simpler method, earlier in the Editor Initialisation Phase
 * Reduced lightmapquality to 10 in setup.ini (reduces texture consumption by 800%)
 * Fixed bug causing lightmapper to use an old mesh buffer after secondary UV added (replace=true)
 * Ensured New Arena and Arena tab work in the new update of the FPS Creator interface
 * Added code to verify no firewall is blocking communication with gamelist server, else useful error
 * Added additional text when IP of local LAN detected, instructing user that any HOSTing is local only
 * New mode added to 'lightmapthreadmax' from SETUP.INI. Mode -2 instructs the engine not to use threads

V1.0.4 RC10 - 05/01/07
----------------------

 * Replaced FPSCreator.exe with statically linked BCG code (no more MFC DLL dependence)
 * Updated Light Mapper to export DDS files for the lightmaps (faster final game loading)
 * Fixed bug causing full lightmapping not to work in final build (cancelled no-real-load mode)
 * Recompiled GAME and MAP programs under DirectX 10 December 2006 (d3dx_32.dll required)
 * Detects DirectX December 2006 version prior to launching map editor (ease of use)

V1.0.4 RC9 - 21/12/06
---------------------

 * Added LOADREPORT field to SETUP.INI, used to track load/build times of each segment of the game level loading sequence
 * Added LIGHTMAPTHREADMAX to specify the maximum number of threads to create (for cross-processor calculation) (in testing)
 * Can now build more than one level under DarkLIGHTS tech - did not free a kernal DLL link causing an error
 * Pre-calculated the physics static geometry in the build phase so it can be loaded in real-time (significant load time saving)
 * Bug in AIKO model revealed with update to new lightmapping, sorted by restoring OFFSET LIMB code dealing with character centering
 * IMPORTANT: Before going direct to BUILD GAME, ensure you load your FPM levels and re-save them to ensure they are using the latest file version
 * A number of brass and decal assets where updated for better quality visuals in the areas of gun firing
 * Added new SMOKEDECAL field which overrides the SMOKE value in GUNSPEC.TXT, allowing any decal name to be specified for the gun smoke
 * When SMOKE=1 is found in GUNSPEC.TXT, the engine replaces functionality and uses GUNSMOKE decal istead of SMOKE1 decal
 
V1.0.4 RC8 - 19/12/06
---------------------

 * Slowed smoke by 50%, and added a new field to GUNSPEC.TXT called SMOKESPEED (with a default of 25)
 * Adjusted brass ejections so they do not behave strangely if shoot machine guns upwards
 * Added new fields in SETUP.INI for DarkLIGHTS minor settings
 * Removed dialog popup during lightmapping phase of test and build process
 * Added two new MFC related DLLs to help compatibility with systems that do not have .NET files installed
 * Fixed bug causing light colour to 'wrap' when averaging the six closest dynamic/static lights (causing weird colour switches)
 * Improved quality of routine pop-up texts and HUD stat texts to use uncompressed DDS formats for best quality, rather than DXT5
 * Increased default texture size from 256 to 512 to combat missing lightmap texture issue on some static entities (possible bug)
 * Removed some lightmap settings data from the FPG, causing lightmap defaults not to carry to the build-game process
 * Fixed bug causing all dynamic light indexes to be lost when saved out to .LGT universe file (flickering lights work in final EXE)

V1.0.4 RC7 - 17/12/06
---------------------

 * Added MFC71.DLL to associate with the new FPSCreator.exe BCG DLL (the origin of the DLL import) - should resolve some compatibility issues
 * Added support for the DarkLIGHTS technology, replacing the built-in static object lightmapper with faster and better lightmapping
 * If the MyGames folder has been deleted, FPSC will automatically restore this folder within the 'FPS Creator' folder
 * The 'Physics Force Damage' field is predictable now. 100 causes no damage, higher values inflict damage on other entities this entity hits
 * If continue to stack entities (using 'find ground' feature), the editor will automatically step up to the next layer to help editing
 * NEW FEATURE : DarkLIGHTS Technology has now been added, replacing the old Quick-Light-Mapper (for improved visuals and speed)

V1.0.4 RC6 - 15/12/06
---------------------

 * Adjusted FOV of smoke, so it emits from the correction position in the gun
 * Adjusted speed and parabola of the shells so they eject and fall more cinematically
 * By popular request, if your FPM level contains entities in the selection list that are no longer used, FPSC will remove them
 * By popular request, if your FPM level contains segments in the selection list that are no longer used, FPSC will remove them
 * Added a new debug string reporter to the SET DIR command (will display any folder name that is invalid during a build process)

V1.0.4 RC5 - 13/12/06
---------------------

 * Muzzle flash now duplicated (for twin weapons) if FIRESPOT02 limb detected in HUD.X of weapon model
 * Changed smoke emission position for guns from brasslimb marker to smokelimb marker (if smoke limb exists)
 * Extended smoke and brass ejections to a twin gun system if FIRESPOT02, SMOKE02 and BRASS02 limb names used
 * Changed tradgectory of brass ejections to orient on the gun, and expell to the right and down (more realistic)
 * Added new fields to GUNSPEC.TXT called MUZZLECOLORR,MUZZLECOLORG,MUZZLECOLORB to control spot light flash colour
 * Changed GAMECORE\BULLETHOLES\ to an alpha based approach (so bullet holes can work in cooperation with fog modes)
 * Zoom accuracy is now affected by a new GUNSPEC.TXT settings called ZOOMACCURACY (0=deadly accurate) - multiplied by zoom factor!

V1.0.4 RC4 - 11/12/06
---------------------

 * Fixed SAVE/LOAD bug that caused objectives to be wiped. Now completed objectives are remembered after a reload
 * Added code so that DEBUGREPORT flag works properly
 * Added SETTARGETNAME as a new FPI action. Specify an entity name to force the entity using the action to target the named entity
 * Restored test game release code (so the time it takes to free resources is less = faster text game exit time)
 * When HUDTEXT is used, without any other image based HUD, decal in corner does not corrupt screen
 * Added new feature of HUD DISPLAY (as in HUDMAKE=DISPLAY) which scrolls any text-only writing upwards when shown (prevents overwrite)

V1.0.4 RC3 - 27/11/06
---------------------

 * Replaced the FPSC-MapEditor.exe with the original V1 version (reduces compatibility problems)
 * Replaced the Basic3D using a new build of the DLL to facilitate FOG fixes (and future fixes - speed test alert)
 * FOG colours now work again, not just black
 * Brought FOG range closer that previous defaults, 1000.0 from camera instead of 4000.0 from camera
 * Modified FASTBONE.FX shader to account for any FOG within the level scene

V1.0.4 RC2 - 22/11/06
---------------------

 * Fixed bug causing allbut first weapon to disappear from hands of characters
 * Changed version number to V104 on the initial splash screen
 * Created a fall-back to older functionality where a sound cannot load using NO_VIRT mode
 * Corrected player speed limit so there is no lower limit, upper limit must remain for stability reasons

V1.0.4 RC1 - 15/11/06
---------------------

 * Increased resource for attachments (guns and shadows) so more entities can be created
 * Increased loading of enemies allowing them to only load one weapon (held weapon)
 * Optimized Sound Manager to avoid a possible 20 million iteration loop (speed up for large levels)
 * Switched 3D Sound Buffers to use faster CPU basic virtualiser, and switched off Listener Doppler (faster fps)
 * All 3D Sounds now created using DSBCAPS_MUTE3DATMAXDISTANCE - distant sounds muted (faster fps)
 * Capped Player Speed to reasonable (none-game-killing) limits 50 to 200 (default is 100)

V1.0.3 RC3 - 06/10/06
---------------------

 * Added an FPSC bug reporting tool written by Thomas Gortler (useful for reporting issues)
 * Fixed bug where sound zones would re-trigger after a saved game was reloaded
 * Added code to auto-expand AILIST() array associated with new FPI scripts
 * Fixed bug caused by spawned objects floating in air after mid-action save/reload action
 * Added new flag in SETUP.INI called EXITPROMPTREPORT=1 (which helps reveal the error 0,0 bug)
 * Added code to auto-expand Segment Arrays to handle as many segments as the level requires
 * Corpse based killed enemies no longer resurrect when reloading a saved game
 * Fixed problem causing FPSCreator.exe not to reload the map editor after a BUILD GAME
 * If a weapon is added via an FPSC model pack, it no longer crashes if a VWEAP.X not exist
 * Fixed icon corruption in FPSC builds once and for all
 * Story Zone crash bug fixed, caused by playing animations after a secondary level load

V1.0.3 RC2 - 25/09/06
---------------------

 * Created program 'FPSCreatorDumpMem.exe' - run it after a freeze to gather important info
 * Added version number to splash screen at start of software
 * Improved FPI wizards for TITLE and GLOBAL SCRIPT to handle the new save load feature
 * When click SAVE/LOAD GAME, mouse pointer moved to area where an instant click is not detected

V1.0.3 Rc1 - 12/09/06
---------------------

 * Restored V1 integrity - replaced all changes from V101 and V102, V1 behaviour restored
 * Fixed bug caused by window smashing changing scale of regular decal usage
 * Fixed material system so it can now handle materials 11-99 properly
 * Save executable now keeps the entity textures that use built-in texture filenames
 * Be aware that you cannot have good multi-material static meshes (use Immobile dynamic instead)
 * Be aware you should not mix a static and non static entity that shares the same multitexture
 * Solved problem when entities take a few seconds to warmup (start spinning/hovering)
 * Reduced redundant array checks in the engine (slightly faster at the cost of poor bugreports)
 * Added internal mechanisms for SAVE/LOAD feature, along with new dialog gfx and text

