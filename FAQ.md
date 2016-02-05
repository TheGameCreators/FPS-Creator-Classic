# FPS Creator X9 Frequently Asked Questions #

Compatibility and Bug Reports - things that are compatible and how to report bugs

Serial Code - problems with entering or getting serial codes

Multiplayer - specific issues about getting multiplayer arena games to work

Performance - issues about the speed of the game

Visuals - problems with how it looks and graphical corruption

Media - all things related to objects, entities, segments, etc.

Debugging - how to find out why the game does not do what you expected

Usage - general questions about how to do certain game specific things within your level


# Compatibility and Bug Reports #

Q> I tried to run FPSC on Windows Vista, but I am getting errors?

A> FPSC is not designed for, and has not been officially tested to run on Vista. The software has been designed for Windows XP.

Q> I know I have latest drivers and I have other directx 9 3d games that work fine?

A> DirectX is constantly being updated, and other programs may interfere with your operating system configuration. It is always best to visit www.microsoft.com/directx and download the latest version of DirectX that they offer, even if you have current DirectX games that work. Never assume the version of DirectX you have installed is sufficient for the later versions of FPS Creator.

Q> I will try to get some screenshots but I don't know a program that takes screenshots?

A> The easiest way to take a snapshot of your crashed screen is to press the PRINT SCREEN button located in the top right of your keyboard. Once you have done this, open the PAINT program which can be found in the START > All Programs > Accessories which is a small paint package. Now go to the EDIT menu and select the PASTE item and it will paste the contents of the clipboard that holds your screen into the workspace of the paint package. Now select FILE and SAVE AS. Select SAVE AS TYPE to JPG and give your shot a unique name that describes what you see as wrong in the screen. You can then send this lightweight JPG file along with your bug report or issue.

Q> When I try to uninstall FPS Creator, it says it has complete but it does not uninstall it from the registry?

A> If your system is not letting you uninstall FPS Creator, you may need to remove the registry manually.  To do this, go to the START menu and select RUN in XP, then type "REGEDIT" which opens the registry editor. Navigate to "HKEY\_LOCAL\_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\". Now look through the initial GUID items (long hexadecimal values) and open each one to find any entries that refer to FPS Creator installed components and delete the entire GUID entry. Do this at your own risk as you are permanently editing your registry. Once all traces of FPS Creator have been removed from the uninstall section of the registry, you should be able to remove the files manually from your computer and run the FPS Creator installer a fresh.

Q> Before I installed the update, FPSC ran fine. After I added the update my laptop freezes when I try to test a level with 2 prefabs, one player and one weapon. It gets to the point of creating "static geometry" and then locks up the machine. I have to hard boot in order to get it unfroze, why?

A> One proven solution is to update your video drivers, set your monitor to LCD instead of AGP and it may work.

Q> I ran my test game or try to build my final game and it says that it has "encountered a problem and that it needs to close" and then the map editor needs to close. What is wrong?

A> For cases like this, you need to report your bug in the FPS Creator Bug Forum. Before you do however, you can go through some standard checks to ensure full compatibility with the software. Installed the latest update to FPS Creator and make the update right after a fresh install of FPS Creator from your CD or installation file. Ensure you make the appropriate backups before you reinstall anything! Check if you have installed the latest drivers available for your graphics card, and that you have installed the latest DirectX from the Microsoft website. When you make a screenshot of the error, take a shot of the whole desktop at the exactly moment you experienced the first error message and save as a JPG. Describe the steps you took immediately after applying the update which leads to the crash. Confirm that you have all background tasks disabled or dormant so they do not interfer with FPSC and that you are running in full administrator mode not impeded by the use of virtual emulation of the operating system. Also confirm that your system can run other DirectX 9 3D games and that there are no problems running those. Following the above will provide a thorough analysis of the software against your system and allow other forum users and the technical support staff every chance to find a solution for you. Typically, during the above process, it is common to discover the reason for the failure first time without having to report the issue or consult technical support.

Q> Can we take an X9 level and make the same game with it under X10?

A> Not really. There are differences between the X9 and X10 versions making your game look different, plus X10 has some limits with X9 content. The content would be limited to what is compatible with X10, as some X9 levels and content will not migrate over perfectly. You could have two executables one for XP and one for Vista, but they would be visibly different and you would have to modify the X10 level to account for the change in content compatibility.

Q> Also for Windows 7 would we be able to use the VISTA version for DirectX?

A> We have no plans to support Windows 7 and all FPS Creator products for the time being will not officially support this operating system.

