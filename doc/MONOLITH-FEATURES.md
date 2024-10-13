# List of X-Ray Monolith patches

* Moved solution to Visual Studio 2022, In case you have problems, make sure you installed the latest Visual C++ Redistributables. You can find them here: https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/

* DLTX by MerelyMezz with edits and bugfixes by demonized, differences compare to original:
  * Attempting to override sections no longer crash the game, but prints the message into the log. All sections that triggers that error will be printed
  * Duplicate section errors now prints the root file where the error happened for easier checking mod_... ltxes
  * DLTX received possibility to create section if it doesn't exists and override section if it does with the same symbol `@`.
  Below is the example for `newsection` that wasn't defined. Firstly its created with one param `override = false`, then its overriden with `override = true`

  ```
  @[newsection]
  override = false

  @[newsection]
  override = true
  
  ```

  * DLTX received possibility to add items to parameter's list if the parameter has structure like 
  
  ```name = item1, item2, item3```
  
    * `>name = item4, item5` will add item4 and item5 to list, the result would be `name = item1, item2, item3, item4, item5`
    * `<name = item3` will remove item3 from the list, the result would be `name = item1, item2`
    * example for mod_system_...ltx: 
    
    ```
      ![info_portions]
      >files                                    = ah_info, info_hidden_threat

      ![dialogs]
      >files                                    = AH_dialogs, dialogs_hidden_threat
      
      ![profiles]
      >files                                    = npc_profile_ah, npc_profile_hidden_threat
      >specific_characters_files                = character_desc_ah, character_desc_hidden_threat
    ```

  * Here you can get the LTXDiff tool with set of scripts for converting ordinary mods to DLTX format (https://www.moddb.com/mods/stalker-anomaly/addons/dltxify-by-right-click-for-modders-tool).

* DXML by demonized
  * Allows to modify contents of loaded xml files before processing by engine by utilizing Lua scripts
  * For more information see DXML.md guide.

* Possibility to unlocalize Lua variables in scripts before loading, making them global to the script namespace
  * For unlocalizing a variable in the script, please refer to documentation in test file in `gamedata/configs/unlocalizers` folder

* Doppler effect of sounds based on code by Cribbledirge and edited by demonized.
* True First Person Death Camera, that will stay with player when he dies and will react accordingly to player's head transforms, with possibility to adjust its settings.
  * Known bugs:
    * If the player falls with face straight into the ground, the camera will clip underground due to model being clipped as well with

* Pseudogiant stomps now can kill and damage any object, stalker or mutant, instead of only actor, configurable via console commands

* In case of missing translation for a string, the engine will fallback to english text for this string.

* Additional functions and console commands described in `lua_help_ex.script`

* Additional callbacks described in `callbacks_gameobject.script`

* Additional edits and bugfixes by demonized
  * Restored "Fatal Error" MessageBox popup in case of encountering fatal engine errors like it was on Windows 7 or lower
  * In case of typical first person model/animation errors, the game will print the section that has defined model
  * MAX_TRIS const increased from 1024 to 16384
  * Enabled death animations for CWeaponAutomaticShotgun class
  * Fixed sorting news in News Tab in PDA
  * Added getting material of ray_pick() result with all of its properties
  * Potential fix for stuck monsters from OGSR Engine repo in `control_animation_base_accel.cpp`
  * Removed maximum engine limit of 5 artefacts on belt
  * Added printing of engine stack trace in the log via StackWalker library https://github.com/JochenKalmbach/StackWalker
   * To make it work you need to download `pdb` file for your DX/AVX version and put it into same place as `exe` file. PDB files are here: https://github.com/themrdemonized/xray-monolith/releases/latest

* Fixes and features by Lucy
  * Reshade shaders won't affect UI, full addon support version of Reshade is required (see TROUBLESHOOTING for details)
  * fix for hands exported from blender (you no longer have to reassign the motion references)
  * fix for silent error / script freeze when getting player accuracy in scripts
  * animation fixes (shotgun shell didn't sync with add cartridge and close anims)
  * game no longer crashes on missing item section when loading a save (should be possible to uninstall most mods mid-game now)
  * fix for mutants stuck running in place (many thanks to Arszi for finding it)
  * fix for two handed detector/device animations (swaying is now applied to both arms instead of only the left one)
  * it's now possible to play script particle effects in hud_mode with :play(true) / :play_at_pos(pos, true)
  * the game will now display a crash message when crashing due to empty translation string
  * Scripted Debug Render functions
  * Debug renderer works on DX10/11
    * Many thanks to OpenXRay and OGSR authors:
    * https://github.com/OpenXRay/xray-16/commit/752cfddc09989b1f6545f420a5c76a3baf3004d7
    * https://github.com/OpenXRay/xray-16/commit/a1e581285c21f2d5fd59ffe8afb089fb7b2da154
    * https://github.com/OpenXRay/xray-16/commit/c17a38abf6318f1a8b8c09e9e68c188fe7b821c1
    * https://github.com/OGSR/OGSR-Engine/commit/d359bde2f1e5548a053faf0c5361520a55b0552c
  * Exported a few more vector and matrix functions to lua
  * Optional third argument for world2ui to return position even if it's off screen instead of -9999
  * Unified bone lua functions and made them safer
  * It's now possible to get player first person hands bone data in lua

* LTX based patrol paths definitions by NLTP_ASHES: https://github.com/themrdemonized/xray-monolith/pull/61

* Redotix: 3D Shader scopes (3DSS) support: https://github.com/themrdemonized/xray-monolith/pull/62

* deggua: HDR10 output support to the DX11 renderer: https://github.com/themrdemonized/xray-monolith/pull/63
  * Included shaders works with vanilla Anomaly. For compatibility with SSS and GAMMA, download GAMMA shaders from here https://github.com/deggua/xray-hdr10-shaders/releases/latest

* Fixes and features by DPurple
  * Fix of using `%c[color]` tag with multibyte font causing unexpected line ending by DPurple
  * Ability to autosave the game before crash occurs, can be disabled with console command `crash_save 0` and enabled with `crash_save 1`. Maximum amount of saves can be specified with command `crash_save_count <number>`, where number is between 0 to 20 (default is 10)

* Smooth Particles with configurable update rate by vegeta1k95
  * To change update rate use console command `particle_update_mod` which takes values from 0.04 to 10.0 (default is 1.0). 1.0 corresponds to 30hz, 0.5 - 60hz and so on. The setting is also available in the options menu in "Modded Exes" group
  * Possibility to set particle update delta in milliseconds in .pe files for fine tuning with `update_step` field

* Shader Scopes by CrookR and enhanced by Edzan, comes ready to use in the archive, delete old version first

  * Dynamic zoom is disabled by default for alternative sights (can be enabled by adding scope_dynamic_zoom_alt = true to the weapon section). For example, if you take SVD Lynx or SVD PMC with March Tactical (or other sights with adjustable zoom) and switch to alternate sight, they wont have dynamic zoom anymore
  * Possibility to set alternative sight crosshair and zoom_factor with `scope_texture_alt = <path to texture>` and `scope_zoom_factor_alt = <number>` parameters in weapon ltx
  * The main sights with dynamic zoom and binoculars now normally remember their state.
  * Added console command sds_enable [on (default)/off] to enable/disable Shader Based 2D Scopes.
  * Added sds_speed_enable [on (default)/off] console command to disable/enable mouse speed (sensitivity) effect of scope_factor when aiming.
  * Added console command sds_zoom_enable [on (default)/off] with which you can disable /enable correction of max. zoom with scope_factor, if this option is enabled then max. zoom will be such as prescribed in settings regardless of scope_factor value, if this option is disabled then max. zoom will be sum of value prescribed in settings and the increase that gives scope_factor.

  Above mentioned options are applicable only for scopes which have prescribed values in file scoperadii.script

  * Added alternative zoom control (toggle with new_zoom_enable [on/off (default)]
    * Minimal zoom is equal to either mechanical zoom or the one prescribed in section min_scope_zoom_factor.
    * The step of zoom adjustment is more precise. Also, it's possible to adjust the step of zoom with the console command zoom_step_count [1.0, 10.0], this option is also applicable to the binoculars.
  * In the new version all implementations from fakelens.script have moved directly to the engine. fakelens.script remained as a layer between the engine and scopeRadii.script

* All settings can be edited from the game options in "Modded Exes" tab

## Below are the edits that are supplemental to the mods, the mods themselves **are not included**, download the mods by the links. If mods in the links provide their own exes, you can ignore them, all necessary edits are already included on this page. 

* BaS engine edits by Mortan (https://www.moddb.com/mods/stalker-anomaly/addons/boomsticks-and-sharpsticks)

* Screen Space Shaders by Ascii1457 (https://www.moddb.com/mods/stalker-anomaly/addons/screen-space-shaders)

* Heatvision by vegeta1k95 (https://www.moddb.com/mods/stalker-anomaly/addons/heatvision-v02-extension-for-beefs-nvg-dx11engine-mod/)