Q> I want to upgrade FPS Creator with the BUILD GAME UPGRADE, how do I do that?

A> Download the latest update here:http://www.fpscreator.com/downloads.html. Once installed, click the BUILD GAME item which will take you to the store where you can register an account, buy some Game Creator Store points and purchase directly within the store. It will download and upgrade your FPS Creator automatically.

Q> I'm keep getting this error (canot create memblock) for over a week! What can i do about it?

A> The most likely reason is that you have introduced something new to your installation of FPS Creator or your operating system in the last week which has subsequently caused the software to malfunction. The quick solution is to trace back your activity to find the component that is causing this new error in FPS Creator. The long-term solution is to back-up your files and required work and then perform a complete re-installation of FPS Creator from the original CD, then apply the latest patch from the FPSC website. Be especially aware of custom media from third parties which have not been approved or tested by TGC, as these media files should be used at your own risk and can cause a number of errors, including the type of error mentioned above.

# Serial Code #

QS01> My serial code will not work, and I cannot go past the serial code dialog?

AS01> Make sure the first letter is a capital V, and the remainder in lower case letters. If the serial code still does not work, it may have been blacklisted. Contact TGC through support mail for further assistance. Do not post your serial code on ANY forum or email, or we will be forced to blacklist the code.

# Multiplayer #

QG01> Sometimes, when I build a multiplayer game, the IP is blank and I cannot host/join a multiplayer game?

AG01> Open up the SETUP.INI file associated with the game, and ensure the field reads MULTIPLAYERGAME=1

QG02> I have created my arena game but when I try to host a game, all my friends say they see waiting to join and stays there?

AG02> This is because your game server is being blocked. if you have a firewall, ensure it is not blocking the game executable, and that it has full permission for inbound and outbound communication. Another likely cause is that your router is blocking the game, hiding the game server from the internet. In this case you must configure your router using the Port Forwarding features of the device. Set the router to allow communication to your local IP address (usually 192.168.0.XXX) where XXX is the ID of the local machine, and specify port 2303 which is the UDP communication port for the game server. You can further determine port activity by using port forwarding detection tools to configure the port for you (such as http://www.soft32.com/download_123463.html).

QG03> If I change my IP address, will my arena game work without the 'waiting to join...' message?

AQ03> The IP address shown is the address of your PC via the ISP you are using, and optionally the local machines IP address if it is on a LAN. You cannot change this number without changing your local machine IP (network settings in Windows). You can only change your dynamic IP address from your ISP when you log off and on again, where upon the ISP issues you a new IP. It is not something you need to change, it is simply your internet address. Look more in the direction of firewalls, routers and network settings to get connected with fellow arena players.

# Performance #

QP01> I noticed my game start to slow down the more I add to my level? Is this normal?

AP01> Yes, the more the game has to do the slower the game will get, until your framerate starts to suffer. Continually test the game as you design the level to ensure the framerate does not fall below 25-30fps.

QP02> Why does my game start to slow down when I start adding lots of enemies into the level?

AP02> Characters, especially those with weapons consume a lot of processing power. You are advised to be very selective where you place enemies, and avoid adding many enemies close together, as this will create a drop in framerate when you encounter them.

QP03> Why does it seem to take longer to load a character the first time?

AP03> The library of game assets are stored using X files and text files to store entity and segment information. When assets are loaded for the first time, they are converted to DBP and binary files so they can be loaded faster the next time you want to use them.

QP04> Why does my system slow down when a save operation or test game build is performed?

AP04> One possible cause is that you have a real-time virus checker (such as DLink Securespot firewall and virus protection) which actively intervenes in the process of saving data. You can resolve this problem by de-activating the real-time scan on any FPS Creator executable. or changing your active virus checker with something less intrusive.

# Visuals #

QV01> Why does the blood decals create a large black shaded area when I lightmap in quick lightmapping mode?

AV01> The blood uses an alpha channel to store semi-transparency information. You need to use Full lightmapping to lightmap decals that have transparency.

QV02> It will sometimes appear that the static lights do not correctly illuminate dynamic objects in the scene, why is this?

AV02> The lighting for dynamic objects is calculated on an average of all the nearest lights in the scene. Therefore if you have two static lights contributing to the dynamic lighting of an object, it may appear brighter than if it was statically lightmapped.

QV03a> Sometimes when I am lightmapping a large level, I get the process freezing during the calculation?

QV03b> I have chained together a large number of levels, but at seamingly random times the BUILD GAME crashes or freezes, why is this?

AV03>  There may be some systems that conflict with the implementation of multithreading used by the lightmapper. Set the 'lightmapthreadmax=-2' in the SETUP.INI file to deactivate threading.

QV04> Why can't I place weapons on their side?

AV04> If the weapon is using the weaponglow.fpi script it won't be able to. You must set the script to weapon.fpi in the entity properties of the weapon.

QV05> When do parts of the corridor disappear when I place a deadend segment and walk towards it?

AV05> A deadend segment will hide the geometry beyond it in one direction, so approaching a deadend segment from the othe direction will have side effects. To solve this, place another deadend segment facing in the opposite direction to ensure there are no culling issues.

QV06> I experience disappearing geometry when I place a large static entity in my scene?

QA06> The workaround solution is to adjust the position so the large entity is fully inside a segment tile so that when culling occurs, the entity is not residing inside a tiled part of the universe that is culled. A good rule of thumb is to place all large entities in the very center of the tile.

QV07> What sort of light system should be used for the game? Whether it should be the light source or the script light which varies the color when moving from place to place?

QA07> You can place many lights of different colours in your level, the engine calculates the lighting of all objects based on the nearest 6 lights and also works out the static lightmapping for your scene aswell. If you are using shaders you are limited to a single light source calculated as an average of the 6 closest lights position and colour. This lighting system cannot be change as it is built into the engine. You can however obtain the latest source code to the engine for free and modify it to use any lighting scheme you require, but you will need experience in Dark Basic programming language.

# Media #

QM01> Which I try to create a weapon, and add the entity to the editor workspace, it crashes?

AM01> Ensure your FLAK and GUNS folders you created are in LOWER CASE, as they need to be lower case to be detected.

QM02> When I load a map created by someone else, I get a 'missing media' error. How can I find out what media is missing?

AM02> Open SETUP.INI and change or add the field 'producelogfiles' to read 'producelogfiles=1'. This will instruct the editor to produce a log file which is written to 'MapEditor.log' located alongside the FPSCreator.exe file.

QM03> I was wondering how can i set the collision on  one of my models so the player can walk through it For example a grass entity that can be walked through. If it is as simple as changing the collision mode then can you say what number it is?

AM03> You simply set COLLISIONMODE in your FPE to something like 9999. This would tell the engine that limb 9998 is solid and the rest of the limbs are walk-through. Essentially meaning all your entity limbs would be walk-through unless you had ten thousand limbs in your model.

QM04> For some reason, no sound issues from guns when I shoot, and no player sounds work at all for that matter, any idea how I can fix this?

AM04> Your sound driver must not be configured correctly, or could be something simpler like the sound volume muted, or a cable removed. If you can hear other application sounds, you can exclude this possibility.  If it is just FPS Creator, then ensure you installed FPS Creator in administrator mode with UAC switched off (Vista) at your own risk and that you are using the software with the same configuration. If it is only 'some' sounds in FPS Creator that do not work, then try updating your sound drivers from the manufacturers website as they may not be supporting the various 2D/3D modes used by FPS Creator.

QM05> I noticed that the "texturebank folder" has been replaced by an "imageblock.bin" file when building my executable games. I need to be able to modify the images in that block, is there a flag to switch this behaviour off?

AM05> Yes there is. You will find the flag in the [GAMEMAKE](GAMEMAKE.md) group inside the SETUP.INI file. The field you need to change is imageblockmode=1 to imageblockmode=0.

QM06> I want to set my tree entity so there is collision only on the trunk, but not on the leaves and vegetation?

QA06> Open the FPE associated with the entity, and locate the COLLISIONMODE field. Change this value to 1000 plus the limb number of the trunk within the 3D object

QM07> What are the collision modes specified in the FPE under the field COLLISIONMODE?

QA07> For dynamic entities the value 0 specifies polygons, 1 specifies boxes and 2 specifies reduced polygon shape (made from boxes). You can also specify 3 for cylinder shapes and 4 for a rough sphere. For static entities the polygons are always used and so special attention should be made to ensure static entities would not cause the static entities to push the player out of the scene or cause erratic behaviour. A special trick to setting up a large static entity as a box shape is to create the entity as dynamic, but set the ISIMMOBILE flag to one to ensure it never moves. You can then set the collision mode to 1 to set the whole entity as a box.

QM08> When i use some objects that i create , sometimes the texture is not added to them and they are shown in white color, specially the objects like trees. What can i do for it?

QA08>  The texture has to be in .dds format and no compression should be used. If you do use compression only dxt1 or dxt5 should be used (no others will do). Other reasons for this problem is that the texture has been left out of the uploaded zip file (if uploading to TGC Store) or that an incorrect path to the texture has been named in the fpe/fps file.

QM09> Is there any special software to create FPSC items. I have already tested Make Entities From X-Files, but when one creates a weapon, like in the way shown in the attachement then I make in fact a link to an other exsisting FPSC weapon DBO like Autoslug. Could you explain to me how to make a weapon of my own, that  can be picked up in the game, like the weapons you guys created?

QA09> We do not have a tool to create weapons you would need to manually create them which takes quite a while. The best way would be to take an existing weapon and slowly modify it so you will be able to see what processes are involved. You could also go to our forums where you will find other artists who have produced weapons and I am sure they would be able to give you some advice. If enough artists show an interest in wanting to know more about this subject we will consider creating a manual on creating weapons for FPSC.

# Debugging #

QD01> How do I make a screenshot so I can send it to TGC for analysis?

AD01> To take a screenshot, press the PRT SCR button (usually in the top right). This will copy your desktop screen to the clipboard. Then launch the MS PAINT application by running the Start\All Programs\Accessories\Paint. When the PAINT application loads, go to the EDIT menu and click PASTE. This will paste the screen into the art package.  Now click FILE menu and select SAVE AS… Select the SAVE AS TYPE format to read JPG (smaller the BMP files). Give your new file a name and save it to the MyDocuments folder. Now when you send an email, you can drag this file into the body of the email and it will send me the screenshot.

QD02> How do I LOG the behaviour of the FPSC-MapEditor.exe and send the results to TGC?

AD02> In the root folder of FPS Creator, you will find a file called SETUP.INI. open this file in NOTEPAD and add a new line under the [GAMERUN](GAMERUN.md) group called producelogfiles=1 and then save the file. When you next launch FPS Creator, a new file will be created called FPSC-MapEditor.log which will contain the log information from that session. You can attach this file to an email by simply dragging the file into the body of your email.

QD03> When I load my level, it says there is missing media, so how do I know what is missing?

AD03> Use the above solution AD02 to produce a log file as the level loads in. View the LOG file to see which files are missing.

# Usage #

QU01> My software is crashing/hanging on loading certain maps after the upgrade?

AU01> Use the FPSC forum to post any bugs found in the latest version, complete with a step by step so we can trace the exact problem, thanks. Each crash is very individual to the user who suffers one, and we need to find out exactly what they did to create that scenario (in quite a lot of detail).

QU02> When testing a map intermittently the graphics run ‘really, really slowly’ (jittery motion)

AU02> If some parts of the level require the rendering of a lot of polygons the performance will drop. Try removing items from your level or slicing the content into smaller rooms, or even separate levels.

QU03> Multiplayer (New Arena) game option not available after the upgrade

AU03> You may have lost your serial code during a recent change to the software. Try re-entering your serial code into userdetails.ini and this should fix it.

QU04> Is it possible to limit the number of weapons the player can hold, i.e. if you only allow a main weapon and a side arm and you pick up another weapon then you automatically drop the previous main weapon?

AU04> This is not possible in the current version.  If you wish to go to the next level, you can get the latest DBP which provides the public source code to FPSC so you can make the changes to reduce the weapon slot max yourself, but be warned this requires a certain degree of programming knowledge and most importantly, persistence. We do not officially support the source code so you will be on your own if you want to make changes to it.  If there is demand for it, we may also add the feature officially so starting a thread in the forums to get this feature recognised as a high priority would be a first step.

QU05> Is it possible to have the AI enemies respond to sounds and player motion more intelligently?

AU05> There are FPI commands that allow an entity to detect when the player (or other item in the game) makes a sound and respond with actions.  This requires a certain knowledge of FPI scripting, and help with this can be found in the forums.

QU06> Can AI enemies patrol areas, if so how?

AU06> FPS Creator X9 only supports basic waypoint patrol for neutral and enemy characters.  FPS Creator X10 additionally supports a more advanced AI sub-system and the addition of allies to fight alongside the player.

QU07> How many triggers should be used in one level?

AU07> You can use quite a lot of triggers per level but we recommend you keep the number below 100.

QU08> Is there a way to view all your purchases in the store at once?

AU08> No, you would have to go to the entitybank/segments and look in the purchased folder from that location

QU09> Why is the segment entity loading time too long?

AU09> This only happens the first time you load objects after clearing the BIN and DBO files. Thereafter the BIN and DBO versions of the object are used which load much faster as they are both binary based