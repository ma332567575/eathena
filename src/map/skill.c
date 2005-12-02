#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "timer.h"
#include "nullpo.h"
#include "malloc.h"

#include "skill.h"
#include "map.h"
#include "clif.h"
#include "pc.h"
#include "status.h"
#include "pet.h"
#include "mob.h"
#include "battle.h"
#include "party.h"
#include "itemdb.h"
#include "script.h"
#include "intif.h"
#include "log.h"
#include "chrif.h"
#include "guild.h"
#include "showmsg.h"
#include "grfio.h"

#define SKILLUNITTIMER_INVERVAL	100
#define swap(x,y) { int t; t = x; x = y; y = t; }

int skill_names_id[MAX_SKILL_DB];
const struct skill_name_db skill_names[] = {
 { AC_CHARGEARROW, "AC_CHARGEARROW", "Arrow_Repel" } ,
 { AC_CONCENTRATION, "AC_CONCENTRATION", "Improve_Concentration" } ,
 { AC_DOUBLE, "AC_DOUBLE", "Double_Strafe" } ,
 { AC_MAKINGARROW, "AC_MAKINGARROW", "Arrow_Crafting" } ,
 { AC_OWL, "AC_OWL", "Owl's_Eye" } ,
 { AC_SHOWER, "AC_SHOWER", "Arrow_Shower" } ,
 { AC_VULTURE, "AC_VULTURE", "Vulture's_Eye" } ,
 { ALL_RESURRECTION, "ALL_RESURRECTION", "Resurrection" } ,
 { AL_ANGELUS, "AL_ANGELUS", "Angelus" } ,
 { AL_BLESSING, "AL_BLESSING", "Blessing" } ,
 { AL_CRUCIS, "AL_CRUCIS", "Signum_Crusis" } ,
 { AL_CURE, "AL_CURE", "Cure" } ,
 { AL_DECAGI, "AL_DECAGI", "Decrease_AGI" } ,
 { AL_DEMONBANE, "AL_DEMONBANE", "Demon_Bane" } ,
 { AL_DP, "AL_DP", "Divine_Protection" } ,
 { AL_HEAL, "AL_HEAL", "Heal" } ,
 { AL_HOLYLIGHT, "AL_HOLYLIGHT", "Holy_Light" } ,
 { AL_HOLYWATER, "AL_HOLYWATER", "Aqua_Benedicta" } ,
 { AL_INCAGI, "AL_INCAGI", "Increase_AGI" } ,
 { AL_PNEUMA, "AL_PNEUMA", "Pneuma" } ,
 { AL_RUWACH, "AL_RUWACH", "Ruwach" } ,
 { AL_TELEPORT, "AL_TELEPORT", "Teleport" } ,
 { AL_WARP, "AL_WARP", "Warp_Portal" } ,
 { AM_ACIDTERROR, "AM_ACIDTERROR", "Acid_Terror" } ,
 { AM_AXEMASTERY, "AM_AXEMASTERY", "Axe_Mastery" } ,
 { AM_BERSERKPITCHER, "AM_BERSERKPITCHER", "Aid_Berserk_Potion" } ,
 { AM_BIOETHICS, "AM_BIOETHICS", "Bioethics" } ,
 { AM_CALLHOMUN, "AM_CALLHOMUN", "Call_Homunculus" } ,
 { AM_CANNIBALIZE, "AM_CANNIBALIZE", "Summon_Flora" } ,
 { AM_CP_ARMOR, "AM_CP_ARMOR", "Synthetic_Armor" } ,
 { AM_CP_HELM, "AM_CP_HELM", "Biochemical_Helm" } ,
 { AM_CP_SHIELD, "AM_CP_SHIELD", "Synthetized_Shield" } ,
 { AM_CP_WEAPON, "AM_CP_WEAPON", "Alchemical_Weapon" } ,
 { AM_CULTIVATION, "AM_CULTIVATION", "Cultivation" } ,
 { AM_DEMONSTRATION, "AM_DEMONSTRATION", "Bomb" } ,
 { AM_LEARNINGPOTION, "AM_LEARNINGPOTION", "Potion_Research" } ,
 { AM_PHARMACY, "AM_PHARMACY", "Prepare_Potion" } ,
 { AM_POTIONPITCHER, "AM_POTIONPITCHER", "Aid_Potion" } ,
 { AM_REST, "AM_REST", "Vaporize" } ,
 { AM_RESURRECTHOMUN, "AM_RESURRECTHOMUN", "Homunculus_Resurrection" } ,
 { AM_SPHEREMINE, "AM_SPHEREMINE", "Summon_Marine_Sphere" } ,
 { AM_TWILIGHT1, "AM_TWILIGHT1", "Twilight_Pharmacy_1" } ,
 { AM_TWILIGHT2, "AM_TWILIGHT2", "Twilight_Pharmacy_2" } ,
 { AM_TWILIGHT3, "AM_TWILIGHT3", "Twilight_Pharmacy_3" } ,
 { ASC_BREAKER, "ASC_BREAKER", "Soul_Destroyer" } ,
 { ASC_CDP, "ASC_CDP", "Create_Deadly_Poison" } ,
 { ASC_EDP, "ASC_EDP", "Enchant_Deadly_Poison" } ,
 { ASC_KATAR, "ASC_KATAR", "Advanced_Katar_Mastery" } ,
 { ASC_METEORASSAULT, "ASC_METEORASSAULT", "Meteor_Assault" } ,
 { AS_CLOAKING, "AS_CLOAKING", "Cloaking" } ,
 { AS_ENCHANTPOISON, "AS_ENCHANTPOISON", "Enchant_Poison" } ,
 { AS_GRIMTOOTH, "AS_GRIMTOOTH", "Grimtooth" } ,
 { AS_KATAR, "AS_KATAR", "Katar_Mastery" } ,
 { AS_LEFT, "AS_LEFT", "Lefthand_Mastery" } ,
 { AS_POISONREACT, "AS_POISONREACT", "Poison_React" } ,
 { AS_RIGHT, "AS_RIGHT", "Righthand_Mastery" } ,
 { AS_SONICACCEL, "AS_SONICACCEL", "Sonic_Acceleration" } ,
 { AS_SONICBLOW, "AS_SONICBLOW", "Sonic_Blow" } ,
 { AS_SPLASHER, "AS_SPLASHER", "Venom_Splasher" } ,
 { AS_VENOMDUST, "AS_VENOMDUST", "Venom_Dust" } ,
 { AS_VENOMKNIFE, "AS_VENOMKNIFE", "Throw_Venom_Knife" } ,
 { BA_APPLEIDUN, "BA_APPLEIDUN", "Song_of_Lutie" } ,
 { BA_ASSASSINCROSS, "BA_ASSASSINCROSS", "Impressive_Riff" } ,
 { BA_DISSONANCE, "BA_DISSONANCE", "Unchained_Serenade" } ,
 { BA_FROSTJOKE, "BA_FROSTJOKE", "Unbarring_Octave" } ,
 { BA_MUSICALLESSON, "BA_MUSICALLESSON", "Music_Lessons" } ,
 { BA_MUSICALSTRIKE, "BA_MUSICALSTRIKE", "Melody_Strike" } ,
 { BA_PANGVOICE, "BA_PANGVOICE", "Pang_Voice" } ,
 { BA_POEMBRAGI, "BA_POEMBRAGI", "Magic_Strings" } ,
 { BA_WHISTLE, "BA_WHISTLE", "Perfect_Tablature" } ,
 { BD_ADAPTATION, "BD_ADAPTATION", "Amp" } ,
 { BD_DRUMBATTLEFIELD, "BD_DRUMBATTLEFIELD", "Battle_Theme" } ,
 { BD_ENCORE, "BD_ENCORE", "Encore" } ,	
 { BD_ETERNALCHAOS, "BD_ETERNALCHAOS", "Down_Tempo" } ,
 { BD_INTOABYSS, "BD_INTOABYSS", "Power_Cord" } ,
 { BD_LULLABY, "BD_LULLABY", "Lullaby" } ,
 { BD_RICHMANKIM, "BD_RICHMANKIM", "Mental_Sensing" } ,
 { BD_RINGNIBELUNGEN, "BD_RINGNIBELUNGEN", "Harmonic_Lick" } ,
 { BD_ROKISWEIL, "BD_ROKISWEIL", "Classical_Pluck" } ,
 { BD_SIEGFRIED, "BD_SIEGFRIED", "Acoustic_Rhythm" } ,
 { BS_ADRENALINE, "BS_ADRENALINE", "Adrenaline_Rush" } ,
 { BS_ADRENALINE2, "BS_ADRENALINE2", "Advanced_Adrenaline_Rush" } ,
 { BS_AXE, "BS_AXE", "Smith_Axe" } ,
 { BS_DAGGER, "BS_DAGGER", "Smith_Dagger" } ,
 { BS_ENCHANTEDSTONE, "BS_ENCHANTEDSTONE", "Enchantedstone_Craft" } ,
 { BS_FINDINGORE, "BS_FINDINGORE", "Ore_Discovery" } ,
 { BS_GREED, "BS_GREED", "Greed" } ,
 { BS_HAMMERFALL, "BS_HAMMERFALL", "Hammer_Fall" } ,
 { BS_HILTBINDING, "BS_HILTBINDING", "Hilt_Binding" } ,
 { BS_IRON, "BS_IRON", "Iron_Tempering" } ,
 { BS_KNUCKLE, "BS_KNUCKLE", "Smith_Knucklebrace" } ,
 { BS_MACE, "BS_MACE", "Smith_Mace" } ,
 { BS_MAXIMIZE, "BS_MAXIMIZE", "Power_Maximize" } ,
 { BS_ORIDEOCON, "BS_ORIDEOCON", "Oridecon_Research" } ,
 { BS_OVERTHRUST, "BS_OVERTHRUST", "Power-Thrust" } ,
 { BS_REPAIRWEAPON, "BS_REPAIRWEAPON", "Weapon_Repair" } ,
 { BS_SKINTEMPER, "BS_SKINTEMPER", "Skin_Tempering" } ,
 { BS_SPEAR, "BS_SPEAR", "Smith_Spear" } ,
 { BS_STEEL, "BS_STEEL", "Steel_Tempering" } ,
 { BS_SWORD, "BS_SWORD", "Smith_Sword" } ,
 { BS_TWOHANDSWORD, "BS_TWOHANDSWORD", "Smith_Two-handed_Sword" } ,
 { BS_UNFAIRLYTRICK, "BS_UNFAIRLYTRICK", "Unfair_Trick" } ,
 { BS_WEAPONPERFECT, "BS_WEAPONPERFECT", "Weapon_Perfection" } ,
 { BS_WEAPONRESEARCH, "BS_WEAPONRESEARCH", "Weaponry_Research" } ,
 { CG_ARROWVULCAN, "CG_ARROWVULCAN", "Vulcan_Arrow" } ,
 { CG_HERMODE, "CG_HERMODE", "Wand_of_Hermode" } ,
 { CG_LONGINGFREEDOM, "CG_LONGINGFREEDOM", "Longing_for_Freedom" } ,
 { CG_MARIONETTE, "CG_MARIONETTE", "Marionette_Control" } ,
 { CG_MOONLIT, "CG_MOONLIT", "Sheltering_Bliss" } ,
 { CG_TAROTCARD, "CG_TAROTCARD", "Tarot_Card_of_Fate" } ,
 { CH_CHAINCRUSH, "CH_CHAINCRUSH", "Chain_Crush_Combo" } ,
 { CH_PALMSTRIKE, "CH_PALMSTRIKE", "Raging_Palm_Strike" } ,
 { CH_SOULCOLLECT, "CH_SOULCOLLECT", "Zen" } ,
 { CH_TIGERFIST, "CH_TIGERFIST", "Glacier_Fist" } ,
 { CR_ACIDDEMONSTRATION, "CR_ACIDDEMONSTRATION", "Acid_Demonstration" } ,
 { CR_ALCHEMY, "CR_ALCHEMY", "Alchemy" } ,
 { CR_CULTIVATION, "CR_CULTIVATION", "Plant_Cultivation" } ,
 { CR_SLIMPITCHER, "CR_SLIMPITCHER", "Slim_Pitcher" } ,
 { CR_FULLPROTECTION, "CR_FULLPROTECTION", "Full_Protection" } ,
 { CR_SYNTHESISPOTION, "CR_SYNTHESISPOTION", "Potion_Synthesis" } ,
 { CR_AUTOGUARD, "CR_AUTOGUARD", "Guard" } ,
 { CR_DEFENDER, "CR_DEFENDER", "Defending_Aura" } ,
 { CR_DEVOTION, "CR_DEVOTION", "Sacrifice" } ,
 { CR_GRANDCROSS, "CR_GRANDCROSS", "Grand_Cross" } ,
 { CR_HOLYCROSS, "CR_HOLYCROSS", "Holy_Cross" } ,
 { CR_PROVIDENCE, "CR_PROVIDENCE", "Resistant_Souls" } ,
 { CR_REFLECTSHIELD, "CR_REFLECTSHIELD", "Shield_Reflect" } ,
 { CR_SHIELDBOOMERANG, "CR_SHIELDBOOMERANG", "Shield_Boomerang" } ,
 { CR_SHIELDCHARGE, "CR_SHIELDCHARGE", "Smite" } ,
 { CR_SHRINK, "CR_SHRINK", "Shrink" } ,
 { CR_SPEARQUICKEN, "CR_SPEARQUICKEN", "Spear_Quicken" } ,
 { CR_TRUST, "CR_TRUST", "Faith" } ,
 { DC_DANCINGLESSON, "DC_DANCINGLESSON", "Dance_Lessons" } ,
 { DC_DONTFORGETME, "DC_DONTFORGETME", "Slow_Grace" } ,
 { DC_FORTUNEKISS, "DC_FORTUNEKISS", "Lady_Luck" } ,
 { DC_HUMMING, "DC_HUMMING", "Focus_Ballet" } ,
 { DC_SCREAM, "DC_SCREAM", "Dazzler" } ,
 { DC_SERVICEFORYOU, "DC_SERVICEFORYOU", "Gypsy's_Kiss" } ,
 { DC_THROWARROW, "DC_THROWARROW", "Slinging_Arrow" } ,
 { DC_UGLYDANCE, "DC_UGLYDANCE", "Hip_Shaker" } ,
 { DC_WINKCHARM, "DC_WINKCHARM", "Sexy_Wink" } ,
 { GD_APPROVAL, "GD_APPROVAL", "Official_Guild_Approval" } ,
 { GD_BATTLEORDER, "GD_BATTLEORDER", "Battle_Command" } ,
 { GD_DEVELOPMENT, "GD_DEVELOPMENT", "Permanent_Development" } ,
 { GD_EMERGENCYCALL, "GD_EMERGENCYCALL", "Urgent_Call" } ,
 { GD_EXTENSION, "GD_EXTENSION", "Guild_Extension" } ,
 { GD_GLORYGUILD, "GD_GLORYGUILD", "Glory_of_Guild" } ,
 { GD_GLORYWOUNDS, "GD_GLORYWOUNDS", "Glorious_Wounds" } ,
 { GD_GUARDUP, "GD_GUARDUP", "Strengthen_Guardian" } ,
 { GD_LEADERSHIP, "GD_LEADERSHIP", "Great_Leadership" } ,
 { GD_HAWKEYES, "GD_HAWKEYES", "Sharp_Gaze" } ,
 { GD_KAFRACONTRACT, "GD_KAFRACONTRACT", "Contract_with_Kafra" } ,
 { GD_REGENERATION, "GD_REGENERATION", "Regeneration" } ,
 { GD_RESTORE, "GD_RESTORE", "Restoration" } ,
 { GD_SOULCOLD, "GD_SOULCOLD", "Cold_Heart" } ,
 { HP_ASSUMPTIO, "HP_ASSUMPTIO", "Assumptio" } ,
 { HP_BASILICA, "HP_BASILICA", "Basilica" } ,
 { HP_MANARECHARGE, "HP_MANARECHARGE", "Mana_Recharge" } ,
 { HP_MEDITATIO, "HP_MEDITATIO", "Meditatio" } ,
 { HT_ANKLESNARE, "HT_ANKLESNARE", "Ankle_Snare" } ,
 { HT_BEASTBANE, "HT_BEASTBANE", "Beast_Bane" } ,
 { HT_BLASTMINE, "HT_BLASTMINE", "Blast_Mine" } ,
 { HT_BLITZBEAT, "HT_BLITZBEAT", "Blitz_Beat" } ,
 { HT_CLAYMORETRAP, "HT_CLAYMORETRAP", "Claymore_Trap" } ,
 { HT_DETECTING, "HT_DETECTING", "Detect" } ,
 { HT_FALCON, "HT_FALCON", "Falconry_Mastery" } ,
 { HT_FLASHER, "HT_FLASHER", "Flasher" } ,
 { HT_FREEZINGTRAP, "HT_FREEZINGTRAP", "Freezing_Trap" } ,
 { HT_LANDMINE, "HT_LANDMINE", "Land_Mine" } ,
 { HT_PHANTASMIC, "HT_PHANTASMIC", "Phantasmic_Arrow" } ,
 { HT_POWER, "HT_POWER", "Beast_Strafing" } ,
 { HT_REMOVETRAP, "HT_REMOVETRAP", "Remove_Trap" } ,
 { HT_SANDMAN, "HT_SANDMAN", "Sandman" } ,
 { HT_SHOCKWAVE, "HT_SHOCKWAVE", "Shockwave_Trap" } ,
 { HT_SKIDTRAP, "HT_SKIDTRAP", "Skid_Trap" } ,
 { HT_SPRINGTRAP, "HT_SPRINGTRAP", "Spring_Trap" } ,
 { HT_STEELCROW, "HT_STEELCROW", "Steel_Crow" } ,
 { HT_TALKIEBOX, "HT_TALKIEBOX", "Talkie_Box" } ,
 { HW_GANBANTEIN, "HW_GANBANTEIN", "Ganbantein" } ,
 { HW_GRAVITATION, "HW_GRAVITATION", "Gravitation_Field" } ,
 { HW_MAGICCRASHER, "HW_MAGICCRASHER", "Stave_Crasher" } ,
 { HW_MAGICPOWER, "HW_MAGICPOWER", "Mystical_Amplification" } ,
 { HW_NAPALMVULCAN, "HW_NAPALMVULCAN", "Napalm_Vulcan" } ,
 { HW_SOULDRAIN, "HW_SOULDRAIN", "Soul_Drain" } ,
 { ITM_TOMAHAWK, "ITM_TOMAHAWK", "Tomahawk_Throwing" } ,
 { KN_AUTOCOUNTER, "KN_AUTOCOUNTER", "Counter_Attack" } ,
 { KN_BOWLINGBASH, "KN_BOWLINGBASH", "Bowling_Bash" } ,
 { KN_BRANDISHSPEAR, "KN_BRANDISHSPEAR", "Brandish_Spear" } ,
 { KN_CAVALIERMASTERY, "KN_CAVALIERMASTERY", "Cavalier_Mastery" } ,
 { KN_CHARGEATK, "KN_CHARGEATK", "Charge_Attack" } ,
 { KN_ONEHAND, "KN_ONEHAND", "Onehand_Quicken" } ,
 { KN_PIERCE, "KN_PIERCE", "Pierce" } ,
 { KN_RIDING, "KN_RIDING", "Peco_Peco_Ride" } ,
 { KN_SPEARBOOMERANG, "KN_SPEARBOOMERANG", "Spear_Boomerang" } ,
 { KN_SPEARMASTERY, "KN_SPEARMASTERY", "Spear_Mastery" } ,
 { KN_SPEARSTAB, "KN_SPEARSTAB", "Spear_Stab" } ,
 { KN_TWOHANDQUICKEN, "KN_TWOHANDQUICKEN", "Twohand_Quicken" } ,
 { LK_AURABLADE, "LK_AURABLADE", "Aura_Blade" } ,
 { LK_BERSERK, "LK_BERSERK", "Frenzy" } ,
 { LK_CONCENTRATION, "LK_CONCENTRATION", "Spear_Dynamo" } ,
 { LK_HEADCRUSH, "LK_HEADCRUSH", "Traumatic_Blow" } ,
 { LK_JOINTBEAT, "LK_JOINTBEAT", "Vital_Strike" } ,
 { LK_PARRYING, "LK_PARRYING", "Parry" } ,
 { LK_SPIRALPIERCE, "LK_SPIRALPIERCE", "Clashing_Spiral" } ,
 { LK_TENSIONRELAX, "LK_TENSIONRELAX", "Relax" } ,
 { MC_CARTREVOLUTION, "MC_CARTREVOLUTION", "Cart_Revolution" } ,
 { MC_CHANGECART, "MC_CHANGECART", "Change_Cart" } ,
 { MC_DISCOUNT, "MC_DISCOUNT", "Discount" } ,
 { MC_IDENTIFY, "MC_IDENTIFY", "Item_Appraisal" } ,
 { MC_INCCARRY, "MC_INCCARRY", "Enlarge_Weight_Limit" } ,
 { MC_LOUD, "MC_LOUD", "Crazy_Uproar" } ,
 { MC_MAMMONITE, "MC_MAMMONITE", "Mammonite" } ,
 { MC_OVERCHARGE, "MC_OVERCHARGE", "Overcharge" } ,
 { MC_PUSHCART, "MC_PUSHCART", "Pushcart" } ,
 { MC_VENDING, "MC_VENDING", "Vending" } ,
 { MG_COLDBOLT, "MG_COLDBOLT", "Cold_Bolt" } ,
 { MG_ENERGYCOAT, "MG_ENERGYCOAT", "Energy_Coat" } ,
 { MG_FIREBALL, "MG_FIREBALL", "Fire_Ball" } ,
 { MG_FIREBOLT, "MG_FIREBOLT", "Fire_Bolt" } ,
 { MG_FIREWALL, "MG_FIREWALL", "Fire_Wall" } ,
 { MG_FROSTDIVER, "MG_FROSTDIVER", "Frost_Diver" } ,
 { MG_LIGHTNINGBOLT, "MG_LIGHTNINGBOLT", "Lightening_Bolt" } ,
 { MG_NAPALMBEAT, "MG_NAPALMBEAT", "Napalm_Beat" } ,
 { MG_SAFETYWALL, "MG_SAFETYWALL", "Safety_Wall" } ,
 { MG_SIGHT, "MG_SIGHT", "Sight" } ,
 { MG_SOULSTRIKE, "MG_SOULSTRIKE", "Soul_Strike" } ,
 { MG_SRECOVERY, "MG_SRECOVERY", "Increase_SP_Recovery" } ,
 { MG_STONECURSE, "MG_STONECURSE", "Stone_Curse" } ,
 { MG_THUNDERSTORM, "MG_THUNDERSTORM", "Thunderstorm" } ,
 { MO_ABSORBSPIRITS, "MO_ABSORBSPIRITS", "Spiritual_Sphere_Absorption" } ,
 { MO_BALKYOUNG, "MO_BALKYOUNG", "Ki_Explosion" } ,
 { MO_BLADESTOP, "MO_BLADESTOP", "Root" } ,
 { MO_BODYRELOCATION, "MO_BODYRELOCATION", "Snap" } ,
 { MO_CALLSPIRITS, "MO_CALLSPIRITS", "Summon_Spirit_Sphere" } ,
 { MO_CHAINCOMBO, "MO_CHAINCOMBO", "Raging_Quadruple_Blow" } ,
 { MO_COMBOFINISH, "MO_COMBOFINISH", "Raging_Thrust" } ,
 { MO_DODGE, "MO_DODGE", "Flee" } ,
 { MO_EXPLOSIONSPIRITS, "MO_EXPLOSIONSPIRITS", "Fury" } ,
 { MO_EXTREMITYFIST, "MO_EXTREMITYFIST", "Guillotine_Fist" } ,
 { MO_FINGEROFFENSIVE, "MO_FINGEROFFENSIVE", "Throw_Spirit_Sphere" } ,
 { MO_INVESTIGATE, "MO_INVESTIGATE", "Occult_Impaction" } ,
 { MO_IRONHAND, "MO_IRONHAND", "Iron_Fists" } ,
 { MO_KITRANSLATION, "MO_KITRANSLATION", "Ki_Translation" } ,
 { MO_SPIRITSRECOVERY, "MO_SPIRITSRECOVERY", "Spiritual_Cadence" } ,
 { MO_STEELBODY, "MO_STEELBODY", "Mental_Strength" } ,
 { MO_TRIPLEATTACK, "MO_TRIPLEATTACK", "Raging_Trifecta_Blow" } ,
 { NPC_ATTRICHANGE, "NPC_ATTRICHANGE", "NPC_ATTRICHANGE" } ,
 { NPC_BARRIER, "NPC_BARRIER", "NPC_BARRIER" } ,
 { NPC_BLINDATTACK, "NPC_BLINDATTACK", "NPC_BLINDATTACK" } ,
 { NPC_BLOODDRAIN, "NPC_BLOODDRAIN", "NPC_BLOODDRAIN" } ,
 { NPC_CHANGEDARKNESS, "NPC_CHANGEDARKNESS", "NPC_CHANGEDARKNESS" } ,
 { NPC_CHANGEFIRE, "NPC_CHANGEFIRE", "NPC_CHANGEFIRE" } ,
 { NPC_CHANGEGROUND, "NPC_CHANGEGROUND", "NPC_CHANGEGROUND" } ,
 { NPC_CHANGEHOLY, "NPC_CHANGEHOLY", "NPC_CHANGEHOLY" } ,
 { NPC_CHANGEPOISON, "NPC_CHANGEPOISON", "NPC_CHANGEPOISON" } ,
 { NPC_CHANGETELEKINESIS, "NPC_CHANGETELEKINESIS", "NPC_CHANGETELEKINESIS" } ,
 { NPC_CHANGEWATER, "NPC_CHANGEWATER", "NPC_CHANGEWATER" } ,
 { NPC_CHANGEWIND, "NPC_CHANGEWIND", "NPC_CHANGEWIND" } ,
 { NPC_COMBOATTACK, "NPC_COMBOATTACK", "NPC_COMBOATTACK" } ,
 { NPC_CRITICALSLASH, "NPC_CRITICALSLASH", "NPC_CRITICALSLASH" } ,
 { NPC_CURSEATTACK, "NPC_CURSEATTACK", "NPC_CURSEATTACK" } ,
 { NPC_DARKBLESSING, "NPC_DARKBLESSING", "NPC_DARKBLESSING" } ,
 { NPC_DARKBREATH, "NPC_DARKBREATH", "NPC_DARKBREATH" } ,
 { NPC_DARKCROSS, "NPC_DARKCROSS", "NPC_DARKCROSS" } ,
 { NPC_DARKNESSATTACK, "NPC_DARKNESSATTACK", "NPC_DARKNESSATTACK" } ,
 { NPC_DEFENDER, "NPC_DEFENDER", "NPC_DEFENDER" } ,
 { NPC_EMOTION, "NPC_EMOTION", "NPC_EMOTION" } ,
 { NPC_EMOTION_ON, "NPC_EMOTION_ON", "NPC_EMOTION_ON" } ,
 { NPC_ENERGYDRAIN, "NPC_ENERGYDRAIN", "NPC_ENERGYDRAIN" } ,
 { NPC_FIREATTACK, "NPC_FIREATTACK", "NPC_FIREATTACK" } ,
 { NPC_GROUNDATTACK, "NPC_GROUNDATTACK", "NPC_GROUNDATTACK" } ,
 { NPC_GUIDEDATTACK, "NPC_GUIDEDATTACK", "NPC_GUIDEDATTACK" } ,
 { NPC_HALLUCINATION, "NPC_HALLUCINATION", "NPC_HALLUCINATION" } ,
 { NPC_HOLYATTACK, "NPC_HOLYATTACK", "NPC_HOLYATTACK" } ,
 { NPC_KEEPING, "NPC_KEEPING", "NPC_KEEPING" } ,
 { NPC_LICK, "NPC_LICK", "NPC_LICK" } ,
 { NPC_MAGICALATTACK, "NPC_MAGICALATTACK", "NPC_MAGICALATTACK" } ,
 { NPC_MENTALBREAKER, "NPC_MENTALBREAKER", "NPC_MENTALBREAKER" } ,
 { NPC_METAMORPHOSIS, "NPC_METAMORPHOSIS", "NPC_METAMORPHOSIS" } ,
 { NPC_PETRIFYATTACK, "NPC_PETRIFYATTACK", "NPC_PETRIFYATTACK" } ,
 { NPC_PIERCINGATT, "NPC_PIERCINGATT", "NPC_PIERCINGATT" } ,
 { NPC_POISON, "NPC_POISON", "NPC_POISON" } ,
 { NPC_POISONATTACK, "NPC_POISONATTACK", "NPC_POISONATTACK" } ,
 { NPC_PROVOCATION, "NPC_PROVOCATION", "NPC_PROVOCATION" } ,
 { NPC_RANDOMATTACK, "NPC_RANDOMATTACK", "NPC_RANDOMATTACK" } ,
 { NPC_RANGEATTACK, "NPC_RANGEATTACK", "NPC_RANGEATTACK" } ,
 { NPC_REBIRTH, "NPC_REBIRTH", "NPC_REBIRTH" } ,
 { NPC_SELFDESTRUCTION, "NPC_SELFDESTRUCTION", "Kabooooom!" } ,
 { NPC_SILENCEATTACK, "NPC_SILENCEATTACK", "NPC_SILENCEATTACK" } ,
 { NPC_SLEEPATTACK, "NPC_SLEEPATTACK", "NPC_SLEEPATTACK" } ,
 { NPC_SMOKING, "NPC_SMOKING", "NPC_SMOKING" } ,
 { NPC_SPLASHATTACK, "NPC_SPLASHATTACK", "NPC_SPLASHATTACK" } ,
 { NPC_STUNATTACK, "NPC_STUNATTACK", "NPC_STUNATTACK" } ,
 { NPC_SUICIDE, "NPC_SUICIDE", "NPC_SUICIDE" } ,
 { NPC_SUMMONMONSTER, "NPC_SUMMONMONSTER", "NPC_SUMMONMONSTER" } ,
 { NPC_SUMMONSLAVE, "NPC_SUMMONSLAVE", "NPC_SUMMONSLAVE" } ,
 { NPC_TELEKINESISATTACK, "NPC_TELEKINESISATTACK", "NPC_TELEKINESISATTACK" } ,
 { NPC_TRANSFORMATION, "NPC_TRANSFORMATION", "NPC_TRANSFORMATION" } ,
 { NPC_WATERATTACK, "NPC_WATERATTACK", "NPC_WATERATTACK" } ,
 { NPC_WINDATTACK, "NPC_WINDATTACK", "NPC_WINDATTACK" } ,
 { NV_BASIC, "NV_BASIC", "Basic_Skill" } ,
 { NV_FIRSTAID, "NV_FIRSTAID", "First Aid" } ,
 { NV_TRICKDEAD, "NV_TRICKDEAD", "Play_Dead" } ,
 { PA_GOSPEL, "PA_GOSPEL", "Battle_Chant" } ,
 { PA_PRESSURE, "PA_PRESSURE", "Gloria_Domini" } ,
 { PA_SACRIFICE, "PA_SACRIFICE", "Martyr's_Reckoning" } ,
 { PA_SHIELDCHAIN, "PA_SHIELDCHAIN", "Shield_Chain" } ,
 { PF_DOUBLECASTING, "PF_DOUBLECASTING", "Double_Casting" } ,
 { PF_FOGWALL, "PF_FOGWALL", "Blinding_Mist" } ,
 { PF_HPCONVERSION, "PF_HPCONVERSION", "Indulge" } ,
 { PF_MEMORIZE, "PF_MEMORIZE", "Foresight" } ,
 { PF_MINDBREAKER, "PF_MINDBREAKER", "Mind_Breaker" } ,
 { PF_SOULBURN, "PF_SOULBURN", "Soul_Siphon" } ,
 { PF_SOULCHANGE, "PF_SOULCHANGE", "Soul_Exhale" } ,
 { PF_SPIDERWEB, "PF_SPIDERWEB", "Fiber_Lock" } ,
 { PR_ASPERSIO, "PR_ASPERSIO", "Aspersio" } ,
 { PR_BENEDICTIO, "PR_BENEDICTIO", "B.S_Sacramenti" } ,
 { PR_GLORIA, "PR_GLORIA", "Gloria" } ,
 { PR_IMPOSITIO, "PR_IMPOSITIO", "Impositio_Manus" } ,
 { PR_KYRIE, "PR_KYRIE", "Kyrie_Eleison" } ,
 { PR_LEXAETERNA, "PR_LEXAETERNA", "Lex_Aeterna" } ,
 { PR_LEXDIVINA, "PR_LEXDIVINA", "Lex_Divina" } ,
 { PR_MACEMASTERY, "PR_MACEMASTERY", "Mace_Mastery" } ,
 { PR_MAGNIFICAT, "PR_MAGNIFICAT", "Magnificat" } ,
 { PR_MAGNUS, "PR_MAGNUS", "Magnus_Exorcismus" } ,
 { PR_REDEMPTIO, "PR_REDEMPTIO", "Redemptio" } ,
 { PR_SANCTUARY, "PR_SANCTUARY", "Sanctuary" } ,
 { PR_SLOWPOISON, "PR_SLOWPOISON", "Slow_Poison" } ,
 { PR_STRECOVERY, "PR_STRECOVERY", "Status_Recovery" } ,
 { PR_SUFFRAGIUM, "PR_SUFFRAGIUM", "Suffragium" } ,
 { PR_TURNUNDEAD, "PR_TURNUNDEAD", "Turn_Undead" } ,
 { RG_BACKSTAP, "RG_BACKSTAP", "Back_Stab" } ,
 { RG_CLEANER, "RG_CLEANER", "Remover" } ,
 { RG_CLOSECONFINE, "RG_CLOSECONFINE", "Close_Confine"} ,
 { RG_COMPULSION, "RG_COMPULSION", "Haggle" } ,
 { RG_FLAGGRAFFITI, "RG_FLAGGRAFFITI", "Piece" } ,
 { RG_GANGSTER, "RG_GANGSTER", "Slyness" } ,
 { RG_GRAFFITI, "RG_GRAFFITI", "Scribble" } ,
 { RG_INTIMIDATE, "RG_INTIMIDATE", "Snatch" } ,
 { RG_PLAGIARISM, "RG_PLAGIARISM", "Intimidate" } ,
 { RG_RAID, "RG_RAID", "Sightless_Mind" } ,
 { RG_SNATCHER, "RG_SNATCHER", "Gank" } ,
 { RG_STEALCOIN, "RG_STEALCOIN", "Mug" } ,
 { RG_STRIPARMOR, "RG_STRIPARMOR", "Divest_Armor" } ,
 { RG_STRIPHELM, "RG_STRIPHELM", "Divest_Helm" } ,
 { RG_STRIPSHIELD, "RG_STRIPSHIELD", "Divest_Shield" } ,
 { RG_STRIPWEAPON, "RG_STRIPWEAPON", "Divest_Weapon" } ,
 { RG_TUNNELDRIVE, "RG_TUNNELDRIVE", "Stalk" } ,
 { SA_ABRACADABRA, "SA_ABRACADABRA", "Hocus-pocus" } ,
 { SA_ADVANCEDBOOK, "SA_ADVANCEDBOOK", "Advanced_Book" } ,
 { SA_AUTOSPELL, "SA_AUTOSPELL", "Hindsight" } ,
 { SA_CASTCANCEL, "SA_CASTCANCEL", "Cast_Cancel" } ,
 { SA_CLASSCHANGE, "SA_CLASSCHANGE", "Class_Change" } ,
 { SA_CREATECON, "SA_CREATECON", "Create_Elemental_Converter" } ,
 { SA_COMA, "SA_COMA", "Coma" } ,
 { SA_DEATH, "SA_DEATH", "Grim_Reaper" } ,
 { SA_DELUGE, "SA_DELUGE", "Deluge" } ,
 { SA_DISPELL, "SA_DISPELL", "Dispell" } ,
 { SA_DRAGONOLOGY, "SA_DRAGONOLOGY", "Dragonology" } ,
 { SA_ELEMENTFIRE, "SA_ELEMENTFIRE", "Elemental_Change_Fire" } ,
 { SA_ELEMENTGROUND, "SA_ELEMENTGROUND", "Elemental_Change_Earth" } ,
 { SA_ELEMENTWATER, "SA_ELEMENTWATER", "Elemental_Change_Water" } ,
 { SA_ELEMENTWIND, "SA_ELEMENTWIND", "Elemental_Change_Wind" } ,
 { SA_FLAMELAUNCHER, "SA_FLAMELAUNCHER", "Endow_Blaze" } ,
 { SA_FORTUNE, "SA_FORTUNE", "Gold_Digger" } ,
 { SA_FREECAST, "SA_FREECAST", "Free_Cast" } ,
 { SA_FROSTWEAPON, "SA_FROSTWEAPON", "Endow_Tsunami" } ,
 { SA_FULLRECOVERY, "SA_FULLRECOVERY", "Rejuvenation" } ,
 { SA_GRAVITY, "SA_GRAVITY", "Gravity" } ,
 { SA_INSTANTDEATH, "SA_INSTANTDEATH", "Suicide" } ,
 { SA_LANDPROTECTOR, "SA_LANDPROTECTOR", "Magnetic_Earth" } ,
 { SA_LEVELUP, "SA_LEVELUP", "Leveling" } ,
 { SA_LIGHTNINGLOADER, "SA_LIGHTNINGLOADER", "Endow_Tornado" } ,
 { SA_MAGICROD, "SA_MAGICROD", "Magic_Rod" } ,
 { SA_MONOCELL, "SA_MONOCELL", "Mono_Cell" } ,
 { SA_QUESTION, "SA_QUESTION", "Questioning" } ,
 { SA_REVERSEORCISH, "SA_REVERSEORCISH", "Grampus_Morph" } ,
 { SA_SEISMICWEAPON, "SA_SEISMICWEAPON", "Endow_Quake" } ,
 { SA_SPELLBREAKER, "SA_SPELLBREAKER", "Spell_Breaker" } ,
 { SA_SUMMONMONSTER, "SA_SUMMONMONSTER", "Monster_Chant" } ,
 { SA_TAMINGMONSTER, "SA_TAMINGMONSTER", "Beastly_Hypnosis" } ,
 { SA_VIOLENTGALE, "SA_VIOLENTGALE", "Whirlwind" } ,
 { SA_VOLCANO, "SA_VOLCANO", "Volcano" } ,
 { SG_DEVIL, "SG_DEVIL", "Devil_of_the_Sun,_Moon_and_Stars" } ,
 { SG_FEEL, "SG_FEEL", "Feeling_of_the_Sun,_Moon_and_Star" } ,
 { SG_FRIEND, "SG_FRIEND", "Companion_of_the_Sun_and_Moon" } ,
 { SG_FUSION, "SG_FUSION", "Union_of_the_Sun,_Moon_and_Stars" } ,
 { SG_HATE, "SG_HATE", "Hatred_of_the_Sun,_Moon_and_Stars" } ,
 { SG_KNOWLEDGE, "SG_KNOWLEDGE", "Knowledge_of_the_Sun,_Moon_and_Stars" } ,
 { SG_MOON_ANGER, "SG_MOON_ANGER", "Fury_of_the_Moon" } ,
 { SG_MOON_BLESS, "SG_MOON_BLESS", "Bless_of_the_Moon" } ,
 { SG_MOON_COMFORT, "SG_MOON_COMFORT", "Comfort_of_the_Moon" } ,
 { SG_MOON_WARM, "SG_MOON_WARM", "Warmth_of_the_Moon" } ,
 { SG_STAR_ANGER, "SG_STAR_ANGER", "Fury_of_the_Stars" } ,
 { SG_STAR_BLESS, "SG_STAR_BLESS", "Bless_of_the_Stars" } ,
 { SG_STAR_COMFORT, "SG_STAR_COMFORT", "Comfort_of_the_Stars" } ,
 { SG_STAR_WARM, "SG_STAR_WARM", "Warmth_of_the_Stars" } ,
 { SG_SUN_ANGER, "SG_SUN_ANGER", "Fury_of_the_Sun" } ,
 { SG_SUN_BLESS, "SG_SUN_BLESS", "Bless_of_the_Sun" } ,
 { SG_SUN_COMFORT, "SG_SUN_COMFORT", "Comfort_of_the_Sun" } ,
 { SG_SUN_WARM, "SG_SUN_WARM", "Warmth_of_the_Sun" } ,
 { SL_ALCHEMIST, "SL_ALCHEMIST", "Spirit_of_Alchemist" } ,
 { SL_ASSASIN, "SL_ASSASIN", "Spirit_of_Assassin" } ,
 { SL_BARDDANCER, "SL_BARDDANCER", "Spirit_of_Bard_and_Dancer" } ,
 { SL_BLACKSMITH, "SL_BLACKSMITH", "Spirit_of_Blacksmith" } ,
 { SL_CRUSADER, "SL_CRUSADER", "Spirit_of_Crusader" } ,
 { SL_HIGH, "SL_HIGH", "Spirit_of_Advanced_1st_Class" } ,
 { SL_HUNTER, "SL_HUNTER", "Spirit_of_Hunter" } ,
 { SL_KAAHI, "SL_KAAHI", "Kaahi" } ,
 { SL_KAINA, "SL_KAINA", "Kaina" } ,
 { SL_KAITE, "SL_KAITE", "Kaite" } ,
 { SL_KAIZEL, "SL_KAIZEL", "Kaizel" } ,
 { SL_KAUPE, "SL_KAUPE", "Kaupe" } ,
 { SL_KNIGHT, "SL_KNIGHT", "Spirit_of_Knight" } ,
 { SL_MONK, "SL_MONK", "Spirit_of_Monk" } ,
 { SL_PRIEST, "SL_PRIEST", "Spirit_of_Priest" } ,
 { SL_ROGUE, "SL_ROGUE", "Spirit_of_Rogue" } ,
 { SL_SAGE, "SL_SAGE", "Spirit_of_Sage" } ,
 { SL_SKA, "SL_SKA", "Eska" } ,
 { SL_SKE, "SL_SKE", "Eske" } ,
 { SL_SMA, "SL_SMA", "Esma" } ,
 { SL_SOULLINKER, "SL_SOULLINKER", "Spirit_of_Soul_Linker" } ,
 { SL_STAR, "SL_STAR", "Spirit_of_Stars" } ,
 { SL_STIN, "SL_STIN", "Estin" } ,
 { SL_STUN, "SL_STUN", "Estun" } ,
 { SL_SUPERNOVICE, "SL_SUPERNOVICE", "Spirit_of_Super_Novice" } ,
 { SL_SWOO, "SL_SWOO", "Eswoo" } ,
 { SL_WIZARD, "SL_WIZARD", "Spirit_of_Wizard" } ,
 { SM_AUTOBERSERK, "SM_AUTOBERSERK", "Berserk" } ,
 { SM_BASH, "SM_BASH", "Bash" } ,
 { SM_ENDURE, "SM_ENDURE", "Endure" } ,
 { SM_FATALBLOW, "SM_FATALBLOW", "Fatal_Blow" } ,
 { SM_MAGNUM, "SM_MAGNUM", "Magnum_Break" } ,
 { SM_MOVINGRECOVERY, "SM_MOVINGRECOVERY", "HP_Recovery_While_Moving" } ,
 { SM_PROVOKE, "SM_PROVOKE", "Provoke" } ,
 { SM_RECOVERY, "SM_RECOVERY", "Increase_HP_Recovery" } ,
 { SM_SWORD, "SM_SWORD", "Sword_Mastery" } ,
 { SM_TWOHAND, "SM_TWOHAND", "Two-Handed_Sword_Mastery" } ,
 { SN_FALCONASSAULT, "SN_FALCONASSAULT", "Falcon_Assault" } ,
 { SN_SHARPSHOOTING, "SN_SHARPSHOOTING", "Focused_Arrow_Strike" } ,
 { SN_SIGHT, "SN_SIGHT", "Falcon_Eyes" } ,
 { SN_WINDWALK, "SN_WINDWALK", "Wind_Walker" } ,
 { ST_CHASEWALK, "ST_CHASEWALK", "Stealth" } ,
 { ST_REJECTSWORD, "ST_REJECTSWORD", "Counter_Instinct" } ,
 { ST_PRESERVE, "ST_PRESERVE", "Preserve" } ,
 { ST_FULLSTRIP, "ST_FULLSTRIP", "Full_Divestment" } ,
 { TF_BACKSLIDING, "TF_BACKSLIDING", "Back_Slide" } ,
 { TF_DETOXIFY, "TF_DETOXIFY", "Detoxify" } ,
 { TF_DOUBLE, "TF_DOUBLE", "Double_Attack" } ,
 { TF_HIDING, "TF_HIDING", "Hiding" } ,
 { TF_MISS, "TF_MISS", "Improve_Dodge" } ,
 { TF_PICKSTONE, "TF_PICKSTONE", "Find_Stone" } ,
 { TF_POISON, "TF_POISON", "Envenom" } ,
 { TF_SPRINKLESAND, "TF_SPRINKLESAND", "Sand_Attack" } ,
 { TF_STEAL, "TF_STEAL", "Steal" } ,
 { TF_THROWSTONE, "TF_THROWSTONE", "Stone_Fling" } ,
 { TK_COUNTER, "TK_COUNTER", "Spin_Kick" } ,
 { TK_DODGE, "TK_DODGE", "Sprint" } ,
 { TK_DOWNKICK, "TK_DOWNKICK", "Heel_Drop" } ,
 { TK_HIGHJUMP, "TK_HIGHJUMP", "Taekwon_Jump" } ,
 { TK_HPTIME, "TK_HPTIME", "Peaceful_Break" } ,
 { TK_JUMPKICK, "TK_JUMPKICK", "Flying_Kick" } ,
 { TK_MISSION,  "TK_MISSION", "Mission" } ,
 { TK_POWER, "TK_POWER", "Kihop" } ,
 { TK_READYCOUNTER, "TK_READYCOUNTER", "Spin_Kick_Stance" } ,
 { TK_READYDOWN, "TK_READYDOWN", "Heel_Drop_Stance" } ,
 { TK_READYSTORM, "TK_READYSTORM", "Tornado_Stance" } ,
 { TK_READYTURN, "TK_READYTURN", "Roundhouse_Stance" } ,
 { TK_RUN, "TK_RUN", "Sprint" } ,
 { TK_SEVENWIND, "TK_SEVENWIND", "Mild_Wind" } ,
 { TK_SPTIME, "TK_SPTIME", "Happy_Break" } ,
 { TK_STORMKICK, "TK_STORMKICK", "Storm_Kick" } ,
 { TK_TURNKICK, "TK_TURNKICK", "Turn_Kick" } ,
 { WE_BABY, "WE_BABY", "Mom,_Dad,_I_love_you!" } ,
 { WE_CALLBABY, "WE_CALLBABY", "Come_to_me,_honey~" } ,
 { WE_CALLPARENT, "WE_CALLPARENT", "Mom,_Dad,_I_miss_you!" } ,
 { WE_CALLPARTNER, "WE_CALLPARTNER", "Romantic_Rendezvous" } ,
 { WE_FEMALE, "WE_FEMALE", "Loving_Touch" } ,
 { WE_MALE, "WE_MALE", "Undying_Love" } ,
 { WS_CARTBOOST, "WS_CARTBOOST", "Cart_Boost" } ,
 { WS_CARTTERMINATION, "WS_CARTTERMINATION", "Cart_Termination" } ,
 { WS_CREATECOIN, "WS_CREATECOIN", "Coin_Craft" } ,
 { WS_CREATENUGGET, "WS_CREATENUGGET", "Nugget_Craft" } ,
 { WS_MELTDOWN, "WS_MELTDOWN", "Shattering_Strike" } ,
 { WS_OVERTHRUSTMAX, "WS_OVERTHRUSTMAX", "Max_Power-Thust" } ,
 { WS_SYSTEMCREATE, "WS_SYSTEMCREATE", "Auto_Attacking_Machine_Craft" } ,
 { WS_WEAPONREFINE, "WS_WEAPONREFINE", "Weapon_Refine" } ,
 { WZ_EARTHSPIKE, "WZ_EARTHSPIKE", "Earth_Spike" } ,
 { WZ_ESTIMATION, "WZ_ESTIMATION", "Sense" } ,
 { WZ_FIREPILLAR, "WZ_FIREPILLAR", "Fire_Pillar" } ,
 { WZ_FROSTNOVA, "WZ_FROSTNOVA", "Frost_Nova" } ,
 { WZ_HEAVENDRIVE, "WZ_HEAVENDRIVE", "Heaven's_Drive" } ,
 { WZ_ICEWALL, "WZ_ICEWALL", "Ice_Wall" } ,
 { WZ_JUPITEL, "WZ_JUPITEL", "Jupitel_Thunder" } ,
 { WZ_METEOR, "WZ_METEOR", "Meteor_Storm" } ,
 { WZ_QUAGMIRE, "WZ_QUAGMIRE", "Quagmire" } ,
 { WZ_SIGHTBLASTER, "WZ_SIGHTBLASTER", "Sight_Blaster" } ,
 { WZ_SIGHTRASHER, "WZ_SIGHTRASHER", "Sightrasher" } ,
 { WZ_STORMGUST, "WZ_STORMGUST", "Storm_Gust" } ,
 { WZ_VERMILION, "WZ_VERMILION", "Lord_of_Vermilion" } ,
 { WZ_WATERBALL, "WZ_WATERBALL", "Water_Ball" } ,
 { 0, "UNKNOWN_SKILL", "Unknown_Skill" }
};

static const int dirx[8]={0,-1,-1,-1,0,1,1,1};
static const int diry[8]={1,1,0,-1,-1,-1,0,1};

static int rdamage;

/* スキルデ?タベ?ス */
struct skill_db skill_db[MAX_SKILL_DB];

/* アイテム?�?ｬデ?タベ?ス */
struct skill_produce_db skill_produce_db[MAX_SKILL_PRODUCE_DB];

/* 矢?�?ｬスキルデ?タベ?ス */
struct skill_arrow_db skill_arrow_db[MAX_SKILL_ARROW_DB];

/* アブラカダブラ?動スキルデ?タベ?ス */
struct skill_abra_db skill_abra_db[MAX_SKILL_ABRA_DB];

// macros to check for out of bounds errors [celest]
// i: Skill ID, l: Skill Level, var: Value to return after checking
// for values that don't require level just put a one (putting 0 will trigger return 0; instead
// for values that might need to use a different function just skill_chk would suffice.
#define skill_chk(i, l) \
	if (i >= 10000 && i < 10015) {i -= 9500;} \
	if (i < 1 || i > MAX_SKILL_DB) {return 0;} \
	if (l <= 0 || l > MAX_SKILL_LEVEL) {return 0;}
#define skill_get(var, i, l) \
	{ skill_chk(i, l); return var; }

// Skill DB
int	skill_get_hit( int id ){ skill_get (skill_db[id].hit, id, 1); }
int	skill_get_inf( int id ){ skill_chk (id, 1); return (id < 500 || id > 1000) ? skill_db[id].inf : guild_skill_get_inf(id); }
int	skill_get_pl( int id ){ skill_get (skill_db[id].pl, id, 1); }
int	skill_get_nk( int id ){ skill_get (skill_db[id].nk, id, 1); }
int	skill_get_max( int id ){ skill_chk (id, 1); return (id < 500 || id > 1000) ? skill_db[id].max : guild_skill_get_max(id); }
int	skill_get_range( int id , int lv ){ skill_chk (id, lv); return (id < 500 || id > 1000) ? skill_db[id].range[lv-1] : 0; }
int	skill_get_hp( int id ,int lv ){ skill_get (skill_db[id].hp[lv-1], id, lv); }
int	skill_get_sp( int id ,int lv ){ skill_get (skill_db[id].sp[lv-1], id, lv); }
int	skill_get_zeny( int id ,int lv ){ skill_get (skill_db[id].zeny[lv-1], id, lv); }
int	skill_get_num( int id ,int lv ){ skill_get (skill_db[id].num[lv-1], id, lv); }
int	skill_get_cast( int id ,int lv ){ skill_get (skill_db[id].cast[lv-1], id, lv); }
int	skill_get_delay( int id ,int lv ){ skill_get (skill_db[id].delay[lv-1], id, lv); }
int	skill_get_time( int id ,int lv ){ skill_get (skill_db[id].upkeep_time[lv-1], id, lv); }
int	skill_get_time2( int id ,int lv ){ skill_get (skill_db[id].upkeep_time2[lv-1], id, lv); }
int	skill_get_castdef( int id ){ skill_get (skill_db[id].cast_def_rate, id, 1); }
int	skill_get_weapontype( int id ){ skill_get (skill_db[id].weapon, id, 1); }
int	skill_get_inf2( int id ){ skill_get (skill_db[id].inf2, id, 1); }
int	skill_get_castcancel( int id ){ skill_get (skill_db[id].castcancel, id, 1); }
int	skill_get_maxcount( int id ){ skill_get (skill_db[id].maxcount, id, 1); }
int	skill_get_blewcount( int id ,int lv ){ skill_get (skill_db[id].blewcount[lv-1], id, lv); }
int	skill_get_mhp( int id ,int lv ){ skill_get (skill_db[id].mhp[lv-1], id, lv); }
int	skill_get_castnodex( int id ,int lv ){ skill_get (skill_db[id].castnodex[lv-1], id, lv); }
int	skill_get_delaynodex( int id ,int lv ){ skill_get (skill_db[id].delaynodex[lv-1], id, lv); }
int	skill_get_delaynowalk( int id ,int lv ){ skill_get (skill_db[id].delaynowalk[lv-1], id, lv); }
int	skill_get_nocast ( int id ){ skill_get (skill_db[id].nocast, id, 1); }
int	skill_get_type( int id ){ skill_get (skill_db[id].skill_type, id, 1); }
int	skill_get_unit_id ( int id, int flag ){ skill_get (skill_db[id].unit_id[flag], id, 1); }
int	skill_get_unit_layout_type( int id ,int lv ){ skill_get (skill_db[id].unit_layout_type[lv-1], id, lv); }
int	skill_get_unit_interval( int id ){ skill_get (skill_db[id].unit_interval, id, 1); }
int	skill_get_unit_range( int id ){ skill_get (skill_db[id].unit_range, id, 1); }
int	skill_get_unit_target( int id ){ skill_get (skill_db[id].unit_target, id, 1); }
int	skill_get_unit_flag( int id ){ skill_get (skill_db[id].unit_flag, id, 1); }
const char*	skill_get_name( int id ){ 
	if (id >= 10000 && id < 10015) id -= 9500;
	if (id < 1 || id > MAX_SKILL_DB) return "UNKNOWN_SKILL"; //Can't use skill_chk because we return a string.
	return skill_db[id].name; 
}

int skill_tree_get_max(int id, int b_class){
	int i, skillid;
	for(i=0;(skillid=skill_tree[b_class][i].id)>0;i++)
		if (id == skillid) return skill_tree[b_class][i].max;
	return skill_get_max (id);
}

/* プ�?トタイプ */
int skill_check_condition( struct map_session_data *sd,int type);
int skill_castend_damage_id( struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag );
int skill_frostjoke_scream(struct block_list *bl,va_list ap);
int status_change_timer_sub(struct block_list *bl, va_list ap);
int skill_attack_area(struct block_list *bl,va_list ap);
int skill_clear_element_field(struct block_list *bl);
struct skill_unit_group *skill_locate_element_field(struct block_list *bl); // [Skotlex]
int skill_graffitiremover(struct block_list *bl, va_list ap); // [Valaris]
int skill_greed(struct block_list *bl, va_list ap);
int skill_landprotector(struct block_list *bl, va_list ap);
int skill_ganbatein(struct block_list *bl, va_list ap);
int skill_trap_splash(struct block_list *bl, va_list ap);
int skill_count_target(struct block_list *bl, va_list ap);
struct skill_unit_group_tickset *skill_unitgrouptickset_search(struct block_list *bl,struct skill_unit_group *sg,int tick);
static int skill_unit_onplace(struct skill_unit *src,struct block_list *bl,unsigned int tick);
static int skill_unit_onleft(int skill_id, struct block_list *bl,unsigned int tick);
static int skill_unit_checktarget(int skill_id, struct block_list *bl);
int skill_unit_effect(struct block_list *bl,va_list ap);
int skill_castend_delay (struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag);
static void skill_moonlit(struct block_list* src, struct block_list* partner, int skilllv);
static int skill_check_pc_partner(struct map_session_data *sd, int skill_id, int* skill_lv, int range, int cast_flag);

int enchant_eff[5] = { 10, 14, 17, 19, 20 };
int deluge_eff[5] = { 5, 9, 12, 14, 15 };

//Returns actual skill range taking into account attack range and AC_OWL [Skotlex]
int skill_get_range2(struct block_list *bl, int id, int lv) {
	int range = skill_get_range(id, lv);
	if(range < 0) {
		if (battle_config.use_weapon_skill_range)
			return status_get_range(bl);
		range *=-1;
	}
	if (bl->type == BL_PC && //TODO: Find a way better than hardcoding the list of skills affected by AC_VULTURE.
		(id == AC_SHOWER || id == AC_DOUBLE || id == HT_BLITZBEAT || id == SN_FALCONASSAULT || id == SN_SHARPSHOOTING))
		range += pc_checkskill((struct map_session_data *)bl, AC_VULTURE);
	return range;
}

// Making plagirize check its own function [Aru]
int can_copy(struct map_session_data *sd, int skillid)
{
	// NPC Skills, never ok to copy
	if(skillid >= NPC_PIERCINGATT && skillid <= NPC_SUMMONMONSTER)
		return 0;
	if(skillid >= NPC_RANDOMMOVE && skillid <= NPC_RUN)
		return 0;
	if(skillid >= WE_BABY && skillid <= NPC_EMOTION_ON)
		return 0;

	// High-class skills
	if(skillid >= LK_AURABLADE)
	{
		if(battle_config.copyskill_restrict == 2)
			return 0;
		else if(battle_config.copyskill_restrict)
			return (sd->status.class_ == JOB_STALKER);
	}

	return 1;
}

// [MouseJstr] - skill ok to cast? and when?
int skillnotok(int skillid, struct map_session_data *sd)
{	
	nullpo_retr (1, sd);
	//if (sd == 0)
		//return 0; 
		//return 1;
	// I think it was meant to be "no skills allowed when not a valid sd"
	
	if (!(skillid >= 10000 && skillid < 10015))
		if ((skillid > MAX_SKILL) || (skillid < 0))
			return 1;

	{
		int i = skillid;
		if (i >= 10000 && i < 10015)
			i -= 9500;
		if (sd->blockskill[i] > 0)
			return 1;
	}

	if (pc_isGM(sd) >= 20)
		return 0;  // gm's can do anything damn thing they want

	// Check skill restrictions [Celest]
	if(!map_flag_vs(sd->bl.m) && skill_get_nocast (skillid) & 1)
		return 1;
	if(map[sd->bl.m].flag.pvp && skill_get_nocast (skillid) & 2)
		return 1;
	if(map_flag_gvg(sd->bl.m) && skill_get_nocast (skillid) & 4)
		return 1;
	if (agit_flag && skill_get_nocast (skillid) & 8)
		return 1;
	if (battle_config.pk_mode && !map[sd->bl.m].flag.nopvp && skill_get_nocast (skillid) & 16)
		return 1;

	switch (skillid) {
		case AL_WARP:
		case AL_TELEPORT:
		case MC_VENDING:
		case MC_IDENTIFY:
			return 0; // always allowed
		default:
			return (map[sd->bl.m].flag.noskill);
	}
}


static int distance(int x0,int y0,int x1,int y1)
{
	int dx,dy;

	dx = abs(x0 - x1);
	dy = abs(y0 - y1);
	return dx > dy ? dx : dy;
}

/* スキルユニットの配置?�報を返す */
struct skill_unit_layout skill_unit_layout[MAX_SKILL_UNIT_LAYOUT];
int firewall_unit_pos;
int icewall_unit_pos;

struct skill_unit_layout *skill_get_unit_layout (int skillid, int skilllv, struct block_list *src, int x, int y)
{	
	int pos = skill_get_unit_layout_type(skillid,skilllv);
	int dir;

	if (pos != -1)
		return &skill_unit_layout[pos];

	if (src->x == x && src->y == y)
		dir = 2;
	else
		dir = map_calc_dir(src,x,y);

	if (skillid == MG_FIREWALL)
		return &skill_unit_layout [firewall_unit_pos + dir];
	else if (skillid == WZ_ICEWALL)
		return &skill_unit_layout [icewall_unit_pos + dir];

	ShowError("Unknown unit layout for skill %d, %d\n",skillid,skilllv);
	return &skill_unit_layout[0];
}

/*==========================================
 * スキル追加?果
 *------------------------------------------
 */
int skill_additional_effect (struct block_list* src, struct block_list *bl, int skillid, int skilllv, int attack_type, unsigned int tick)
{
	/* MOB追加?果スキル用 */
	const int sc[]={
		SC_POISON, SC_BLIND, SC_SILENCE, SC_STAN,
		SC_STONE, SC_CURSE, SC_SLEEP
	};
	const int sc2[]={ //Note: We use Sonic Blow's stun duration for the confusion lasting time (dummy value): 12 secs at lv7
		MG_STONECURSE,MG_FROSTDIVER,NPC_STUNATTACK,
		NPC_SLEEPATTACK,TF_POISON,NPC_CURSEATTACK,
		NPC_SILENCEATTACK,AS_SONICBLOW,NPC_BLINDATTACK,
		LK_HEADCRUSH
	};

	struct map_session_data *sd=NULL;
	struct map_session_data *dstsd=NULL;
	struct mob_data *md=NULL;
	struct mob_data *dstmd=NULL;
	struct pet_data *pd=NULL;

	int skill,skill2;
	int rate;

	int sc_def_mdef,sc_def_vit,sc_def_int,sc_def_luk;

	nullpo_retr(0, src);
	nullpo_retr(0, bl);

	if(skillid < 0) 
	{	// remove the debug print when this case is finished
		ShowDebug("skill_additional_effect: skillid=%i\ncall: %p %p %i %i %i %i",skillid,
						src, bl,skillid,skilllv,attack_type,tick);
		return 0;
	}
	if(skillid > 0 && skilllv <= 0) return 0;	// don't forget auto attacks! - celest

	switch (src->type) {
	case BL_PC:
		sd = (struct map_session_data *)src;
		break;
	case BL_MOB:
		md = (struct mob_data *)src;
		break;
	case BL_PET:
		pd = (struct pet_data *)src; // [Valaris]
		break;
	}
	
	switch (bl->type) {
	case BL_PC:
		dstsd=(struct map_session_data *)bl;
		break;
	case BL_MOB:
		dstmd=(struct mob_data *)bl;
		break;
	default:
		return 0;
	}

	//??ﾛの耐?ｫ
	sc_def_mdef = status_get_sc_def_mdef(bl);
	sc_def_vit = status_get_sc_def_vit(bl);
	sc_def_int = status_get_sc_def_int(bl);
	sc_def_luk = status_get_sc_def_luk(bl);

	switch(skillid){
	case 0: // Normal attacks (no skill used)
		if(sd) {
			struct status_change *sc_data = status_get_sc_data(bl);
			// Automatic trigger of Blitz Beat
			if (pc_isfalcon(sd) && sd->status.weapon == 11 && (skill=pc_checkskill(sd,HT_BLITZBEAT))>0 &&
				rand()%1000 <= sd->paramc[5]*10/3+1 ) {
				int lv=(sd->status.job_level+9)/10;
				skill_castend_damage_id(src,bl,HT_BLITZBEAT,(skill<lv)?skill:lv,tick,0xf00000);
			}
			// Gank
			if(dstmd && !dstmd->state.steal_flag && sd->status.weapon != 11 && (skill=pc_checkskill(sd,RG_SNATCHER)) > 0 &&
				(skill*15 + 55) + (skill2 = pc_checkskill(sd,TF_STEAL))*10 > rand()%1000) {
				if(pc_steal_item(sd,bl))
					clif_skill_nodamage(src,bl,TF_STEAL,skill2,1);
				else if (battle_config.display_snatcher_skill_fail)
					clif_skill_fail(sd,skillid,0,0);
			}
			// Enchant Poison gives a chance to poison attacked enemies
			if(sd->sc_data[SC_ENCPOISON].timer != -1 && sc_data && sc_data[SC_POISON].timer == -1 &&
				rand() % 100 < sd->sc_data[SC_ENCPOISON].val1 * sc_def_vit / 100)
				status_change_start(bl,SC_POISON,sd->sc_data[SC_ENCPOISON].val1,0,0,0,skill_get_time2(AS_ENCHANTPOISON,sd->sc_data[SC_ENCPOISON].val1),0);
			// Enchant Deadly Poison gives a chance to deadly poison attacked enemies
			if(sd->sc_data[SC_EDP].timer != -1 && !(status_get_mode(bl)&MD_BOSS) && sc_data && sc_data[SC_DPOISON].timer == -1 &&
				rand() % 100 < sd->sc_data[SC_EDP].val2 * sc_def_vit / 100)
				status_change_start(bl,SC_DPOISON,sd->sc_data[SC_EDP].val1,0,0,0,skill_get_time2(ASC_EDP,sd->sc_data[SC_EDP].val1),0);
			// Chance to trigger Taekwon kicks [Dralnu]
			if(sd->sc_data[SC_READYSTORM].timer != -1 && sd->sc_data[SC_COMBO].timer == -1 && rand()%100 < 15)
				status_change_start(src,SC_COMBO, TK_STORMKICK,0,0,0,2000,0);
			if(sd->sc_data[SC_READYDOWN].timer != -1 && sd->sc_data[SC_COMBO].timer == -1 && rand()%100 < 15)
				status_change_start(src,SC_COMBO, TK_READYDOWN,0,0,0,2000,0);
			if(sd->sc_data[SC_READYTURN].timer != -1 && sd->sc_data[SC_COMBO].timer == -1 && rand()%100 < 15)
				status_change_start(src,SC_COMBO, TK_TURNKICK,0,0,0,2000,0);
			if(sd->sc_data[SC_READYCOUNTER].timer != -1 && sd->sc_data[SC_COMBO].timer == -1 && rand()%100 < 20)
				status_change_start(src,SC_COMBO, TK_COUNTER,bl->id,0,0,2000,0);
		}
		break;

	case SM_BASH:			/* バッシュ?i急?�?U??j */
		if( sd && skilllv > 5 && pc_checkskill(sd,SM_FATALBLOW)>0 ){
			if( rand()%100 < (5*(skilllv-5)+(int)sd->status.base_level/10)*sc_def_vit/100 ) //TODO: How much % per base level it actually is?
				status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(SM_FATALBLOW,skilllv),0);
		}
		break;

	case AS_VENOMKNIFE:
		if (sd) //Poison chance must be that of Envenom. [Skotlex]
			skilllv = pc_checkskill(sd, TF_POISON);
	case TF_POISON:			/* インベナム */
	case AS_SPLASHER:		/* ベナムスプラッシャ? */
		if(rand()%100< (2*skilllv+10)*sc_def_vit/100 )
			status_change_start(bl,SC_POISON,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		else{
			if(sd && skillid==TF_POISON)
				clif_skill_fail(sd,skillid,0,0);
		}
		break;

	case AS_SONICBLOW:		/* ソニックブ�?? */
		if( rand()%100 < (2*skilllv+10)*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case AS_GRIMTOOTH:
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			if (sd)
			{
				if (sc_data && sc_data[SC_SLOWDOWN].timer == -1)
					status_change_start(bl,SC_SLOWDOWN,0,0,0,0,skill_get_time2(skillid, skilllv),0);
			}
			else
				if (sc_data && sc_data[SC_STOP].timer == -1)
				status_change_start(bl,SC_STOP,0,0,0,0,skill_get_time2(skillid, skilllv), 0);
			break;
		}
	case MG_FROSTDIVER:		/* フ�?ストダイバ? */
	case WZ_FROSTNOVA:		/* フ�?ストノヴァ */
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			rate = (skilllv*3+35)*sc_def_mdef/100-(status_get_int(bl)+status_get_luk(bl))/15;
			if (rate <= 5)
				rate = 5;
			if(sc_data && sc_data[SC_FREEZE].timer == -1 && rand()%100 < rate)
				status_change_start(bl,SC_FREEZE,skilllv,0,0,0,skill_get_time2(skillid,skilllv)*(1-sc_def_mdef/100),0);
		}
		break;

	case WZ_STORMGUST:		/* スト?ムガスト */
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			if(sc_data) {
				sc_data[SC_FREEZE].val3++;
				if(sc_data[SC_FREEZE].val3 >= 3)
					status_change_start(bl,SC_FREEZE,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
			}
		}
		break;

	case WZ_METEOR:
		if(rand()%100 < 3*skilllv*sc_def_vit/100)
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case WZ_VERMILION:
		if(rand()%100 < 4*skilllv*sc_def_int/100)
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;
		
	case HT_FREEZINGTRAP:	/* フリ?ジングトラップ */
		if(rand()%100 < (3*skilllv+35)*sc_def_mdef/100)
			status_change_start(bl,SC_FREEZE,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	 case HT_FLASHER:  /* Flasher */
		if (!(status_get_mode(bl) & (MD_BOSS|MD_PLANT)) &&
			rand()%100 < (10*skilllv+30)*sc_def_int/100)
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case HT_LANDMINE:		/* ランドマイン */
		if( rand()%100 < (5*skilllv+30)*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case HT_SHOCKWAVE:				//it can't affect mobs, because they have no SP...
		if(dstsd){
			dstsd->status.sp -= dstsd->status.sp*(15*skilllv+5)/100;
			clif_updatestatus(dstsd,SP_SP);
		}
		break;

	case HT_SANDMAN:		/* サンドマン */
		if( rand()%100 < (10*skilllv+40)*sc_def_int/100 )
			status_change_start(bl,SC_SLEEP,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case TF_SPRINKLESAND:	/* ?ｻまき */
		if( rand()%100 < 20*sc_def_int/100 )
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case TF_THROWSTONE:		/* ?ﾎ投げ */
		if( rand()%100 < 3*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		if( rand()%100 < 3*sc_def_int/100 )
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case NPC_DARKCROSS:
	case CR_HOLYCROSS:		/* ホ?リ?クロス */
		if( rand()%100 < 3*skilllv*sc_def_int/100 )
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case CR_GRANDCROSS:		/* グランドク�?ス */
	case NPC_GRANDDARKNESS:	/*闇グランドク�?ス*/
		{
			int race = status_get_race(bl);
			if( (battle_check_undead(race,status_get_elem_type(bl)) || race == 6) && rand()%100 < 100000*sc_def_int/100)	//??ｧ付?だが完全耐?ｫには無?
				status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		}
		break;

	case AM_ACIDTERROR:
		if (rand()%100 < (skilllv*3)*sc_def_vit/100 ) {
			status_change_start(bl,SC_BLEEDING,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		}
		if (dstsd && rand()%100 < skill_get_time(skillid,skilllv) * battle_config.equip_skill_break_rate / 100) { //fixed
			if(pc_breakarmor(dstsd))
				clif_emotion(bl,23);
		}

		break;

	case AM_DEMONSTRATION:
		if (dstsd && rand()%10000 < skilllv * battle_config.equip_skill_break_rate )
			pc_breakweapon(dstsd);
		break;
		
	case CR_SHIELDCHARGE:		/* シ?ルドチャ?ジ */
		if( rand()%100 < (15 + skilllv*5)*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case PA_PRESSURE:	/* プレッシャ? */
		{
			/* Official servers seem to indicate this causes neither stun nor bleeding. [Skotlex]
			int race = status_get_race(bl);
			if (rand()%100 < 50 * sc_def_vit / 100)	// is chance 50%?
				status_change_start(bl, SC_STAN, skilllv, 0, 0, 0, skill_get_time2(PA_PRESSURE,skilllv), 0);
			if (!(battle_check_undead(race, status_get_elem_type(bl)) || race == 6) && rand()%100 < 50 * sc_def_vit / 100)
				status_change_start(bl, SC_BLEEDING, skilllv, 0, 0, 0, skill_get_time2(skillid,skilllv), 0);
				*/
			if (dstsd) {
				dstsd->status.sp -= dstsd->status.sp * (15 + 5 * skilllv) / 100;
				clif_updatestatus(dstsd,SP_SP);
			}
		}
		break;

	case RG_RAID:		/* サプライズアタック */
		if( rand()%100 < (10+3*skilllv)*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		if( rand()%100 < (10+3*skilllv)*sc_def_int/100 )
			status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case BA_FROSTJOKE:
		if(rand()%100 < (15+5*skilllv)*sc_def_mdef/100)
			status_change_start(bl,SC_FREEZE,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case DC_SCREAM:
		if( rand()%100 < (25+5*skilllv)*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case BD_LULLABY:	/* 子守唄 */
		if( rand()%100 < 15*sc_def_int/100 )
			status_change_start(bl,SC_SLEEP,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case DC_UGLYDANCE:
		if (dstsd) {
				int skill, sp = 5+5*skilllv;
				if(sd && (skill=pc_checkskill(sd,DC_DANCINGLESSON)))
				    sp += 5+skill;
				dstsd->status.sp -= sp;
				if(dstsd->status.sp<0)
					dstsd->status.sp=0;
				clif_updatestatus(dstsd,SP_SP);
		}
  		break;

	/* MOBの追加?果付きスキル */

	case NPC_PETRIFYATTACK:
		if(rand()%100 < sc_def_mdef)
			status_change_start(bl,sc[skillid-NPC_POISON],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;
	case NPC_POISON:
	case NPC_SILENCEATTACK:
	case NPC_STUNATTACK:
		if(rand()%100 < sc_def_vit && src->type!=BL_PET)
			status_change_start(bl,sc[skillid-NPC_POISON],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		if(src->type==BL_PET)
			status_change_start(bl,sc[skillid-NPC_POISON],skilllv,0,0,0,skilllv*1000,0);
		break;
	case NPC_CURSEATTACK:
		if(rand()%100 < sc_def_luk)
			status_change_start(bl,sc[skillid-NPC_POISON],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;
	case NPC_SLEEPATTACK:
	case NPC_BLINDATTACK:
		if(rand()%100 < sc_def_int)
			status_change_start(bl,sc[skillid-NPC_POISON],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case NPC_MENTALBREAKER:
		if(dstsd) {
			int sp = dstsd->status.max_sp*(10+skilllv)/100;
			if(sp < 1) sp = 1;
			pc_heal(dstsd,0,-sp);
		}
		break;

	case CH_TIGERFIST:
		if (rand()%100 < (10 + skilllv*10)*sc_def_vit/100) {
			int sec = skill_get_time2 (skillid,skilllv) - status_get_agi(bl)/10;
			if (dstsd) {
				dstsd->canmove_tick += sec;
				dstsd->canact_tick += sec;
			} else if (dstmd)
				dstmd->canmove_tick += sec;
		}
		break;

	case LK_SPIRALPIERCE:
		if (rand()%100 < (15 + skilllv*5)*sc_def_vit/100)
			status_change_start(bl,SC_STOP,0,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case ST_REJECTSWORD:	/* フリ?ジングトラップ */
		if( rand()%100 < (skilllv*15) )
			status_change_start(bl,SC_AUTOCOUNTER,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case PF_FOGWALL:		/* ホ?リ?ク�?ス */
		if (src != bl) {
			struct status_change *sc_data = status_get_sc_data(bl);
			if (sc_data && sc_data[SC_DELUGE].timer == -1)
				status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		}
		break;

	case LK_HEADCRUSH:				/* ヘッドクラッシュ */
		{
			//?�?が良く分からないので適?に
			int race = status_get_race(bl);
			if (!(battle_check_undead(race, status_get_elem_type(bl)) || race == 6) && rand()%100 < 50 * sc_def_vit/100)
				status_change_start(bl, SC_BLEEDING, skilllv, 0, 0, 0, skill_get_time2(skillid,skilllv), 0);
		}
		break;

	case LK_JOINTBEAT:				/* ジョイントビ?ト */
		//?�?が良く分からないので適?に
		if( rand()%100 < (5*skilllv+5)*sc_def_vit/100 )
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case PF_SPIDERWEB:		/* スパイダ?ウェッブ */
		{
			int sec = skill_get_time2(skillid,skilllv);
			if(map[src->m].flag.pvp) //PvPでは?S束時間半減?H
				sec = sec/2;
			battle_stopwalking(bl,1);
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,sec,0);
		}
		break;

	case ASC_METEORASSAULT:			/* �?テオアサルト */
		//Any enemies hit by this skill will receive Stun, Darkness, or external bleeding status ailment with a 5%+5*SkillLV% chance.
		if( rand()%100 < (5+skilllv*5) ) //5%+5*SkillLV%
			switch(rand()%3) {
				case 0:
					status_change_start(bl,SC_BLIND,skilllv,0,0,0,skill_get_time2(skillid,1),0);
					break;
				case 1:
					status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,2),0);
					break;
				default:
					status_change_start(bl,SC_BLEEDING,skilllv,0,0,0,skill_get_time2(skillid,3),0);
  			}
		break;

	case HW_NAPALMVULCAN:			/* ナパ?ムバルカン */
		// skilllv*5%の確率で呪い
		if (rand()%10000 < 5*skilllv*sc_def_luk)
			status_change_start(bl,SC_CURSE,7,0,0,0,skill_get_time2(NPC_CURSEATTACK,7),0);
		break;

	case WS_CARTTERMINATION:	// Cart termination
		if (rand() % 10000 < 5 * skilllv * sc_def_vit)
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;

	case CR_ACIDDEMONSTRATION:
		if (dstsd) {
			if (rand()%10000 < skilllv * battle_config.equip_skill_break_rate)
				pc_breakweapon(dstsd);
			// separate chances?
			if (rand()%10000 < skilllv * battle_config.equip_skill_break_rate)
				pc_breakarmor(dstsd);
		}
		break;

	case TK_DOWNKICK:
		if(rand()%100 < 100*sc_def_vit/100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
        break;

	case MO_BALKYOUNG: //Note: attack_type is passed as BF_WEAPON for the actual target, BF_MISC for the splash-affected mobs.
		if(attack_type == BF_MISC &&  rand()%100 < 70*sc_def_vit/100 ) //70% base stun chance...
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;
	}

	if (md && battle_config.summons_inherit_effects && md->master_id && md->special_state.ai)
	{	//Pass heritage to Master for status causing effects. [Skotlex]
		sd = map_id2sd(md->master_id);
	}

	if(sd && skillid != MC_CARTREVOLUTION && skillid != AM_DEMONSTRATION && attack_type&BF_WEAPON){	/* カ?ドによる追加?果 */
		int i, type;
		int sc_def_card=100;

		for(i=SC_COMMON_MIN;i<=SC_COMMON_MAX;i++){
			type=i-SC_COMMON_MIN;
			if (!sd->addeff[type] && (!sd->state.arrow_atk || !sd->arrow_addeff[type]))
				continue; //Code Speedup.
			//??ﾛに?態異?�
			switch (i) {
				case SC_STONE:
				case SC_FREEZE:
					sc_def_card=sc_def_mdef;
					break;
				case SC_STAN:
				case SC_POISON:
				case SC_DPOISON:
				case SC_SILENCE:
				case SC_BLEEDING:
					sc_def_card=sc_def_vit;
					break;
				case SC_SLEEP:
				case SC_CONFUSION:
				case SC_BLIND:
					sc_def_card=sc_def_int;
					break;
				case SC_CURSE:
					sc_def_card=sc_def_luk;
			}

			if	(rand()%10000 < (sd->addeff[type]+(sd->state.arrow_atk?sd->arrow_addeff[type]:0))*sc_def_card/100 )
			{	//Inflicted status effect.
				if(battle_config.battle_log)
					ShowInfo("PC %d skill_additional_effect: caused status effect (pos %d): %d\n",sd->bl.id,i,sd->addeff[type]);
				status_change_start(bl,i,7,0,0,0,skill_get_time2(sc2[type],7),0);
			}
		}
	}

	//Reports say that autospell effects get triggered on skills and pretty much everything including splash attacks. [Skotlex]
	//Here we use the nk value to trigger spells only on damage causing skills (otherwise stuff like AL_HEAL will trigger them)
	if(sd && !status_isdead(bl) && src != bl &&(!skillid || skillid == KN_AUTOCOUNTER || skill_get_nk(skillid)!=NK_NO_DAMAGE)) 
	{
		struct block_list *tbl;
		int i, auto_skillid, auto_skilllv, rate;

		for (i = 0; i < MAX_PC_BONUS; i++) {
			if (sd->autospell[i].id == 0)
				break;

			auto_skillid = (sd->autospell[i].id > 0) ? sd->autospell[i].id : -sd->autospell[i].id;

			if (auto_skillid == skillid) //Prevents skill from retriggering themselves. [Skotlex]
				continue;

			auto_skilllv = (sd->autospell[i].lv > 0) ? sd->autospell[i].lv : 1;
			rate = (!sd->state.arrow_atk) ? sd->autospell[i].rate : sd->autospell[i].rate / 2;

			if (rand()%1000 > rate)
				continue;
			if (sd->autospell[i].id < 0)
				tbl = src;
			else
				tbl = bl;
			
			if (tbl != src && !battle_check_range(src, tbl, skill_get_range2(src, auto_skillid, auto_skilllv)))
				continue; //Autoskills DO check for target-src range. [Skotlex]
			
			if (skill_get_inf(auto_skillid) & INF_GROUND_SKILL)
				skill_castend_pos2(src, tbl->x, tbl->y, auto_skillid, auto_skilllv, tick, 0);
			else {
				switch (skill_get_nk(auto_skillid)) {
					case NK_NO_DAMAGE:
						if ((auto_skillid == AL_HEAL || (auto_skillid == ALL_RESURRECTION && tbl->type != BL_PC)) &&
							battle_check_undead(status_get_race(tbl),status_get_elem_type(tbl)))
							skill_castend_damage_id(src, tbl, auto_skillid, auto_skilllv, tick, 0);
						else
							skill_castend_nodamage_id(src, tbl, auto_skillid, auto_skilllv, tick, 0);
						break;
					case NK_SPLASH_DAMAGE:
					default:
						skill_castend_damage_id(src, tbl, auto_skillid, auto_skilllv, tick, 0);
						break;
				}
			}
		}
	}
	return 0;
}

/* Splitted off from skill_additional_effect, which is never called when the
 * attack skill kills the enemy. Place in this function counter status effects 
 * when using skills (eg: Asura's sp regen penalty, or counter-status effects 
 * from cards) that will take effect on the source, not the target. [Skotlex]
 * Note: Currently this function only applies to Extremity Fist and BF_WEAPON 
 * type of skills, so not every instance of skill_additional_effect needs a call
 * to this one.
 */
int skill_counter_additional_effect (struct block_list* src, struct block_list *bl, int skillid, int skilllv, int attack_type, unsigned int tick)
{
	const int sc2[]={
		MG_STONECURSE,MG_FROSTDIVER,NPC_STUNATTACK,
		NPC_SLEEPATTACK,TF_POISON,NPC_CURSEATTACK,
		NPC_SILENCEATTACK,AS_SONICBLOW,NPC_BLINDATTACK,
		LK_HEADCRUSH
	};

	struct map_session_data *sd=NULL;
	struct map_session_data *dstsd=NULL;
	struct mob_data *md=NULL;
	struct mob_data *dstmd=NULL;
//	struct pet_data *pd=NULL; Pet's can't be inflicted!

	int sc_def_mdef,sc_def_vit,sc_def_int,sc_def_luk;

	nullpo_retr(0, src);
	nullpo_retr(0, bl);

	if(skillid < 0) 
	{	// remove the debug print when this case is finished
		ShowDebug("skill_counter_additional_effect: skillid=%i\ncall: %p %p %i %i %i %i",skillid,
						src, bl,skillid,skilllv,attack_type,tick);
		return 0;
	}
	if(skillid > 0 && skilllv <= 0) return 0;	// don't forget auto attacks! - celest

	switch (src->type) {
		case BL_PC:
			sd = (struct map_session_data *)src;
			break;
		case BL_MOB:
			md = (struct mob_data *)src;
			break;
		case BL_PET:	//Only mobs/players can be affected. [Skotlex]
//			pd = (struct pet_data *)src;
//			break;
		default:
			return 0;
	}
	
	switch (bl->type) {
		case BL_PC:
			dstsd=(struct map_session_data *)bl;
			break;
		case BL_MOB:
			dstmd=(struct mob_data *)bl;
			break;
		default:
			return 0;
	}

	//自分の耐?ｫ
	sc_def_mdef = status_get_sc_def_mdef(src);
	sc_def_vit = status_get_sc_def_vit(src);
	sc_def_int = status_get_sc_def_int(src);
	sc_def_luk = status_get_sc_def_luk(src);

	switch(skillid){
	case 0: //Normal Attack - Nothing here yet.
		break;
	case MO_EXTREMITYFIST:			/* 阿?C羅覇凰�? */
		//阿?C羅を使うと5分間自然回復しないようになる
		status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0 );
		break;
	}
	
	if((sd||dstsd) && skillid != MC_CARTREVOLUTION && attack_type&BF_WEAPON){	/* カ?ドによる追加?果 */
		int i, type;
		int sc_def_card=100;

		for(i=SC_COMMON_MIN;i<=SC_COMMON_MAX;i++){
			type=i-SC_COMMON_MIN;
					
			switch (i) {
				case SC_STONE:
				case SC_FREEZE:
					sc_def_card=sc_def_mdef;
					break;
				case SC_STAN:
				case SC_POISON:
				case SC_DPOISON:
				case SC_SILENCE:
				case SC_BLEEDING:
					sc_def_card=sc_def_vit;
					break;
				case SC_SLEEP:
				case SC_CONFUSION:
				case SC_BLIND:
					sc_def_card=sc_def_int;
					break;
				case SC_CURSE:
					sc_def_card=sc_def_luk;
			}

			if (sd && (rand()%10000 < (sd->addeff2[type]+(sd->state.arrow_atk?sd->arrow_addeff2[type]:0))*sc_def_card/100 ))
			{	//Self infliced status from attacking.
				if(battle_config.battle_log)
					ShowInfo("PC %d skill_addeff: self inflicted effect (pos %d): %d\n",src->id,i,sd->addeff2[type]);
				status_change_start(src,i,7,0,0,0,skill_get_time2(sc2[type],7),0);
			}
			if (dstsd &&
				(dstsd->addeff3_type[type] == 1 || ((sd && sd->state.arrow_atk) || (status_get_range(src)>2))) &&
				(rand()%10000 < dstsd->addeff3[type]*sc_def_card/100)
			) {	//Counter status effect.
				if(battle_config.battle_log)
					ShowInfo("PC %d skill_addeff: counter inflicted effect (pos %d): %d\n",src->id,i,dstsd->addeff3[type]);
				status_change_start(src,i,7,0,0,0,skill_get_time2(sc2[type],7),0);
			}
		}
	}

	//Trigger counter-spells to retaliate against damage causing skills. [Skotlex]
	if(dstsd && !status_isdead(bl) && src != bl &&(!skillid || skill_get_nk(skillid)!=NK_NO_DAMAGE)) 
	{
		struct block_list *tbl;
		int i, skillid, skilllv, rate;

		for (i = 0; i < MAX_PC_BONUS; i++) {
			if (dstsd->autospell2[i].id == 0)
				break;

			skillid = (dstsd->autospell2[i].id > 0) ? dstsd->autospell2[i].id : -dstsd->autospell2[i].id;
			skilllv = (dstsd->autospell2[i].lv > 0) ? dstsd->autospell2[i].lv : 1;
			rate = ((sd && !sd->state.arrow_atk) || (status_get_range(src)<=2)) ?
				dstsd->autospell2[i].rate : dstsd->autospell2[i].rate / 2;
			
			if (rand()%1000 > rate)
				continue;
			if (dstsd->autospell2[i].id < 0)
				tbl = bl;
			else
				tbl = src;

			if (skill_get_inf(skillid) & INF_GROUND_SKILL)
				skill_castend_pos2(bl, tbl->x, tbl->y, skillid, skilllv, tick, 0);
			else {
				switch (skill_get_nk(skillid)) {
					case NK_NO_DAMAGE:
						if ((skillid == AL_HEAL || (skillid == ALL_RESURRECTION && tbl->type != BL_PC)) &&
							battle_check_undead(status_get_race(tbl), status_get_elem_type(tbl)))
							skill_castend_damage_id(bl, tbl, skillid, skilllv, tick, 0);
						else
							skill_castend_nodamage_id(bl, tbl, skillid, skilllv, tick, 0);
						break;
					case NK_SPLASH_DAMAGE:
					default:
						skill_castend_damage_id(bl, tbl, skillid, skilllv, tick, 0);
						break;
				}
			}
		}
	}
	return 0;
}

/*=========================================================================
 Used to knock back players, monsters, traps, etc
 If count&0xf00000, the direction is send in the 6th byte.
 If count&0x10000, the direction is to the back of the target, otherwise is away from the src.
 If count&0x20000, position update packets must not be sent.
-------------------------------------------------------------------------*/
int skill_blown( struct block_list *src, struct block_list *target,int count)
{
	int dx=0,dy=0,nx,ny;
	int x=target->x,y=target->y;
	int dir,ret;
	int moveblock;
	struct map_session_data *sd=NULL;
	struct mob_data *md=NULL;
	struct pet_data *pd=NULL;
	struct skill_unit *su=NULL;
	struct status_change* sc_data=NULL;

	nullpo_retr(0, src);
	nullpo_retr(0, target);

	if (src != target && map_flag_gvg(target->m) && target->type != BL_SKILL)
		return 0; //No knocking back in WoE, except for skills... because traps CAN be knocked back.

	switch (target->type) {
		case BL_PC:
			sd=(struct map_session_data *)target;
			break;
		case BL_MOB:
			md=(struct mob_data *)target;
			break;
		case BL_PET:
			pd=(struct pet_data *)target;
			break;
		case BL_SKILL:
			su=(struct skill_unit *)target;
			break;
		default:
			return 0;
	}
	if (target->type != BL_SKILL)
		sc_data = status_get_sc_data(target);

	if (count&0xf00000)
		dir = (count>>20)&0xf;
	else if (count&0x10000 || (target->x==src->x && target->y==src->y))
		dir = status_get_dir(target);
	else
		dir = map_calc_dir(target,src->x,src->y);
	if (dir>=0 && dir<8){
		dx = -dirx[dir];
		dy = -diry[dir];
	}

	ret=path_blownpos(target->m,x,y,dx,dy,count&0xffff);
	nx=ret>>16;
	ny=ret&0xffff;
	//Avoid "moving" a target that is not registered in the map (otherwise a dangling pointer is caused) [Skotlex]
	//This happens when a skill knocks back an enemy and the same hit kills it, so we are pushing around a corpse.
	moveblock= (target->prev!=NULL) && ( x/BLOCK_SIZE != nx/BLOCK_SIZE || y/BLOCK_SIZE != ny/BLOCK_SIZE);
	
	battle_stopwalking(target,0); 

	dx = nx - x;
	dy = ny - y;

	if(sd)	/* ?面外に?oたので?ﾁ去 */
		map_foreachinmovearea(clif_pcoutsight,target->m,x-AREA_SIZE,y-AREA_SIZE,x+AREA_SIZE,y+AREA_SIZE,dx,dy,0,sd);
	else if(md)
		map_foreachinmovearea(clif_moboutsight,target->m,x-AREA_SIZE,y-AREA_SIZE,x+AREA_SIZE,y+AREA_SIZE,dx,dy,BL_PC,md);
	else if(pd)
		map_foreachinmovearea(clif_petoutsight,target->m,x-AREA_SIZE,y-AREA_SIZE,x+AREA_SIZE,y+AREA_SIZE,dx,dy,BL_PC,pd);
	
	if (sc_data && sc_data[SC_DANCING].timer != -1) {//Move the song/dance [Skotlex]
		if (sc_data[SC_DANCING].val1 == CG_MOONLIT) //Cancel Moonlight Petals if moved from casting position. [Skotlex]
			skill_stop_dancing(target);
		else
			skill_unit_move_unit_group((struct skill_unit_group *)sc_data[SC_DANCING].val2, target->m, dx, dy);
	}
		
	if(su){
		skill_unit_move_unit_group(su->group,target->m,dx,dy);
	}else{
		int tick = gettick();
		skill_unit_move(target,tick,2);
		if(moveblock) map_delblock(target);
		target->x=nx;
		target->y=ny;
		if(moveblock) map_addblock(target);
		skill_unit_move(target,tick,3);
	}

	if(sd)	/* ?面?に入ってきたので表示 */
		map_foreachinmovearea(clif_pcinsight,target->m,nx-AREA_SIZE,ny-AREA_SIZE,nx+AREA_SIZE,ny+AREA_SIZE,-dx,-dy,0,sd);
	else if(md)
		map_foreachinmovearea(clif_mobinsight,target->m,nx-AREA_SIZE,ny-AREA_SIZE,nx+AREA_SIZE,ny+AREA_SIZE,-dx,-dy,BL_PC,md);
	else if(pd)
		map_foreachinmovearea(clif_petinsight,target->m,nx-AREA_SIZE,ny-AREA_SIZE,nx+AREA_SIZE,ny+AREA_SIZE,-dx,-dy,BL_PC,pd);

	if(!(count&0x20000)) 
		clif_blown(target);

	return 0;
}

/*
 * =========================================================================
 * スキル?U??果?�?まとめ
 * flagの?明?B16?i?
 *	00XRTTff
 *  ff	= magicで計算に渡される?j
 *	TT	= パケットのtype部分(0でデフォルト?j
 *  X   = パケットのスキルLv
 *  R	= 予約?iskill_area_subで使用する?j
 *-------------------------------------------------------------------------
 */

int skill_attack( int attack_type, struct block_list* src, struct block_list *dsrc,
	 struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag )
{
	struct Damage dmg;
	struct status_change *sc_data;
	struct map_session_data *sd=NULL, *tsd=NULL;
	int type,lv,damage;
	static int tmpdmg = 0;

	if(skillid > 0 && skilllv <= 0) return 0;

	rdamage = 0;
	nullpo_retr(0, src);	//Source is the master behind the attack (player/mob/pet)
	nullpo_retr(0, dsrc); //dsrc is the actual originator of the damage, can be the same as src, or a skill casted by src.
	nullpo_retr(0, bl); //Target to be attacked.

	sc_data = status_get_sc_data(bl);

	if(src->prev == NULL || dsrc->prev == NULL || bl->prev == NULL)
		return 0;

	//uncomment the following to do a check between caster and target. [Skotlex]
	//eg: if you want storm gust to do no damage if the caster runs to another map after invoking the skill.
//	if(src->m != bl->m) 
//		return 0;

	if(status_isdead(dsrc) || status_isdead(bl))
		return 0;

	//Uncomment the following to disable trap-ground skills from hitting when the caster is dead [Skotlex]
	//eg: You cast meteor and then are killed, if you uncomment the following the meteors that fall afterwards cause no damage.
//	if(src != dsrc && status_isdead(src))
//		return 0;

	if (dsrc->type == BL_PC)
		sd = (struct map_session_data *)dsrc;
	if (bl->type == BL_PC)
		tsd = (struct map_session_data *)bl;

//Shouldn't be needed, skillnotok's return value is highly unlikely to have changed after you started casting. [Skotlex]
//	if(dsrc->type == BL_PC && skillnotok(skillid, (struct map_session_data *)dsrc))
//		return 0; // [MouseJstr]
	if(sc_data && sc_data[SC_HIDING].timer != -1) {
		if(skill_get_pl(skillid) != 2) // Earth-elemental skills can attack hidden people??
			return 0;
	}
	if(sc_data && sc_data[SC_TRICKDEAD].timer != -1)
		return 0;
	if(skillid == WZ_STORMGUST) {
		if(sc_data && sc_data[SC_FREEZE].timer != -1)
			return 0;
	}
	if(skillid == WZ_FROSTNOVA && dsrc->x == bl->x && dsrc->y == bl->y) //使用スキルがフ�?ストノヴァで?Adsrcとblが同じ?�?鰍ﾈら何もしない
		return 0;
	if(sd && sd->chatID) //術者がPCでチャット中なら何もしない
		return 0;

//何もしない判定ここまで

	type=-1;
	lv=(flag>>20)&0xf;
	dmg=battle_calc_attack(attack_type,src,bl,skillid,skilllv,flag&0xff ); //ダ�??ジ計算

	//Skotlex: Adjusted to the new system
	if(src->type==BL_PET && (struct pet_data *)src)
	{ // [Valaris]
		struct pet_data *pd = (struct pet_data *)src;
		if (pd->a_skill && pd->a_skill->div_ && pd->a_skill->id == skillid)
		{
			int element = skill_get_pl(skillid);
			if (skillid == -1)
				element = status_get_attack_element(src);
			dmg.damage=battle_attr_fix(src, bl, skilllv, element, status_get_element(bl));
			dmg.damage2=0;
			dmg.div_= pd->a_skill->div_;
		}
	}

//マジックロッド?理ここから
	if(attack_type&BF_MAGIC && sc_data && sc_data[SC_MAGICROD].timer != -1 && src == dsrc) { //魔法攻?でマジックロッド?態でsrc=dsrcなら
		dmg.damage = dmg.damage2 = 0; //ダメ?ジ0
		dmg.dmg_lv = ATK_FLEE; //This will prevent skill additional effect from taking effect. [Skotlex]
		if(tsd) {
			int sp = skill_get_sp(skillid,skilllv); //使用されたスキルのSPを吸?
			sp = sp * sc_data[SC_MAGICROD].val2 / 100; //吸?率計算
			if(skillid == WZ_WATERBALL && skilllv > 1) //ウォ?タ?ボ?ルLv1以上
				sp = sp/((skilllv|1)*(skilllv|1)); //さらに計算？
			if(sp > 0x7fff) sp = 0x7fff; //SP多すぎの場合は理論最大値
			else if(sp < 1) sp = 1; //1以下の場合は1
			if(tsd->status.sp + sp > tsd->status.max_sp) { //回復SP+現在のSPがMSPより大きい場合
				sp = tsd->status.max_sp - tsd->status.sp; //SPをMSP-現在SPにする
				tsd->status.sp = tsd->status.max_sp; //現在のSPにMSPを代入
			}
			else //回復SP+現在のSPがMSPより小さい場合は回復SPを加算
				tsd->status.sp += sp;
			clif_heal(tsd->fd,SP_SP,sp); //SP回復エフェクトの表示
			tsd->canact_tick = tick + skill_delayfix(bl, SA_MAGICROD, sc_data[SC_MAGICROD].val1, 0);
		}
		clif_skill_nodamage(bl,bl,SA_MAGICROD,sc_data[SC_MAGICROD].val1,1); //マジックロッドエフェクトを表示
	}
//マジックロッド?理ここまで

	damage = dmg.damage + dmg.damage2;

	if(lv==15)
		lv=-1;

	if( flag&0xff00 )
		type=(flag&0xff00)>>8;

	if((damage <= 0 || damage < dmg.div_)
			&& skillid != CH_PALMSTRIKE) //Palm Strike is the only skill that will knockback even if it misses. [Skotlex]
		dmg.blewcount = 0;

	if(skillid == CR_GRANDCROSS||skillid == NPC_GRANDDARKNESS) {//グランドクロス
		if(battle_config.gx_disptype) dsrc = src;	// 敵ダメ?ジ白文字表示
		if(src == bl) type = 4;	// 反動はダメ?ジモ?ションなし
	}

//使用者がPCの?�?�の?�?ここから
	if(sd) {
		//Sorry for removing the Japanese comments, but they were actually distracting 
		//from the actual code and I couldn't understand a thing anyway >.< [Skotlex]
		switch(skillid)
		{
			case MO_TRIPLEATTACK:
			{
				int delay = 1000 - 4 * status_get_agi(src) - 2 *  status_get_dex(src);
				if (damage < status_get_hp(bl) &&
					pc_checkskill(sd, MO_CHAINCOMBO) > 0)
					delay += 300 * battle_config.combo_delay_rate / 100;
				status_change_start(src,SC_COMBO,MO_TRIPLEATTACK,skilllv,0,0,delay,0);
				sd->attackabletime = sd->canmove_tick = tick + delay;
				clif_combo_delay(src, delay);
				break;
			}
			case MO_CHAINCOMBO:
			{
				int delay = 1000 - 4 * status_get_agi(src) - 2 *  status_get_dex(src);
				if(damage < status_get_hp(bl) &&
					(pc_checkskill(sd, MO_COMBOFINISH) > 0 && sd->spiritball > 0))
					delay += 300 * battle_config.combo_delay_rate /100;
				status_change_start(src,SC_COMBO,MO_CHAINCOMBO,skilllv,0,0,delay,0);
				sd->attackabletime = sd->canmove_tick = tick + delay;
				clif_combo_delay(src,delay);
				break;
			}
			case MO_COMBOFINISH:
			{
				int delay = 700 - 4 * status_get_agi(src) - 2 *  status_get_dex(src);
				if(damage < status_get_hp(bl) &&
				(
					(pc_checkskill(sd, MO_EXTREMITYFIST) > 0 && sd->spiritball >= 4 && sd->sc_data[SC_EXPLOSIONSPIRITS].timer != -1) ||
					(pc_checkskill(sd, CH_TIGERFIST) > 0 && sd->spiritball > 0) ||
					(pc_checkskill(sd, CH_CHAINCRUSH) > 0 && sd->spiritball > 1)
				))
					delay += 300 * battle_config.combo_delay_rate /100;
				status_change_start(src,SC_COMBO,MO_COMBOFINISH,skilllv,0,0,delay,0);
				sd->attackabletime = sd->canmove_tick = tick + delay;
				clif_combo_delay(src,delay);
				break;
			}
			case CH_TIGERFIST:
			{	//Tigerfist is now a combo-only skill. [Skotlex]
				int delay = 1000 - 4 * status_get_agi(src) - 2 *  status_get_dex(src);
				if(damage < status_get_hp(bl) &&
				(
					(pc_checkskill(sd, MO_EXTREMITYFIST) > 0 && sd->spiritball >= 3 && sd->sc_data[SC_EXPLOSIONSPIRITS].timer != -1) ||
					(pc_checkskill(sd, CH_CHAINCRUSH) > 0)
				))
					delay += 300 * battle_config.combo_delay_rate /100;
				status_change_start(src,SC_COMBO,CH_TIGERFIST,skilllv,0,0,delay,0);
				sd->attackabletime = sd->canmove_tick = tick + delay;
				clif_combo_delay(src,delay);
				break;
			}
			case CH_CHAINCRUSH:
			{
				int delay = 1000 - 4 * status_get_agi(src) - 2 *  status_get_dex(src);
				if(damage < status_get_hp(bl))
					delay += 300 * battle_config.combo_delay_rate /100;
				status_change_start(src,SC_COMBO,CH_CHAINCRUSH,skilllv,0,0,delay,0);
				sd->attackabletime = sd->canmove_tick = tick + delay;
				clif_combo_delay(src,delay);
				break;
			}
		}	//Switch End
	}
	if(attack_type&BF_WEAPON && damage > 0 && src != bl && src == dsrc)
	{ //武器スキル＆ダメ?ジあり＆使用者と?象者が違う＆src=dsrc
		if(dmg.flag&BF_SHORT) { //近距離攻?時？※
			if(tsd && tsd->short_weapon_damage_return > 0)
			{ //近距離攻?跳ね返し？※
				rdamage += damage * tsd->short_weapon_damage_return / 100;
				if(rdamage < 1) rdamage = 1;
			}
			if(sc_data && sc_data[SC_REFLECTSHIELD].timer != -1) { //リフレクトシ?ルド時
				rdamage += damage * sc_data[SC_REFLECTSHIELD].val2 / 100; //跳ね返し計算
				if(rdamage < 1) rdamage = 1;
			}
		}
		else if(dmg.flag&BF_LONG) { //遠距離攻?時？※
			if(tsd && tsd->long_weapon_damage_return > 0)
			{ //遠距離攻?跳ね返し？※
				rdamage += damage * tsd->long_weapon_damage_return / 100;
				if(rdamage < 1) rdamage = 1;
			}
		}
	} else
	// magic_damage_return by [AppleGirl] and [Valaris]
	if(attack_type&BF_MAGIC && damage > 0 && src != bl && src == dsrc)
	{
		if(tsd && tsd->magic_damage_return > 0 )
		{
			rdamage += damage * tsd->magic_damage_return / 100;
			if(rdamage < 1) rdamage = 1;
		}
	}

//武器スキル？ここまで
	switch(skillid){
	case AS_SPLASHER:
		clif_skill_damage(dsrc,bl,tick,dmg.amotion,dmg.dmotion, damage, dmg.div_, skillid, -1, 5);
		break;
	case ASC_BREAKER:	// [celest]
		if (attack_type&BF_WEAPON) { // the 1st attack won't really deal any damage
			tmpdmg = damage;	// store the temporary weapon damage
		} else {	// only display damage for the 2nd attack
			if (tmpdmg == 0 || damage == 0)	// if one or both attack(s) missed, display a 'miss'
				clif_skill_damage(dsrc, bl, tick, dmg.amotion, dmg.dmotion, 0, dmg.div_, skillid, skilllv, type);
			damage += tmpdmg;	// add weapon and magic damage
			tmpdmg = 0;	// clear the temporary weapon damage
			if (damage > 0)	// if both attacks missed, do not display a 2nd 'miss'
				clif_skill_damage(dsrc, bl, tick, dmg.amotion, dmg.dmotion, damage, dmg.div_, skillid, skilllv, type);
		}
		break;
	case NPC_SELFDESTRUCTION:
		if(src->type==BL_PC)
			dmg.blewcount = 10;
		break;
	case KN_AUTOCOUNTER: //Skills that need be passed as a normal attack for the client to display correctly.
	case SN_SHARPSHOOTING:
		clif_damage(src,bl,tick,dmg.amotion,dmg.dmotion,damage,dmg.div_,dmg.type,dmg.damage2);
		break;
	default:
		clif_skill_damage(dsrc,bl,tick,dmg.amotion,dmg.dmotion, damage, dmg.div_, skillid, (lv!=0)?lv:skilllv, (skillid==0)? 5:type );
	}
	
	map_freeblock_lock();

	if(damage > 0 && dmg.flag&BF_SKILL && tsd
		&& pc_checkskill(tsd,RG_PLAGIARISM) && sc_data[SC_PRESERVE].timer == -1
		&& damage < tsd->status.hp)
	{	//Updated to not be able to copy skills if the blow will kill you. [Skotlex]
		if ((!tsd->status.skill[skillid].id || tsd->status.skill[skillid].flag >= 13) &&
			can_copy(tsd,skillid))	// Split all the check into their own function [Aru]
		{
			//?に?んでいるスキルがあれば該?スキルを消す
			if (tsd->cloneskill_id && tsd->status.skill[tsd->cloneskill_id].flag == 13){
				tsd->status.skill[tsd->cloneskill_id].id = 0;
				tsd->status.skill[tsd->cloneskill_id].lv = 0;
				tsd->status.skill[tsd->cloneskill_id].flag = 0;
			}
			tsd->cloneskill_id = skillid;
			tsd->status.skill[skillid].id = skillid;
			tsd->status.skill[skillid].lv = skilllv;
			if ((lv = pc_checkskill(tsd,RG_PLAGIARISM)) < skilllv)
				tsd->status.skill[skillid].lv = lv;
			tsd->status.skill[skillid].flag = 13;//cloneskill flag
			pc_setglobalreg(tsd, "CLONE_SKILL", tsd->cloneskill_id);
			pc_setglobalreg(tsd, "CLONE_SKILL_LV", tsd->status.skill[skillid].lv);
			clif_skillinfoblock(tsd);
		}
	}
	if (skillid != WZ_HEAVENDRIVE && bl->type == BL_SKILL && damage > 0) {
		struct skill_unit* su = (struct skill_unit*)bl;
		if (su->group && skill_get_inf(su->group->skill_id)&INF2_TRAP)
			damage = 0; //Only Heaven's drive may damage traps. [Skotlex]
	}
	if ((skillid || flag) && !(attack_type&BF_WEAPON)) {  // do not really deal damage for ASC_BREAKER's 1st attack
		battle_damage(src,bl,damage, 0); //Deal damage before knockback to allow stuff like firewall+storm gust combo.
		if (!status_isdead(bl) && (dmg.dmg_lv == ATK_DEF || damage > 0))
			skill_additional_effect(src,bl,skillid,skilllv,attack_type,tick);
	}

	//Only knockback if it's still alive, otherwise a "ghost" is left behind. [Skotlex]
	if (dmg.blewcount > 0 && !status_isdead(bl))
		skill_blown(dsrc,bl,dmg.blewcount);
	
	//Delayed damage must be dealt after the knockback (it needs to know actual position of target)
	if ((skillid || flag) && attack_type&BF_WEAPON && skillid != ASC_BREAKER) {  // do not really deal damage for ASC_BREAKER's 1st attack
			battle_delay_damage(tick+dmg.amotion,src,bl,attack_type,skillid,skilllv,damage,dmg.dmg_lv,0);
	}

	if(skillid == RG_INTIMIDATE && damage > 0 && !(status_get_mode(bl)&MD_BOSS)/* && !map_flag_gvg(src->m)*/) {
		int s_lv = status_get_lv(src),t_lv = status_get_lv(bl);
		int rate = 50 + skilllv * 5;
		rate = rate + (s_lv - t_lv);
		if(rand()%100 < rate)
			skill_addtimerskill(src,tick + 800,bl->id,0,0,skillid,skilllv,0,flag);
	}

	if (dmg.dmg_lv == ATK_DEF || damage > 0) //Counter status effects [Skotlex] 
		skill_counter_additional_effect(dsrc,bl,skillid,skilllv,attack_type,tick);
	
	/* ダ�??ジがあるなら追加?果判定 */	
	if(!status_isdead(bl) && bl->type==BL_MOB && src!=bl)	/* スキル使用?件のMOBスキル */
	{
		struct mob_data *md=(struct mob_data *)bl;
//		nullpo_retr(0, md); //Just so you know.. these are useless. When you cast a pointer, the pointer still is the same, so if bl is not null, the after-casted pointer will never be nulll :/ [Skotlex]
		if(battle_config.mob_changetarget_byskill && sd)
		{
			int target ;
			target=md->target_id;
			md->target_id=src->id;
			mobskill_use(md,tick,MSC_SKILLUSED|(skillid<<16));
			md->target_id=target;
		}
		else
			mobskill_use(md,tick,MSC_SKILLUSED|(skillid<<16));
	}

	if(sd && dmg.flag&BF_WEAPON && src != bl && src == dsrc && damage > 0) {
		int hp = 0,sp = 0;
		if(sd->right_weapon.hp_drain_rate && sd->right_weapon.hp_drain_per > 0 && dmg.damage > 0 && rand()%1000 < sd->right_weapon.hp_drain_rate) {
			hp += (dmg.damage * sd->right_weapon.hp_drain_per)/100;
			if(sd->right_weapon.hp_drain_rate > 0 && hp < 1) hp = 1;
			else if(sd->right_weapon.hp_drain_rate < 0 && hp > -1) hp = -1;
		}
		if(sd->left_weapon.hp_drain_rate && sd->left_weapon.hp_drain_per > 0 && dmg.damage2 > 0 && rand()%1000 < sd->left_weapon.hp_drain_rate) {
			hp += (dmg.damage2 * sd->left_weapon.hp_drain_per)/100;
			if(sd->left_weapon.hp_drain_rate > 0 && hp < 1) hp = 1;
			else if(sd->left_weapon.hp_drain_rate < 0 && hp > -1) hp = -1;
		}
		if(sd->right_weapon.sp_drain_rate > 0 && sd->right_weapon.sp_drain_per > 0 && dmg.damage > 0 && rand()%1000 < sd->right_weapon.sp_drain_rate) {
			sp += (dmg.damage * sd->right_weapon.sp_drain_per)/100;
			if(sd->right_weapon.sp_drain_rate > 0 && sp < 1) sp = 1;
			else if(sd->right_weapon.sp_drain_rate < 0 && sp > -1) sp = -1;
		}
		if(sd->left_weapon.sp_drain_rate > 0 && sd->left_weapon.sp_drain_per > 0 && dmg.damage2 > 0 && rand()%1000 < sd->left_weapon.sp_drain_rate) {
			sp += (dmg.damage2 * sd->left_weapon.sp_drain_per)/100;
			if(sd->left_weapon.sp_drain_rate > 0 && sp < 1) sp = 1;
			else if(sd->left_weapon.sp_drain_rate < 0 && sp > -1) sp = -1;
		}
		if(hp || sp)
			pc_heal(sd,hp,sp);
		if (sd->sp_drain_type && bl->type == BL_PC)
			battle_heal(NULL,bl,0,-sp,0);
	}

	if (/*(skillid || flag) &&*/ rdamage>0) { //Is the skillid/flag check really necessary? [Skotlex]
		if (attack_type&BF_WEAPON)
			battle_delay_damage(tick+dmg.amotion,bl,src,0,0,0,rdamage,ATK_DEF,0);
		else
			battle_damage(bl,src,rdamage,0);
		clif_damage(src,src,tick, dmg.amotion,0,rdamage,1,4,0);
	}

	if (!(flag & 1) &&
		(	
			skillid == MG_COLDBOLT || skillid == MG_FIREBOLT || skillid == MG_LIGHTNINGBOLT
		) &&
		(sc_data = status_get_sc_data(src)) &&
		sc_data[SC_DOUBLECAST].timer != -1 &&
		rand() % 100 < 40+10*sc_data[SC_DOUBLECAST].val1)
	{
		skill_castend_delay (src, bl, skillid, skilllv, tick + dmg.div_*dmg.amotion, flag|1);
	}

	map_freeblock_unlock();

	return (dmg.damage+dmg.damage2);	/* ?ダ�?を返す */
}

/*==========================================
 * スキル範??U?用(map_foreachinareaから呼ばれる)
 * flagについて?F16?i?を確認
 * MSB <- 00fTffff ->LSB
 *	T	=タ?ゲット選?用(BCT_*)
 *  ffff=自由に使用可能
 *  0	=予約?B0に固定
 *------------------------------------------
 */
static int skill_area_temp[8];	/* 一時???B必要なら使う?B */
static int skill_unit_temp[8];	/* For storing skill_unit ids as players move in/out of them. [Skotlex] */
static int skill_unit_index=0;	//Well, yeah... am too lazy to pass pointers around :X
typedef int (*SkillFunc)(struct block_list *,struct block_list *,int,int,unsigned int,int);
int skill_area_sub( struct block_list *bl,va_list ap )
{
	struct block_list *src;
	int skill_id,skill_lv,flag;
	unsigned int tick;
	SkillFunc func;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	if(bl->type!=BL_PC && bl->type!=BL_MOB && bl->type!=BL_SKILL)
		return 0;

	src=va_arg(ap,struct block_list *); //ここではsrcの値を??ﾆしていないのでNULLチェックはしない
	skill_id=va_arg(ap,int);
	skill_lv=va_arg(ap,int);
	tick=va_arg(ap,unsigned int);
	flag=va_arg(ap,int);
	func=va_arg(ap,SkillFunc);

	if(battle_check_target(src,bl,flag) > 0)
		func(src,bl,skill_id,skill_lv,tick,flag);
	return 0;
}

static int skill_check_unit_range_sub( struct block_list *bl,va_list ap )
{
	struct skill_unit *unit;
	int *c;
	int skillid,g_skillid;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, unit = (struct skill_unit *)bl);
	nullpo_retr(0, c = va_arg(ap,int *));

	if(bl->prev == NULL || bl->type != BL_SKILL)
		return 0;

	if(!unit->alive)
		return 0;

	skillid = va_arg(ap,int);
	g_skillid = unit->group->skill_id;

	switch (skillid)
	{
		case MG_SAFETYWALL:
		case AL_PNEUMA:
			if(g_skillid != MG_SAFETYWALL && g_skillid != AL_PNEUMA)
				return 0;
			break;
		case AL_WARP:
		case HT_SKIDTRAP:
		case HT_LANDMINE:
		case HT_ANKLESNARE:
		case HT_SHOCKWAVE:
		case HT_SANDMAN:
		case HT_FLASHER:
		case HT_FREEZINGTRAP:
		case HT_BLASTMINE:
		case HT_CLAYMORETRAP:
		case HT_TALKIEBOX:
		case HP_BASILICA:
			//Non stackable on themselves and traps (including venom dust which does not has the trap inf2 set)
			if (skillid != g_skillid && !(skill_get_inf2(g_skillid)&INF2_TRAP) && g_skillid != AS_VENOMDUST)
				return 0;
			break;
		default: //Avoid stacking with same kind of trap. [Skotlex]
			if (g_skillid != skillid)
				return 0;
			break;
	}

	(*c)++;

	return 1;
}

int skill_check_unit_range(int m,int x,int y,int skillid,int skilllv)
{
	int c = 0;
	int range = skill_get_unit_range(skillid);
	int layout_type = skill_get_unit_layout_type(skillid,skilllv);
	if (layout_type==-1 || layout_type>MAX_SQUARE_LAYOUT) {
		ShowError("skill_check_unit_range: unsupported layout type %d for skill %d\n",layout_type,skillid);
		return 0;
	}

	// とりあえず?ｳ方形のユニットレイアウトのみ対応
	range += layout_type;
	map_foreachinarea(skill_check_unit_range_sub,m,
			x-range,y-range,x+range,y+range,BL_SKILL,&c,skillid);

	return c;
}

static int skill_check_unit_range2_sub( struct block_list *bl,va_list ap )
{
	int *c;
	int skillid;


	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, c = va_arg(ap,int *));

	if(bl->prev == NULL || (bl->type != BL_PC && bl->type != BL_MOB))
		return 0;

	if(status_isdead(bl))
		return 0;

	skillid = va_arg(ap,int);
	if (skillid==HP_BASILICA && bl->type==BL_PC)
		return 0;

	if (skillid==AM_DEMONSTRATION && bl->type==BL_MOB && ((struct mob_data*)bl)->class_ == MOBID_EMPERIUM)
		return 0; //Allow casting Bomb/Demonstration Right under emperium [Skotlex]
	
	(*c)++;

	return 0;
}

int skill_check_unit_range2(struct block_list *bl, int m,int x,int y,int skillid, int skilllv)
{
	int c = 0, range, type;

	switch (skillid) {	// to be expanded later
	case WZ_ICEWALL:
		range = 2;
		break;
	default:
		{
			int layout_type = skill_get_unit_layout_type(skillid,skilllv);
			if (layout_type==-1 || layout_type>MAX_SQUARE_LAYOUT) {
				ShowError("skill_check_unit_range2: unsupported layout type %d for skill %d\n",layout_type,skillid);
				return 0;
			}
			// とりあえず?ｳ方形のユニットレイアウトのみ対応
			range = skill_get_unit_range(skillid) + layout_type;
		}
		break;
	}

	// if the caster is a monster/NPC, only check for players
	// otherwise just check everything
	if (bl->type == BL_PC)
		type = 0;
	else type = BL_PC;

	map_foreachinarea(skill_check_unit_range2_sub, m,
		x - range, y - range, x + range, y + range,
		type, &c, skillid);

	return c;
}

int skill_guildaura_sub (struct block_list *bl,va_list ap)
{
	struct map_session_data *sd;
	int gid, id, *flag;
	
	nullpo_retr(0, sd = (struct map_session_data *)bl);
	nullpo_retr(0, ap);

	id = va_arg(ap,int);
	gid = va_arg(ap,int);
	if (sd->status.guild_id != gid)
		return 0;
	nullpo_retr(0, flag = va_arg(ap,int *));

	if (flag && *flag > 0) {
		if (sd->sc_count && sd->sc_data[SC_GUILDAURA].timer != -1) {
			if (sd->sc_data[SC_GUILDAURA].val4 != *flag) {
				sd->sc_data[SC_GUILDAURA].val4 = *flag;
				status_calc_pc (sd, 0);
			}
			return 0;
		}
		status_change_start(&sd->bl, SC_GUILDAURA, 1, id, 0, *flag, 0, 0);
	}

	return 0;
}

/*=========================================================================
 * 範?スキル使用?�??ｬ分けここから
 */
/* ??ﾛの?をカウントする?B?iskill_area_temp[0]を?炎�化しておくこと?j */
int skill_area_sub_count(struct block_list *src,struct block_list *target,int skillid,int skilllv,unsigned int tick,int flag)
{
	//FIXME: Counting is not currently used by any skills that need to target "skills", and this count causes problems with traps.
	//Since traps are universal targets, they are being counted in Blitz Beats.... and people don't want that :/ [Skotlex]
	if(target->type == BL_SKILL)
		return 0;
	
	if(skill_area_temp[0] < 0xffff)
		skill_area_temp[0]++;
	return 1;
}

int skill_count_water(struct block_list *src,int range)
{
	int i,x,y,cnt = 0,size = range*2+1;
	struct skill_unit *unit;

	for (i=0;i<size*size;i++) {
		x = src->x+(i%size-range);
		y = src->y+(i/size-range);
		if (map_getcell(src->m,x,y,CELL_CHKWATER)) {
			cnt++;
			continue;
		}
		unit = map_find_skill_unit_oncell(src,x,y,SA_DELUGE,NULL);
		if (unit) {
			cnt++;
			skill_delunit(unit);
		}
	}
	return cnt;
}

/*==========================================
 *
 *------------------------------------------
 */
static int skill_timerskill(int tid, unsigned int tick, int id,int data )
{
	struct map_session_data *sd = NULL;
	struct mob_data *md = NULL;
	struct pet_data *pd = NULL;
	struct block_list *src = map_id2bl(id),*target;
	struct skill_timerskill *skl = NULL;
	int range;

	nullpo_retr(0, src);

	if(src->type == BL_PC) {
		nullpo_retr(0, sd = (struct map_session_data *)src);
		skl = &sd->skilltimerskill[data];
	}
	else if(src->type == BL_MOB) {
		nullpo_retr(0, md = (struct mob_data *)src);
		skl = &md->skilltimerskill[data];
	}
	else if(src->type == BL_PET) { // [Valaris]
		nullpo_retr(0, pd = (struct pet_data *)src);
		skl = &pd->skilltimerskill[data];
	}

	else
		return 0;

	nullpo_retr(0, skl);

	skl->timer = -1;

	if (sd) {
		sd->timerskill_count--;
	}

	//Check moved here because otherwise the timer is not reset to -1 and later on we'll see problems when clearing. [Skotlex]
	if(src->prev == NULL)
		return 0;

	if(skl->target_id) {
		struct block_list tbl;
		target = map_id2bl(skl->target_id);
		if(skl->skill_id == RG_INTIMIDATE) {
			if(target == NULL) {
				target = &tbl; //?炎�化してないのにアドレス突っ?んでいいのかな?H
				target->type = BL_NUL;
				target->m = src->m;
				target->prev = target->next = NULL;
			}
		}
		if(target == NULL)
			return 0;

		if(target->prev == NULL && skl->skill_id != RG_INTIMIDATE)
			return 0;
		if(src->m != target->m)
			return 0;
		if(sd && pc_isdead(sd))
			return 0;
		if(target->type == BL_PC && pc_isdead((struct map_session_data *)target) && skl->skill_id != RG_INTIMIDATE)
			return 0;

		switch(skl->skill_id) {
			case RG_INTIMIDATE:
				if(sd && !map[src->m].flag.noteleport) {
					int x,y,i,j;
					pc_randomwarp(sd,3);
					for(i=0;i<16;i++) {
						j = rand()%8;
						x = sd->bl.x + dirx[j];
						y = sd->bl.y + diry[j];
						if(map_getcell(sd->bl.m,x,y,CELL_CHKPASS))
							break;
					}
					if(i >= 16) {
						x = sd->bl.x;
						y = sd->bl.y;
					}
					if(target->prev != NULL) {
						if(target->type == BL_PC && !pc_isdead((struct map_session_data *)target))
							pc_setpos((struct map_session_data *)target,map[sd->bl.m].name,x,y,3);
						else if(target->type == BL_MOB)
							mob_warp((struct mob_data *)target,-1,x,y,3);
					}
				}
				else if(md && !map[src->m].flag.monster_noteleport) {
					int x,y,i,j;
					mob_warp(md,-1,-1,-1,3);
					for(i=0;i<16;i++) {
						j = rand()%8;
						x = md->bl.x + dirx[j];
						y = md->bl.y + diry[j];
						if(map_getcell(md->bl.m,x,y,CELL_CHKPASS))
							break;
					}
					if(i >= 16) {
						x = md->bl.x;
						y = md->bl.y;
					}
					if(target->prev != NULL) {
						if(target->type == BL_PC && !pc_isdead((struct map_session_data *)target))
							pc_setpos((struct map_session_data *)target,map[md->bl.m].name,x,y,3);
						else if(target->type == BL_MOB)
							mob_warp((struct mob_data *)target,-1,x,y,3);
					}
				}
				break;

			case BA_FROSTJOKE:			/* 寒いジョ?ク */
			case DC_SCREAM:				/* スクリ?ム */
				range=battle_config.area_size;		//視界全?
				map_foreachinarea(skill_frostjoke_scream,src->m,src->x-range,src->y-range,
					src->x+range,src->y+range,0,src,skl->skill_id,skl->skill_lv,tick);
				break;

			case WZ_WATERBALL:
				if (skl->type>1) {
					skl->timer = 0;	// skill_addtimerskillで使用されないように
					skill_addtimerskill(src,tick+150,target->id,0,0,skl->skill_id,skl->skill_lv,skl->type-1,skl->flag);
					skl->timer = -1;
				}
				skill_attack(BF_MAGIC,src,src,target,skl->skill_id,skl->skill_lv,tick,skl->flag);
				if (skl->type <= 1) {	// partial fix: it still doesn't end if the target dies
					// should put outside of the switch, but since this is the only
					// mage targetted spell for now,
					struct status_change *sc_data = status_get_sc_data(src);
					if(sc_data && sc_data[SC_MAGICPOWER].timer != -1)	//マジックパ�??の?果?I了
						status_change_end(src,SC_MAGICPOWER,-1);
				}
				break;
			default:
				skill_attack(skl->type,src,src,target,skl->skill_id,skl->skill_lv,tick,skl->flag);
				break;
		}
	}
	else {
		if(src->m != skl->map)
			return 0;
		switch(skl->skill_id) {
			case WZ_METEOR:
				if(skl->type >= 0) {
					skill_unitsetting(src,skl->skill_id,skl->skill_lv,skl->type>>16,skl->type&0xFFFF,skl->flag);
					clif_skill_poseffect(src,skl->skill_id,skl->skill_lv,skl->x,skl->y,tick);
				}
				else
					skill_unitsetting(src,skl->skill_id,skl->skill_lv,skl->x,skl->y,skl->flag);
				break;
		}
	}

	return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int skill_addtimerskill(struct block_list *src,unsigned int tick,int target,int x,int y,int skill_id,int skill_lv,int type,int flag)
{
	int i;

	nullpo_retr(1, src);

	if(src->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)src;
		nullpo_retr(1, sd);
		for(i=0;i<MAX_SKILLTIMERSKILL;i++) {
			if(sd->skilltimerskill[i].timer == -1) {
				sd->skilltimerskill[i].timer = add_timer(tick, skill_timerskill, src->id, i);
				sd->skilltimerskill[i].src_id = src->id;
				sd->skilltimerskill[i].target_id = target;
				sd->skilltimerskill[i].skill_id = skill_id;
				sd->skilltimerskill[i].skill_lv = skill_lv;
				sd->skilltimerskill[i].map = src->m;
				sd->skilltimerskill[i].x = x;
				sd->skilltimerskill[i].y = y;
				sd->skilltimerskill[i].type = type;
				sd->skilltimerskill[i].flag = flag;
				sd->timerskill_count++;

				return 0;
			}
		}
		return 1;
	}
	else if(src->type == BL_MOB) {
		struct mob_data *md = (struct mob_data *)src;
		nullpo_retr(1, md);
		for(i=0;i<MAX_MOBSKILLTIMERSKILL;i++) {
			if(md->skilltimerskill[i].timer == -1) {
				md->skilltimerskill[i].timer = add_timer(tick, skill_timerskill, src->id, i);
				md->skilltimerskill[i].src_id = src->id;
				md->skilltimerskill[i].target_id = target;
				md->skilltimerskill[i].skill_id = skill_id;
				md->skilltimerskill[i].skill_lv = skill_lv;
				md->skilltimerskill[i].map = src->m;
				md->skilltimerskill[i].x = x;
				md->skilltimerskill[i].y = y;
				md->skilltimerskill[i].type = type;
				md->skilltimerskill[i].flag = flag;

				return 0;
			}
		}
		return 1;
	}
	else if(src->type == BL_PET) { // [Valaris]
		struct pet_data *pd = (struct pet_data *)src;
		nullpo_retr(1, pd);
		for(i=0;i<MAX_MOBSKILLTIMERSKILL;i++) {
			if(pd->skilltimerskill[i].timer == -1) {
				pd->skilltimerskill[i].timer = add_timer(tick, skill_timerskill, src->id, i);
				pd->skilltimerskill[i].src_id = src->id;
				pd->skilltimerskill[i].target_id = target;
				pd->skilltimerskill[i].skill_id = skill_id;
				pd->skilltimerskill[i].skill_lv = skill_lv;
				pd->skilltimerskill[i].map = src->m;
				pd->skilltimerskill[i].x = x;
				pd->skilltimerskill[i].y = y;
				pd->skilltimerskill[i].type = type;
				pd->skilltimerskill[i].flag = flag;

				return 0;
			}
		}
		return 1;
	}
	return 1;
}

/*==========================================
 *
 *------------------------------------------
 */
int skill_cleartimerskill(struct block_list *src)
{
	int i;

	nullpo_retr(0, src);

	if(src->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)src;
		nullpo_retr(0, sd);

		if (sd->timerskill_count <= 0)
			return 0;

		for(i=0;i<MAX_SKILLTIMERSKILL && sd->timerskill_count > 0;i++) {
			if(sd->skilltimerskill[i].timer != -1) {
				delete_timer(sd->skilltimerskill[i].timer, skill_timerskill);
				sd->skilltimerskill[i].timer = -1;
				sd->timerskill_count--;
			}
		}
	}
	else if(src->type == BL_MOB) {
		struct mob_data *md = (struct mob_data *)src;
		nullpo_retr(0, md);
		for(i=0;i<MAX_MOBSKILLTIMERSKILL;i++) {
			if(md->skilltimerskill[i].timer != -1) {
				delete_timer(md->skilltimerskill[i].timer, skill_timerskill);
				md->skilltimerskill[i].timer = -1;
			}
		}
	}
	else if(src->type == BL_PET) { // Ya forgot this one, Valaris. [Skotlex]
		struct pet_data *pd = (struct pet_data *)src;
		nullpo_retr(1, pd);
		for(i=0;i<MAX_MOBSKILLTIMERSKILL;i++) {
			if(pd->skilltimerskill[i].timer != -1) {
				delete_timer(pd->skilltimerskill[i].timer, skill_timerskill);
				pd->skilltimerskill[i].timer = -1;
			}
		}
	}
	return 0;
}

struct castend_delay {
	struct block_list *src;
	int target;
	int id;
	int lv;
	int flag;
};
int skill_castend_delay_sub (int tid, unsigned int tick, int id, int data)
{
	struct castend_delay *dat = (struct castend_delay *)data;
	struct block_list *target = map_id2bl(dat->target);
	
	if (target && dat && map_id2bl(id) == dat->src && target->prev != NULL)
		skill_castend_damage_id(dat->src, target, dat->id, dat->lv, tick, dat->flag);
	aFree(dat);
	return 0;
}
int skill_castend_delay (struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag)
{
	struct castend_delay *dat;
	nullpo_retr(0, src);
	nullpo_retr(0, bl);

	dat = (struct castend_delay *)aCalloc(1, sizeof(struct castend_delay));
	dat->src = src;
	dat->target = bl->id;
	dat->id = skillid;
	dat->lv = skilllv;
	dat->flag = flag;
	add_timer (tick, skill_castend_delay_sub, src->id, (int)dat);

	return 0;
}

/* 範?スキル使用?�??ｬ分けここまで
 * -------------------------------------------------------------------------
 */

/*==========================================
 * スキル使用?i詠?･完了?AID指定?U?系?j
 * ?iスパゲッティに向けて１?前?i?I(ダ�?ポ)?j
 *------------------------------------------
 */
int skill_castend_damage_id (struct block_list* src, struct block_list *bl,int skillid,int skilllv,unsigned int tick,int flag)
{
	struct map_session_data *sd = NULL, *tsd = NULL;
	struct status_change *sc_data;

	if(skillid < 0)	
	{	// remove the debug print when this case is finished
		ShowDebug("skill_castend_damage_id: skillid=%i\ncall: %p %p %i %i %i %i",skillid,
						src, bl,skillid,skilllv,tick,flag);
		return 0;
	}
	if (skillid > 0 && skilllv <= 0) return 0;

	nullpo_retr(1, src);
	nullpo_retr(1, bl);	

	if (src->m != bl->m)
		return 1;
	
	if (src->type == BL_PC) {
		nullpo_retr(1, sd = (struct map_session_data *)src);
	}
	if (sd && pc_isdead(sd))
		return 1;

	if (bl->prev == NULL)
		return 1;
	if (bl->type == BL_PC) {
		nullpo_retr(1, tsd = (struct map_session_data *)bl);
	}
	if (tsd && pc_isdead(tsd))
		return 1;

	if ((skillid == CR_GRANDCROSS || skillid == NPC_GRANDDARKNESS) && src != bl)
		bl = src;
	sc_data = status_get_sc_data(src);
	
	map_freeblock_lock();

	switch(skillid)
	{
	/* �?器?U?系スキル */
	case SM_BASH:			/* バッシュ */
	case MC_MAMMONITE:		/* �?マ?ナイト */
	case AC_DOUBLE:			/* ダブルストレイフィング */
	case AC_SHOWER:			/* ア�??シャ�?? */
	case AS_SONICBLOW:		/* ソニックブ�?? */
	case KN_PIERCE:			/* ピア?ス */
	case KN_SPEARBOOMERANG:	/* スピアブ?�?ラン */
	case KN_BRANDISHSPEAR:		/* ブランディッシュスピア */
	case TF_POISON:			/* インベナム */
	case TF_SPRINKLESAND:	/* ?ｻまき */
	case AC_CHARGEARROW:	/* チャ?ジア�?? */
	case RG_RAID:		/* サプライズアタック */
	case RG_INTIMIDATE:		/* インティミデイト */
	case AM_ACIDTERROR:		/* アシッドテラ? */
	case BA_MUSICALSTRIKE:	/* ミュ?ジカルストライク */
	case DC_THROWARROW:		/* 矢?ち */
	case BA_DISSONANCE:		/* 不協和音 */
	case CR_HOLYCROSS:		/* ホ?リ?ク�?ス */
	case NPC_DARKCROSS:
	case CR_SHIELDCHARGE:
	case CR_SHIELDBOOMERANG:
	/* 以下MOB?用 */
	/* ???U??ASP減?ｭ?U??A遠距離?U??A防御無視?U??A多段?U? */
	case NPC_PIERCINGATT:
	case NPC_MENTALBREAKER:
	case NPC_RANGEATTACK:
	case NPC_CRITICALSLASH:
	case NPC_COMBOATTACK:
	/* 必中?U??A毒?U??A暗??U??A沈??U??Aスタン?U? */
	case NPC_GUIDEDATTACK:
	case NPC_POISON:
	case NPC_BLINDATTACK:
	case NPC_SILENCEATTACK:
	case NPC_STUNATTACK:
	/* ?ﾎ化?U??A呪い?U??A?�眠?U??AランダムATK?U? */
	case NPC_PETRIFYATTACK:
	case NPC_CURSEATTACK:
	case NPC_SLEEPATTACK:
	case NPC_RANDOMATTACK:
	/* ?�??ｫ?U??A地??ｫ?U??A火??ｫ?U??A風??ｫ?U? */
	case NPC_WATERATTACK:
	case NPC_GROUNDATTACK:
	case NPC_FIREATTACK:
	case NPC_WINDATTACK:
	/* 毒??ｫ?U??A?ｹ??ｫ?U??A闇??ｫ?U??A念??ｫ?U??ASP減?ｭ?U? */
	case NPC_POISONATTACK:
	case NPC_HOLYATTACK:
	case NPC_DARKNESSATTACK:
	case NPC_TELEKINESISATTACK:
	case NPC_UNDEADATTACK:
	case NPC_BREAKARMOR:
	case NPC_BREAKWEAPON:
	case NPC_BREAKHELM:
	case NPC_BREAKSHIELD:
	case LK_AURABLADE:		/* オ?ラブレ?ド */
	case LK_SPIRALPIERCE:	/* スパイラルピア?ス */
	case LK_HEADCRUSH:	/* ヘッドクラッシュ */
	case LK_JOINTBEAT:	/* ジョイントビ?ト */
	case CG_ARROWVULCAN:			/* ア�??バルカン */
	case HW_MAGICCRASHER:		/* マジッククラッシャ? */
	case ASC_METEORASSAULT:	/* �?テオアサルト */
	case ITM_TOMAHAWK:
	case MO_TRIPLEATTACK:
	case MO_COMBOFINISH:	/* 猛龍�? */
	case CH_CHAINCRUSH:		/* 連柱崩? */
	case CH_TIGERFIST:		/* 伏虎�? */
	case PA_SHIELDCHAIN:	// Shield Chain
	case PA_SACRIFICE:	// Sacrifice, Aru's style.
	case WS_CARTTERMINATION:	// Cart Termination
	case AS_VENOMKNIFE:
	case HT_PHANTASMIC:
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		break;

	case TK_STORMKICK: // Taekwon kicks [Dralnu]
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		map_foreachinarea(skill_attack_area, src->m,
			src->x-2, src->y-2, src->x+2, src->y+2, 0,
			BF_WEAPON, src, src, skillid, skilllv, tick, flag, BCT_ENEMY);	
		if (sc_data && sc_data[SC_COMBO].timer != -1)
			status_change_end(src, SC_COMBO, -1);
		break;

	case TK_DOWNKICK:
	case TK_COUNTER:
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			if (sc_data && sc_data[SC_COMBO].timer != -1)
				status_change_end(src, SC_COMBO, -1);
      	break;
	case TK_JUMPKICK:
		if(sd) {
			if (!pc_can_move(sd))
				return 0;
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			pc_movepos(sd,bl->x,bl->y,0); 
			clif_slide(src,bl->x,bl->y);
		}
		break;
	case ASC_BREAKER:				/* ソウルブレ?カ? */	// [DracoRPG]
		// Separate weapon and magic attacks
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		break;
	
	case SN_SHARPSHOOTING:			/* シャ?プシュ?ティング */
		// Does it stop if touch an obstacle? it shouldn't shoot trough walls
			map_foreachinpath (skill_attack_area,src->m,					// function, map
					src->x,src->y,											// source xy
					bl->x,bl->y,											// target xy
					2,0,													// range, type
					BF_WEAPON,src,src,skillid,skilllv,tick,flag,BCT_ENEMY);	// varargs		
		break;

	case NPC_DARKBREATH:
		clif_emotion(src,7);
		skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,flag);
		break;

	case MO_INVESTIGATE:	/* ?勁 */
		skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
		if (sc_data && sc_data[SC_BLADESTOP].timer != -1)
			status_change_end(src,SC_BLADESTOP,-1);
		break;

	case SN_FALCONASSAULT:			/* ファルコンアサルト */
	case PA_PRESSURE:	/* プレッシャ? */
	case CR_ACIDDEMONSTRATION:  // Acid Demonstration
		skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,flag);
		break;

	case RG_BACKSTAP:		/* バックスタブ */
		{
			int dir = map_calc_dir(src, bl->x, bl->y), t_dir = status_get_dir(bl);
			int dist = distance(src->x, src->y, bl->x, bl->y);
			if ((dist > 0 && !map_check_dir(dir, t_dir)) || bl->type == BL_SKILL) {
				if (sc_data && sc_data[SC_HIDING].timer != -1)
					status_change_end(src, SC_HIDING, -1);	// ハイディング解?�
				skill_attack(BF_WEAPON, src, src, bl, skillid, skilllv, tick, flag);
				dir = dir < 4 ? dir+4 : dir-4; // change direction [Celest]
				if (tsd)
					tsd->dir = dir;
				else if (bl->type == BL_MOB) {
					struct mob_data *tmd = (struct mob_data *)bl;
					if (tmd) tmd->dir = dir;
				}
				clif_changed_dir(bl);
			}
			else if (sd)
				clif_skill_fail(sd,skillid,0,0);
		}
		break;

	case MO_FINGEROFFENSIVE:	/* 指? */
		{
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			if (battle_config.finger_offensive_type && sd) {
				int i;
				for (i = 1; i < sd->spiritball_old; i++)
					skill_addtimerskill(src, tick + i * 200, bl->id, 0, 0, skillid, skilllv, BF_WEAPON, flag);
				sd->canmove_tick = tick + (sd->spiritball_old - 1) * 200;
			}
			if (sc_data && sc_data[SC_BLADESTOP].timer != -1)
				status_change_end(src,SC_BLADESTOP,-1);
		}
		break;

	case MO_CHAINCOMBO:		/* 連打?ｶ */
		{
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			if (sc_data && sc_data[SC_BLADESTOP].timer != -1)
				status_change_end(src,SC_BLADESTOP,-1);
		}
		break;
	
	case KN_CHARGEATK:
	case MO_EXTREMITYFIST:	/* 阿?C羅覇鳳�? */
		{
			if(sd) {
				struct walkpath_data wpd;
				int dx,dy;

				dx = bl->x - sd->bl.x;
				dy = bl->y - sd->bl.y;
				if(dx > 0) dx++;
				else if(dx < 0) dx--;
				if (dy > 0) dy++;
				else if(dy < 0) dy--;
				if(dx == 0 && dy == 0) dx++;
				if (path_search(&wpd,src->m,sd->bl.x,sd->bl.y,sd->bl.x+dx,sd->bl.y+dy,1) == -1) {
					dx = bl->x - sd->bl.x;
					dy = bl->y - sd->bl.y;
					if(path_search(&wpd,src->m,sd->bl.x,sd->bl.y,sd->bl.x+dx,sd->bl.y+dy,1) == -1) {
						clif_skill_fail(sd,skillid,0,0);
						break;
					}
				}
				sd->to_x = sd->bl.x + dx;
				sd->to_y = sd->bl.y + dy;
				if (skillid == KN_CHARGEATK) //Store distance in flag [Skotlex]
					flag = wpd.path_len; //Path_len is a pretty good approximate of the distance.
				if (skillid != MO_EXTREMITYFIST || battle_check_target(src, bl, BCT_ENEMY) > 0) //Check must be done here because EF should be broken this way.. [Skotlex]
					skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
				else
					clif_skill_fail(sd,skillid,0,0);
				clif_walkok(sd);
				clif_movechar(sd);
				if(dx < 0) dx = -dx;
				if(dy < 0) dy = -dy;
				sd->attackabletime = sd->canmove_tick = tick + 100 + sd->speed * ((dx > dy)? dx:dy);
				if(sd->canact_tick < sd->canmove_tick)
					sd->canact_tick = sd->canmove_tick;
				pc_movepos(sd,sd->to_x,sd->to_y,1);
				if (skillid == MO_EXTREMITYFIST)
					status_change_end(&sd->bl,SC_COMBO,-1);
			}
			else
				skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,flag);
			if (skillid == MO_EXTREMITYFIST)
			{
				status_change_end(src, SC_EXPLOSIONSPIRITS, -1);
				if (sc_data && sc_data[SC_BLADESTOP].timer != -1)
					status_change_end(src,SC_BLADESTOP,-1);
			}
		}
		break;

	/* �?器系範??U?スキル */
	case AS_GRIMTOOTH:		/* グリムトゥ?ス */
	case MC_CARTREVOLUTION:	/* カ?トレヴォリュ?ション */
	case NPC_SPLASHATTACK:	/* スプラッシュアタック */
		if(flag&1){
			/* 個別にダ�??ジを?える */
			if(bl->id!=skill_area_temp[1]){
				skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,
					0x0500);				
			}
		} else {
			int ar = 1;
			int x = bl->x, y = bl->y;
			switch (skillid) {
				case NPC_SPLASHATTACK:
					ar=3;
					break;
			}

			skill_area_temp[1]=bl->id;
			skill_area_temp[2]=x;
			skill_area_temp[3]=y;
			/* まずタ?ゲットに?U?を加える */
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0);
			/* その後タ?ゲット以外の範??の敵全?に?�?を?sう */
			map_foreachinarea(skill_area_sub,
				bl->m,x-ar,y-ar,x+ar,y+ar,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;

	case AS_SPLASHER:
		if (flag & 1) {	//Invoked from map_foreachinarea, skill_area_temp[0] holds number of targets to divide damage by.
			if (bl->id != skill_area_temp[1])
				skill_attack(BF_WEAPON, src, src, bl, skillid, skilllv, tick, skill_area_temp[0]);
			else
				skill_attack(BF_WEAPON, src, src, bl, skillid, skilllv, tick, 0);
		} else {
			skill_area_temp[0] = 0;
			skill_area_temp[1] = bl->id;
			map_foreachinarea(skill_area_sub, bl->m, 
				bl->x-2, bl->y-2, bl->x+2, bl->y+2, 0,
				src, skillid, skilllv, tick, BCT_ENEMY, skill_area_sub_count);
			skill_area_temp[0]--; //Substract one, the original target shouldn't count. [Skotlex]
			map_foreachinarea(skill_area_sub, bl->m,
				bl->x-1, bl->y-1, bl->x+1, bl->y+1, 0,
				src, skillid, skilllv, tick, BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;


	case SM_MAGNUM:
		if(flag&1){
			int dist = distance (bl->x, bl->y, skill_area_temp[2], skill_area_temp[3]);
			skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,
				0x0500|dist);
		} else {
			skill_area_temp[1]=src->id;
			skill_area_temp[2]=src->x;
			skill_area_temp[3]=src->y;
			map_foreachinarea(skill_area_sub,
				src->m,src->x-2,src->y-2,src->x+2,src->y+2,0,
				src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
			clif_skill_nodamage (src,src,skillid,skilllv,1);
			status_change_start (src,SC_WATK_ELEMENT,3,20,0,0,10000,0); //Initiate 10% of your damage becomes fire element.
		}
		break;

	case KN_BOWLINGBASH:	/* ボウリングバッシュ */
		if(flag&1){
			/* 個別にダ�??ジを?える */
			if(bl->id!=skill_area_temp[1])
				skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0x0500);
		} else {
				int i,c;	/* 他?lから聞いた動きなので間違ってる可能?ｫ大?�?率が?いっす?�?� */
				/* まずタ?[ゲットに?U撃を加える */
				c = skill_get_blewcount(skillid,skilllv);
				if(map_flag_gvg(bl->m) || status_get_mexp(bl)) 
					c = 0;
				for(i=0;i<c;i++){
					skill_blown(src,bl,0x20000|1);
					skill_area_temp[0]=0;
					map_foreachinarea(skill_area_sub,
						bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
						src,skillid,skilllv,tick, flag|BCT_ENEMY ,
						skill_area_sub_count);
					if(skill_area_temp[0]>1) break;
				}
				clif_blown(bl); //Update target pos.
				skill_area_temp[1]=bl->id;
				/* その後タ?ゲット以外の範??の敵全?に?�?を?sう */
				map_foreachinarea(skill_area_sub,
					bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
					src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
					skill_castend_damage_id);
				skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0);
		}
		break;
	
	case KN_SPEARSTAB:		/* スピアスタブ */
		if(flag&1){
			/* 個別にダ�??[ジを与える */
			if (bl->id==skill_area_temp[1])
				break;
			if (skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0x0500) && !status_get_mexp(bl))
				skill_blown(src,bl,skill_area_temp[2]);
		} else {
			int x=bl->x,y=bl->y,i,dir;
			/* まずタ?[ゲットに?U撃を加える */
			dir = map_calc_dir(bl,src->x,src->y);
			skill_area_temp[1] = bl->id;
			skill_area_temp[2] = skill_get_blewcount(skillid,skilllv)|dir<<20;
			if (skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0) && !status_get_mexp(bl))
				skill_blown(src,bl,skill_area_temp[2]);
			for (i=0;i<4;i++) {
				map_foreachinarea(skill_area_sub,bl->m,x,y,x,y,0,
					src,skillid,skilllv,tick,flag|BCT_ENEMY|1,
					skill_castend_damage_id);
				x += dirx[dir];
				y += diry[dir];
			}
		}
		break;

	case TK_TURNKICK:
	case MO_BALKYOUNG: //Active part of the attack. Skill-attack [Skotlex]
	{
		skill_area_temp[1] = bl->id; //NOTE: This is used in skill_castend_nodamage_id to avoid affecting the target.
		if (skill_attack(BF_WEAPON,src,src,bl,skillid,skilllv,tick,0))
			map_foreachinarea(skill_area_sub,bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
				src,skillid,skilllv,tick,flag|BCT_ENEMY|1,
				skill_castend_nodamage_id);
	}
		break;	
	case CH_PALMSTRIKE: //	Palm Strike takes effect 1sec after casting. [Skotlex]
	//	clif_skill_nodamage(src,bl,skillid,skilllv,0); //Can't make this one display the correct attack animation delay :/
		clif_damage(src,bl,tick,status_get_amotion(src),0,0,1,4,0); //Displays MISS, but better than nothing :X
		skill_addtimerskill(src, tick + 1000, bl->id, 0, 0, skillid, skilllv, BF_WEAPON, flag);
		break;	

	case ALL_RESURRECTION:		/* リザレクション */
	case PR_TURNUNDEAD:			/* タ?ンアンデッド */
		if (bl->type != BL_PC && battle_check_undead(status_get_race(bl),status_get_elem_type(bl)))
			skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		else {
			map_freeblock_unlock();
			return 1;
		}
		break;

	/* 魔法系スキル */
	case MG_SOULSTRIKE:			/* ソウルストライク */
	case NPC_DARKSTRIKE:		/*闇ソウルストライク*/
	case MG_COLDBOLT:			/* コ?[ルドボルト */
	case MG_FIREBOLT:			/* ファイア?[ボルト */
	case MG_LIGHTNINGBOLT:		/* ライトニングボルト */
	case WZ_EARTHSPIKE:			/* ア?[ススパイク */
	case AL_HEAL:				/* ヒ?[ル */
	case AL_HOLYLIGHT:			/* ホ?[リ?[ライト */
	case WZ_JUPITEL:			/* ユピテルサンダ?[ */
	case NPC_DARKTHUNDER:			/*闇ユピテル*/
	case NPC_MAGICALATTACK:		/* MOB:魔法打??U? */
	case PR_ASPERSIO:			/* アスペルシオ */
	case MG_FROSTDIVER:		/* フ�?ストダイバ?[ */
	case WZ_SIGHTBLASTER:
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		break;

	case WZ_WATERBALL:			/* ウォ?タ?ボ?ル */
		skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,flag);
		if (skilllv>1) {
			int range = skilllv > 5 ? 2 : skilllv/2;
			//Rain doesn't affect WATERBALL (Rain has been removed at kRO) [Lupus]
			//int cnt = (!map[src->m].flag.rain) ? skill_count_water(src,range) - 1 : skill_get_num(skillid,skilllv) - 1;
			int cnt = (src->type==BL_PC)?skill_count_water(src,range) - 1:(skilllv>3?24:8);
			if (cnt > 0)
				skill_addtimerskill(src,tick+150,bl->id,0,0,
					skillid,skilllv,cnt,flag);
		}
		break;

	case PR_BENEDICTIO:			/* ?ｹ??~福 */
		if (status_get_race(bl) == 1 || status_get_race(bl) == 6)
			skill_attack(BF_MAGIC, src, src, bl, skillid, skilllv, tick, flag);
		break;

	/* 魔法系範??U?スキル */
	case MG_NAPALMBEAT:			/* ナパ?ムビ?ト */
	case MG_FIREBALL:			/* ファイヤ?ボ?ル */
	case WZ_SIGHTRASHER:		/* サイトラッシャ?[ */
		if (flag & 1) {
			/* 個別にダ�??ジを?える */
			if (bl->id != skill_area_temp[1]){
				if(skillid == MG_FIREBALL){	/* ファイヤ?ボ?ルなら中?Sからの距離を計算 */
					skill_area_temp[0] = distance(bl->x, bl->y, skill_area_temp[2], skill_area_temp[3]);
				}
				skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,
					skill_area_temp[0]| 0x0500);
			}
		} else {
			int ar;
			skill_area_temp[0]=0;
			skill_area_temp[1]=bl->id;
			switch (skillid) {
				case MG_NAPALMBEAT:
					ar = 1;
					/* ナパ?[ムビ?[トは分散ダ�??[ジなので敵の?狽�?狽ｦる */
					map_foreachinarea(skill_area_sub,
							bl->m,bl->x-ar,bl->y-ar,bl->x+ar,bl->y+ar,0,
							src,skillid,skilllv,tick,flag|BCT_ENEMY,
							skill_area_sub_count);
					break;
				case MG_FIREBALL:
					ar = 2;
					skill_area_temp[2]=bl->x;
					skill_area_temp[3]=bl->y;
					break;
				case WZ_SIGHTRASHER:
				default:
					ar = 3;
					bl = src;
					status_change_end(src,SC_SIGHT,-1);
					break;
			}
			if (skillid==WZ_SIGHTRASHER) {
				/* スキルエフェクト表示 */
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
			} else {
				/* タ?[ゲットに?U撃を加える(スキルエフェクト表示) */
				skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,
						skill_area_temp[0]);
			}
			/* タ?[ゲット以外の範囲内の敵全体に?��?を?sう */
			map_foreachinarea(skill_area_sub,
					bl->m,bl->x-ar,bl->y-ar,bl->x+ar,bl->y+ar,0,
					src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
					skill_castend_damage_id);
		}
		break;

	case HW_NAPALMVULCAN: // Fixed By SteelViruZ
		if (flag & 1) {
			if (bl->id != skill_area_temp[1])
				skill_attack(BF_MAGIC, src, src, bl, skillid, skilllv, tick, skill_area_temp[0]);
		} else {
			skill_area_temp[0] = 0;
			skill_area_temp[1] = bl->id;
			map_foreachinarea(skill_area_sub, bl->m,
					bl->x-1, bl->y-1, bl->x+1, bl->y+1, 0,
					src, skillid, skilllv, tick, flag|BCT_ENEMY,
					skill_area_sub_count);
			skill_attack(BF_MAGIC, src, src, bl, skillid, skilllv, tick, skill_area_temp[0]);
			map_foreachinarea(skill_area_sub, bl->m,
				bl->x-1, bl->y-1, bl->x+1, bl->y+1, 0,
				src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;

	case WZ_FROSTNOVA:			/* フ�?ストノヴァ */
		map_foreachinarea(skill_attack_area, src->m,
				src->x-5, src->y-5, bl->x+5, bl->y+5, 0,
				BF_MAGIC, src, src, skillid, skilllv, tick, flag, BCT_ENEMY);
		break;

	/* その他 */
	case HT_BLITZBEAT:			/* ブリッツビ?ト */
		if (flag & 1) {	//Invoked from map_foreachinarea, skill_area_temp[0] holds number of targets to divide damage by.
			skill_attack(BF_MISC, src, src, bl, skillid, skilllv, tick, skill_area_temp[0]);
		} else {
			skill_area_temp[0] = 0;
			skill_area_temp[1] = bl->id;
			if (flag & 0xf00000) //Warning, 0x100000 is currently BCT_NEUTRAL, so don't mix it when asking for the enemy. [Skotlex]
				map_foreachinarea(skill_area_sub, bl->m, 
					bl->x-1, bl->y-1, bl->x+1, bl->y+1, 0,
					src, skillid, skilllv, tick, BCT_ENEMY, skill_area_sub_count);
			map_foreachinarea(skill_area_sub, bl->m,
				bl->x-1, bl->y-1, bl->x+1, bl->y+1, 0,
				src, skillid, skilllv, tick, BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;

	case CR_GRANDCROSS:			/* グランドク�?ス */
	case NPC_GRANDDARKNESS:		/*闇グランドク�?ス*/
		/* スキルユニット配置 */
		skill_castend_pos2(src,bl->x,bl->y,skillid,skilllv,tick,0);
		if(sd)
			sd->canmove_tick = tick + 900;
		else if(src->type == BL_MOB)
			mob_changestate((struct mob_data *)src,MS_DELAY,900);
		break;

	case TF_THROWSTONE:			/* ?ﾎ投げ */
	case NPC_SMOKING:			/* スモ?キング */
		skill_attack(BF_MISC,src,src,bl,skillid,skilllv,tick,0 );
		break;

	// Celest
	case PF_SOULBURN:
		if (rand()%100 < (skilllv < 5 ? 30 + skilllv * 10 : 70)) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (skilllv == 5)
				skill_attack(BF_MAGIC,src,src,bl,skillid,skilllv,tick,0 );
			if (tsd) {
				tsd->status.sp = 0;
				clif_updatestatus(tsd,SP_SP);
			}
		} else {
			clif_skill_nodamage(src,src,skillid,skilllv,1);
			if (skilllv == 5)
				skill_attack(BF_MAGIC,src,src,src,skillid,skilllv,tick,0 );
			if (sd) {
				sd->status.sp = 0;
				clif_updatestatus(sd,SP_SP);
			}
		}		
		if (sd) pc_blockskill_start (sd, skillid, (skilllv < 5 ? 10000: 15000));
		break;

	case NPC_SELFDESTRUCTION:	/* 自爆 */
			if (flag & 1) {
				/* 個別にダ�??[ジを与える */
				if (bl->id != skill_area_temp[1])
					skill_attack(BF_MISC, src, src, bl, NPC_SELFDESTRUCTION, skilllv, tick, flag);
			} else {
				skill_area_temp[1] = bl->id;
				if(bl->type==BL_PC)
					skill_area_temp[2] = 999999;
				else
					skill_area_temp[2] = status_get_hp(src);
				clif_skill_nodamage(src, src, NPC_SELFDESTRUCTION, -1, 1);
				if(bl->type==BL_PC)
					map_foreachinarea(skill_area_sub, bl->m,
							bl->x-10, bl->y-10, bl->x+10, bl->y+10, 0,
							src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
							skill_castend_damage_id);
				else
					map_foreachinarea(skill_area_sub, bl->m,
							bl->x-5, bl->y-5, bl->x+5, bl->y+5, 0,
							src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
							skill_castend_damage_id);
				battle_damage(src, src, skill_area_temp[2], 0);
			}
			break;

	/* HP吸?/HP吸?魔法 */
	case NPC_BLOODDRAIN:
	case NPC_ENERGYDRAIN:
		{
			int heal = skill_attack( (skillid == NPC_BLOODDRAIN) ? BF_WEAPON : BF_MAGIC,
					src, src, bl, skillid, skilllv, tick, flag);
			if (heal > 0){
				struct block_list tbl;
				tbl.id = 0;
				tbl.type = BL_NUL;
				tbl.m = src->m;
				tbl.x = src->x;
				tbl.y = src->y;
				clif_skill_nodamage(&tbl, src, AL_HEAL, heal, 1);
				battle_heal(NULL, src, heal, 0, 0);
			}
		}
		break;

	case 0:
		if(sd) {
			if (flag & 3){
				if (bl->id != skill_area_temp[1])
					skill_attack(BF_WEAPON, src, src, bl, skillid, skilllv, tick, 0x0500);
			} else {
				int ar = sd->splash_range;
				skill_area_temp[1] = bl->id;
				map_foreachinarea(skill_area_sub,
					bl->m, bl->x - ar, bl->y - ar, bl->x + ar, bl->y + ar, 0,
					src, skillid, skilllv, tick, flag | BCT_ENEMY | 1,
					skill_castend_damage_id);
			}
		}
		break;

	default:
		ShowWarning("Unknown skill used:%d\n",skillid);
		map_freeblock_unlock();
		return 1;
	}

	map_freeblock_unlock();	

	return 0;
}

/*==========================================
 * スキル使用?i詠?･完了?AID指定支援系?j
 *------------------------------------------
 */
int skill_castend_nodamage_id (struct block_list *src, struct block_list *bl, int skillid, int skilllv, unsigned int tick, int flag)
{
	struct map_session_data *sd = NULL;
	struct map_session_data *dstsd = NULL;
	struct mob_data *md = NULL;
	struct mob_data *dstmd = NULL;
	int i;
	int sc_def_vit, sc_def_mdef;
//	int sc_dex, sc_luk;
	
	if(skillid < 0) 
	{	// remove the debug print when this case is finished
		ShowDebug("skill_castend_damage_id: skillid=%i\ncall: %p %p %i %i %i %i",skillid,
						src, bl,skillid,skilllv,tick,flag);
		return 0;
	}
	if(skillid > 0 && skilllv <= 0) return 0;	// celest

	nullpo_retr(1, src);
	nullpo_retr(1, bl);

	if (src->m != bl->m)
		return 1;

	if (src->type == BL_PC) {
		nullpo_retr (1, sd = (struct map_session_data *)src);
	} else if (src->type == BL_MOB) {
		nullpo_retr (1, md = (struct mob_data *)src);
	}

	sc_def_vit = status_get_sc_def_vit (bl);
	sc_def_mdef = status_get_sc_def_mdef (bl);

	if (bl->type == BL_PC){
		nullpo_retr (1, dstsd = (struct map_session_data *)bl);
	} else if (bl->type == BL_MOB){
		nullpo_retr (1, dstmd = (struct mob_data *)bl);
	}

	if(bl->prev == NULL)
		return 1;
	if(sd && pc_isdead(sd))
		return 1;
	if(dstsd && pc_isdead(dstsd) && skillid != ALL_RESURRECTION && skillid != PR_REDEMPTIO)
		return 1;
//Shouldn't be needed, skillnotok's return value is highly unlikely to have changed after you started casting. [Skotlex]
//	if (sd && skillnotok(skillid, sd)) // [MouseJstr]
//		return 0;

	map_freeblock_lock();
	switch(skillid)
	{
	case AL_HEAL:				/* ヒ?ル */
		{
			int heal = skill_calc_heal(src, skilllv);
			int heal_get_jobexp;
			int skill;
			
			if (skilllv > 10)
				heal = 9999; //9999ヒ?[ル
			if (status_isimmune(bl) || (dstmd && dstmd->class_ == MOBID_EMPERIUM))
				heal=0;	/* ?金蟲カ?ド?iヒ?ル量０?j */
			if (sd) {
				if ((skill = pc_checkskill(sd, HP_MEDITATIO)) > 0) // �?ディテイティオ
					heal += heal * skill * 2 / 100;
				if (sd && dstsd && sd->status.partner_id == dstsd->status.char_id &&
					(sd->class_&MAPID_UPPERMASK) == MAPID_SUPER_NOVICE && sd->status.sex == 0) //自分も?象もPC、?象が自分のパ?トナ?、自分がスパノビ、自分が♀なら
					heal = heal*2;	//スパノビの嫁が旦那にヒ?ルすると2倍になる
			}

			clif_skill_nodamage (src, bl, skillid, heal, 1);
			heal_get_jobexp = battle_heal(NULL,bl,heal,0,0);

			// JOB??値獲得
			if(sd && dstsd && heal > 0 && sd != dstsd && battle_config.heal_exp > 0){
				heal_get_jobexp = heal_get_jobexp * battle_config.heal_exp / 100;
				if (heal_get_jobexp <= 0)
					heal_get_jobexp = 1;
				pc_gainexp (sd, 0, heal_get_jobexp);
			}
		}
		break;

	case PR_REDEMPTIO:
		if (sd && !(flag&1)) {
			if (sd->status.party_id == 0) {
				clif_skill_fail(sd,skillid,0,0);
				break;
			}
			skill_area_temp[0] = 0;
			party_foreachsamemap(skill_area_sub,
				sd,1,
				src,skillid,skilllv,tick, flag|BCT_PARTY|1,
				skill_castend_nodamage_id);
			if (skill_area_temp[0] == 0) {
				clif_skill_fail(sd,skillid,0,0);
				break;
			}
			skill_area_temp[0] = 5 - skill_area_temp[0]; // The actual penalty...
			if (skill_area_temp[0] > 0 && !map[src->m].flag.nopenalty) { //Apply penalty
				sd->status.base_exp -= pc_nextbaseexp(sd) * skill_area_temp[0] * 2/1000; //0.2% penalty per each.
				sd->status.job_exp -= pc_nextjobexp(sd) * skill_area_temp[0] * 2/1000;
				clif_updatestatus(sd,SP_BASEEXP);
				clif_updatestatus(sd,SP_JOBEXP);
			}
			status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,0,0,0,0); //SC_COMA :P
			break;
		} else if (dstsd && pc_isdead(dstsd) && flag&1) { //Revive
			skill_area_temp[0]++; //Count it in, then fall-through to the Resurrection code.
			skilllv = 3; //Resurrection level 3 is used
		} else //Invalid target, skip resurrection.
			break;
		
	case ALL_RESURRECTION:		/* リザレクション */
		if(dstsd) {
			int per = 0;
			if (map[bl->m].flag.pvp && dstsd->pvp_point < 0)
				break;			/* PVPで復活不可能?態 */

			if (pc_isdead(dstsd)) {	/* 死亡判定 */
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				switch(skilllv){
				case 1: per=10; break;
				case 2: per=30; break;
				case 3: per=50; break;
				case 4: per=80; break;
				}
				dstsd->status.hp = dstsd->status.max_hp * per / 100;
				if (dstsd->status.hp <= 0) dstsd->status.hp = 1;
				if (dstsd->special_state.restart_full_recover) {	/* オシリスカ?ド */
					dstsd->status.hp = dstsd->status.max_hp;
					dstsd->status.sp = dstsd->status.max_sp;
				}
				pc_setstand(dstsd);
				if(battle_config.pc_invincible_time > 0)
					pc_setinvincibletimer(dstsd, battle_config.pc_invincible_time);
				clif_updatestatus(dstsd, SP_HP);
				clif_resurrection(bl, 1);
				if(sd && sd != dstsd && battle_config.resurrection_exp > 0) {
					int exp = 0,jexp = 0;
					int lv = dstsd->status.base_level - sd->status.base_level, jlv = dstsd->status.job_level - sd->status.job_level;
					if(lv > 0) {
						exp = (int)((double)dstsd->status.base_exp * (double)lv * (double)battle_config.resurrection_exp / 1000000.);
						if (exp < 1) exp = 1;
					}
					if(jlv > 0) {
						jexp = (int)((double)dstsd->status.job_exp * (double)lv * (double)battle_config.resurrection_exp / 1000000.);
						if (jexp < 1) jexp = 1;
					}
					if(exp > 0 || jexp > 0)
						pc_gainexp (sd, exp, jexp);
				}
			}
		}
		break;

	case AL_DECAGI:			/* 速度減?ｭ */
		if (status_isimmune(bl))
			break;
		if (rand() % 100 < (40 + skilllv * 2 + (status_get_lv(src) + status_get_int(src))/5 +(sc_def_mdef-100))) { //0 defense is sc_def_mdef == 100! [Skotlex]
			clif_skill_nodamage (src, bl, skillid, skilllv, 1);
			i = skill_get_time(skillid,skilllv);
			if (bl->type == BL_PC) i/=2; //Halved duration for Players
			status_change_start (bl, SkillStatusChangeTable[skillid], skilllv, 0, 0, 0, i, 0);
		}
		break;

	case AL_CRUCIS:
		if (flag & 1) {
			int race = status_get_race (bl), ele = status_get_elem_type (bl);
			if (battle_check_target (src, bl, BCT_ENEMY) && (race == 6 || battle_check_undead (race, ele))) {
				int slv = status_get_lv (src),tlv = status_get_lv (bl);
				int rate = 23 + skilllv*4 + slv - tlv;
				if (rand()%100 < rate)
					status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,0,0);
			}
		} else {
			clif_skill_nodamage(src, bl, skillid, skilllv, 1);
			map_foreachinarea(skill_area_sub,
				src->m, src->x-15, src->y-15, src->x+15, src->y+15, 0,
				src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
				skill_castend_nodamage_id);
		}
		break;

	case PR_LEXDIVINA:		/* レックスディビ?ナ */
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			clif_skill_nodamage (src, bl, skillid, skilllv, 1);
			if (status_isimmune(bl))
				break;
			if (sc_data && sc_data[SC_SILENCE].timer != -1)
				status_change_end(bl,SC_SILENCE, -1);
			else if (rand() % 100 < sc_def_vit) {
				status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
			}
		}
		break;

	case SA_ABRACADABRA:
		{
			int abra_skillid = 0, abra_skilllv;
			if (sd)
			{ //Crash-fix [Skotlex]
				//require 1 yellow gemstone even with mistress card or Into the Abyss
				if ((i = pc_search_inventory(sd, 715)) < 0 )
				{ //bug fixed by Lupus (item pos can be 0, too!)
					clif_skill_fail(sd,skillid,0,0);
					break;
				}
				pc_delitem(sd, i, 1, 0);
			}
			do {
				abra_skillid = rand() % MAX_SKILL_ABRA_DB;
				if (skill_abra_db[abra_skillid].req_lv > skilllv ||
					rand()%10000 >= skill_abra_db[abra_skillid].per ||		//dbに基づくレベル?確率判定
					(abra_skillid >= NPC_PIERCINGATT && abra_skillid <= NPC_SUMMONMONSTER) ||	//NPCスキルはダ�?
					skill_get_unit_flag(abra_skillid) & UF_DANCE)	//演奏スキルはダ�?
						abra_skillid = 0;	// reset to get a new id
			} while (abra_skillid == 0);
			abra_skilllv = skill_get_max(abra_skillid) >  skilllv ? skilllv : skill_get_max(abra_skillid);
			clif_skill_nodamage (src, bl, skillid, skilllv, 1);
			if (sd)
			{	//Crash-protection against Abracadabra casting pets
				sd->skillitem = abra_skillid;
				sd->skillitemlv = abra_skilllv;
				sd->state.abra_flag = 1;
				clif_item_skill (sd, abra_skillid, abra_skilllv, "Abracadabra");
			} else if(src->type == BL_PET)
			{	// [Skotlex]
				struct pet_data *pd = (struct pet_data *)src;
				int inf = skill_get_inf(abra_skillid);
				if (inf&INF_SELF_SKILL || inf&INF_SUPPORT_SKILL) { 
					nullpo_retr(1,(struct map_session_data *)pd->msd);
					petskill_use(pd, &pd->msd->bl, abra_skillid, abra_skilllv, tick); 
				} else //Assume offensive skills
					petskill_use(pd, bl, abra_skillid, abra_skilllv, tick); 
			}
		}
		break;

	case SA_COMA:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (status_isimmune(bl))
			break;
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0 );
		break;
	case SA_FULLRECOVERY:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (status_isimmune(bl))
			break;
		if (dstsd) pc_heal (dstsd, dstsd->status.max_hp, dstsd->status.max_sp);
		else if (dstmd) dstmd->hp = status_get_max_hp(bl);
		break;
	case SA_SUMMONMONSTER:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (sd) mob_once_spawn(sd,map[src->m].name,src->x,src->y,"--ja--",-1,1,"");
		break;
	case SA_LEVELUP:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (sd && pc_nextbaseexp(sd)) pc_gainexp(sd, pc_nextbaseexp(sd) * 10 / 100, 0);
		break;
	case SA_INSTANTDEATH:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		battle_damage(NULL,src,status_get_max_hp(src),0);
		break;
	case SA_QUESTION:
	case SA_GRAVITY:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;
	case SA_CLASSCHANGE:
		{
			//クラスチェンジ用ボスモンスタ?ID
			static int changeclass[]={1038,1039,1046,1059,1086,1087,1112,1115
				,1157,1159,1190,1272,1312,1373,1492};
			int class_ = mob_random_class (changeclass,sizeof(changeclass)/sizeof(changeclass[0]));
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(dstmd) mob_class_change(dstmd,class_);
		}
		break;
	case SA_MONOCELL:
		{
			static int poringclass[]={1002};
			int class_ = mob_random_class (poringclass,sizeof(poringclass)/sizeof(poringclass[0]));
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(dstmd) mob_class_change(dstmd,class_);
		}
		break;
	case SA_DEATH:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		battle_damage(NULL,bl,status_get_max_hp(bl),1);
		break;
	case SA_REVERSEORCISH:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl, SkillStatusChangeTable[skillid], skilllv,0,0,0,skill_get_time(skillid, skilllv),0);
		break;
	case SA_FORTUNE:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(sd) pc_getzeny(sd,status_get_lv(bl)*100);
		break;
	case SA_TAMINGMONSTER:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (dstmd) {
			for (i = 0; i < MAX_PET_DB; i++) {
				if (dstmd->class_ == pet_db[i].class_) {
					pet_catch_process1 (sd, dstmd->class_);
					break;
				}
			}
		}
		break;

	case AL_INCAGI:			/* 速度?加 */
	case AL_BLESSING:		/* ブレッシング */
	case PR_SLOWPOISON:
	case PR_IMPOSITIO:		/* イムポシティオマヌス */
	case PR_LEXAETERNA:		/* レックスエ?テルナ */
	case PR_SUFFRAGIUM:		/* サフラギウム */
	case PR_BENEDICTIO:		/* ?ｹ??~福 */
		if (status_isimmune(bl))
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		else {
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case CR_PROVIDENCE:		/* プ�?ヴィデンス */
		if (status_isimmune(bl))
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		else {
			if(sd && dstsd){ //Check they are not another crusader [Skotlex]
				if ((dstsd->class_&MAPID_UPPERMASK) == MAPID_CRUSADER) {	
					clif_skill_fail(sd,skillid,0,0);
					map_freeblock_unlock();
					return 1;
				}
			}
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;
		
	case CG_MARIONETTE:		/* マリオネットコント�??ル */
		{
			struct status_change *sc_data = status_get_sc_data(src);
			struct status_change *tsc_data = status_get_sc_data(bl);
			int sc = SkillStatusChangeTable[skillid];
			int sc2 = SC_MARIONETTE2;

			if(sc_data && tsc_data){
				if (sc_data[sc].timer == -1 && tsc_data[sc2].timer == -1) {
					status_change_start (src,sc,skilllv,0,bl->id,0,skill_get_time(skillid,skilllv),0);
					status_change_start (bl,sc2,skilllv,0,src->id,0,skill_get_time(skillid,skilllv),0);
					clif_marionette(src, bl);
				}
				else if (sc_data[sc].timer != -1 && tsc_data[sc2].timer != -1 &&
					sc_data[sc].val3 == bl->id && tsc_data[sc2].val3 == src->id) {
					status_change_end(src, sc, -1);
					status_change_end(bl, sc2, -1);
					clif_marionette(src, 0);
				}
				else {
					if (sd) clif_skill_fail(sd,skillid,0,0);
					map_freeblock_unlock();
					return 1;
				}
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
			}
		}
		break;

	case RG_CLOSECONFINE:
		break;

	case SA_FLAMELAUNCHER:	// added failure chance and chance to break weapon if turned on [Valaris]
	case SA_FROSTWEAPON:
	case SA_LIGHTNINGLOADER:
	case SA_SEISMICWEAPON:
		if (dstsd) {
			if (status_isimmune(bl)) {
				clif_skill_nodamage(src,bl,skillid,skilllv,0);
				break;
			}
			if(dstsd->status.weapon == 0 ||
				dstsd->sc_data[SC_FIREWEAPON].timer != -1 ||
				dstsd->sc_data[SC_WATERWEAPON].timer != -1 ||
				dstsd->sc_data[SC_WINDWEAPON].timer != -1 ||
				dstsd->sc_data[SC_EARTHWEAPON].timer != -1 ||
				dstsd->sc_data[SC_SHADOWWEAPON].timer != -1 ||
				dstsd->sc_data[SC_GHOSTWEAPON].timer != -1 ||
				dstsd->sc_data[SC_ENCPOISON].timer != -1) {
				if (sd) clif_skill_fail(sd,skillid,0,0);
				clif_skill_nodamage(src,bl,skillid,skilllv,0);
				break;
			}
		}
		if (sd) {
			int i = pc_search_inventory (sd, skill_db[skillid].itemid[0]);
			if(i < 0 || sd->status.inventory[i].amount < skill_db[skillid].amount[0]) {
				clif_skill_fail(sd,skillid,0,0);
				break;
			}
			pc_delitem(sd, i, skill_db[skillid].amount[0], 0);
		}

		if(skilllv < 4 && rand()%100 > (60+skilllv*10) ) { // 100% success rate at lv4 & 5, but lasts longer at lv5
			if (sd) clif_skill_fail(sd,skillid,0,0);
			clif_skill_nodamage(src,bl,skillid,skilllv,0);
			if(dstsd && battle_config.equip_self_break_rate) {
				if(sd && sd != dstsd) clif_displaymessage(sd->fd,"You broke target's weapon");
				pc_breakweapon(dstsd);
			}
			break;
		} else {
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case PR_ASPERSIO:		/* アスペルシオ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (status_isimmune(bl) || dstmd)
			break;
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case TK_SEVENWIND:
		{
		int sc=-1;
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		switch(skilllv){
			case 1:
				sc=SC_EARTHWEAPON;
				break;
			case 2:
				sc=SC_WINDWEAPON;
				break;
			case 3:
				sc=SC_WATERWEAPON;
				break;
			case 4:
				sc=SC_FIREWEAPON;
				break;
			case 5:
				sc=SC_GHOSTWEAPON;
				break;
			case 6:
				sc=SC_SHADOWWEAPON;
				break;
			case 7:
				sc=SC_ASPERSIO;
				break;
		}
		status_change_start(bl,sc,skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		}
		break;

	case PR_KYRIE:			/* キリエエレイソン */
		clif_skill_nodamage(bl,bl,skillid,skilllv,1);
		if (status_isimmune(bl))
			break;
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case LK_BERSERK:		/* バ?サ?ク */
		if(battle_config.berserk_cancels_buffs)
		{
			status_change_end(bl,SC_TWOHANDQUICKEN,-1);
			status_change_end(bl,SC_CONCENTRATION,-1);
			status_change_end(bl,SC_PARRYING,-1);
			status_change_end(bl,SC_ENDURE,-1);
			status_change_end(bl,SC_AURABLADE,-1);
		}
	case KN_AUTOCOUNTER:		/* オ?トカウンタ? */
	case KN_TWOHANDQUICKEN:	/* ツ?ハンドクイッケン */
	case CR_SPEARQUICKEN:	/* スピアクイッケン */
	case CR_REFLECTSHIELD:
	case AS_POISONREACT:	/* ポイズンリアクト */
	case MC_LOUD:			/* ラウドボイス */
	case MG_ENERGYCOAT:		/* エナジ?コ?ト */
	case MG_SIGHT:			/* サイト */
	case AL_RUWACH:			/* ルアフ */
	case MO_EXPLOSIONSPIRITS:	// 爆裂波動
	case MO_STEELBODY:		// 金?�
	case LK_AURABLADE:		/* オ?ラブレ?ド */
	case LK_PARRYING:		/* パリイング */
	case LK_CONCENTRATION:	/* コンセントレ?ション */
	case WS_CARTBOOST:		/* カ?トブ?スト */
	case SN_SIGHT:			/* トゥル?サイト */
	case WS_MELTDOWN:		/* �?ルトダウン */
	case WS_OVERTHRUSTMAX:	// Overthrust Max [Celest]
	case ST_REJECTSWORD:	/* リジェクトソ?ド */
	case HW_MAGICPOWER:		/* 魔法力?�? */
	case PF_MEMORIZE:		/* �?モライズ */
	case PA_SACRIFICE:
	case ASC_EDP:			// [Celest]
	case NPC_STOP:
	case WZ_SIGHTBLASTER:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case CG_MOONLIT:		/* 月明りの泉に落ちる花びら */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (sd && battle_config.player_skill_partner_check) {
			skill_check_pc_partner(sd, skillid, &skilllv, 1, 1);
		} else
			skill_moonlit(bl, NULL, skilllv); //The knockback must be invoked before starting the effect which places down the map cells. [Skotlex]
		
		break;
	
	case HP_ASSUMPTIO:
		if (flag&1)
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		else
		{
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			map_foreachinarea(skill_area_sub,
				bl->m, bl->x-1, bl->y-1, bl->x+1, bl->y+1, BL_PC,
				src, skillid, skilllv, tick, flag|BCT_ALL|1,
				skill_castend_nodamage_id);
		}
		break;

	case SM_ENDURE:			/* インデュア */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		if (sd)
			pc_blockskill_start (sd, skillid, 10000);
		break;

	case AS_ENCHANTPOISON: // Prevent spamming [Valaris]
		if (dstsd) {
			if(dstsd->sc_data[SC_FIREWEAPON].timer != -1 ||
				dstsd->sc_data[SC_WATERWEAPON].timer != -1 ||
				dstsd->sc_data[SC_WINDWEAPON].timer != -1 ||
				dstsd->sc_data[SC_EARTHWEAPON].timer != -1 ||
				dstsd->sc_data[SC_SHADOWWEAPON].timer != -1 ||
				dstsd->sc_data[SC_GHOSTWEAPON].timer != -1 ||
				dstsd->sc_data[SC_ENCPOISON].timer != -1) {
					clif_skill_nodamage(src,bl,skillid,skilllv,0);
					clif_skill_fail(sd,skillid,0,0);
					break;
			}
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case LK_TENSIONRELAX:	/* テンションリラックス */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		pc_setsit(sd);
		clif_sitting(sd);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case MC_CHANGECART:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;

	case TK_MISSION:
		break;

	case AC_CONCENTRATION:	/* ?W中力向?� */
		{
			int range = 1;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
			map_foreachinarea( status_change_timer_sub,
				src->m, src->x-range, src->y-range, src->x+range,src->y+range,0,
				src,SkillStatusChangeTable[skillid],tick);
		}
		break;

	case SM_PROVOKE:		/* プ�?ボック */
		{
			struct status_change *sc_data = status_get_sc_data(bl);

			/* MVPmobと不死には?かない */
			if((dstmd && status_get_mode(bl)&MD_BOSS) || battle_check_undead(status_get_race(bl),status_get_elem_type(bl))) { //不死には?かない
				map_freeblock_unlock();
				return 1;
			}

			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (rand()%100 > 50 + 3*skilllv + status_get_lv(src) - status_get_lv(bl)) //TODO: How much does base level affects? Dummy value of 1% per level difference used. [Skotlex]
			{
				if (sd) 
					clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );

			if(dstmd && dstmd->skilltimer!=-1 && dstmd->state.skillcastcancel)	// 詠?･妨害
				skill_castcancel(bl,0);
			if(dstsd && dstsd->skilltimer!=-1 && (!dstsd->special_state.no_castcancel || map_flag_gvg(bl->m))
				&& dstsd->state.skillcastcancel	&& !dstsd->special_state.no_castcancel2)
				skill_castcancel(bl,0);

			if(sc_data){
				if(sc_data[SC_FREEZE].timer!=-1)
					status_change_end(bl,SC_FREEZE,-1);
				if(sc_data[SC_STONE].timer!=-1 && sc_data[SC_STONE].val2==0)
					status_change_end(bl,SC_STONE,-1);
				if(sc_data[SC_SLEEP].timer!=-1)
					status_change_end(bl,SC_SLEEP,-1);
			}

			if(dstmd) {
				dstmd->state.provoke_flag = src->id;
				mob_target(dstmd,src,skill_get_range2(src,skillid,skilllv));
			}
		}
		break;

	case CR_DEVOTION:		/* ディボ?ション */
		if(sd && dstsd)
		{
			//??ｶや養子の?�?�の元の?E業を算?oする

			int lv = sd->status.base_level - dstsd->status.base_level;
			if (lv < 0) lv = -lv;
			if (lv > battle_config.devotion_level_difference ||
				(dstsd->sc_data[SC_DEVOTION].timer != -1 && dstsd->sc_data[SC_DEVOTION].val1 != src->id) || //Avoid overriding [Skotlex]
				(dstsd->class_&MAPID_UPPERMASK) == MAPID_CRUSADER) {
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 1;
			}
			//Look for an empty slot (or reuse in case you cast it twice in the same char. [Skotlex]
			for (i = 0; i < skilllv && i < 5 && sd->devotion[i]!=bl->id && sd->devotion[i]; i++);
			if (i == skilllv)
			{
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 1;
			}
			sd->devotion[i] = bl->id;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			clif_devotion(sd);
			status_change_start(bl,SkillStatusChangeTable[skillid],src->id,i,skill_get_range2(src,skillid,skilllv),skill_get_time2(skillid, skilllv),1000,0);
		}
		else
			if (sd)
				clif_skill_fail(sd,skillid,0,0);
		break;

	case MO_CALLSPIRITS:	// ?功
		if(sd) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			pc_addspiritball(sd,skill_get_time(skillid,skilllv),skilllv);
		}
		break;

	case CH_SOULCOLLECT:	// 狂?功
		if(sd) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			for (i = 0; i < 5; i++)
				pc_addspiritball(sd,skill_get_time(skillid,skilllv),5);
		}
		break;

	case MO_KITRANSLATION:
		if(dstsd) {
			pc_addspiritball(dstsd,skill_get_time(skillid,skilllv),5);
		}
		break;

	case TK_TURNKICK:
	case MO_BALKYOUNG: //Passive part of the attack. Splash knock-back+stun. [Skotlex]
		if (skill_area_temp[1] != bl->id) {
			skill_blown(src,bl,skill_get_blewcount(skillid,skilllv));
			skill_additional_effect(src,bl,skillid,skilllv,BF_MISC,tick); //Use Misc rather than weapon to signal passive pushback
		}
		break;	

	case MO_BLADESTOP:	// 白?n取り
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case MO_ABSORBSPIRITS:	// ?奪
		i = 0;
		if (dstsd && dstsd->spiritball > 0 &&
			((sd && sd == dstsd) || map_flag_vs(src->m)))
		{
			i = dstsd->spiritball * 7;
			pc_delspiritball(dstsd,dstsd->spiritball,0);
		} else if (dstmd && //??ﾛがモンスタ?の?�?�
			//20%の確率で??ﾛのLv*2のSPを回復する?B?ｬ功したときはタ?ゲット(σ?Д?)σ????!!
			!(status_get_mode(bl)&MD_BOSS) && rand() % 100 < 20)
		{
			i = 2 * dstmd->db->lv;
			mob_target(dstmd,src,0);
		}
		if (sd){
			if (i > 0x7FFF)
				i = 0x7FFF;
			if (sd->status.sp + i > sd->status.max_sp)
				i = sd->status.max_sp - sd->status.sp;
			if (i) {
				sd->status.sp += i;
				clif_heal(sd->fd,SP_SP,i);
			}
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,0);
		break;

	case AC_MAKINGARROW:			/* 矢?�?ｬ */
		if(sd) {
			clif_arrow_create_list(sd);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case AM_PHARMACY:			/* ポ?ション?�?ｬ */
		if(sd) {
			clif_skill_produce_mix_list(sd,22);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case SA_CREATECON:
		if(sd) {
			clif_skill_produce_mix_list(sd,23);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case BS_HAMMERFALL:		/* ハンマ?フォ?ル */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(dstsd && dstsd->special_state.no_weapon_damage)
			break;
		if(rand() % 100 < (20 + 10 * skilllv) * sc_def_vit / 100 )
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
		break;
	case RG_RAID:			/* サプライズアタック */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		map_foreachinarea(skill_area_sub,
			bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
		status_change_end(src, SC_HIDING, -1);	// ハイディング解?�
		break;

	case ASC_METEORASSAULT:	/* �?テオアサルト */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		map_foreachinarea(skill_area_sub,
			bl->m,bl->x-2,bl->y-2,bl->x+2,bl->y+2,0,
			src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
		break;

	case KN_BRANDISHSPEAR:	/*ブランディッシュスピア*/
		{
			int c,n=4,ar;
			int dir = map_calc_dir(src,bl->x,bl->y);
			struct square tc;
			int x=bl->x,y=bl->y;
			ar=skilllv/3;
			skill_brandishspear_first(&tc,dir,x,y);
			skill_brandishspear_dir(&tc,dir,4);
			/* 範?�C */
			if(skilllv == 10){
				for(c=1;c<4;c++){
					map_foreachinarea(skill_area_sub,
						bl->m,tc.val1[c],tc.val2[c],tc.val1[c],tc.val2[c],0,
						src,skillid,skilllv,tick, flag|BCT_ENEMY|n,
						skill_castend_damage_id);
				}
			}
			/* 範?�B�A */
			if(skilllv > 6){
				skill_brandishspear_dir(&tc,dir,-1);
				n--;
			}else{
				skill_brandishspear_dir(&tc,dir,-2);
				n-=2;
			}

			if(skilllv > 3){
				for(c=0;c<5;c++){
					map_foreachinarea(skill_area_sub,
						bl->m,tc.val1[c],tc.val2[c],tc.val1[c],tc.val2[c],0,
						src,skillid,skilllv,tick, flag|BCT_ENEMY|n,
						skill_castend_damage_id);
					if(skilllv > 6 && n==3 && c==4){
						skill_brandishspear_dir(&tc,dir,-1);
						n--;c=-1;
					}
				}
			}
			/* 範?�@ */
			for(c=0;c<10;c++){
				if(c==0||c==5) skill_brandishspear_dir(&tc,dir,-1);
				map_foreachinarea(skill_area_sub,
					bl->m,tc.val1[c%5],tc.val2[c%5],tc.val1[c%5],tc.val2[c%5],0,
					src,skillid,skilllv,tick, flag|BCT_ENEMY|1,
					skill_castend_damage_id);
			}
		}
		break;

	/* パ?ティスキル */
	case AL_ANGELUS:		/* エンジェラス */
	case PR_MAGNIFICAT:		/* マグニフィカ?ト */
	case PR_GLORIA:			/* グ�?リア */
	case SN_WINDWALK:		/* ウインドウォ?ク */
		if (sd == NULL || sd->status.party_id == 0 || (flag & 1)) {
			/* 個別の?�? */
			clif_skill_nodamage(bl,bl,skillid,skilllv,1);
			if(status_isimmune(bl))
				break;
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
		} else if (sd) {
			/* パ?ティ全?への?�? */
			party_foreachsamemap (skill_area_sub,
				sd,1,
				src,skillid,skilllv,tick, flag|BCT_PARTY|1,
				skill_castend_nodamage_id);
		}
		break;

	case BS_ADRENALINE:		/* アドレナリンラッシュ */
	case BS_WEAPONPERFECT:	/* ウェポンパ?フェクション */
	case BS_OVERTHRUST:		/* オ?バ?トラスト */
		if (sd == NULL || sd->status.party_id == 0 || (flag & 1)) {
			/* 個別の?�? */
			clif_skill_nodamage(bl,bl,skillid,skilllv,1);
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,(src == bl)? 1:0,0,0,skill_get_time(skillid,skilllv),0);
		} else if (sd) {
			/* パ?ティ全?への?�? */
			party_foreachsamemap(skill_area_sub,
				sd,1,
				src,skillid,skilllv,tick, flag|BCT_PARTY|1,
				skill_castend_nodamage_id);
		}
		break;

	case BS_MAXIMIZE:
	case NV_TRICKDEAD:
	case CR_DEFENDER:
	case CR_AUTOGUARD:
	case TK_READYSTORM:
	case TK_READYDOWN:
	case TK_READYTURN:
	case TK_READYCOUNTER:
	case TK_DODGE:
	case CR_SHRINK:
		{
			struct status_change *tsc_data = status_get_sc_data(bl);
			int sc = SkillStatusChangeTable[skillid];
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (tsc_data && tsc_data[sc].timer != -1)
				status_change_end(bl, sc, -1);
			else
				status_change_start(bl,sc,skilllv,0,0,0,skill_get_time(skillid,skilllv),0);				
		}
		break;

	case SM_AUTOBERSERK:	// Celest
		{
			struct status_change *tsc_data = status_get_sc_data(bl);
			int sc = SkillStatusChangeTable[skillid];
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (tsc_data && tsc_data[sc].timer != -1)
				status_change_end(bl, sc, -1);				
			else
				status_change_start(bl,sc,skilllv,0,0,0,0,0);				
		}
		break;
	case TF_HIDING:			/* ハイディング */
	case ST_CHASEWALK:			/* ハイディング */
		{
			struct status_change *tsc_data = status_get_sc_data(bl);
			int sc = SkillStatusChangeTable[skillid];
			clif_skill_nodamage(src,bl,skillid,-1,1); // Don't display the skill name as it is a hiding skill
			if (tsc_data && tsc_data[sc].timer != -1)
				status_change_end(bl, sc, -1);
			else
				status_change_start(bl,sc,skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
		}
		break;
	case TK_RUN:
		if (dstsd) {
			if (dstsd->sc_data[SC_RUN].timer == -1)
			{
				pc_run(dstsd, skilllv, (int)dstsd->dir);
				if(skilllv>=7 && dstsd->status.weapon == 0 && (dstsd->class_&MAPID_UPPERMASK) == MAPID_TAEKWON)
					status_change_start(bl,SC_SPORT,10,0,0,0,skill_get_time2(skillid,skilllv),0);
			} else
				pc_stop_walking(dstsd, 0);
		}
		break;
	case AS_CLOAKING:		/* ク�??キング */
		{
			struct status_change *tsc_data = status_get_sc_data(bl);
			int sc=SkillStatusChangeTable[skillid];
			clif_skill_nodamage(src,bl,skillid,-1,1);
			if(tsc_data && tsc_data[sc].timer!=-1 )
				/* 解?怩ｷる */
				status_change_end(bl, sc, -1);
			else
				/* 付加する */
			{	//Avoid cloaking with no wall and low skill level. [Skotlex]
				if (sd && skilllv < 3 && skill_check_cloaking(bl))
					clif_skill_fail(sd,skillid,0,0);
				else
					status_change_start(bl,sc,skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
			}
		}
		break;

	/* ?地スキル */
	case BD_LULLABY:			/* 子守唄 */
	case BD_RICHMANKIM:			/* ニヨルドの宴 */
	case BD_ETERNALCHAOS:		/* 永遠の?ｬ沌 */
	case BD_DRUMBATTLEFIELD:	/* ?太鼓の響き */
	case BD_RINGNIBELUNGEN:		/* ニ?ベルングの指輪 */
	case BD_ROKISWEIL:			/* �?キの叫び */
	case BD_INTOABYSS:			/* ?[淵の中に */
	case BD_SIEGFRIED:			/* 不死?gのジ?クフリ?ド */
	case BA_DISSONANCE:			/* 不協和音 */
	case BA_POEMBRAGI:			/* ブラギの�? */
	case BA_WHISTLE:			/* 口笛 */
	case BA_ASSASSINCROSS:		/* 夕陽のアサシンク�?ス */
	case BA_APPLEIDUN:			/* イドゥンの林檎 */
	case DC_UGLYDANCE:			/* 自分?沁閧ﾈダンス */
	case DC_HUMMING:			/* ハミング */
	case DC_DONTFORGETME:		/* 私を忘れないで?c */
	case DC_FORTUNEKISS:		/* ?K運のキス */
	case DC_SERVICEFORYOU:		/* サ?ビスフォ?ユ? */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_unitsetting(src,skillid,skilllv,src->x,src->y,0);
		break;

	case HP_BASILICA:			/* バジリカ */
	case CG_HERMODE:			// Wand of Hermod
		{
			struct skill_unit_group *sg;
			battle_stopwalking(src,1);
			skill_clear_unitgroup(src);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			sg = skill_unitsetting(src,skillid,skilllv,src->x,src->y,0);
			if(skillid == CG_HERMODE)
				status_change_start(src,SC_DANCING,skillid,0,0,sg->group_id,skill_get_time(skillid,skilllv),0);
			else
				status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,BCT_SELF,sg->group_id,
					skill_get_time(skillid,skilllv),0);
		}
		break;

	case PA_GOSPEL:				/* ゴスペル */
		{
			struct status_change *sc_data = status_get_sc_data(src);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (sc_data[SC_GOSPEL].timer != -1 && sc_data[SC_GOSPEL].val4 == BCT_SELF) {
				status_change_end(src,SC_GOSPEL,-1);   
			} else {
				struct skill_unit_group *sg = skill_unitsetting(src,skillid,skilllv,src->x,src->y,0);
				status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,(int)sg,BCT_SELF,skill_get_time(skillid,skilllv),0);
			}
		}
		break;

	case BD_ADAPTATION:			/* アドリブ */
		{
			struct status_change *sc_data = status_get_sc_data(src);
			if(sc_data && sc_data[SC_DANCING].timer!=-1){
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				skill_stop_dancing(src);
			}
		}
		break;

	case BA_FROSTJOKE:			/* 寒いジョ?ク */
	case DC_SCREAM:				/* スクリ?ム */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_addtimerskill(src,tick+3000,bl->id,0,0,skillid,skilllv,0,flag);
		if (md) {	// Mobは�?れないから?Aスキル名を叫ばせてみる
			char temp[128];
			if (strlen(md->name) + strlen(skill_db[skillid].desc) > 120)
				break; //Message won't fit on buffer. [Skotlex]
			sprintf(temp,"%s : %s !!",md->name,skill_db[skillid].desc);
			clif_GlobalMessage(&md->bl,temp);
		}
		break;


	case BA_PANGVOICE://パンボイス
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(status_get_mode(bl)&MD_BOSS){
				if(sd)
					clif_skill_fail(sd,skillid,0,0);
				break;
			}
			if(rand()%100 < 50){
				status_change_start(bl,SC_CONFUSION,7,0,0,0,10000+7000,0);
			}else{
				if(sd)
					clif_skill_fail(sd,skillid,0,0);
			}
		break;

	case DC_WINKCHARM://魅惑のウィンク
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(dstsd){
				if(rand()%100 < 30)
					status_change_start(bl,SC_CONFUSION,7,0,0,0,10000+7000,0);
				else{
					if(sd)
						clif_skill_fail(sd,skillid,0,0);
				}
			}else if(dstmd)
			{
				int race = status_get_race(bl);
				if(!(status_get_mode(bl)&MD_BOSS) && status_get_lv(src)>status_get_lv(bl) && (race == 6 || race == 7 || race == 8) && rand()%100 < 70)
						status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,10000,0);
				else{
					if(sd)
						clif_skill_fail(sd,skillid,0,0);
				}
			}
		break;

	case TF_STEAL:			// スティ?ル
		if(sd) {
			if(pc_steal_item(sd,bl))
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
			else
				clif_skill_fail(sd,skillid,0x0a,0);
		}
		break;

	case RG_STEALCOIN:		// スティ?ルコイン
		if(sd) {
			if(pc_steal_coin(sd,bl)) {
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				mob_target((struct mob_data *)bl,src,skill_get_range2(src,skillid,skilllv));
			}
			else
				clif_skill_fail(sd,skillid,0,0);
		}
		break;

	case MG_STONECURSE:			/* スト?ンカ?ス */
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			// Level 6-10 doesn't consume a red gem if it fails [celest]
			int i, gem_flag = 1, fail_flag = 0;
			if (status_get_mode(bl)&MD_BOSS) {
				if (sd) clif_skill_fail(sd,skillid,0,0);
				break;
			}
			if(status_isimmune(bl))
				break;
			if (sc_data && sc_data[SC_STONE].timer != -1) {
				status_change_end(bl,SC_STONE,-1);
				if (sd) {
					fail_flag = 1;
					clif_skill_fail(sd,skillid,0,0);
				}
			}
			else if( rand()%100 < skilllv*4+20 && !battle_check_undead(status_get_race(bl),status_get_elem_type(bl)))
			{
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				status_change_start(bl,SC_STONE,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
			} else if(sd) {
				if (skilllv > 5) gem_flag = 0;
				clif_skill_fail(sd,skillid,0,0);
				fail_flag = 1;
			}
			if (dstmd)
				mob_target(dstmd,src,skill_get_range2(src,skillid,skilllv));
			if (sd && gem_flag) {
				if ((i=pc_search_inventory(sd, skill_db[skillid].itemid[0])) < 0 ) {
					if (!fail_flag) clif_skill_fail(sd,skillid,0,0);
					break;
				}
				pc_delitem(sd, i, skill_db[skillid].amount[0], 0);
			}
		}
		break;

	case NV_FIRSTAID:			/* ?急手? */
		clif_skill_nodamage(src,bl,skillid,5,1);
		battle_heal(NULL,bl,5,0,0);
		break;

	case AL_CURE:				/* キュア? */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(status_isimmune(bl))
			break;
		status_change_end(bl, SC_SILENCE	, -1 );
		status_change_end(bl, SC_BLIND	, -1 );
		status_change_end(bl, SC_CONFUSION, -1 );
		if( battle_check_undead(status_get_race(bl),status_get_elem_type(bl)) ){//アンデッドなら暗闇?果
			status_change_start(bl, SC_CONFUSION,1,0,0,0,6000,0);
		}
		break;

	case TF_DETOXIFY:			/* 解毒 */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_end(bl, SC_POISON	, -1 );
		status_change_end(bl, SC_DPOISON	, -1 );
		break;

	case PR_STRECOVERY:			/* リカバリ? */
		{
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(status_isimmune(bl))
				break;
			status_change_end(bl, SC_FREEZE	, -1 );
			status_change_end(bl, SC_STONE	, -1 );
			status_change_end(bl, SC_SLEEP	, -1 );
			status_change_end(bl, SC_STAN	, -1 );
			if( battle_check_undead(status_get_race(bl),status_get_elem_type(bl)) ){//アンデッドなら暗闇?果
				if(rand()%100 < (100-(status_get_int(bl)/2+status_get_vit(bl)/3+status_get_luk(bl)/10))) {
					status_change_start(bl, SC_BLIND,1,0,0,0,
						1000 * 30 * (100-(status_get_int(bl)+status_get_vit(bl))/2)/100,0);
				}
			}
			if(dstmd){
				dstmd->attacked_id=0;
				dstmd->target_id=0;
				dstmd->state.targettype = NONE_ATTACKABLE;
				dstmd->state.skillstate=MSS_IDLE;
				dstmd->next_walktime=tick+rand()%3000+3000;
			}
		}
		break;

	case WZ_ESTIMATION:			/* モンスタ??�報 */
		if(sd) {
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			clif_skill_estimation((struct map_session_data *)src,bl);
		}
		break;

	case BS_REPAIRWEAPON:			/* �?器?C�? */
		if(sd && dstsd)
			clif_item_repair_list(sd,dstsd);
		break;

	case MC_IDENTIFY:			/* アイテム鑑定 */
		if(sd)
			clif_item_identify_list(sd);
		break;

	// Weapon Refining [Celest]
	case WS_WEAPONREFINE:
		if(sd)
			clif_item_refine_list(sd);
		break;

	case MC_VENDING:			/* 露店開?ﾝ */
		if(sd)
		{	//Prevent vending of GMs with unnecessary Level to trade/drop. [Skotlex]
			if ( pc_can_give_items(pc_isGM(sd)) )
				clif_skill_fail(sd,skillid,0,0);
			else
				clif_openvendingreq(sd,2+sd->skilllv);
		}
		break;

	case AL_TELEPORT:			/* テレポ?ト */
		if(sd) {
			if (map[sd->bl.m].flag.noteleport) {	/* テレポ禁止 */
				clif_skill_teleportmessage(sd,0);
				break;
			}
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(sd->skilllv == 1)
				clif_skill_warppoint(sd,skillid,"Random","","","");
			else {
				clif_skill_warppoint(sd,skillid,"Random",
					sd->status.save_point.map,"","");
			}
		} else if(dstmd)
			mob_warp(dstmd,-1,-1,-1,3);
		break;

	case AL_HOLYWATER:			/* アクアベネディクタ */
		if(sd) {
			int eflag;
			struct item item_tmp;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			memset(&item_tmp,0,sizeof(item_tmp));
			item_tmp.nameid = 523;
			item_tmp.identify = 1;
			if(battle_config.holywater_name_input) {
				item_tmp.card[0] = 0xfe;
				item_tmp.card[1] = 0;
				item_tmp.card[2] = GetWord(sd->char_id,0); // CharId
				item_tmp.card[3] = GetWord(sd->char_id,1);
			}
			eflag = pc_additem(sd,&item_tmp,1);
			if(eflag) {
				clif_additem(sd,0,0,eflag);
				map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
			}
		}
		break;
	case TF_PICKSTONE:
		if(sd) {
			int eflag;
			struct item item_tmp;
			struct block_list tbl;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			memset(&item_tmp,0,sizeof(item_tmp));
			memset(&tbl,0,sizeof(tbl)); // [MouseJstr]
			item_tmp.nameid = 7049;
			item_tmp.identify = 1;
			tbl.id = 0;
			clif_takeitem(&sd->bl,&tbl);
			eflag = pc_additem(sd,&item_tmp,1);
			if(eflag) {
				clif_additem(sd,0,0,eflag);
				map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
			}
		}
		break;
	case ASC_CDP:
       	if(sd) {
			int eflag;
			struct item item_tmp;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(rand()%10000 < (2000 + 40*sd->paramc[4] + 20*sd->paramc[5])){
				clif_produceeffect(sd,2,678);
				clif_misceffect(&sd->bl,5);
				memset(&item_tmp,0,sizeof(item_tmp));
				item_tmp.nameid = 678;
				item_tmp.identify = 1;
				if(battle_config.cdp_name_input) {
					item_tmp.card[0] = 0xfe;
					item_tmp.card[1] = 0;
					item_tmp.card[2] = GetWord(sd->char_id,0); // CharId
					item_tmp.card[3] = GetWord(sd->char_id,1);
				}
				eflag = pc_additem(sd,&item_tmp,1);
				if(eflag) {
					clif_additem(sd,0,0,eflag);
					map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
				}
			} else {
				clif_produceeffect(sd,3,678);
				clif_misceffect(&sd->bl,6);
				pc_heal(sd,-(sd->status.max_hp>>2),0);
			}
		}
		break;

	case RG_STRIPWEAPON:		/* ストリップウェポン */
	case RG_STRIPSHIELD:		/* ストリップシ?[ルド */
	case RG_STRIPARMOR:			/* ストリップア?[マ?[ */
	case RG_STRIPHELM:			/* ストリップヘルム */
	case ST_FULLSTRIP:			// Rewritten most of the code [DracoRPG]
		{
		struct status_change *tsc_data = status_get_sc_data(bl);
		int strip_fix, equip = 0;
		int sclist[4] = {0,0,0,0};

		if (skillid == RG_STRIPWEAPON || skillid == ST_FULLSTRIP)
		   equip |= EQP_WEAPON;
		if (skillid == RG_STRIPSHIELD || skillid == ST_FULLSTRIP)
		   equip |= EQP_SHIELD;
		if (skillid == RG_STRIPARMOR || skillid == ST_FULLSTRIP)
		   equip |= EQP_ARMOR;
		if (skillid == RG_STRIPHELM || skillid == ST_FULLSTRIP)
		   equip |= EQP_HELM;

		strip_fix = status_get_dex(src) - status_get_dex(bl);
		if(strip_fix < 0)
			strip_fix=0;
		if (rand()%100 >= 5+2*skilllv+strip_fix/5)
		{
			if (sd)
				clif_skill_fail(sd,skillid,0,0);
			break;
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,1);

		if (dstsd) {
			for (i=0;i<11;i++) {
				if (dstsd->equip_index[i]>=0 && dstsd->inventory_data[dstsd->equip_index[i]]) {
					if (equip &EQP_WEAPON && (i == 9 || (i == 8 && dstsd->inventory_data[dstsd->equip_index[8]]->type == 4)) && !(dstsd->unstripable_equip &EQP_WEAPON) && !(tsc_data && tsc_data[SC_CP_WEAPON].timer != -1)) {
						sclist[0] = SC_STRIPWEAPON; // Okay, we found a weapon to strip - It can be a right-hand, left-hand or two-handed weapon
						pc_unequipitem(dstsd,dstsd->equip_index[i],3);
					} else if (equip &EQP_SHIELD && i == 8 && dstsd->inventory_data[dstsd->equip_index[8]]->type == 5 && !(dstsd->unstripable_equip &EQP_SHIELD) && !(tsc_data && tsc_data[SC_CP_SHIELD].timer != -1)) {
						sclist[1] = SC_STRIPSHIELD; // Okay, we found a shield to strip - It is really a shield, not a two-handed weapon or a left-hand weapon
						pc_unequipitem(dstsd,dstsd->equip_index[i],3);
					} else if (equip &EQP_ARMOR && i == 7 && !(dstsd->unstripable_equip &EQP_ARMOR) && !(tsc_data && tsc_data[SC_CP_ARMOR].timer != -1)) {
						sclist[2] = SC_STRIPARMOR; // Okay, we found an armor to strip
						pc_unequipitem(dstsd,dstsd->equip_index[i],3);
					} else if (equip &EQP_HELM && i == 6 && !(dstsd->unstripable_equip &EQP_HELM) && !(tsc_data && tsc_data[SC_CP_HELM].timer != -1)) {
						sclist[3] = SC_STRIPHELM; // Okay, we found a helm to strip
						pc_unequipitem(dstsd,dstsd->equip_index[i],3);
					}
				}
			}
		} else if (dstmd) {
			if (equip &EQP_WEAPON)
				sclist[0] = SC_STRIPWEAPON;
			if (equip &EQP_SHIELD)
				sclist[1] = SC_STRIPSHIELD;
			if (equip &EQP_ARMOR)
				sclist[2] = SC_STRIPARMOR;
			if (equip &EQP_HELM)
				sclist[3] = SC_STRIPHELM;
		}

		for (i=0;i<4;i++) {
			if (sclist[i] != 0) // Start the SC only if an equipment was stripped from this location
			   status_change_start(bl,sclist[i],skilllv,0,0,0,skill_get_time(skillid,skilllv)+strip_fix/2,0);
		}

		break;
		}

	/* PotionPitcher */
	case AM_POTIONPITCHER:		/* ポ?ションピッチャ? */
		{
			struct block_list tbl;
			int i,x,hp = 0,sp = 0,bonus=100;
			if(sd) {
				x = skilllv%11 - 1;
				i = pc_search_inventory(sd,skill_db[skillid].itemid[x]);
				if(i < 0 || skill_db[skillid].itemid[x] <= 0) {
					clif_skill_fail(sd,skillid,0,0);
					map_freeblock_unlock();
					return 1;
				}
				if(sd->inventory_data[i] == NULL || sd->status.inventory[i].amount < skill_db[skillid].amount[x]) {
					clif_skill_fail(sd,skillid,0,0);
					map_freeblock_unlock();
					return 1;
				}
				potion_flag = 1;
				potion_hp = potion_sp = potion_per_hp = potion_per_sp = 0;
				potion_target = bl->id;
				run_script(sd->inventory_data[i]->script,0,sd->bl.id,0);
				pc_delitem(sd,i,skill_db[skillid].amount[x],0);
				potion_flag = potion_target = 0;
				if (sd->sc_data[SC_SPIRIT].timer != -1 && sd->sc_data[SC_SPIRIT].val2 == SL_ALCHEMIST)
					bonus += sd->status.base_level;
				if(potion_per_hp > 0 || potion_per_sp > 0) {
					hp = status_get_max_hp(bl) * potion_per_hp / 100;
					hp = hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
					if(dstsd) {
						sp = dstsd->status.max_sp * potion_per_sp / 100;
						sp = sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
					}
				}
				else {
					if(potion_hp > 0) {
						hp = potion_hp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
						hp = hp * (100 + (status_get_vit(bl)<<1)) / 100;
						if(dstsd)
							hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
					}
					if(potion_sp > 0) {
						sp = potion_sp * (100 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)*bonus/10000;
						sp = sp * (100 + (status_get_int(bl)<<1)) / 100;
						if(dstsd)
							sp = sp * (100 + pc_checkskill(dstsd,MG_SRECOVERY)*10) / 100;
					}
				}
			}
			else {
				hp = (1 + rand()%400) * (100 + skilllv*10) / 100;
				hp = hp * (100 + (status_get_vit(bl)<<1)) / 100;
				if(dstsd)
					hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10) / 100;
			}
			tbl.id = 0;
			tbl.m = src->m;
			tbl.x = src->x;
			tbl.y = src->y;
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(hp > 0 || (hp <= 0 && sp <= 0))
				clif_skill_nodamage(&tbl,bl,AL_HEAL,hp,1);
			if(sp > 0)
				clif_skill_nodamage(&tbl,bl,MG_SRECOVERY,sp,1);
			battle_heal(src,bl,hp,sp,0);
		}
		break;
	case AM_CP_WEAPON:
	case AM_CP_SHIELD:
	case AM_CP_ARMOR:
	case AM_CP_HELM:
		{
			int scid = SC_STRIPWEAPON + (skillid - AM_CP_WEAPON);
			struct status_change *tsc_data = status_get_sc_data(bl);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(tsc_data && tsc_data[scid].timer != -1)
				status_change_end(bl, scid, -1 );
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		}
		break;

	case SA_DISPELL:			/* ディスペル */
		{
			int i;
			struct status_change *tsc_data = status_get_sc_data(bl);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (rand()%100 >= (50+10*skilllv)*sc_def_mdef/100 || tsc_data == NULL) // Fixed & changed to use a proportionnal reduction (no info, but seems far more logical) [DracoRPG]
			{
				if (sd)
					clif_skill_fail(sd,skillid,0,0);
				break;
			}
			if(status_isimmune(bl))
				break;
			for(i=0;i<SC_MAX;i++){
				if (tsc_data[i].timer == -1)
					continue;
				if(i==SC_HALLUCINATION || i==SC_WEIGHT50 || i==SC_WEIGHT90
					|| i==SC_STRIPWEAPON || i==SC_STRIPSHIELD || i==SC_STRIPARMOR || i==SC_STRIPHELM
					|| i==SC_CP_WEAPON || i==SC_CP_SHIELD || i==SC_CP_ARMOR || i==SC_CP_HELM
					|| i==SC_COMBO || i==SC_DANCING || i==SC_GUILDAURA || i==SC_STEELBODY || i==SC_EDP
					|| i==SC_CARTBOOST || i==SC_MELTDOWN || i==SC_MOONLIT
					)
					continue;
				status_change_end(bl,i,-1);
			}
		}
		break;

	case TF_BACKSLIDING: //This is the correct implementation as per packet logging information. [Skotlex]
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_blown(src,bl,skill_get_blewcount(skillid,skilllv)|0x10000);
		break;

	case TK_HIGHJUMP:
		if(sd) {
			int x,y;

			if (!pc_can_move(sd))
				return 0; 

			x = src->x + dirx[(int)sd->dir]*skilllv*2;
			y = src->y + diry[(int)sd->dir]*skilllv*2;

			clif_skill_nodamage(src,bl,TK_HIGHJUMP,skilllv,1);
			if(map_getcell(src->m,x,y,CELL_CHKPASS)) {
				pc_movepos(sd,x,y,0); 
				clif_slide(src,x,y);
			}
		}
		break;

	case SA_CASTCANCEL:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		skill_castcancel(src,1);
		if(sd) {
			int sp = skill_get_sp(sd->skillid_old,sd->skilllv_old);
			sp = sp * (90 - (skilllv-1)*20) / 100;
			if(sp < 0) sp = 0;
			pc_heal(sd,0,-sp);
		}
		break;
	case SA_SPELLBREAKER:	// スペルブレイカ?
		{
			struct status_change *sc_data = status_get_sc_data(bl);
			int sp;
			if(sc_data && sc_data[SC_MAGICROD].timer != -1) {
				if(dstsd) {
					sp = skill_get_sp(skillid,skilllv);
					sp = sp * sc_data[SC_MAGICROD].val2 / 100;
					if(sp > 0x7fff) sp = 0x7fff;
					else if(sp < 1) sp = 1;
					if(dstsd->status.sp + sp > dstsd->status.max_sp) {
						sp = dstsd->status.max_sp - dstsd->status.sp;
						dstsd->status.sp = dstsd->status.max_sp;
					}
					else
						dstsd->status.sp += sp;
					clif_heal(dstsd->fd,SP_SP,sp);
				}
				clif_skill_nodamage(bl,bl,SA_MAGICROD,sc_data[SC_MAGICROD].val1,1);
				if(sd) {
					sp = sd->status.max_sp/5;
					if(sp < 1) sp = 1;
					pc_heal(sd,0,-sp);
				}
			}
			else {
				int bl_skillid=0,bl_skilllv=0,hp = 0;
				if(bl->type == BL_PC) {
					if(dstsd && dstsd->skilltimer != -1) {
						bl_skillid = dstsd->skillid;
						bl_skilllv = dstsd->skilllv;
						if (map_flag_vs(bl->m))
							hp = status_get_max_hp(bl)/50; //Recover 2% HP [Skotlex]
					}
				}
				else if(bl->type == BL_MOB) {
					if(dstmd && dstmd->skilltimer != -1) {
						if (status_get_mode(bl) & MD_BOSS)
						{	//Only 10% success chance against bosses. [Skotlex]
							if (rand()%100 < 90)
							{
								clif_skill_fail(sd,skillid,0,0);
								break;
							}
						} else
							hp = status_get_max_hp(bl)/50; //Recover 2% HP [Skotlex]
						bl_skillid = dstmd->skillid;
						bl_skilllv = dstmd->skilllv;
					}
				}
				if(bl_skillid > 0 && bl_skillid != PA_PRESSURE/* && skill_db[bl_skillid].skill_type == BF_MAGIC*/) { //Reports indicate Spell Break cancels any type of skill, except Pressure. [Skotlex]
					clif_skill_nodamage(src,bl,skillid,skilllv,1);
					skill_castcancel(bl,0);
					sp = skill_get_sp(bl_skillid,bl_skilllv);
					if(dstsd)
						pc_heal(dstsd,-hp,-sp);
					if(sd) {
						sp = sp*(25*(skilllv-1))/100;
						if(skilllv > 1 && sp < 1) sp = 1;
						if(sp > 0x7fff) sp = 0x7fff;
						else if(sp < 1) sp = 1;
						if(sd->status.sp + sp > sd->status.max_sp) {
							sp = sd->status.max_sp - sd->status.sp;
							sd->status.sp = sd->status.max_sp;
						}
						else
							sd->status.sp += sp;

						if (hp && skilllv > 5)
						{	//Recover half damaged HP at levels 6-10 [Skotlex]
							hp /=2;
							if(sd->status.hp + hp > sd->status.max_hp) {
								hp = sd->status.max_hp - sd->status.hp;
								sd->status.hp = sd->status.max_hp;
							}
							else
								sd->status.hp += hp;

							clif_heal(sd->fd,SP_HP,hp);
						}
						clif_heal(sd->fd,SP_SP,sp);
					}
				}
				else if(sd)
					clif_skill_fail(sd,skillid,0,0);
			}
		}
		break;
	case SA_MAGICROD:
		if (status_isimmune(bl))
			break;
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;
	case SA_AUTOSPELL:			/* オ?トスペル */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(sd)
			clif_autospell(sd,skilllv);
		else {
			int maxlv=1,spellid=0;
			static const int spellarray[3] = { MG_COLDBOLT,MG_FIREBOLT,MG_LIGHTNINGBOLT };
			if(skilllv >= 10) {
				spellid = MG_FROSTDIVER;
				maxlv = skilllv - 9;
			}
			else if(skilllv >=8) {
				spellid = MG_FIREBALL;
				maxlv = skilllv - 7;
			}
			else if(skilllv >=5) {
				spellid = MG_SOULSTRIKE;
				maxlv = skilllv - 4;
			}
			else if(skilllv >=2) {
				int i = rand()%3;
				spellid = spellarray[i];
				maxlv = skilllv - 1;
			}
			else if(skilllv > 0) {
				spellid = MG_NAPALMBEAT;
				maxlv = 3;
			}
			if(spellid > 0)
				status_change_start(src,SC_AUTOSPELL,skilllv,spellid,maxlv,0,
					skill_get_time(SA_AUTOSPELL,skilllv),0);
		}
		break;

	case BS_GREED:
		if(sd){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			map_foreachinarea(skill_greed,bl->m,bl->x-2,bl->y-2,bl->x+2,bl->y+2,BL_ITEM,bl);
		}
		break;

	case SA_ELEMENTWATER:
	case SA_ELEMENTFIRE:
	case SA_ELEMENTGROUND:
	case SA_ELEMENTWIND:
		if(dstmd && !(status_get_mode(bl)&MD_BOSS)){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			dstmd->def_ele = skill_get_pl(skillid);
			dstmd->def_ele += (1+rand()%4)*20;
		}
		break;

	/* ランダム??ｫ?化?A?�??ｫ?化?A地?A火?A風 */
	case NPC_ATTRICHANGE:
	case NPC_CHANGEWATER:
	case NPC_CHANGEGROUND:
	case NPC_CHANGEFIRE:
	case NPC_CHANGEWIND:
	/* 毒?A?ｹ?A念?A闇 */
	case NPC_CHANGEPOISON:
	case NPC_CHANGEHOLY:
	case NPC_CHANGEDARKNESS:
	case NPC_CHANGETELEKINESIS:
	case NPC_CHANGEUNDEAD:
		if(md){
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			md->def_ele = skill_get_pl(skillid);
			if (md->def_ele == 0)			/* ランダム?化?Aただし?A*/
				md->def_ele = rand()%10;	/* 不死??ｫは?怩ｭ */
			md->def_ele += (1+rand()%4)*20;	/* ??ｫレベルはランダム */
		}
		break;

	case NPC_PROVOCATION:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(md)
			clif_pet_performance(src,md->db->skill[md->skillidx].val[0]);
		break;

	case NPC_HALLUCINATION:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(status_isimmune(bl))
			break;
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case NPC_KEEPING:
	case NPC_BARRIER:
		{
			int skill_time = skill_get_time(skillid,skilllv);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_time,0 );
			if (md)
				mob_changestate(md,MS_DELAY,skill_time);
			else if (sd)
				sd->attackabletime = sd->canmove_tick = tick + skill_time;
		}
		break;

	case NPC_REBIRTH:
		if (md && md->state.state == MS_DEAD) {
			mob_setdelayspawn (md->bl.id);
		}
		break;

	case NPC_DARKBLESSING:
		{
			int sc_def = 100 - status_get_mdef(bl);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if(status_isimmune(bl))
				break;
			if(status_get_elem_type(bl) == 7 || status_get_race(bl) == 6)
				break;
			if(rand()%100 < sc_def*(50+skilllv*5)/100)
				status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0 );
		}
		break;

	case NPC_LICK:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if (dstsd) {
			if (dstsd->special_state.no_weapon_damage )
				break;
			pc_heal(dstsd,0,-100);
		}
		if(rand()%100 < (skilllv*5)*sc_def_vit/100)
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time2(skillid,skilllv),0 );
		break;

	case NPC_SUICIDE:			/* 自決 */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		battle_damage(NULL,src,status_get_hp(bl),0);
		break;

	case NPC_SUMMONSLAVE:		/* 手下?｢喚 */
	case NPC_SUMMONMONSTER:		/* MOB?｢喚 */
		if(md)
			mob_summonslave(md,md->db->skill[md->skillidx].val,skilllv,skillid);
		break;

	case NPC_CALLSLAVE:		//取り巻き呼び戻し
		mob_warpslave(src,AREA_SIZE/2);
		break;

	case NPC_RANDOMMOVE:
		if (md) {
			md->next_walktime = tick - 1;
			mob_randomwalk(md,tick);
		}
		break;
	
	case NPC_SPEEDUP:
		{
			// or does it increase casting rate? just a guess xD
			int i = SC_ASPDPOTION0 + skilllv - 1;
			if (i > SC_ASPDPOTION3)
				i = SC_ASPDPOTION3;
			status_change_start(bl,i,skilllv,0,0,0,skilllv * 60000,0);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case NPC_REVENGE:
		// not really needed... but adding here anyway ^^
		if (md && md->master_id > 0) {
			struct block_list *mbl, *tbl;
			if ((mbl = map_id2bl(md->master_id)) == NULL ||
				(tbl = battle_gettargeted(mbl)) == NULL)
				break;
			md->state.provoke_flag = tbl->id;
			mob_target(md, tbl, md->db->range);
		}
		break;

	case NPC_RUN:		//後退
		if(md) {
			int dist = skilllv;//後退する距離
			int dir = md->dir; //自分がどの方向に向いてるかチェック
			int mask[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

			md->attacked_id = 0;
			md->attacked_count = 0;
			md->target_id = 0;
			md->state.targettype = NONE_ATTACKABLE;
			md->state.skillstate = MSS_IDLE;			
			mob_walktoxy(md, md->bl.x + dist * mask[dir][0], md->bl.y + dist * mask[dir][1], 0);
		}
		break;

	case NPC_TRANSFORMATION:
	case NPC_METAMORPHOSIS:
		if(md) {
			if (skilllv > 1)
			{	//Multiply skilllv times, the original instance must be silently killed. [Skotlex] 
				mob_summonslave(md,md->db->skill[md->skillidx].val,skilllv,skillid);
				mob_delete(md);
			}
			else
			{	//Transform into another class.
				int class_ = mob_random_class (md->db->skill[md->skillidx].val,0);
				if (class_) mob_class_change(md, class_);
			}
		}
		break;

	case NPC_EMOTION_ON:
	case NPC_EMOTION:
		if(md)
		{
			clif_emotion(&md->bl,md->db->skill[md->skillidx].val[0]);
			if(!md->special_state.ai && (md->db->skill[md->skillidx].val[1] || md->db->skill[md->skillidx].val[2]))
			{ //NPC_EMOTION & NPC_EMOTION_ON can change a mob's mode 'permanently' [Skotlex]
				//val[1] 'sets' the mode, val[2] can add/remove from the current mode based on skill used:
				//NPC_EMOTION_ON adds a mode / NPC_EMOTION removes it.
				int mode, mode2;
				mode = status_get_mode(src);
				mode2 =  (md->db->skill[md->skillidx].val[1])?(md->db->skill[md->skillidx].val[1]):mode;
				if (md->db->skill[md->skillidx].val[2]) { //Alter the mode.
					if (skillid == NPC_EMOTION_ON) //Add a mode
						mode2|= md->db->skill[md->skillidx].val[2];
					else	//Remove a mode
						mode2&= ~(md->db->skill[md->skillidx].val[2]);
				}
				if (mode == mode2)
					break; //No change
				md->mode = mode2;
				if (md->mode == md->db->mode)
					md->mode = 0; //Fallback to the db's mode.
				//Since mode changed, reset their state.
				mob_stopattack(md);
				mob_stop_walking(md,0);
			}
		}
		break;

	case NPC_DEFENDER:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;

	// Equipment breaking monster skills [Celest]
	case NPC_BREAKWEAPON:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(dstsd && battle_config.equip_skill_break_rate)
			pc_breakweapon(dstsd);
		break;

	case NPC_BREAKARMOR:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(dstsd && battle_config.equip_skill_break_rate)
			pc_breakarmor(dstsd);
		break;

	case NPC_BREAKHELM:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(dstsd && battle_config.equip_skill_break_rate)
			pc_breakhelm(dstsd);
		break;

	case NPC_BREAKSHIELD:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(dstsd && battle_config.equip_skill_break_rate)
			pc_breakshield(dstsd);
		break;

	case NPC_POWERUP:	//NPC爆裂波動
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SC_EXPLOSIONSPIRITS,skilllv,0,0,0,skilllv * 60000,0);
		// another random guess xP
		status_change_start(bl,SC_INCALLSTATUS,skilllv * 5,0,0,0,skilllv * 60000,0);
		break;

	case NPC_AGIUP:
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SC_INCAGI,skilllv * 10,0,0,0,skilllv * 60000,0);
		break;

	case NPC_SIEGEMODE:
	case NPC_INVISIBLE:
		// not sure what it does
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;

	case WE_MALE:				/* 君だけは護るよ */
		if(sd && dstsd){
			int hp_rate=(skilllv <= 0)? 0:skill_db[skillid].hp_rate[skilllv-1];
			int gain_hp=dstsd->status.max_hp*abs(hp_rate)/100;// The earned is the same % of the target HP than it costed the caster. [Skotlex]
			gain_hp = battle_heal(NULL,bl,gain_hp,0,0);
			clif_skill_nodamage(src,bl,skillid,gain_hp,1);
		}
		break;
	case WE_FEMALE:				/* あなたの?に??ｵになります */
		if(sd && dstsd){
			int sp_rate=(skilllv <= 0)? 0:skill_db[skillid].sp_rate[skilllv-1];
			int gain_sp=dstsd->status.max_sp*abs(sp_rate)/100;// The earned is the same % of the target SP than it costed the caster. [Skotlex]
			gain_sp = battle_heal(NULL,bl,0,gain_sp,0);
			clif_skill_nodamage(src,bl,skillid,gain_sp,1);
		}
		break;

	case WE_CALLPARTNER:			/* あなたに?いたい */
		if(sd){
			if((dstsd = pc_get_partner(sd)) == NULL){
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			if(map[sd->bl.m].flag.nomemo || map[sd->bl.m].flag.nowarpto || map[dstsd->bl.m].flag.nowarp){
				clif_skill_teleportmessage(sd,1);
				map_freeblock_unlock();
				return 0;
			}
			skill_unitsetting(src,skillid,skilllv,sd->bl.x,sd->bl.y,0);
			pc_blockskill_start (sd, skillid, skill_get_time(skillid, skilllv));
		}
		break;

// parent-baby skills
	case WE_BABY:
		if(sd){
			struct map_session_data *f_sd = pc_get_father(sd);
			struct map_session_data *m_sd = pc_get_mother(sd);
			// if neither was found
			if(!f_sd && !m_sd){
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			status_change_start(bl,SC_STAN,skilllv,0,0,0,skill_get_time2(skillid,skilllv),0);
			if (f_sd) status_change_start(&f_sd->bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
			if (m_sd) status_change_start(&m_sd->bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		}
		break;

	case WE_CALLPARENT:
		if(sd){
			struct map_session_data *f_sd = pc_get_father(sd);
			struct map_session_data *m_sd = pc_get_mother(sd);
			// if neither was found
			if(!f_sd && !m_sd){
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			if(map[sd->bl.m].flag.nomemo || map[sd->bl.m].flag.nowarpto)
			{
				clif_skill_teleportmessage(sd,1);
				map_freeblock_unlock();
				return 0;
			}
			if((!f_sd && m_sd && map[m_sd->bl.m].flag.nowarp) ||
				(!m_sd && f_sd && map[f_sd->bl.m].flag.nowarp))
			{	//Case where neither one can be warped.
				clif_skill_teleportmessage(sd,1);
				map_freeblock_unlock();
				return 0;
			}
			//Warp those that can be warped.
			if (f_sd && !map[f_sd->bl.m].flag.nowarp)
				pc_setpos(f_sd,map[sd->bl.m].name,sd->bl.x,sd->bl.y,3);
			if (m_sd && !map[m_sd->bl.m].flag.nowarp)
				pc_setpos(m_sd,map[sd->bl.m].name,sd->bl.x,sd->bl.y,3);
		}
		break;

	case WE_CALLBABY:
		if(sd && dstsd)
		{
			if(map[sd->bl.m].flag.nomemo || map[sd->bl.m].flag.nowarpto || map[dstsd->bl.m].flag.nowarp){
				clif_skill_teleportmessage(sd,1);
				map_freeblock_unlock();
				return 0;
			}
			pc_setpos(dstsd,map[sd->bl.m].name,sd->bl.x,sd->bl.y,3);
		}
		break;

	case PF_HPCONVERSION:			/* ライフ置き換え */
		clif_skill_nodamage(src, bl, skillid, skilllv, 1);
		if (sd) {
			int hp, sp;
			hp = sd->status.max_hp / 10; //基本はHPの10%
			sp = hp * 10 * skilllv / 100;
			if (sd->status.sp + sp > sd->status.max_sp)
				sp = sd->status.max_sp - sd->status.sp;
			// we need to check with the sp that was taken away when casting too
			if (sd->status.sp + skill_get_sp(skillid, skilllv) >= sd->status.max_sp)
				hp = sp = 0;
			pc_heal(sd, -hp, sp);
			clif_heal(sd->fd, SP_SP, sp);
			clif_updatestatus(sd, SP_SP);
		}
		break;
	case HT_REMOVETRAP:				/* リム?ブトラップ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		{
			struct skill_unit *su=NULL;
			struct item item_tmp;
			int flag;
			if((bl->type==BL_SKILL) &&
			   (su=(struct skill_unit *)bl) &&
			   (su->group->src_id == src->id || map_flag_vs(bl->m)) &&
				(skill_get_inf2(su->group->skill_id) & INF2_TRAP))
			{	
				if(sd && su->group->val3 != BD_INTOABYSS)
				{	//Avoid collecting traps when it does not costs to place them down. [Skotlex]
					if(battle_config.skill_removetrap_type){
						for(i=0;i<10;i++) {
							if(skill_db[su->group->skill_id].itemid[i] > 0){
								memset(&item_tmp,0,sizeof(item_tmp));
								item_tmp.nameid = skill_db[su->group->skill_id].itemid[i];
								item_tmp.identify = 1;
								if(item_tmp.nameid && (flag=pc_additem(sd,&item_tmp,skill_db[su->group->skill_id].amount[i]))){
									clif_additem(sd,0,0,flag);
									map_addflooritem(&item_tmp,skill_db[su->group->skill_id].amount[i],sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
								}
							}
						}
					}else{
						memset(&item_tmp,0,sizeof(item_tmp));
						item_tmp.nameid = 1065;
						item_tmp.identify = 1;
						if(item_tmp.nameid && (flag=pc_additem(sd,&item_tmp,1))){
							clif_additem(sd,0,0,flag);
							map_addflooritem(&item_tmp,1,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
						}
					}
				}
				if(su->group->unit_id == UNT_ANKLESNARE && su->group->val2){
					struct block_list *target=map_id2bl(su->group->val2);
					if(target && (target->type == BL_PC || target->type == BL_MOB))
						status_change_end(target,SC_ANKLE,-1);
				}
				skill_delunit(su);
			}
		}
		break;
	case HT_SPRINGTRAP:				/* スプリングトラップ */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		{
			struct skill_unit *su=NULL;
			if((bl->type==BL_SKILL) && (su=(struct skill_unit *)bl) && (su->group) ){
				switch(su->group->unit_id){
					case UNT_ANKLESNARE:	// ankle snare
						if (su->group->val2 != 0)
							// if it is already trapping something don't spring it,
							// remove trap should be used instead
							break;
						// otherwise fallthrough to below
					case UNT_BLASTMINE:
					case UNT_SKIDTRAP:
					case UNT_LANDMINE:
					case UNT_SHOCKWAVE:
					case UNT_SANDMAN:
					case UNT_FLASHER:
					case UNT_FREEZINGTRAP:
					case UNT_CLAYMORETRAP:
					case UNT_TALKIEBOX:
						su->group->unit_id = UNT_USED_TRAPS;
						clif_changelook(bl,LOOK_BASE,su->group->unit_id);
						su->group->limit=DIFF_TICK(tick+1500,su->group->tick);
						su->limit=DIFF_TICK(tick+1500,su->group->tick);
				}
			}
		}
		break;
	case BD_ENCORE:					/* アンコ?ル */
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		if(sd)
			skill_use_id(sd,src->id,sd->skillid_dance,sd->skilllv_dance);
		break;

	case AS_SPLASHER:		/* ベナムスプラッシャ? */
		if(status_get_max_hp(bl)*2/3 < status_get_hp(bl)) { //HPが2/3以?�?っていたら失敗
			map_freeblock_unlock();
			return 1;
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,skillid,src->id,skill_get_time(skillid,skilllv),1000,0 );
		break;

	case PF_MINDBREAKER:		/* プ�?ボック */
		{
			struct status_change *sc_data = status_get_sc_data(bl);

			/* MVPmobと不死には?かない */
			if((dstmd && status_get_mode(bl)&MD_BOSS) || battle_check_undead(status_get_race(bl),status_get_elem_type(bl))) //不死には?かない
			{
				map_freeblock_unlock();
				return 1;
			}

			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			if (rand()%100 > 55 + skilllv*5)
			{	//Has a 55% + skilllv*5% success chance.
				if (sd)
					clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );

			if(dstmd && dstmd->skilltimer!=-1 && dstmd->state.skillcastcancel)	// 詠?･妨害
				skill_castcancel(bl,0);
			if(dstsd && dstsd->skilltimer!=-1 && (!dstsd->special_state.no_castcancel || map_flag_gvg(bl->m))
				&& dstsd->state.skillcastcancel	&& !dstsd->special_state.no_castcancel2)
				skill_castcancel(bl,0);

			if(sc_data){
				if(sc_data[SC_FREEZE].timer!=-1)
					status_change_end(bl,SC_FREEZE,-1);
				if(sc_data[SC_STONE].timer!=-1 && sc_data[SC_STONE].val2==0)
					status_change_end(bl,SC_STONE,-1);
				if(sc_data[SC_SLEEP].timer!=-1)
					status_change_end(bl,SC_SLEEP,-1);
			}

			if(dstmd)
				mob_target(dstmd,src,skill_get_range2(src,skillid,skilllv));
		}
		break;

	case PF_SOULCHANGE:
		{
			int sp1 = 0, sp2 = 0;
			if (sd) {
				if (dstsd) {
					sp1 = sd->status.sp > dstsd->status.max_sp ? dstsd->status.max_sp : sd->status.sp;
					sp2 = dstsd->status.sp > sd->status.max_sp ? sd->status.max_sp : dstsd->status.sp;
					sd->status.sp = sp2;
					dstsd->status.sp = sp1;
					clif_heal(sd->fd,SP_SP,sp2);
					clif_updatestatus(sd,SP_SP);
					clif_heal(dstsd->fd,SP_SP,sp1);
					clif_updatestatus(dstsd,SP_SP);
				} else if (dstmd) {
					if (dstmd->state.soul_change_flag) {
						clif_skill_fail(sd,skillid,0,0);
						map_freeblock_unlock();
						return 0;
					}
					sp2 = sd->status.max_sp * 3 /100;
					if (sd->status.sp + sp2 > sd->status.max_sp)
						sp2 = sd->status.max_sp - sd->status.sp;
					sd->status.sp += sp2;
					clif_heal(sd->fd,SP_SP,sp2);
					clif_updatestatus(sd,SP_SP);
					dstmd->state.soul_change_flag = 1;
				}
			}
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	// Slim Pitcher
	case CR_SLIMPITCHER:
		{
			if (sd && flag&1) {
				struct block_list tbl;
				int hp = potion_hp * (100 + pc_checkskill(sd,CR_SLIMPITCHER)*10 + pc_checkskill(sd,AM_POTIONPITCHER)*10 + pc_checkskill(sd,AM_LEARNINGPOTION)*5)/100;
				hp = hp * (100 + (status_get_vit(bl)<<1))/100;
				if (dstsd) {
					hp = hp * (100 + pc_checkskill(dstsd,SM_RECOVERY)*10)/100;
				}
				tbl.id = 0;
				tbl.m = src->m;
				tbl.x = src->x;
				tbl.y = src->y;
				clif_skill_nodamage(&tbl,bl,AL_HEAL,hp,1);
				battle_heal(NULL,bl,hp,0,0);
			}
		}
		break;
	// Full Chemical Protection
	case CR_FULLPROTECTION:
		{
			int i, skilltime;
			struct status_change *tsc_data = status_get_sc_data(bl);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
			skilltime = skill_get_time(skillid,skilllv);
			for (i=0; i<4; i++) {
				if(tsc_data && tsc_data[SC_STRIPWEAPON + i].timer != -1)
					status_change_end(bl, SC_STRIPWEAPON + i, -1 );
				status_change_start(bl,SC_CP_WEAPON + i,skilllv,0,0,0,skilltime,0 );
			}
		}
		break;

	case RG_CLEANER:	//AppleGirl
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		break;

	case ST_PRESERVE:
		if (sd){
			if (sd->sc_count && sd->sc_data[SC_PRESERVE].timer != -1)
				status_change_end(src, SC_PRESERVE, -1 );
			else
				status_change_start(src,SC_PRESERVE,skilllv,0,0,0,skill_get_time(skillid, skilllv),0 );
			clif_skill_nodamage(src,src,skillid,skilllv,1);
		}
		break;

	case PF_DOUBLECASTING:
		if (rand() % 100 > 30 + skilllv * 10) {
			clif_skill_fail(sd,skillid,0,0);
			map_freeblock_unlock();
			return 0;
		}
		clif_skill_nodamage(src,bl,skillid,skilllv,1);
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	case CG_LONGINGFREEDOM:
		{
			struct status_change *sc_data = status_get_sc_data(src);
			if (sc_data && sc_data[SC_LONGING].timer == -1 && sc_data[SC_DANCING].timer != -1 && sc_data[SC_DANCING].val4
				&& sc_data[SC_DANCING].val1 != CG_MOONLIT) //Can't use Longing for Freedom while under Moonlight Petals. [Skotlex]
			{
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,0,0,skill_get_time(skillid,skilllv),0);
			}
		}
		break;

	case CG_TAROTCARD:
		{
			int eff, count = -1;
			if (rand() % 100 > skilllv * 8) {
				if (sd) clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			do {
				eff = rand() % 14;
				clif_specialeffect(bl, 523 + eff, 0);
				switch (eff)
				{
				case 0:	// heals SP to 0
					if (dstsd) pc_heal(dstsd,0,-dstsd->status.sp);
					break;
				case 1:	// matk halved
					status_change_start(bl,SC_INCMATKRATE,-50,0,0,0,30000,0);
					break;
				case 2:	// all buffs removed
					status_change_clear_buffs(bl);
					break;
				case 3:	// 1000 damage, random armor destroyed
					{
						int where[] = { EQP_ARMOR, EQP_SHIELD, EQP_HELM };
						battle_damage(src, bl, 1000, 0);
						clif_damage(src,bl,tick,0,0,1000,0,0,0);
						if (dstsd && battle_config.equip_skill_break_rate) pc_break_equip(dstsd, where[rand() % 3]);
					}
					break;
				case 4:	// atk halved
					status_change_start(bl,SC_INCATKRATE,-50,0,0,0,30000,0);
					break;
				case 5:	// 2000HP heal, random teleported
					battle_heal(src, src, 2000, 0, 0);
					if(sd && !map[src->m].flag.noteleport) pc_setpos(sd,sd->mapname,-1,-1,3);
					else if(md && !map[src->m].flag.monster_noteleport) mob_warp(md,-1,-1,-1,3);
					break;
				case 6:	// random 2 other effects
					if (count == -1)
						count = 3;
					else
						count++; //Should not retrigger this one.
					break;
				case 7:	// stop freeze or stoned
					{
						int sc[] = { SC_STOP, SC_FREEZE, SC_STONE };
						status_change_start(bl,sc[rand()%3],skilllv,0,0,0,30000,0);
					}
					break;
				case 8:	// curse coma and poison
					status_change_start(bl,SC_COMA,skilllv,0,0,0,30000,0);
					status_change_start(bl,SC_CURSE,skilllv,0,0,0,30000,0);
					status_change_start(bl,SC_POISON,skilllv,0,0,0,30000,0);
					break;
				case 9:	// chaos
					status_change_start(bl,SC_CONFUSION,skilllv,0,0,0,30000,0);
					break;
				case 10:	// 6666 damage, atk matk halved, cursed
					battle_damage(src, bl, 6666, 0);
					clif_damage(src,bl,tick,0,0,6666,0,0,0);
					status_change_start(bl,SC_INCATKRATE,-50,0,0,0,30000,0);
					status_change_start(bl,SC_INCMATKRATE,-50,0,0,0,30000,0);
					status_change_start(bl,SC_CURSE,skilllv,0,0,0,30000,0);
					break;
				case 11:	// 4444 damage
					battle_damage(src, bl, 4444, 0);
					clif_damage(src,bl,tick,0,0,4444,0,0,0);
					break;
				case 12:	// stun
					status_change_start(bl,SC_STAN,skilllv,0,0,0,5000,0);
					break;
				case 13:	// atk,matk,hit,flee,def reduced
					status_change_start(bl,SC_INCATKRATE,-20,0,0,0,30000,0);
					status_change_start(bl,SC_INCMATKRATE,-20,0,0,0,30000,0);
					status_change_start(bl,SC_INCHITRATE,-20,0,0,0,30000,0);
					status_change_start(bl,SC_INCFLEERATE,-20,0,0,0,30000,0);
					status_change_start(bl,SC_INCDEFRATE,-20,0,0,0,30000,0);
					break;
				default:
					break;			
				}			
			} while ((--count) > 0);
			clif_skill_nodamage(src,bl,skillid,skilllv,1);
		}
		break;

	case SL_ALCHEMIST:
	case SL_ASSASIN:
	case SL_BARDDANCER:
	case SL_BLACKSMITH:
	case SL_CRUSADER:
	case SL_HUNTER:
	case SL_KNIGHT:
	case SL_MONK:
	case SL_PRIEST:
	case SL_ROGUE:
	case SL_SAGE:
	case SL_SOULLINKER:
	case SL_STAR:
	case SL_SUPERNOVICE:
	case SL_WIZARD:
		if (sd && !(dstsd && (dstsd->class_&MAPID_UPPERMASK) == SkillStatusChangeTable[skillid])) {
			clif_skill_fail(sd,skillid,0,0);
			break;
		}
		status_change_start(bl,SC_SPIRIT,skilllv,skillid,0,0,skill_get_time(skillid,skilllv),0);
		break;
	case SL_HIGH:
		if (sd && !(dstsd && (dstsd->class_&JOBL_UPPER) && !(dstsd->class_&JOBL_2))) {
			clif_skill_fail(sd,skillid,0,0);
			break;
		}
		status_change_start(bl,SkillStatusChangeTable[skillid],skilllv,skillid,0,0,skill_get_time(skillid,skilllv),0 );
		break;

	// New guild skills [Celest]
	case GD_BATTLEORDER:
		{
			struct guild *g = NULL;
			// Only usable during WoE
			if (!agit_flag) {
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			if(flag&1) {
				if (dstsd && dstsd->status.guild_id == sd->status.guild_id)	{
					status_change_start(&dstsd->bl,SC_BATTLEORDERS,skilllv,0,0,0,0,0 );
				}
			}
			else if (sd && sd->status.guild_id > 0 && (g = guild_search(sd->status.guild_id)) &&
				strcmp(sd->status.name,g->master)==0) {
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				map_foreachinarea(skill_area_sub,
					src->m,src->x-15,src->y-15,src->x+15,src->y+15,0,
					src,skillid,skilllv,tick, flag|BCT_ALL|1,
					skill_castend_nodamage_id);
				pc_blockskill_start (sd, skillid, 300000);
			}
		}
		break;
	case GD_REGENERATION:
		{
			struct guild *g = NULL;
			// Only usable during WoE
			if (!agit_flag) {
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			if(flag&1) {
				if (dstsd && dstsd->status.guild_id == sd->status.guild_id)	{
					status_change_start(&dstsd->bl,SC_REGENERATION,skilllv,0,0,0,0,0 );
				}
			}
			else if (sd && sd->status.guild_id > 0 && (g = guild_search(sd->status.guild_id)) &&
				strcmp(sd->status.name,g->master)==0) {
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				map_foreachinarea(skill_area_sub,
					src->m,src->x-15,src->y-15,src->x+15,src->y+15,0,
					src,skillid,skilllv,tick, flag|BCT_ALL|1,
					skill_castend_nodamage_id);
				pc_blockskill_start (sd, skillid, 300000);
			}
		}
		break;
	case GD_RESTORE:
		{
			struct guild *g = NULL;
			// Only usable during WoE
			if (!agit_flag) {
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			if(flag&1) {
				if (dstsd && dstsd->status.guild_id == sd->status.guild_id)	{
					int hp, sp;
					hp = dstsd->status.max_hp*9/10;
					sp = dstsd->status.max_sp*9/10;
					sp = dstsd->status.sp + sp <= dstsd->status.max_sp ? sp : dstsd->status.max_sp - dstsd->status.sp;
					clif_skill_nodamage(src,bl,AL_HEAL,hp,1);
					battle_heal(NULL,bl,hp,sp,0);
				}
			}
			else if (sd && sd->status.guild_id > 0 && (g = guild_search(sd->status.guild_id)) &&
				strcmp(sd->status.name,g->master)==0) {
				clif_skill_nodamage(src,bl,skillid,skilllv,1);
				map_foreachinarea(skill_area_sub,
					src->m,src->x-15,src->y-15,src->x+15,src->y+15,0,
					src,skillid,skilllv,tick, flag|BCT_ALL|1,
					skill_castend_nodamage_id);
				pc_blockskill_start (sd, skillid, 300000);
			}
		}
		break;
	case GD_EMERGENCYCALL:
		{
			int dx[9]={-1, 1, 0, 0,-1, 1,-1, 1, 0};
			int dy[9]={ 0, 0, 1,-1, 1,-1,-1, 1, 0};
			int j = 0;
			struct guild *g = NULL;
			if (!sd || !sd->state.gmaster_flag)
				break;
			//Reports say this particular skill is usable anywhere! o.o [Skotlex]
			if	(map[sd->bl.m].flag.nowarpto && !map_flag_gvg(sd->bl.m))
			{	//if not allowed to warp to the map (castles are always allowed)
				clif_skill_fail(sd,skillid,0,0);
				map_freeblock_unlock();
				return 0;
			}
			// i don't know if it actually summons in a circle, but oh well. ;P
			g = sd->state.gmaster_flag;
			for(i = 0; i < g->max_member; i++, j++) {
				if (j>8) j=0;
				if ((dstsd = g->member[i].sd) != NULL && sd != dstsd) {
					 if (map[dstsd->bl.m].flag.nowarp && !map_flag_gvg(dstsd->bl.m))
						 continue;
					clif_skill_nodamage(src,bl,skillid,skilllv,1);
					if(map_getcell(sd->bl.m,sd->bl.x+dx[j],sd->bl.y+dy[j],CELL_CHKNOPASS))
						dx[j] = dy[j] = 0;
					pc_setpos(dstsd, sd->mapname, sd->bl.x+dx[j], sd->bl.y+dy[j], 2);
				}
			}
			pc_blockskill_start (sd, skillid, 300000);
		}
		break;

	default:
		ShowWarning("Unknown skill used:%d\n",skillid);
		map_freeblock_unlock();
		return 1;
	}

	map_freeblock_unlock();
	return 0;
}

/*==========================================
 * スキル使用?i詠?･完了?AID指定?j
 *------------------------------------------
 */
int skill_castend_id( int tid, unsigned int tick, int id,int data )
{
	struct map_session_data* sd = map_id2sd(id)/*,*target_sd=NULL*/;
	struct block_list *bl;
	int delay,inf2;

	nullpo_retr(0, sd);

//Code cleanup.
#undef skill_failed
#define skill_failed(sd) { sd->skillid = sd->skilllv = sd->skillitem = sd->skillitemlv = -1; sd->canact_tick = sd->canmove_tick = tick; }

	if(sd->skillid != SA_CASTCANCEL && sd->skilltimer != tid )
	{	/* タイマIDの確認 */
		ShowError("skill_castend_id: Timer mismatch %d!=%d!\n", sd->skilltimer, tid);
		sd->skilltimer = -1;
		return 0;
	}

	if( sd->bl.prev == NULL || sd->skillid == -1 || sd->skilllv == -1)
	{	//Finished casting between maps, or the skill has failed after starting casting{
		sd->skilltimer = -1;
		skill_failed(sd);
		return 0;
	}

	if(sd->skillid != SA_CASTCANCEL && sd->skilltimer != -1 && (delay = pc_checkskill(sd,SA_FREECAST) > 0)) //Hope ya don't mind me borrowing delay :X
		status_quick_recalc_speed(sd, SA_FREECAST, delay, 0);

	if(sd->skillid != SA_CASTCANCEL)
		sd->skilltimer=-1;

	if((bl=map_id2bl(sd->skilltarget))==NULL ||
		bl->prev==NULL || sd->bl.m != bl->m) {
		skill_failed(sd);
		return 0;
	}
	
	if(sd->skillid == RG_BACKSTAP) {
		int dir = map_calc_dir(&sd->bl,bl->x,bl->y),t_dir = status_get_dir(bl);
		int dist = distance(sd->bl.x,sd->bl.y,bl->x,bl->y);
		if(bl->type != BL_SKILL && (dist == 0 || map_check_dir(dir,t_dir))) {
			clif_skill_fail(sd,sd->skillid,0,0);
			skill_failed(sd);
			return 0;
		}
	}

	if (sd->skillid == PR_LEXDIVINA)
	{
		struct status_change *sc_data = status_get_sc_data(bl);
		if (battle_check_target(&sd->bl,bl, BCT_ENEMY)<=0 &&
			(!sc_data || sc_data[SC_SILENCE].timer == -1)) //If it's not an enemy, and not silenced, you can't use the skill on them. [Skotlex]
		{
			skill_failed(sd);
			return 0;
		}
	} else {
		inf2 = skill_get_inf(sd->skillid);
		if((inf2&INF_ATTACK_SKILL ||
			(inf2&INF_SELF_SKILL && sd->bl.id != bl->id && skill_get_nk(sd->skillid) != NK_NO_DAMAGE)) //Self skills that cause damage (EF, Combo Skills, etc)
			&& battle_check_target(&sd->bl,bl, BCT_ENEMY)<=0
		) {
			skill_failed(sd);
			return 0;
		}
	}
	if (tid != -1 && !status_check_skilluse(&sd->bl, bl, sd->skillid, 1))
	{	//Avoid doing double checks for instant-cast skills.
		if(sd->skillid == PR_LEXAETERNA) //Eh.. assuming skill failed due to opponent frozen/stone-cursed. [Skotlex]
			clif_skill_fail(sd,sd->skillid,0,0);
		skill_failed(sd);
		return 0;
	}

	inf2 = skill_get_inf2(sd->skillid);
	if(inf2 & (INF2_PARTY_ONLY|INF2_GUILD_ONLY) && sd->bl.id != bl->id) {
		int fail_flag = 1;
		if(inf2 & INF2_PARTY_ONLY && battle_check_target(&sd->bl,bl, BCT_PARTY) > 0)
			fail_flag = 0;
		else if(inf2 & INF2_GUILD_ONLY && battle_check_target(&sd->bl,bl, BCT_GUILD) > 0)
			fail_flag = 0;
		
		if (sd->skillid == PF_SOULCHANGE && map_flag_vs(sd->bl.m))
			//Soul Change overrides this restriction during pvp/gvg [Skotlex]
			fail_flag = 0;
		
		if(fail_flag) {
			clif_skill_fail(sd,sd->skillid,0,0);
			skill_failed(sd);
			return 0;
		}
	}

	if(skill_get_range2(&sd->bl,sd->skillid,sd->skilllv)+battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,bl->x,bl->y))
	{
		clif_skill_fail(sd,sd->skillid,0,0);
		if(battle_config.skill_out_range_consume) //Consume items anyway. [Skotlex]
			skill_check_condition(sd,1);
			
		skill_failed(sd);
		return 0;
	}

	if(!skill_check_condition(sd,1)) {		/* 使用?�?チェック */
		skill_failed(sd);
		return 0;
	}

	if(battle_config.pc_skill_log)
		ShowInfo("PC %d skill castend skill=%d\n",sd->bl.id,sd->skillid);
	pc_stop_walking(sd,0);

	if (sd->skillid == SA_MAGICROD)
		delay = 0;
	else
		delay = skill_delayfix(&sd->bl, sd->skillid, sd->skilllv, 0);
		
	sd->canact_tick = tick + delay;
	if (skill_get_delaynowalk(sd->skillid, sd->skilllv)) //Skills that block you from moving until delay ends. [Skotlex]
		sd->canmove_tick = tick + delay;
	switch( skill_get_nk(sd->skillid) )
	{
	case NK_NO_DAMAGE:
		if( (sd->skillid==AL_HEAL || (sd->skillid==ALL_RESURRECTION && bl->type != BL_PC) || sd->skillid==PR_ASPERSIO) && battle_check_undead(status_get_race(bl),status_get_elem_type(bl)))
		{
			if (sd->skillid == AL_HEAL && battle_check_target(&sd->bl, bl, BCT_ENEMY) < 1)
				clif_skill_fail(sd,sd->skillid,0,0); //Offensive heal does not works on non-enemies. [Skotlex]
			else
				skill_castend_damage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		}	
		else
			skill_castend_nodamage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		break;
	case NK_SPLASH_DAMAGE:
	default:
		skill_castend_damage_id(&sd->bl,bl,sd->skillid,sd->skilllv,tick,0);
		break;
	}

	if(sd->sc_data[SC_MAGICPOWER].timer != -1 && sd->skillid != HW_MAGICPOWER)
		status_change_end(&sd->bl,SC_MAGICPOWER,-1);		
		
	if (sd->skillid != AL_TELEPORT && sd->skillid != WS_WEAPONREFINE)
		sd->skillid = sd->skilllv = -1; //Clean this up for future references to battle_getcurrentskill. [Skotlex]
	return 0;
}

/*---------------------------------------------------------------------------- */

/*==========================================
 * スキル使用?i詠?･完了?A?�?且w定?j
 *------------------------------------------
 */
int skill_castend_pos( int tid, unsigned int tick, int id,int data )
{
	struct map_session_data* sd=map_id2sd(id)/*,*target_sd=NULL*/;
	int delay,maxcount;

	nullpo_retr(0, sd);

//Code cleanup.
#undef skill_failed
#define skill_failed(sd) { sd->skillid = sd->skilllv = sd->skillitem = sd->skillitemlv = -1; sd->canact_tick = sd->canmove_tick = tick; }

	if( sd->skilltimer != tid )
	{	/* タイマIDの確認 */
		ShowError("skill_castend_pos: Timer mismatch %d!=%d\n", sd->skilltimer, tid);
		sd->skilltimer = -1;
		return 0;
	}

	if(sd->skillid != SA_CASTCANCEL && sd->skilltimer != -1 && (delay = pc_checkskill(sd,SA_FREECAST) > 0)) //Hope ya don't mind me borrowing delay :X
		status_quick_recalc_speed(sd, SA_FREECAST, delay, 0);

	sd->skilltimer=-1;
	if (sd->bl.prev == NULL || sd->skillid == -1 || sd->skilllv <= 0)
	{	// skill has failed after starting casting
		return 0;
	}

	if (!battle_config.pc_skill_reiteration &&
			skill_get_unit_flag(sd->skillid)&UF_NOREITERATION &&
			skill_check_unit_range(sd->bl.m,sd->skillx,sd->skilly,sd->skillid,sd->skilllv)) {
		clif_skill_fail(sd,sd->skillid,0,0);
		skill_failed(sd);
		return 0;
	}

	if (battle_config.pc_skill_nofootset &&
			skill_get_unit_flag(sd->skillid)&UF_NOFOOTSET &&
			skill_check_unit_range2(&sd->bl,sd->bl.m,sd->skillx,sd->skilly,sd->skillid,sd->skilllv)) {
		clif_skill_fail(sd,sd->skillid,0,0);
		skill_failed(sd);
		return 0;
	}
	if(battle_config.pc_land_skill_limit) {
		maxcount = skill_get_maxcount(sd->skillid);
		if(maxcount > 0) {
			int i,c;
			for(i=c=0;i<MAX_SKILLUNITGROUP;i++) {
				if(sd->skillunit[i].alive_count > 0 && sd->skillunit[i].skill_id == sd->skillid)
					c++;
			}
			if(c >= maxcount) {
				clif_skill_fail(sd,sd->skillid,0,0);
				skill_failed(sd);
				return 0;
			}
		}
	}

	if(tid != -1)
	{	//Avoid double checks on instant cast skills. [Skotlex]
		if (!status_check_skilluse(&sd->bl, NULL, sd->skillid, 1))
		{
			skill_failed(sd);
			return 0;
		}
		if(skill_get_range2(&sd->bl,sd->skillid,sd->skilllv)+battle_config.pc_skill_add_range < distance(sd->bl.x,sd->bl.y,sd->skillx,sd->skilly)) {
			clif_skill_fail(sd,sd->skillid,0,0);
			if(battle_config.skill_out_range_consume) //Consume items anyway.
				skill_check_condition(sd,1);
			
			skill_failed(sd);
			return 0;
		}
	}

	if(!skill_check_condition(sd,1)) {		/* 使用?�?チェック */
		skill_failed(sd);
		return 0;
	}
	if(battle_config.pc_skill_log)
		ShowInfo("PC %d skill castend skill=%d\n",sd->bl.id,sd->skillid);
	pc_stop_walking(sd,0);

	delay = skill_delayfix(&sd->bl, sd->skillid, sd->skilllv, 0);
	sd->canact_tick = tick + delay;
	if (skill_get_delaynowalk(sd->skillid, sd->skilllv)) //Skills that block you from moving until delay ends. [Skotlex]
		sd->canmove_tick = tick + delay;

	skill_castend_pos2(&sd->bl,sd->skillx,sd->skilly,sd->skillid,sd->skilllv,tick,0);

	if (sd->skillid != AL_WARP)
		sd->skillid = sd->skilllv = -1; //Clean this up for future references to battle_getcurrentskill. [Skotlex]
	return 0;
}

/*==========================================
 * スキル使用?i詠?･完了?A?�?且w定の??ﾛの?�??j
 *------------------------------------------
 */
int skill_castend_pos2( struct block_list *src, int x,int y,int skillid,int skilllv,unsigned int tick,int flag)
{
	struct map_session_data *sd=NULL;
	struct status_change *sc_data;
	int i,tmpx = 0,tmpy = 0, x1 = 0, y1 = 0;

	//if(skilllv <= 0) return 0;
	if(skillid > 0 && skilllv <= 0) return 0;	// celest

	nullpo_retr(0, src);

	if(src->type==BL_PC)
		sd=(struct map_session_data *)src;

	sc_data = status_get_sc_data(src); //Needed for Magic Power checks.
	
	if( skillid != WZ_METEOR &&
		skillid != AM_CANNIBALIZE &&
		skillid != AM_SPHEREMINE &&
		skillid != CR_CULTIVATION &&
		skillid != AC_SHOWER)
		clif_skill_poseffect(src,skillid,skilllv,x,y,tick);

//Shouldn't be needed, skillnotok's return value is highly unlikely to have changed after you started casting. [Skotlex]
//	if (sd && skillnotok(skillid, sd)) // [MouseJstr]
//		return 0;

	switch(skillid)
	{
	case PR_BENEDICTIO:			/* ?ｹ??~福 */
		skill_area_temp[1] = src->id;
		map_foreachinarea(skill_area_sub, 
			src->m, x-1, y-1, x+1, y+1, BL_PC,
			src, skillid, skilllv, tick, flag|BCT_ALL|1,
			skill_castend_nodamage_id);
		map_foreachinarea(skill_area_sub,
			src->m, x-1, y-1, x+1, y+1, 0,
			src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
			skill_castend_damage_id);
		break;

	case AC_SHOWER:
		{
			int r = 2;
			map_foreachinarea (skill_area_sub,
				src->m, x-r, y-r, x+r, y+r, 0,
				src, skillid, skilllv, tick, flag|BCT_ENEMY|1,
				skill_castend_damage_id);
		}
		break;

	case BS_HAMMERFALL:
		{
			int r = 2;
			if (skilllv > 5) {
				r = 14;
				skilllv = 5;	// スタン率?繧ｪりすぎるため計算はLv5で固定
			}
			skill_area_temp[1] = src->id;
			skill_area_temp[2] = x;
			skill_area_temp[3] = y;
			map_foreachinarea (skill_area_sub,
				src->m, x-r, y-r, x+r, y+r, 0,
				src, skillid, skilllv, tick, flag|BCT_ENEMY|2,
				skill_castend_nodamage_id);
		}
		break;

	case HT_DETECTING:				/* ディテクティング */
		map_foreachinarea( status_change_timer_sub,
			src->m, x-1, y-1, x+1,y+1,0,
			src,SC_SIGHT,tick);
		break;

	case MG_SAFETYWALL:			/* セイフティウォ?ル */
	case MG_FIREWALL:			/* ファイヤ?ウォ?ル */
	case MG_THUNDERSTORM:		/* サンダ?スト?ム */
	case AL_PNEUMA:				/* ニュ?マ */
	case WZ_ICEWALL:			/* アイスウォ?ル */
	case WZ_FIREPILLAR:			/* ファイアピラ? */
	case WZ_QUAGMIRE:			/* クァグマイア */
	case WZ_VERMILION:			/* �??ドオブヴァ?ミリオン */
	case WZ_STORMGUST:			/* スト?ムガスト */
	case WZ_HEAVENDRIVE:		/* ヘヴンズドライブ */
	case PR_SANCTUARY:			/* サンクチュアリ */
	case PR_MAGNUS:				/* マグヌスエクソシズム */
	case CR_GRANDCROSS:			/* グランドク�?ス */
	case NPC_GRANDDARKNESS:		/*闇グランドク�?ス*/
	case HT_SKIDTRAP:			/* スキッドトラップ */
	case HT_LANDMINE:			/* ランドマイン */
	case HT_ANKLESNARE:			/* アンクルスネア */
	case HT_SHOCKWAVE:			/* ショックウェ?ブトラップ */
	case HT_SANDMAN:			/* サンドマン */
	case HT_FLASHER:			/* フラッシャ? */
	case HT_FREEZINGTRAP:		/* フリ?ジングトラップ */
	case HT_BLASTMINE:			/* ブラストマイン */
	case HT_CLAYMORETRAP:		/* クレイモア?トラップ */
	case AS_VENOMDUST:			/* ベノムダスト */
	case AM_DEMONSTRATION:			/* デモンストレ?ション */
	case PF_FOGWALL:			/* フォグウォ?ル */
	case PF_SPIDERWEB:			/* スパイダ?ウェッブ */
	case HT_TALKIEBOX:			/* ト?キ?ボックス */
		skill_unitsetting(src,skillid,skilllv,x,y,0);
		break;

	case RG_GRAFFITI:			/* Graffiti [Valaris] */
		skill_clear_unitgroup(src);
		skill_unitsetting(src,skillid,skilllv,x,y,0);
		break;

	case RG_CLEANER: // [Valaris]
		map_foreachinarea(skill_graffitiremover,src->m,x-5,y-5,x+5,y+5,BL_SKILL);
		break;
	case SA_VOLCANO:		/* ボルケ?ノ */
	case SA_DELUGE:			/* デリュ?ジ */
	case SA_VIOLENTGALE:	/* バイオレントゲイル */
	case SA_LANDPROTECTOR:	/* ランドプ�?テクタ? */
		skill_unitsetting(src,skillid,skilllv,x,y,0);
		break;

	case WZ_METEOR:				//�?テオスト?ム
		{
			int flag=0;
			if (sc_data && sc_data[SC_MAGICPOWER].timer != -1)
				flag = flag|2; //Store the magic power flag for future use. [Skotlex]
			for(i=0;i<2+(skilllv>>1);i++) {
				int j=0;
				do {
					tmpx = x + (rand()%7 - 3);
					tmpy = y + (rand()%7 - 3);
					if(tmpx < 0)
						tmpx = 0;
					else if(tmpx >= map[src->m].xs)
						tmpx = map[src->m].xs - 1;
					if(tmpy < 0)
						tmpy = 0;
					else if(tmpy >= map[src->m].ys)
						tmpy = map[src->m].ys - 1;
					j++;
				} while((map_getcell(src->m,tmpx,tmpy,CELL_CHKNOPASS)) && j<100);
				if(j >= 100)
					continue;
				if(!(flag&1)){
					clif_skill_poseffect(src,skillid,skilllv,tmpx,tmpy,tick);
					flag=flag|1;
				}
				if(i > 0)
					skill_addtimerskill(src,tick+i*1000,0,tmpx,tmpy,skillid,skilllv,(x1<<16)|y1,flag&2); //Only pass the Magic Power flag
				x1 = tmpx;
				y1 = tmpy;
			}
			skill_addtimerskill(src,tick+i*1000,0,tmpx,tmpy,skillid,skilllv,-1,flag&2); //Only pass the Magic Power flag
		}
		break;

	case AL_WARP:				/* �??プポ?タル */
		if(sd) {
			if(map[sd->bl.m].flag.noteleport)	/* テレポ禁止 */
				break;
			clif_skill_warppoint(sd,skillid,sd->status.save_point.map,
				(sd->skilllv>1)?sd->status.memo_point[0].map:"",
				(sd->skilllv>2)?sd->status.memo_point[1].map:"",
				(sd->skilllv>3)?sd->status.memo_point[2].map:"");
		}
		break;

	case MO_BODYRELOCATION:
		if (sd) {
			pc_movepos(sd, x, y, 1);
			pc_blockskill_start (sd, MO_EXTREMITYFIST, 2000);
		} else if (src->type == BL_MOB) {
			struct mob_data *md = (struct mob_data *)src;
			mob_warp(md, -1, x, y, 0);
			clif_spawnmob(md);
		}
		break;
	case AM_CANNIBALIZE:	// バイオプラント
		if(sd) {
			int id;
			int summons[5] = { 1020, 1068, 1118, 1500, 1368 };
			struct mob_data *md;

			// Correct info, don't change any of this! [celest]
			id = mob_once_spawn (sd, "this", x, y, sd->status.name, summons[skilllv-1] ,1,"");

			if( (md=(struct mob_data *)map_id2bl(id)) !=NULL ){
				md->master_id = sd->bl.id;
				// different levels of HP according to skill level
				md->hp = 1500 + skilllv * 200 + sd->status.base_level * 10;
				md->max_hp = md->hp; //Update the max, too! [Skotlex]
				md->special_state.ai = 1;
				//非移動でアクティブで反撃する[0x0:非移動 0x1:移動 0x4:ACT 0x8:非ACT 0x40:反撃無 0x80:反撃有]
				md->mode = MD_CANATTACK|MD_AGGRESSIVE;
				md->deletetimer = add_timer (gettick() + skill_get_time(skillid,skilllv), mob_timer_delete, id, 0);
			}
			// To-do: ?｢還されるモンスタ?[には?｢還したプレ?[ヤ?[の名前が付きます
			// (attach name of player?)
			clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
		}
		break;
	case AM_SPHEREMINE:	// スフィア?マイン
		if(sd){
			int id;
			struct mob_data *md;

			id = mob_once_spawn(sd, "this", x, y, sd->status.name, 1142, 1, "");
			if( (md=(struct mob_data *)map_id2bl(id)) !=NULL ){
				md->master_id = sd->bl.id;
				md->hp = 2000 + skilllv * 400;
				md->max_hp = md->hp; //Update the max, too! [Skotlex]
				md->mode = md->db->mode|MD_CANMOVE; //Needed for the skill
				md->special_state.ai = 2;
				md->deletetimer = add_timer (gettick() + skill_get_time(skillid,skilllv), mob_timer_delete, id, 0);
			}
			clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
		}
		break;

	// Slim Pitcher [Celest]
	case CR_SLIMPITCHER:
		{
			if (sd) {
				int i = skilllv%11 - 1;
				int j = pc_search_inventory(sd,skill_db[skillid].itemid[i]);
				if(j < 0 || skill_db[skillid].itemid[i] <= 0 || sd->inventory_data[j] == NULL ||
					sd->status.inventory[j].amount < skill_db[skillid].amount[i]) {
					clif_skill_fail(sd,skillid,0,0);
					return 1;
				}
				potion_flag = 1;
				potion_hp = 0;
				run_script(sd->inventory_data[j]->script,0,sd->bl.id,0);
				pc_delitem(sd,j,skill_db[skillid].amount[i],0);
				potion_flag = 0;
				clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
				if(potion_hp > 0) {
					map_foreachinarea(skill_area_sub,
						src->m,x-3,y-3,x+3,y+3,0,
						src,skillid,skilllv,tick,flag|BCT_PARTY|BCT_GUILD|1,
						skill_castend_nodamage_id);
				}
			}
		}
		break;

	case HW_GANBANTEIN:
		if (rand()%100 < 80) {
			clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
			map_foreachinarea (skill_ganbatein, src->m, x-1, y-1, x+1, y+1, BL_SKILL);
		} else {
			clif_skill_fail(sd,skillid,0,0);
			return 1;
		}
		break;
	
	case HW_GRAVITATION:
		{
			struct skill_unit_group *sg;
			clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
			sg = skill_unitsetting(src,skillid,skilllv,x,y,0);	
			status_change_start(src,SkillStatusChangeTable[skillid],skilllv,0,BCT_SELF,(int)sg,
				skill_get_time(skillid,skilllv),0);
		}
		break;

	// Plant Cultivation [Celest]
	case CR_CULTIVATION:
		{
			if (sd) {
				int i = skilllv - 1;
				int j = pc_search_inventory(sd,skill_db[skillid].itemid[i]);
				if(j < 0 || skill_db[skillid].itemid[i] <= 0 || sd->inventory_data[j] == NULL ||
					sd->status.inventory[j].amount < skill_db[skillid].amount[i]) {
					clif_skill_fail(sd,skillid,0,0);
					return 1;
				}
				pc_delitem(sd,j,skill_db[skillid].amount[i],0);
				clif_skill_poseffect(src,skillid,skilllv,x,y,tick);
				if (rand()%100 < 50)
					mob_once_spawn(sd, "this", x, y, "--ja--",(skilllv < 2 ? 1084+rand()%2 : 1078+rand()%6), 1, "");
				else
					clif_skill_fail(sd,skillid,0,0);
			}
		}
		break;
	}

	if (sc_data && sc_data[SC_MAGICPOWER].timer != -1)
		status_change_end(&sd->bl,SC_MAGICPOWER,-1);

	return 0;
}

/*==========================================
 * スキル使用?i詠?･完了?Amap指定?j
 *------------------------------------------
 */
int skill_castend_map( struct map_session_data *sd,int skill_num, const char *map)
{
	int x=0,y=0;

	nullpo_retr(0, sd);

//Simplify skill_failed code.
#undef skill_failed
#define skill_failed(sd) { sd->skillid = sd->skilllv = sd->skillitem = sd->skillitemlv = -1; }

	if( sd->bl.prev == NULL || pc_isdead(sd) )
		return 0;

//Shouldn't be needed, skillnotok's return value is highly unlikely to have changed after you started casting. [Skotlex]
//	if(skillnotok(skill_num, sd))
//		return 0;

	if( sd->opt1>0 || sd->status.option&2 ) {
		skill_failed(sd);
		return 0;
	}
	//スキルが使えない?態異?�中
	if(sd->sc_count){
		if( sd->sc_data[SC_SILENCE].timer!=-1 ||
			sd->sc_data[SC_ROKISWEIL].timer!=-1 ||
			sd->sc_data[SC_AUTOCOUNTER].timer != -1 ||
			sd->sc_data[SC_STEELBODY].timer != -1 ||
			sd->sc_data[SC_DANCING].timer!=-1 ||
			sd->sc_data[SC_BERSERK].timer != -1 ||
			sd->sc_data[SC_MARIONETTE].timer != -1)
			return 0;
	}

	if( skill_num != sd->skillid) /* 不?ｳパケットらしい */
		return 0;

	if (strlen(map) > MAP_NAME_LENGTH-1)
	{	//Map_length check, as it is sent by the client and we shouldn't trust it [Skotlex]
		if (battle_config.error_log)
			ShowError("skill_castend_map: Received map name '%s' too long!\n", map);
		skill_failed(sd);
		return 0;
	}
	pc_stopattack(sd);

	if(battle_config.pc_skill_log)
		ShowInfo("PC %d skill castend skill =%d map=%s\n",sd->bl.id,skill_num,map);
	pc_stop_walking(sd,0);

	if(strcmp(map,"cancel")==0) {
		skill_failed(sd);
		return 0;
	}
	
	switch(skill_num){
	case AL_TELEPORT:		/* テレポ?ト */
		if(strcmp(map,"Random")==0)
			pc_randomwarp(sd,3);
		else
			pc_setpos(sd,sd->status.save_point.map,
				sd->status.save_point.x,sd->status.save_point.y,3);
		break;

	case AL_WARP:			/* �??プポ?タル */
		{
			const struct point *p[4];
			struct skill_unit_group *group;
			int i;
			int maxcount=0;
			p[0] = &sd->status.save_point;
			p[1] = &sd->status.memo_point[0];
			p[2] = &sd->status.memo_point[1];
			p[3] = &sd->status.memo_point[2];

			if((maxcount = skill_get_maxcount(skill_num)) > 0) {
				int c;
				for(i=c=0;i<MAX_SKILLUNITGROUP;i++) {
					if(sd->skillunit[i].alive_count > 0 && sd->skillunit[i].skill_id == skill_num)
						c++;
				}
				if(c >= maxcount) {
					clif_skill_fail(sd,skill_num,0,0);
					skill_failed(sd);
					return 0;
				}
			}

			if(sd->skilllv <= 0) return 0;
			for(i=0;i<sd->skilllv;i++){
				if(strcmp(map,p[i]->map)==0){
					x=p[i]->x;
					y=p[i]->y;
					break;
				}
			}
			if(x==0 || y==0) {	/* 不?ｳパケット?H */
				skill_failed(sd);
				return 0;
			}
			
			if(!skill_check_condition(sd,3))
			{
				skill_failed(sd);
				return 0;
			}
			
			if(skill_check_unit_range2(&sd->bl,sd->bl.m,sd->skillx,sd->skilly,skill_num,sd->skilllv) > 0) {
				clif_skill_fail(sd,0,0,0);
				skill_failed(sd);
				return 0;
			}
			if((group=skill_unitsetting(&sd->bl,skill_num,sd->skilllv,sd->skillx,sd->skilly,0))==NULL) {
				skill_failed(sd);
				return 0;
			}
			group->valstr=(char *)aCallocA(MAP_NAME_LENGTH,sizeof(char));
			memcpy(group->valstr,map,MAP_NAME_LENGTH-1);
			group->val2=(x<<16)|y;
		}
		break;
	}

	sd->skillid = sd->skilllv = -1;
	return 0;
}

/*==========================================
 * Initializes and sets a ground skill.
 * flag&1 is used to determine when the skill 'morphs' (Warp portal becomes active, or Fire Pillar becomes active)
 * flag&2 is used to determine if this skill was casted with Magic Power active.
 *------------------------------------------
 */
struct skill_unit_group *skill_unitsetting( struct block_list *src, int skillid,int skilllv,int x,int y,int flag)
{
	struct skill_unit_group *group;
	int i,limit,val1=0,val2=0,val3=0;
	int count=0;
	int target,interval,range,unit_flag;
	struct skill_unit_layout *layout;
	struct status_change *sc_data;
	int active_flag=1;

	nullpo_retr(0, src);

	limit = skill_get_time(skillid,skilllv);
	range = skill_get_unit_range(skillid);
	interval = skill_get_unit_interval(skillid);
	target = skill_get_unit_target(skillid);
	unit_flag = skill_get_unit_flag(skillid);
	layout = skill_get_unit_layout(skillid,skilllv,src,x,y);

	if (unit_flag&UF_DEFNOTENEMY && battle_config.defnotenemy)
		target = BCT_NOENEMY;

	sc_data = status_get_sc_data(src);	// for traps, firewall and fogwall - celest

	switch(skillid){	/* ?ﾝ定 */

	case MG_SAFETYWALL:			/* セイフティウォ?ル */
		val2=skilllv+1;
		break;
	case MG_FIREWALL:			/* ファイヤ?ウォ?ル */
		if(sc_data && sc_data[SC_VIOLENTGALE].timer!=-1)
			limit = limit*3/2;
		val2=4+skilllv;
		break;

	case AL_WARP:				/* �??プポ?タル */
		val1=skilllv+6;
		if(!(flag&1))
			limit=2000;
		active_flag=0;
		break;

	case PR_SANCTUARY:			/* サンクチュアリ */
		val1=(skilllv+3)*2;
		val2=(skilllv>6)?777:skilllv*100;
		break;

	case WZ_FIREPILLAR:			/* ファイア?ピラ? */
		if((flag&1)!=0)
			limit=1000;
		val1=skilllv+2;
		if(skilllv >= 6)
			range=2;
		break;
	case WZ_METEOR:
		if (skilllv > 10)			//?L範囲�?テオ
			range = 10;
		break;
	case WZ_VERMILION:
		if (skilllv > 10)			//?L範囲LOV
			range = 25;
		break;
	case WZ_QUAGMIRE:	//The target changes to "all" if used in a gvg map. [Skotlex]
	case AM_DEMONSTRATION:
		if (map_flag_gvg(src->m) && battle_config.gvg_traps_bctall)
			target = BCT_ALL;
		break;
	case HT_SHOCKWAVE:			/* ショックウェ?ブトラップ */
		val1=skilllv*15+10;
	case HT_SANDMAN:			/* サンドマン */
	case HT_CLAYMORETRAP:		/* クレイモア?トラップ */
	case HT_SKIDTRAP:			/* スキッドトラップ */
	case HT_LANDMINE:			/* ランドマイン */
	case HT_ANKLESNARE:			/* アンクルスネア */
	case HT_FLASHER:			/* フラッシャ? */
	case HT_FREEZINGTRAP:		/* フリ?ジングトラップ */
	case HT_BLASTMINE:			/* ブラストマイン */
		if (sc_data && sc_data[SC_INTOABYSS].timer != -1)
			val3 = BD_INTOABYSS;	//Store into abyss state, to know it shouldn't give traps back. [Skotlex]
		if (map_flag_gvg(src->m))
		{
			limit *= 4; // longer trap times in WOE [celest]
			if (battle_config.gvg_traps_bctall)
				target = BCT_ALL; //Change target to all [Skotlex]
		}
		break;

	case SA_LANDPROTECTOR:	/* グランドク�?ス */
		{
			int aoe_diameter;	// -- aoe_diameter (moonsoul) added for sage Area Of Effect skills
			val1=skilllv*15+10;
			aoe_diameter=skilllv+skilllv%2+5;
			count=aoe_diameter*aoe_diameter;	// -- this will not function if changed to ^2 (moonsoul)
		}
	//No break because we also have to check if we use gemstones. [Skotlex]
	case SA_VOLCANO:
	case SA_DELUGE:
	case SA_VIOLENTGALE:
	{
		struct skill_unit_group *old_sg;
		if ((old_sg = skill_locate_element_field(src)) != NULL)
		{
			if (old_sg->skill_id == skillid && old_sg->limit > 0)
			{	//Use the previous limit (minus the elapsed time) [Skotlex]
				limit = old_sg->limit - DIFF_TICK(gettick(), old_sg->tick);
				if (limit < 0)	//This can happen... 
					limit = skill_get_time(skillid,skilllv);
			}
			skill_clear_element_field(src);
		}
		break;
	}

	case BA_DISSONANCE:
	case DC_UGLYDANCE:
		val1 = 10;	//FIXME: This value is not used anywhere, what is it for? [Skotlex]
		break;
	case BA_WHISTLE:
		val1 = skilllv+(status_get_agi(src)/10); // Flee increase
		val2 = ((skilllv+1)/2)+(status_get_luk(src)/10); // Perfect dodge increase
		if(src->type == BL_PC){
			val1 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
			val2 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
		}
		break;
	case DC_HUMMING:
        val1 = 2*skilllv+(status_get_dex(src)/10); // Hit increase
		if(src->type == BL_PC)
			val1 += 2*pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
		break;
	case BA_POEMBRAGI:
		val1 = 3*skilllv+(status_get_dex(src)/10); // Casting time reduction
		val2 = 3*skilllv+(status_get_int(src)/10); // After-cast delay reduction
		if(src->type == BL_PC){
			val1 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
			val2 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
		}
		break;
	case DC_DONTFORGETME:
		val1 = 3*skilllv+(status_get_dex(src)/10); // ASPD decrease
		val2 = 2*skilllv+(status_get_agi(src)/10); // Movement speed decrease
		if(src->type == BL_PC){
			val1 += pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
			val2 += pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
		}
		break;
	case BA_APPLEIDUN:
		val1 = 5+2*skilllv+(status_get_vit(src)/10); // MaxHP percent increase
		val2 = 30+5*skilllv+5*(status_get_vit(src)/10); // HP recovery
		if(src->type == BL_PC){
			val1 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
			val2 += 5*pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
		}
		break;
	case DC_SERVICEFORYOU:
		val1 = 10+skilllv+(status_get_int(src)/10); // MaxSP percent increase
		val2 = 10+3*skilllv+(status_get_int(src)/10); // SP cost reduction
		if(src->type == BL_PC){
			val1 += pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
			val2 += pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
		}
		break;
	case BA_ASSASSINCROSS:
		val1 = 10+skilllv+(status_get_agi(src)/10); // ASPD increase
		if(src->type == BL_PC)
			val1 += pc_checkskill((struct map_session_data *)src,BA_MUSICALLESSON);
		break;
	case DC_FORTUNEKISS:
		val1 = 10+skilllv+(status_get_luk(src)/10); // Critical increase
		if(src->type == BL_PC)
			val1 += pc_checkskill((struct map_session_data *)src,DC_DANCINGLESSON);
		break;
	case BD_LULLABY:
		val1 = 11;	//FIXME: This value is not used anywhere, what is it for? [Skotlex]
		break;
	case BD_DRUMBATTLEFIELD:
		val1 = (skilllv+1)*25;	//Watk increase
		val2 = (skilllv+1)*2;	//Def increase
		break;
	case BD_RINGNIBELUNGEN:
		val1 = (skilllv+2)*25;	//Watk increase
		break;
	case BD_SIEGFRIED:
		val1 = 55 + skilllv*5;	//Elemental Resistance
		val2 = skilllv*10;	//Status ailment resistance
		break;
	case BD_ETERNALCHAOS:
		break;
	case PF_FOGWALL:	/* フォグウォ?ル */
		if(sc_data && sc_data[SC_DELUGE].timer!=-1) limit *= 2;
		break;

	case RG_GRAFFITI:			/* Graffiti */
		count=1;	// Leave this at 1 [Valaris]
		break;
	}

	if (val3==0 && (flag&2 || (sc_data && sc_data[SC_MAGICPOWER].timer != -1)))
		val3 = HW_MAGICPOWER; //Store the magic power flag. [Skotlex]
		
	nullpo_retr(NULL, group=skill_initunitgroup(src,(count > 0 ? count : layout->count),
		skillid,skilllv,skill_get_unit_id(skillid,flag&1)));
	group->limit=limit;
	group->val1=val1;
	group->val2=val2;
	group->val3=val3;
	group->target_flag=target;
	group->interval=interval;
	if(skillid==HT_TALKIEBOX ||
	   skillid==RG_GRAFFITI){
		group->valstr=(char *) aCallocA(MESSAGE_SIZE, sizeof(char));
		if(group->valstr==NULL){
			ShowFatalError("skill_castend_map: out of memory !\n");
			exit(1);
		}
		memcpy(group->valstr,talkie_mes,MESSAGE_SIZE-1);
	}

	//Why redefine local variables when the ones of the function can be reused? [Skotlex]
	val1=skilllv;
	val2=0;
	limit=group->limit;
	for(i=0;i<layout->count;i++){
		struct skill_unit *unit;
		int ux,uy,alive=1;
		ux = x + layout->dx[i];
		uy = y + layout->dy[i];
		switch (skillid) {
		case MG_FIREWALL:		/* ファイヤ?ウォ?ル */
			val2=group->val2;
			break;
		case WZ_ICEWALL:		/* アイスウォ?ル */
			if(skilllv <= 1)
				val1 = 500;
			else
				val1 = 200 + 200*skilllv;
			break;
		case RG_GRAFFITI:	/* Graffiti [Valaris] */
			ux+=(i%5-2);
			uy+=(i/5-2);
			break;
		}
		//直?繝Xキルの?�?�?ﾝ置?ﾀ標?繧ﾉランドプ�?テクタ?がないかチェック
		if(range<=0)
			map_foreachincell(skill_landprotector,src->m,ux,uy,BL_SKILL,skillid,&alive, src);
		
		if(alive && map_getcell(src->m,ux,uy,CELL_CHKWALL))
			alive = 0;
		
		if(alive && skillid == WZ_ICEWALL) {
			if(src->x == x && src->y==y) // Ice Wall not allowed on self [DracoRPG]
				alive=0;
			else {
				val2=map_getcell(src->m,ux,uy,CELL_GETTYPE);
				if(val2==5 || val2==1)
					alive=0;
				else {
					map_setcell(src->m,ux,uy,5);
					clif_changemapcell(src->m,ux,uy,5,0);
				}
			}
		}

		if(alive){
			nullpo_retr(NULL, unit=skill_initunit(group,i,ux,uy));
			unit->val1=val1;
			unit->val2=val2;
			unit->limit=limit;
			unit->range=range;
				
			if (range==0 && active_flag)
				map_foreachincell(skill_unit_effect,unit->bl.m,
					unit->bl.x,unit->bl.y,0,&unit->bl,gettick(),1);
		}
	}
	
	return group;
}

/*==========================================
 * スキルユニットの?動イベント
 *------------------------------------------
 */
int skill_unit_onplace(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg;
	struct block_list *ss;
	struct status_change *sc_data;
	int type;
	short *opt;

	nullpo_retr(0, src);
	nullpo_retr(0, bl);
	
	if(bl->prev==NULL || !src->alive || status_isdead(bl))
		return 0;

	nullpo_retr(0, sg=src->group);
	nullpo_retr(0, ss=map_id2bl(sg->src_id));

	if (map_getcell(bl->m, bl->x, bl->y, CELL_CHKLANDPROTECTOR))
		return 0; //AoE skills are ineffective. [Skotlex]
	
	if (battle_check_target(&src->bl,bl,sg->target_flag)<=0)
		return 0;
	
	if ((opt = status_get_option(bl)) && ((*opt)&OPTION_HIDE) && sg->skill_id != WZ_HEAVENDRIVE)
		return 0; //Hidden characters are inmune to AoE skills except Heaven's Drive. [Skotlex]
	
	sc_data = status_get_sc_data(bl);
	type = SkillStatusChangeTable[sg->skill_id];

	switch (sg->unit_id) {
	case UNT_SAFETYWALL:
		//TODO: Find a more reliable way to handle the link to sg, this could cause dangling pointers. [Skotlex]
		if (sc_data && sc_data[type].timer == -1)
			status_change_start(bl,type,sg->skill_lv,sg->group_id,(int)sg,0,sg->limit,0);
		break;

	case UNT_PNEUMA:
		if (sc_data && sc_data[type].timer == -1)
			status_change_start(bl,type,sg->skill_lv,sg->group_id,0,0,sg->limit,0);
		break;

	case UNT_WARP_WAITING:
		if(bl->type==BL_PC){
			struct map_session_data *sd = (struct map_session_data *)bl;
			if((!sd->chatID || battle_config.chat_warpportal)
				&& sd->to_x == src->bl.x && sd->to_y == src->bl.y) {
				if (pc_setpos(sd,sg->valstr,sg->val2>>16,sg->val2&0xffff,3) == 0) {
					if (--sg->val1<=0 || sg->src_id == bl->id)
						skill_delunitgroup(sg);
				}
			}
		} else if(bl->type==BL_MOB && battle_config.mob_warpportal){
			int m = map_mapname2mapid(sg->valstr);
			mob_warp((struct mob_data *)bl,m,sg->val2>>16,sg->val2&0xffff,3);
		}
		break;

	case UNT_QUAGMIRE:
		if(status_isimmune(bl))
			return 0;
		if(sc_data && sc_data[type].timer==-1)
			status_change_start(bl,type,sg->skill_lv,sg->group_id,0,0,sg->limit,0);
		break;

	case UNT_VOLCANO:
	case UNT_DELUGE:
	case UNT_VIOLENTGALE:
		if(sc_data && sc_data[type].timer==-1)
			status_change_start(bl,type,sg->skill_lv,sg->group_id,0,0,
				skill_get_time2(sg->skill_id,sg->skill_lv),0);
		break;

	case UNT_RICHMANKIM:
	case UNT_ETERNALCHAOS:
	case UNT_DRUMBATTLEFIELD:
	case UNT_RINGNIBELUNGEN:
	case UNT_ROKISWEIL:
	case UNT_INTOABYSS:
	case UNT_SIEGFRIED:
		if (sg->src_id==bl->id) //Needed to check when a dancer/bard leaves their ensemble area.
			return sg->skill_id;
		if (sc_data && sc_data[type].timer==-1)
			status_change_start(bl,type,sg->skill_lv,sg->val1,sg->val2,0,sg->limit,0);
		break;
	
	case UNT_WHISTLE:
	case UNT_ASSASSINCROSS:
	case UNT_POEMBRAGI:
	case UNT_APPLEIDUN:
	case UNT_HUMMING:
	case UNT_DONTFORGETME:
	case UNT_FORTUNEKISS:
	case UNT_SERVICEFORYOU:
	case UNT_HERMODE:
		if (sg->src_id==bl->id)
			return 0;
		if (sc_data && sc_data[type].timer==-1)
			status_change_start(bl,type,sg->skill_lv,sg->val1,sg->val2,0,sg->limit,0);
		break;

	case UNT_BASILICA:
		if (!(status_get_mode(bl)&MD_BOSS) && battle_check_target(&src->bl,bl,BCT_ENEMY)>0)
			skill_blown(&src->bl,bl,1);
		break;

	case UNT_FOGWALL:
		if (sc_data && sc_data[type].timer==-1)
		{
			status_change_start (bl, type, sg->skill_lv, sg->val1, sg->val2, sg->group_id, sg->limit, 0);
			if (battle_check_target(&src->bl,bl,BCT_ENEMY)>0)
				skill_additional_effect (ss, bl, sg->skill_id, sg->skill_lv, BF_MISC, tick);
		}
		break;

	case UNT_GRAVITATION:
		if (sc_data && sc_data[type].timer==-1 && !status_get_mode(bl)&MD_BOSS)
			status_change_start(bl,type,sg->skill_lv,5*sg->skill_lv,BCT_ENEMY,sg->group_id,sg->limit,0);
		break;
	
	case UNT_ICEWALL: //Destroy the cell. [Skotlex]
		src->val1 = 0;
		if(src->limit + sg->tick > tick + 700)
			src->limit = DIFF_TICK(tick+700,sg->tick);
		break;
	}

	return sg->skill_id;
}

/*==========================================
 * スキルユニットの発動イベント(タイマ?[発動)
 *------------------------------------------
 */
int skill_unit_onplace_timer(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg;
	struct block_list *ss;
	struct map_session_data *sd = NULL;
	int splash_count=0;
	struct status_change *sc_data, *ssc_data;
	struct skill_unit_group_tickset *ts;
	int type;
	int diff=0;

	nullpo_retr(0, src);
	nullpo_retr(0, bl);

	if (bl->prev==NULL || !src->alive || status_isdead(bl))
		return 0;

	nullpo_retr(0, sg=src->group);
	nullpo_retr(0, ss=map_id2bl(sg->src_id));
	if (ss->type == BL_PC) sd = (struct map_session_data*)ss;
	ssc_data = status_get_sc_data(ss); //For magic power. 
	sc_data = status_get_sc_data(bl);
	type = SkillStatusChangeTable[sg->skill_id];

	if (sg->interval == -1 && (sg->unit_id == UNT_ANKLESNARE || sg->unit_id == UNT_SPIDERWEB || sg->unit_id == UNT_FIREPILLAR_ACTIVE))
		//Ok, this case only happens with Ankle Snare/Spider Web (only skills that sets its interval to -1), 
		//and only happens when more than one target is stepping on the trap at the moment it was triggered
		//(yet only the first mob standing on the trap will be captured) [Skotlex]
		return 0;

	if ((ts = skill_unitgrouptickset_search(bl,sg,tick)))
	{	//Not all have it, eg: Traps don't have it even though they can be hit by Heaven's Drive [Skotlex]
		diff = DIFF_TICK(tick,ts->tick);
		if (diff < 0)
			return 0;
		ts->tick = tick+sg->interval;
		
		// GXは?dなっていたら3HITしない
		if ((sg->skill_id==CR_GRANDCROSS || sg->skill_id==NPC_GRANDDARKNESS) && !battle_config.gx_allhit)
			ts->tick += sg->interval*(map_count_oncell(bl->m,bl->x,bl->y,0)-1);
	}
	//Temporarily set magic power to have it take effect. [Skotlex]
	if (sg->val3 == HW_MAGICPOWER && ssc_data && ssc_data[SC_MAGICPOWER].timer == -1 && ssc_data[SC_MAGICPOWER].val1 > 0)
	{
		if (sd)
		{	//This is needed since we are not going to recall status_calc_pc...
			sd->matk1 += sd->matk1 * 5*ssc_data[SC_MAGICPOWER].val1/100;
			sd->matk2 += sd->matk2 * 5*ssc_data[SC_MAGICPOWER].val1/100;
		} else
			ssc_data[SC_MAGICPOWER].timer = -2; //Note to NOT return from the function until this is unset!
	}
	
	switch (sg->unit_id) {
	case UNT_FIREWALL:
		{
			int flag=1, t_ele = status_get_elem_type(bl);
			if (t_ele == 3 || battle_check_undead(status_get_race(bl), t_ele))
				flag = src->val2>battle_config.firewall_hits_on_undead?battle_config.firewall_hits_on_undead:src->val2; 
			
			skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,flag);
			src->val2-=flag;
			if (src->val2<=0)
				skill_delunit(src);
		break;
		}
	case UNT_SANCTUARY:
		{
			int race = status_get_race(bl);

			if (battle_check_undead(race, status_get_elem_type(bl)) || race==6) {
				if (skill_attack(BF_MAGIC, ss, &src->bl, bl, sg->skill_id, sg->skill_lv, tick, 0)) {
					// reduce healing count if this was meant for damaging [hekate]
					sg->val1 -= 2;
				}
			} else {
				int heal = sg->val2;
				if (status_get_hp(bl) >= status_get_max_hp(bl))
					break;
				if (status_isimmune(bl))
					heal = 0;	/* 黄金蟲カ?[ド?iヒ?[ル量０?j */
				clif_skill_nodamage(&src->bl, bl, AL_HEAL, heal, 1);
				battle_heal(NULL, bl, heal, 0, 0);
				if (diff >= 500)
					sg->val1--;	// ?V規に入ったユニットだけカウント
			}
			if (sg->val1 <= 0)
				skill_delunitgroup(sg);
			break;
		}

	case UNT_MAGNUS:
		{
			int race = status_get_race(bl);
			if (!battle_check_undead(race,status_get_elem_type(bl)) && race!=6)
				break;
			skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
			src->val2++;
			break;
		}

	case UNT_MAGIC_SKILLS:
		skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
		break;

	case UNT_FIREPILLAR_WAITING:
		skill_delunit(src);
		skill_unitsetting(ss,sg->skill_id,sg->skill_lv,src->bl.x,src->bl.y,1);
		break;

	case UNT_FIREPILLAR_ACTIVE:
		map_foreachinarea(skill_attack_area,bl->m,bl->x-1,bl->y-1,bl->x+1,bl->y+1,0,
			BF_MAGIC,ss,&src->bl,sg->skill_id,sg->skill_lv,tick,0,BCT_ENEMY);  // area damage [Celest]
		sg->interval = -1; //Mark it used up so others can't trigger it for massive splash damage. [Skotlex]
		sg->limit=DIFF_TICK(tick,sg->tick) + 1500;
		break;

	case UNT_SKIDTRAP:
		{
			int c = skill_get_blewcount(sg->skill_id,sg->skill_lv);
			if(map_flag_gvg(bl->m)) c = 0;
			skill_blown(&src->bl,bl,c|0x10000);
			sg->unit_id = UNT_USED_TRAPS;
			clif_changelook(&src->bl,LOOK_BASE,sg->unit_id);
			sg->limit=DIFF_TICK(tick,sg->tick)+1500;
		}
		break;

	case UNT_ANKLESNARE:
		if(sg->val2==0 && sc_data && sc_data[SC_ANKLE].timer==-1){
			int moveblock = ( bl->x/BLOCK_SIZE != src->bl.x/BLOCK_SIZE || bl->y/BLOCK_SIZE != src->bl.y/BLOCK_SIZE);
			int sec = skill_get_time2(sg->skill_id,sg->skill_lv) - status_get_agi(bl)*100;
			if(status_get_mode(bl)&MD_BOSS) // Lasts 5 times less on bosses
				sec = sec/5;
			if (sec < 3000+30*sg->skill_lv)	// Minimum trap time of 3+0.03*skilllv seconds [celest]
				sec = 3000+30*sg->skill_lv;
			battle_stopwalking(bl,1);
			status_change_start(bl,SC_ANKLE,sg->skill_lv,0,0,0,sec,0);

			skill_unit_move(bl,tick,2);
			if(moveblock) map_delblock(bl);
				bl->x = src->bl.x;
				bl->y = src->bl.y;
			if(moveblock) map_addblock(bl);
			skill_unit_move(bl,tick,3);
			clif_fixpos(bl);
			//clif_01ac(&src->bl); //Removed? Check the openkore description of this packet: [Skotlex]
			// 01AC: long ID
			// Indicates that an object is trapped, but ID is not a
			// valid monster or player ID.
			sg->limit=DIFF_TICK(tick,sg->tick) + sec;
			sg->val2=bl->id;
			sg->interval = -1;
			src->range = 0;
		}
		break;

	case UNT_VENOMDUST:
		if(sc_data && sc_data[type].timer==-1 )
			status_change_start(bl,type,sg->skill_lv,sg->group_id,0,0,skill_get_time2(sg->skill_id,sg->skill_lv),0);
		break;

	case UNT_LANDMINE:
		skill_attack(BF_MISC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);
		sg->unit_id = UNT_USED_TRAPS;
		clif_changelook(&src->bl,LOOK_BASE,UNT_FIREPILLAR_ACTIVE);
		sg->limit=DIFF_TICK(tick,sg->tick)+1500;
		break;

	case UNT_BLASTMINE:
	case UNT_SHOCKWAVE:
	case UNT_SANDMAN:
	case UNT_FLASHER:
	case UNT_FREEZINGTRAP:
	case UNT_CLAYMORETRAP:
		map_foreachinarea(skill_count_target,src->bl.m
					,src->bl.x-src->range,src->bl.y-src->range
					,src->bl.x+src->range,src->bl.y+src->range
					,0,&src->bl,&splash_count);
		map_foreachinarea(skill_trap_splash,src->bl.m
					,src->bl.x-src->range,src->bl.y-src->range
					,src->bl.x+src->range,src->bl.y+src->range
					,0,&src->bl,tick,splash_count);
		sg->unit_id = UNT_USED_TRAPS;
		clif_changelook(&src->bl,LOOK_BASE,sg->unit_id);
		sg->limit=DIFF_TICK(tick,sg->tick)+1500;
		break;
		
	case UNT_TALKIEBOX:
		if (sg->src_id == bl->id) //自分が踏んでも発動しない
			break;
		if (sg->val2 == 0){
			clif_talkiebox(&src->bl, sg->valstr);
			sg->unit_id = UNT_USED_TRAPS;
			clif_changelook(&src->bl, LOOK_BASE, sg->unit_id);
			sg->limit = DIFF_TICK(tick, sg->tick) + 5000;
			sg->val2 = -1; //踏んだ
		}
		break;

	case UNT_LULLABY:
		if (ss->id == bl->id)
			break;
		skill_additional_effect(ss, bl, sg->skill_id, sg->skill_lv, BF_LONG|BF_SKILL|BF_MISC, tick);
		break;

	case UNT_UGLYDANCE:	//Ugly Dance [Skotlex]
		if (ss->id == bl->id)
			break;
		if (bl->type == BL_PC)
			skill_additional_effect(ss, bl, sg->skill_id, sg->skill_lv, BF_LONG|BF_SKILL|BF_MISC, tick);
		break;

	case UNT_DISSONANCE:
		skill_attack(BF_MISC, ss, &src->bl, bl, sg->skill_id, sg->skill_lv, tick, 0);
		break;

	case UNT_APPLEIDUN: //Apple of Idun [Skotlex]
	{
		int heal;
		if (sg->src_id == bl->id)
			break;
		heal = sg->val2;
		clif_skill_nodamage(&src->bl, bl, AL_HEAL, heal, 1);
		battle_heal(NULL, bl, heal, 0, 0);
		break;	
	}

	case UNT_DEMONSTRATION:
		skill_attack(BF_WEAPON, ss, &src->bl, bl, sg->skill_id, sg->skill_lv, tick, 0);
		break;

	case UNT_GOSPEL:
		if (rand()%100 > sg->skill_lv*10)
			break;
		if (ss != bl && battle_check_target(ss,bl,BCT_PARTY)>0) { // Support Effect only on party, not guild
			int i = rand()%13; // Positive buff count
			switch (i)
			{
				case 0: // Heal 1~9999 HP
					{
						int heal = rand() %9999+1;
						clif_skill_nodamage(ss,bl,AL_HEAL,heal,1);
						battle_heal(NULL,bl,heal,0,0);
					}
					break;
				case 1: // End all negative status
					status_change_clear_debuffs (bl);
					break;
				case 2: // Level 10 Blessing
					status_change_start(bl,SC_BLESSING,10,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 3: // Level 10 Increase AGI
					status_change_start(bl,SC_INCREASEAGI,10,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 4: // Enchant weapon with Holy element
					status_change_start(bl,SC_ASPERSIO,1,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 5: // Enchant armor with Holy element
					status_change_start(bl,SC_BENEDICTIO,1,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 6: // MaxHP +100%
					status_change_start(bl,SC_INCMHPRATE,100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
				case 7: // MaxSP +100%
					status_change_start(bl,SC_INCMSPRATE,100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
				case 8: // All stats +20
					status_change_start(bl,SC_INCALLSTATUS,20,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
   				case 9: // DEF +25%
					status_change_start(bl,SC_INCDEFRATE,25,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
				case 10: // ATK +100%
					status_change_start(bl,SC_INCATKRATE,100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
				case 11: // HIT/Flee +50
					status_change_start(bl,SC_INCHIT,50,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					status_change_start(bl,SC_INCFLEE,50,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
				case 12: // Immunity to all status
				    status_change_start(bl,SC_GOSPEL,1,0,0,BCT_PARTY,skill_get_time2(sg->skill_id, sg->skill_lv),0);
				    break;
			}
		}			
		else if (battle_check_target(&src->bl,bl,BCT_ENEMY)>0) { // Offensive Effect
			int i = rand()%9; // Negative buff count
			switch (i)
			{
				case 0: // Deal 1~9999 damage
				{
					int dmg = rand() % 9999 +1;
					clif_skill_damage(bl, bl, sg->tick,0,0,dmg,0,CR_HOLYCROSS,1,-1);
					battle_damage(ss, bl, dmg,0);
					break;
				}
				case 1: // Curse
					status_change_start(bl,SC_CURSE,1,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 2: // Blind
					status_change_start(bl,SC_BLIND,1,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 3: // Poison
					status_change_start(bl,SC_POISON,1,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 4: // Level 10 Provoke
					status_change_start(bl,SC_PROVOKE,10,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 5: // DEF -100%
					status_change_start(bl,SC_INCDEFRATE,-100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
   				case 6: // ATK -100%
					status_change_start(bl,SC_INCATKRATE,-100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
   				case 7: // Flee -100%
					status_change_start(bl,SC_INCFLEERATE,-100,0,0,0,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
				case 8: // Speed/ASPD -25%
				    status_change_start(bl,SC_GOSPEL,1,0,0,BCT_ENEMY,skill_get_time2(sg->skill_id, sg->skill_lv),0);
					break;
			}
		}
		break;

	case UNT_SPIDERWEB:
		if(sg->val2==0 && (!sc_data || sc_data[type].timer==-1 )){
			int moveblock = ( bl->x/BLOCK_SIZE != src->bl.x/BLOCK_SIZE || bl->y/BLOCK_SIZE != src->bl.y/BLOCK_SIZE);
			skill_additional_effect(ss,bl,sg->skill_id,sg->skill_lv,BF_MISC,tick);
			skill_unit_move(bl,tick,2);
			if(moveblock) map_delblock(bl);
				bl->x = src->bl.x;
				bl->y = src->bl.y;
			if(moveblock) map_addblock(bl);
			skill_unit_move(bl,tick,3);
 			clif_fixpos(bl);
			sg->limit = DIFF_TICK(tick,sg->tick)+skill_get_time2(sg->skill_id,sg->skill_lv);
			sg->val2=bl->id;
			sg->interval = -1;
			src->range = 0;
		}
		break;

	case UNT_GRAVITATION:
		skill_attack(BF_MAGIC,ss,&src->bl,bl,sg->skill_id,sg->skill_lv,tick,0);		
		break;
	}
	if (sg->val3 == HW_MAGICPOWER && ssc_data && ssc_data[SC_MAGICPOWER].timer < 0 && ssc_data[SC_MAGICPOWER].val1 > 0)
	{	//Unset Magic Power.
		if (sd)
		{
			sd->matk1 = 100*sd->matk1/(100 + 5*ssc_data[SC_MAGICPOWER].val1);
			sd->matk2 = 100*sd->matk2/(100 + 5*ssc_data[SC_MAGICPOWER].val1);
		} else
			ssc_data[SC_MAGICPOWER].timer = -1;
	}
	
	if (bl->type == BL_MOB && ss != bl) {	/* スキル使用?�?のMOBスキル */
		struct mob_data *md = (struct mob_data *)bl;
		if (!md) return 0;
		if (battle_config.mob_changetarget_byskill == 1) {
			int target = md->target_id;
			if (ss->type == BL_PC)
				md->target_id = ss->id;
			mobskill_use(md, tick, MSC_SKILLUSED|(sg->skill_id << 16));
			md->target_id = target;
		} else
			mobskill_use(md, tick, MSC_SKILLUSED|(sg->skill_id << 16));
	}

	return sg->skill_id;
}
/*==========================================
 * スキルユニットから離?する(もしくはしている)?�?�
 *------------------------------------------
 */
int skill_unit_onout(struct skill_unit *src,struct block_list *bl,unsigned int tick)
{
	struct skill_unit_group *sg;
	struct status_change *sc_data;
	int type;

	nullpo_retr(0, src);
	nullpo_retr(0, bl);
	nullpo_retr(0, sg=src->group);
	sc_data = status_get_sc_data(bl);
	type = SkillStatusChangeTable[sg->skill_id];

	if (bl->prev==NULL || !src->alive || //Need to delete the trap if the source died.
		(status_isdead(bl) && sg->unit_id != UNT_ANKLESNARE && sg->unit_id != UNT_SPIDERWEB))
		return 0;

	switch(sg->unit_id){
	case UNT_SAFETYWALL:
		if (sc_data && sc_data[type].timer!=-1)
			status_change_end(bl,type,-1);
		break;
	case UNT_ANKLESNARE:
	{
		struct block_list *target = map_id2bl(sg->val2);
		if(target && target == bl){
			status_change_end(bl,SC_ANKLE,-1);
			sg->limit=DIFF_TICK(tick,sg->tick)+1000;
		}
		else
			return 0;
		break;
	}
	case UNT_BASILICA: //Clear basilica if the owner moved [Skotlex]
	case UNT_HERMODE:	//Clear Hermode if the owner moved.
		if (sc_data && sc_data[type].timer!=-1 && sc_data[type].val3 == BCT_SELF && sc_data[type].val4 == sg->group_id)
			status_change_end(bl,type,-1);
		break;
		
	case UNT_SPIDERWEB:	/* スパイダ?ウェッブ */
		{
			struct block_list *target = map_id2bl(sg->val2);
			if (target && target==bl)
			{
				status_change_end(bl,SC_SPIDERWEB,-1);
				sg->limit = DIFF_TICK(tick,sg->tick)+1000;
			}
			break;
		}
	}
	return sg->skill_id;
}

/*==========================================
 * Triggered when a char steps out of a skill group [Skotlex]
 *------------------------------------------
 */
static int skill_unit_onleft(int skill_id, struct block_list *bl,unsigned int tick)
{
	struct status_change *sc_data;
	int type;

	sc_data = status_get_sc_data(bl);
	type = SkillStatusChangeTable[skill_id];

	switch (skill_id)
	{
		case WZ_QUAGMIRE:
			if (bl->type==BL_MOB)
				return 0;
			if (sc_data && sc_data[type].timer != -1)
				status_change_end(bl, type, -1);
			break;

		case BD_RICHMANKIM:
		case BD_ETERNALCHAOS:
		case BD_DRUMBATTLEFIELD:
		case BD_RINGNIBELUNGEN:	
		case BD_ROKISWEIL:
		case BD_INTOABYSS:
		case BD_SIEGFRIED:
			if(sc_data && sc_data[SC_DANCING].timer != -1 && sc_data[SC_DANCING].val1 == skill_id)
			{	//Check if you just stepped out of your ensemble skill to cancel dancing. [Skotlex]
				//We don't check for SC_LONGING because someone could always have knocked you back and out of the song/dance.
				//FIXME: This code is not perfect, it doesn't checks for the real ensemble's owner,
				//it only checks if you are doing the same ensemble. So if there's two chars doing an ensemble
				//which overlaps, by stepping outside of the other parther's ensemble will cause you to cancel 
				//your own. Let's pray that scenario is pretty unlikely and noone will complain too much about it.
				skill_stop_dancing(bl);
			}
		case MG_SAFETYWALL:
		case AL_PNEUMA:
		case SA_VOLCANO:
		case SA_DELUGE:
		case SA_VIOLENTGALE:
		case CG_HERMODE:
		case HW_GRAVITATION:
			if (sc_data && sc_data[type].timer != -1)
				status_change_end(bl, type, -1);
			break;
			
		case BA_POEMBRAGI:
		case BA_WHISTLE:
		case BA_ASSASSINCROSS:
		case BA_APPLEIDUN:
		case DC_HUMMING:
		case DC_DONTFORGETME:
		case DC_FORTUNEKISS:	
		case DC_SERVICEFORYOU:
			if (sc_data && sc_data[type].timer != -1)
			{
				delete_timer(sc_data[type].timer, status_change_timer);
				//NOTE: It'd be nice if we could get the skill_lv for a more accurate extra time, but alas...
				//not possible on our current implementation.
				sc_data[type].timer = add_timer(tick+skill_get_time2(skill_id,1), status_change_timer, bl->id, type);
			}
			break;
		case PF_FOGWALL:
			if (sc_data && sc_data[type].timer != -1)
			{
				status_change_end(bl,type,-1);
				if (sc_data[SC_BLIND].timer!=-1)
				{
					delete_timer(sc_data[SC_BLIND].timer, status_change_timer);
					sc_data[SC_BLIND].timer = add_timer(30000+tick, status_change_timer, bl->id, SC_BLIND);
				}
			}
			break;
	}
	return skill_id;
}

/*==========================================
 * Checks ot see if this skill can target the bl given considering 
 * the UF_NOPC, UF_NOMOB and UF_SKILL flags [Skotlex]
 */
static int skill_unit_checktarget(int skill_id, struct block_list *bl) {
	int type = skill_get_unit_flag(skill_id);
	
	switch (bl->type) {
		case BL_PC:
			if (type&UF_NOPC)
				return 0;
			break;
		case BL_MOB:
			if (type&UF_NOMOB)
				return 0;
			break;
		case BL_SKILL:
			if (type&UF_SKILL)
				break;
		default:
			return 0;
	}
	return 1;
}
/*==========================================
 * Invoked when a unit cell has been placed/removed/deleted.
 * flag values:
 * flag&1: Invoke onplace function (otherwise invoke onout)
 * flag&4: Invoke a onleft call (the unit might be scheduled for deletion)
 *------------------------------------------
 */
int skill_unit_effect(struct block_list *bl,va_list ap)
{
	struct skill_unit *unit;
	struct skill_unit_group *group;
	int flag;
	unsigned int tick;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, unit=va_arg(ap,struct skill_unit*));
	tick = va_arg(ap,unsigned int);
	flag = va_arg(ap,unsigned int);

	if (!unit->alive || bl->prev==NULL)
		return 0;

	nullpo_retr(0, group=unit->group);

	if (!skill_unit_checktarget(group->skill_id, bl))
		return 0;

	if (flag&1)
		skill_unit_onplace(unit,bl,tick);
	else
		skill_unit_onout(unit,bl,tick);

	if (flag&4) skill_unit_onleft(group->skill_id, bl, tick);
	return 0;
}

/*==========================================
 * スキルユニットの限界イベント
 *------------------------------------------
 */
int skill_unit_onlimit(struct skill_unit *src,unsigned int tick)
{
	struct skill_unit_group *sg;
	nullpo_retr(0, src);
	nullpo_retr(0, sg=src->group);

	switch(sg->unit_id){
	case UNT_WARP_ACTIVE:	/* �??プポ?タル(?動前) */
		{
			struct skill_unit_group *group=
				skill_unitsetting(map_id2bl(sg->src_id),sg->skill_id,sg->skill_lv,
					src->bl.x,src->bl.y,1);
			if(group == NULL)
				return 0;
			group->valstr=(char *) aCallocA(MESSAGE_SIZE, sizeof(char));
			if(group->valstr==NULL){
				ShowFatalError("skill_unit_onlimit: out of memory !\n");
				exit(1);
			}
			strncpy(group->valstr,sg->valstr,MESSAGE_SIZE-1);
			group->val2=sg->val2;
		}
		break;

	case UNT_ICEWALL:	/* アイスウォ?ル */
		map_setcell(src->bl.m,src->bl.x,src->bl.y,src->val2);
		clif_changemapcell(src->bl.m,src->bl.x,src->bl.y,src->val2,1);
		break;
	case UNT_CALLPARTNER:	/* あなたに?いたい */
		{
			struct map_session_data *sd = NULL;
			if((sd = map_id2sd(sg->src_id)) == NULL)
				return 0;
			if((sd = pc_get_partner(sd)) == NULL)
				return 0;

			pc_setpos(sd,map[src->bl.m].name,src->bl.x,src->bl.y,3);
		}
		break;
	}

	return 0;
}
/*==========================================
 * スキルユニットのダ�??ジイベント
 *------------------------------------------
 */
int skill_unit_ondamaged(struct skill_unit *src,struct block_list *bl,
	int damage,unsigned int tick)
{
	struct skill_unit_group *sg;

	nullpo_retr(0, src);
	nullpo_retr(0, sg=src->group);

	if (skill_get_inf2(sg->skill_id)&INF2_TRAP && damage > 0)
		skill_delunitgroup(sg);
	else 
	switch(sg->unit_id){
	case UNT_ICEWALL:
		src->val1-=damage;
		break;
	default:
		damage = 0;
		break;
	}
	return damage;
}

static int skill_moonlit_sub(struct block_list *bl, va_list ap) {
	struct block_list *src = va_arg(ap, struct block_list*);
	struct block_list *partner = va_arg(ap, struct block_list*);
	int blowcount = va_arg(ap, int);
	if (bl == src || bl == partner)
		return 0;
	if (bl->type == BL_MOB || bl->type == BL_PC || bl->type == BL_PET)
		skill_blown(src, bl, blowcount);
	return 1;
}

/*==========================================
 * Starts the moonlit effect by first knocking back all other characters in the vecinity.
 * partner may be null, but src cannot be.
 *------------------------------------------
 */
static void skill_moonlit(struct block_list* src, struct block_list* partner, int skilllv)
{
	int range = skill_get_range2(src, CG_MOONLIT, skilllv);
	int blowcount = range+1, time = skill_get_time(CG_MOONLIT,skilllv);
	
	map_foreachinarea(skill_moonlit_sub,src->m
					,src->x-range,src->y-range
					,src->x+range,src->y+range
					,0,src,partner,blowcount);
	if(partner)
		map_foreachinarea(skill_moonlit_sub,partner->m
					,partner->x-range,partner->y-range
					,partner->x+range,partner->y+range
					,0,src,partner,blowcount);
		
	status_change_start(src,SC_DANCING,CG_MOONLIT,0,0,partner?partner->id:BCT_SELF,time+1000,0);
	status_change_start(src,SkillStatusChangeTable[CG_MOONLIT],skilllv,0,0,0,time,0);
	
	if (partner) {
		status_change_start(partner,SC_DANCING,CG_MOONLIT,0,0,src->id,time+1000,0);
		status_change_start(partner,SkillStatusChangeTable[CG_MOONLIT],skilllv,0,0,0,time,0);
	}
	
}
/*==========================================
 * 範??キャラ存?ﾝ確認判定?�?(foreachinarea)
 *------------------------------------------
 */

static int skill_check_condition_char_sub (struct block_list *bl, va_list ap)
{
	int *c, skillid;
	struct block_list *src;
	struct map_session_data *sd;
	struct map_session_data *tsd;
	int *p_sd;	//Contains the list of characters found.
	unsigned int tick = gettick();

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, tsd=(struct map_session_data*)bl);
	nullpo_retr(0, src=va_arg(ap,struct block_list *));
	nullpo_retr(0, sd=(struct map_session_data*)src);

	c=va_arg(ap,int *);
	p_sd = va_arg(ap, int *);
	skillid = va_arg(ap,int);

	if ((skillid != PR_BENEDICTIO && *c >=1) || *c >=2)
		return 0; //Partner found for ensembles, or the two companions for Benedictio. [Skotlex]
	
	if (bl == src)
		return 0;

	if(pc_issit(tsd) || pc_isdead(tsd) || tsd->skilltimer!=-1 || tsd->canmove_tick > tick)
		return 0;

	if (tsd->sc_count && (tsd->sc_data[SC_SILENCE].timer != -1 || tsd->sc_data[SC_STAN].timer != -1))
		return 0;
	
	switch(skillid)
	{
		case PR_BENEDICTIO:				/* ?ｹ??~福 */
			if ((tsd->class_&MAPID_BASEMASK) == MAPID_ACOLYTE &&
//				(sd->bl.x == tsd->bl.x - 1 || sd->bl.x == tsd->bl.x + 1) && <- the heck? This check seems unnecessary, and even if it is necessary, where's the parallel check on the y axis? [Skotlex]
				sd->status.sp >= 10)
				p_sd[(*c)++]=tsd->bl.id;
			return 1;
		default: //Warning: Assuming Ensemble Dance/Songs for code speed. [Skotlex]
			{
				int skilllv;
				if (sd->status.sex != tsd->status.sex &&
						(tsd->class_&MAPID_UPPERMASK) == MAPID_BARDDANCER &&
						(skilllv = pc_checkskill(tsd, skillid)) > 0 &&
						(tsd->weapontype1==13 || tsd->weapontype1==14) &&
						sd->status.party_id && tsd->status.party_id &&
						sd->status.party_id == tsd->status.party_id &&
						tsd->sc_data[SC_DANCING].timer == -1)
				{
					p_sd[(*c)++]=tsd->bl.id;
					return skilllv;
				} else {
					return 0;
				}
			}
			break;
	}
	return 0;
}

/*==========================================
 * Checks and stores partners for ensemble skills [Skotlex]
 *------------------------------------------
 */
static int skill_check_pc_partner(struct map_session_data *sd, int skill_id, int* skill_lv, int range, int cast_flag)
{
	static int c=0;
	static int p_sd[2] = { 0, 0 };
	int i;
	if (cast_flag)
	{	//Execute the skill on the partners.
		struct map_session_data* tsd;
		switch (skill_id)
		{
			case PR_BENEDICTIO:
				for (i = 0; i < c; i++)
				{
					if ((tsd = map_id2sd(p_sd[i])) != NULL)
					{
						tsd->status.sp -= 10;
						clif_updatestatus(tsd,SP_SP);
					}
				}
				return c;
			case CG_MOONLIT:
				if (c > 0 && (tsd = map_id2sd(p_sd[0])) != NULL)
				{
					clif_skill_nodamage(&tsd->bl, &sd->bl, skill_id, *skill_lv, 1);
					skill_moonlit(&sd->bl, &tsd->bl, *skill_lv);
					tsd->skillid_dance = tsd->skillid = skill_id;
					tsd->skilllv_dance = tsd->skilllv = *skill_lv;
				}
				return c;
			default: //Warning: Assuming Ensemble skills here (for speed)
				if (c > 0 && (tsd = map_id2sd(p_sd[0])) != NULL)
				{
					sd->sc_data[SC_DANCING].val4= tsd->bl.id;
					clif_skill_nodamage(&tsd->bl, &sd->bl, skill_id, *skill_lv, 1);
					status_change_start(&tsd->bl,SC_DANCING,skill_id,sd->sc_data[SC_DANCING].val2,0,sd->bl.id,skill_get_time(skill_id,*skill_lv)+1000,0);
					tsd->skillid_dance = tsd->skillid = skill_id;
					tsd->skilllv_dance = tsd->skilllv = *skill_lv;
				}
				return c;
		}
	}
	//Else: new search for partners.
	c = 0;
	memset (p_sd, 0, sizeof(p_sd));
	i = map_foreachinarea(skill_check_condition_char_sub, sd->bl.m,
		sd->bl.x-range, sd->bl.y-range, sd->bl.x+range,
		sd->bl.y+range, BL_PC, &sd->bl, &c, &p_sd, skill_id);

	if (skill_id != PR_BENEDICTIO) //Apply the average lv to encore skills.
		*skill_lv = (i+(*skill_lv))/(c+1); //I know c should be one, but this shows how it could be used for the average of n partners.
	return c;
}

/*==========================================
 * 範??バイオプラント?Aスフィアマイン用Mob存?ﾝ確認判定?�?(foreachinarea)
 *------------------------------------------
 */

static int skill_check_condition_mob_master_sub(struct block_list *bl,va_list ap)
{
	int *c,src_id=0,mob_class=0;
	struct mob_data *md;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, md=(struct mob_data*)bl);
	nullpo_retr(0, src_id=va_arg(ap,int));
	nullpo_retr(0, mob_class=va_arg(ap,int));
	nullpo_retr(0, c=va_arg(ap,int *));

	if(md->class_==mob_class && md->master_id==src_id)
		(*c)++;
	return 0;
}

static int skill_check_condition_hermod_sub(struct block_list *bl,va_list ap)
{
	int *c;
	struct npc_data *nd;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, nd=(struct npc_data*)bl);
	nullpo_retr(0, c=va_arg(ap,int *));

	if (nd->bl.subtype == WARP)
		(*c)++;
	return 0;
}

/*==========================================
 * スキル使用?�??i?で使用失敗?j
 *------------------------------------------
 */
int skill_check_condition(struct map_session_data *sd,int type)
{
	int i,hp,sp,hp_rate,sp_rate,zeny,weapon,state,spiritball,skill,lv,mhp;
	int index[10],itemid[10],amount[10];
	int arrow_flag = 0;
	int force_gem_flag = 0;
	int delitem_flag = 1, checkitem_flag = 1;

	nullpo_retr(0, sd);

	if( battle_config.gm_skilluncond>0 &&
		pc_isGM(sd)>= battle_config.gm_skilluncond &&
		sd->skillitem != sd->skillid)
	{	//GMs don't override the skillItem check, otherwise they can use items without them being consumed! [Skotlex]
		sd->skillitem = sd->skillitemlv = -1;
		return 1;
	}

	if( sd->opt1>0) {
		clif_skill_fail(sd,sd->skillid,0,0);
		sd->skillitem = sd->skillitemlv = -1;
		return 0;
	}
	if(pc_is90overweight(sd)) {
		clif_skill_fail(sd,sd->skillid,9,0);
		sd->skillitem = sd->skillitemlv = -1;
		return 0;
	}

	if (sd->state.abra_flag)
	{
		sd->skillitem = sd->skillitemlv = -1;
		if(type&1) sd->state.abra_flag = 0;
		return 1;
	}

	if (sd->state.produce_flag &&
		(sd->skillid == AM_PHARMACY || sd->skillid == AC_MAKINGARROW || sd->skillid == BS_REPAIRWEAPON))
	{
		sd->skillitem = sd->skillitemlv = -1;
		return 0;
	}

	if(sd->skillitem == sd->skillid) {	/* アイテムの?�?�無?�??ｬ功 */
		if(!type) //When a target was selected
		{	//Consume items that were skipped in pc_use_item [Skotlex]
			 if((i = sd->itemindex) == -1 ||
				sd->status.inventory[i].nameid != sd->itemid ||
				!sd->inventory_data[i]->flag.delay_consume ||
				sd->status.inventory[i].amount < 1
				)
			{	//Something went wrong, item exploit?
				sd->itemid = sd->itemindex = -1;
				return 0;
			}
			//Consume
			sd->itemid = sd->itemindex = -1;
			pc_delitem(sd,i,1,0);
		}
		if (type&1) //Casting finished
			sd->skillitem = sd->skillitemlv = -1;
		return 1;
	}
	if( sd->opt1>0 ){
		clif_skill_fail(sd,sd->skillid,0,0);
		return 0;
	}
	if(sd->sc_count){
		if( sd->sc_data[SC_SILENCE].timer!=-1 ||
			sd->sc_data[SC_ROKISWEIL].timer!=-1 ||
			(sd->sc_data[SC_AUTOCOUNTER].timer != -1 && sd->skillid != KN_AUTOCOUNTER) ||
			sd->sc_data[SC_STEELBODY].timer != -1 ||
			sd->sc_data[SC_BERSERK].timer != -1 ||
			(sd->sc_data[SC_MARIONETTE].timer != -1 && sd->skillid != CG_MARIONETTE)){
			clif_skill_fail(sd,sd->skillid,0,0);
			return 0;	/* ?態異?�や沈?など */
		}
	}

	skill = sd->skillid;
	lv = sd->skilllv;
	if (lv <= 0) return 0;
	// for the guild skills [celest]
	if (skill >= 10000 && skill < 10015) skill-= 9500;
	hp = skill_get_hp(skill, lv);	/* ?ﾁ費HP */
	sp = skill_get_sp(skill, lv);	/* ?ﾁ費SP */
	if((sd->skillid_old == BD_ENCORE) && skill == sd->skillid_dance)
		sp=sp/2;	//アンコ?ル時はSP?ﾁ費が半分
	hp_rate = (lv <= 0)? 0:skill_db[skill].hp_rate[lv-1];
	sp_rate = (lv <= 0)? 0:skill_db[skill].sp_rate[lv-1];
	zeny = skill_get_zeny(skill,lv);
	weapon = skill_db[skill].weapon;
	state = skill_db[skill].state;
	spiritball = (lv <= 0)? 0:skill_db[skill].spiritball[lv-1];
	mhp = skill_get_mhp(skill, lv);	/* ?ﾁ費HP */
	for(i = 0; i < 10; i++) {
		itemid[i] = skill_db[skill].itemid[i];
		amount[i] = skill_db[skill].amount[i];
	}
	if(mhp > 0)
		hp += (sd->status.max_hp * mhp)/100;
	if(hp_rate > 0)
		hp += (sd->status.hp * hp_rate)/100;
	else
		hp += (sd->status.max_hp * abs(hp_rate))/100;
	if(sp_rate > 0)
		sp += (sd->status.sp * sp_rate)/100;
	else
		sp += (sd->status.max_sp * abs(sp_rate))/100;

	switch(skill) { // Check for cost reductions due to skills & SCs
		case MC_MAMMONITE:
			if(pc_checkskill(sd,BS_UNFAIRLYTRICK)>0)
				zeny -= zeny*10/100;
			break;
		case AL_HOLYLIGHT:
			if(sd->sc_data[SC_SPIRIT].timer!=-1 && sd->sc_data[SC_SPIRIT].val2 == SL_PRIEST)
				sp *= 5;
			break;
		case SL_SMA:
		case SL_STUN:
		case SL_STIN:
		{
			int kaina_lv = pc_checkskill(sd,SL_KAINA);

			if(kaina_lv==0)
				break;
			if(sd->status.base_level>=90)
				sp -= sp*7*kaina_lv/100;
			else if(sd->status.base_level>=80)
				sp -= sp*5*kaina_lv/100;
			else if(sd->status.base_level>=70)
				sp -= sp*3*kaina_lv/100;
		}
			break;
		case MO_CHAINCOMBO:
		case MO_COMBOFINISH:
		case CH_TIGERFIST:
		case CH_CHAINCRUSH:
			if(sd->sc_data[SC_SPIRIT].timer!=-1 && sd->sc_data[SC_SPIRIT].val2 == MAPID_MONK)
				sp -= sp*sd->sc_data[SC_SPIRIT].val1/10;
			break;
	}

	if(sd->dsprate!=100)
		sp=sp*sd->dsprate/100;	/* ?ﾁ費SP?C?ｳ */

	switch(skill) {
	case SA_CASTCANCEL:
		if(sd->skilltimer == -1) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case BS_MAXIMIZE:		/* マキシマイズパ�?? */
	case NV_TRICKDEAD:		/* 死んだふり */
	case TF_HIDING:			/* ハイディング */
	case AS_CLOAKING:		/* ク�??キング */
	case CR_AUTOGUARD:				/* オ?トガ?ド */
	case CR_DEFENDER:				/* ディフェンダ? */
	case ST_CHASEWALK:
	case PA_GOSPEL:
	case TK_RUN:
	case CR_SHRINK:
		if(sd->sc_data[SkillStatusChangeTable[skill]].timer!=-1)
			return 1;			/* 解?怩ｷる?�?�はSP?ﾁ費しない */
		break;
	case AL_WARP:
		if(!(type&2)) //Delete the item when the portal has been selected (type&2). [Skotlex]
			delitem_flag = 0;
	case AL_TELEPORT:
		if(map[sd->bl.m].flag.noteleport) {
			clif_skill_teleportmessage(sd,0);
			return 0;
		}
		break;
	case MO_CALLSPIRITS:	/* ?功 */
		if(sd->spiritball >= lv) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case CH_SOULCOLLECT:	/* 狂?功 */
		if(sd->spiritball >= 5) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case MO_FINGEROFFENSIVE:				//指?
		if (sd->spiritball > 0 && sd->spiritball < spiritball) {
			spiritball = sd->spiritball;
			sd->spiritball_old = sd->spiritball;
		}
		else sd->spiritball_old = lv;
		break;
	case MO_BODYRELOCATION:
		if (sd->sc_data[SC_EXPLOSIONSPIRITS].timer!=-1)
			spiritball = 0;
		break;
	case MO_CHAINCOMBO:			//連打?ｶ			
		if(sd->sc_data[SC_BLADESTOP].timer==-1){
		if(sd->sc_data[SC_COMBO].timer == -1 || sd->sc_data[SC_COMBO].val1 != MO_TRIPLEATTACK)
			return 0;
		}
		break;
	case MO_COMBOFINISH:					//猛龍�?
		if(sd->sc_data[SC_COMBO].timer == -1 || sd->sc_data[SC_COMBO].val1 != MO_CHAINCOMBO)
			return 0;
		break;
	case CH_TIGERFIST:						//伏虎�?
		if(sd->sc_data[SC_COMBO].timer == -1 || sd->sc_data[SC_COMBO].val1 != MO_COMBOFINISH)
			return 0;
		break;
	case CH_CHAINCRUSH:						//連柱崩?
		if(sd->sc_data[SC_COMBO].timer == -1)
			return 0;
		if(sd->sc_data[SC_COMBO].val1 != MO_COMBOFINISH && sd->sc_data[SC_COMBO].val1 != CH_TIGERFIST)
			return 0;
		break;
	case MO_EXTREMITYFIST:					// 阿?C羅覇鳳�?
		if(sd->sc_data[SC_BLADESTOP].timer!=-1)
			spiritball--;
		else if (sd->sc_data[SC_COMBO].timer != -1) {
			if (sd->sc_data[SC_COMBO].val1 == MO_COMBOFINISH)
				spiritball = 4;
			else if (sd->sc_data[SC_COMBO].val1 == CH_TIGERFIST)
				spiritball = 3;
			else if (sd->sc_data[SC_COMBO].val1 == CH_CHAINCRUSH)
				spiritball = sd->spiritball?sd->spiritball:1;
			//It should consume whatever is left as long as it's at least 1.
		}
		break;
	case TK_TURNKICK:
	case TK_STORMKICK:
	case TK_DOWNKICK:
	case TK_COUNTER:
		if (sd->sc_data[SC_COMBO].timer == -1 || sd->sc_data[SC_COMBO].val1 != skill)
			if (!pc_istop10fame(sd->char_id,MAPID_TAEKWON))
			return 0;
		break;
	case BD_ADAPTATION:				/* アドリブ */
		{
			struct skill_unit_group *group=NULL;
			if(sd->sc_data[SC_DANCING].timer==-1 || ((group=(struct skill_unit_group*)sd->sc_data[SC_DANCING].val2) && (skill_get_time(sd->sc_data[SC_DANCING].val1,group->skill_lv) - sd->sc_data[SC_DANCING].val3*1000) <= skill_get_time2(skill,lv))){ //ダンス中で使用後5秒以?繧ﾌみ?H
				clif_skill_fail(sd,skill,0,0);
				return 0;
			}
		}
		break;
	case PR_BENEDICTIO:				/* ?ｹ??~福 */
		{
			if (!battle_config.player_skill_partner_check)
				break; //No need to do any partner checking [Skotlex]
			if (!(type&1))
			{	//Started casting.
				if (skill_check_pc_partner(sd, skill, &lv, 1, 0) < 2)
				{
					clif_skill_fail(sd,skill,0,0);
					return 0;
				}
			}
			else
			{	//Done casting
				//Should I repeat the check? If so, it would be best to only do this on cast-ending. [Skotlex]
				skill_check_pc_partner(sd, skill, &lv, 1, 1);
			}
		}
		break;
	case WE_CALLPARTNER:		/* あなたに逢いたい */
		if(!sd->status.partner_id){
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case AM_CANNIBALIZE:		/* バイオプラント */
	case AM_SPHEREMINE:			/* スフィア?マイン */
		if(type&1){
			int c=0;
			int summons[5] = { 1020, 1068, 1118, 1500, 1368 };
			int maxcount = (skill==AM_CANNIBALIZE)? 6-lv : skill_get_maxcount(skill);
			int mob_class = (skill==AM_CANNIBALIZE)? summons[lv-1] :1142;
			if(battle_config.pc_land_skill_limit && maxcount>0) {
				map_foreachinarea(skill_check_condition_mob_master_sub ,sd->bl.m, 0, 0, map[sd->bl.m].xs, map[sd->bl.m].ys, BL_MOB, sd->bl.id, mob_class,&c );
				if(c >= maxcount){
					clif_skill_fail(sd,skill,0,0);
					return 0;
				}
			}
		}
		break;
	case MG_FIREWALL:		/* ファイア?ウォ?ル */
	case WZ_QUAGMIRE:
	case PF_FOGWALL:
		/* ??ｧ限 */
		if(battle_config.pc_land_skill_limit) {
			int maxcount = skill_get_maxcount(skill);
			if(maxcount > 0) {
				int i,c;
				for(i=c=0;i<MAX_SKILLUNITGROUP;i++) {
					if(sd->skillunit[i].alive_count > 0 && sd->skillunit[i].skill_id == skill)
						c++;
				}
				if(c >= maxcount) {
					clif_skill_fail(sd,skill,0,0);
					return 0;
				}
			}
		}
		break;
	case WZ_FIREPILLAR: // celest
		if (lv <= 5)	// no gems required at level 1-5
			itemid[0] = 0;
		if(battle_config.pc_land_skill_limit) {
			int maxcount = skill_get_maxcount(skill);
			if(maxcount > 0) {
				int i,c;
				for(i=c=0;i<MAX_SKILLUNITGROUP;i++) {
					if(sd->skillunit[i].alive_count > 0 && sd->skillunit[i].skill_id == skill)
						c++;
				}
				if(c >= maxcount) {
					clif_skill_fail(sd,skill,0,0);
					return 0;
				}
			}
		}
		break;
	// skills require arrows as of 12/07 [celest]
	case AC_DOUBLE:
	case AC_SHOWER:
	case AC_CHARGEARROW:
	case BA_MUSICALSTRIKE:
	case DC_THROWARROW:
	case SN_SHARPSHOOTING:
	case CG_ARROWVULCAN:
	case AS_VENOMKNIFE:
		if(sd->equip_index[10] < 0) {
			clif_arrow_fail(sd,0);
			return 0;
		}
		arrow_flag = 1;
		break;
	case RG_BACKSTAP:
		if(sd->status.weapon == 11) {
			if (sd->equip_index[10] < 0) {
				clif_arrow_fail(sd,0);
				return 0;
			}
			arrow_flag = 1;
		}
		break;
	case HW_GANBANTEIN:
		force_gem_flag = 1;
		break;
	case AM_POTIONPITCHER:
	case CR_SLIMPITCHER:
	case MG_STONECURSE:
	case CR_CULTIVATION:
	case SA_FLAMELAUNCHER:
	case SA_FROSTWEAPON:
	case SA_LIGHTNINGLOADER:
	case SA_SEISMICWEAPON:
		delitem_flag = 0;
		break;
	case SA_DELUGE:
	case SA_VOLCANO:
	case SA_VIOLENTGALE:
	case SA_LANDPROTECTOR:
	{	//Does not consumes if the skill is already active. [Skotlex]
		struct skill_unit_group *sg;
		if ((sg= skill_locate_element_field(&sd->bl)) != NULL && sg->skill_id == skill)
		{
			if (sg->limit - DIFF_TICK(gettick(), sg->tick) > 0)
				checkitem_flag = delitem_flag = 0;
			else sg->limit = 0; //Disable it.
		}
		break;
	}
	case CG_HERMODE:
		{
			int c = 0;
			map_foreachinarea (skill_check_condition_hermod_sub, sd->bl.m,
				sd->bl.x-3, sd->bl.y-3, sd->bl.x+3, sd->bl.y+3, BL_NPC, &c);
			if (c < 1) {
				clif_skill_fail(sd,skill,0,0);
				return 0;
			}
		}
		break;
	case CG_MOONLIT: //Check there's no wall in the range+1 area around the caster. [Skotlex]
		{
			int i,x,y,range = skill_get_range2(&sd->bl, skill, lv)+1;
			int size = range*2+1;
			for (i=0;i<size*size;i++) {
				x = sd->bl.x+(i%size-range);
				y = sd->bl.y+(i/size-range);
				if (map_getcell(sd->bl.m,x,y,CELL_CHKWALL)) {
					clif_skill_fail(sd,skill,0,0);
					return 0;
				}	
			}
		}
		break;
	case PR_REDEMPTIO:
		{
			int exp;
			if(((exp = pc_nextbaseexp(sd)) > 0 && sd->status.base_exp*100/exp < 1) ||
				((exp = pc_nextjobexp(sd)) > 0 && sd->status.job_exp*100/exp < 1)) {
				clif_skill_fail(sd,skill,0,0); //Not enough exp.
				return 0;
			}
			break;
		}
	}

	if(!(type&2)){
		if( hp>0 && sd->status.hp < hp) {				/* HPチェック */
			clif_skill_fail(sd,skill,2,0);		/* HP不足?F失敗通知 */
			return 0;
		}
		if( sp>0 && sd->status.sp < sp) {				/* SPチェック */
			clif_skill_fail(sd,skill,1,0);		/* SP不足?F失敗通知 */
			return 0;
		}
		if( zeny>0 && sd->status.zeny < zeny) {
			clif_skill_fail(sd,skill,5,0);
			return 0;
		}
		if(!(weapon & (1<<sd->status.weapon) ) ) {
			clif_skill_fail(sd,skill,6,0);
			return 0;
		}
		if( spiritball > 0 && sd->spiritball < spiritball) {
			clif_skill_fail(sd,skill,0,0);		// 氣球不足
			return 0;
		}
	}

	switch(state) {
	case ST_HIDING:
		if(!(sd->status.option&2)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_CLOAKING:
		if(!pc_iscloaking(sd)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_HIDDEN:
		if(!pc_ishiding(sd)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_RIDING:
		if(!pc_isriding(sd)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_FALCON:
		if(!pc_isfalcon(sd)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_CART:
		if(!pc_iscarton(sd)) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_SHIELD:
		if(sd->status.shield <= 0) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_SIGHT:
		if(sd->sc_data[SC_SIGHT].timer == -1 && type&1) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_EXPLOSIONSPIRITS:
		if(sd->sc_data[SC_EXPLOSIONSPIRITS].timer == -1) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_CARTBOOST:
		if(!pc_iscarton(sd) || sd->sc_data[SC_CARTBOOST].timer == -1) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_RECOV_WEIGHT_RATE:
		if(battle_config.natural_heal_weight_rate <= 100 && sd->weight*100/sd->max_weight >= battle_config.natural_heal_weight_rate) {
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	case ST_MOVE_ENABLE:
		{
			struct walkpath_data wpd;
			if(path_search(&wpd,sd->bl.m,sd->bl.x,sd->bl.y,sd->skillx,sd->skilly,1)==-1) {
				clif_skill_fail(sd,skill,0,0);
				return 0;
			}
		}
		break;
	case ST_WATER:
		//?�?齡ｻ定
		//(!map[sd->bl.m].flag.rain) && //they have removed RAIN effect. [Lupus]
		if ( (sd->sc_data[SC_DELUGE].timer == -1) &&
			(!map_getcell(sd->bl.m,sd->bl.x,sd->bl.y,CELL_CHKWATER)))
		{
			clif_skill_fail(sd,skill,0,0);
			return 0;
		}
		break;
	}

	if (checkitem_flag) {
		for(i=0;i<10;i++) {
			int x = lv%11 - 1;
			index[i] = -1;
			if(itemid[i] <= 0)
				continue;
			if(itemid[i] >= 715 && itemid[i] <= 717 && sd->special_state.no_gemstone && !force_gem_flag)
				continue;
			if(((itemid[i] >= 715 && itemid[i] <= 717) || itemid[i] == 1065)
				&& sd->sc_data[SC_INTOABYSS].timer != -1 && !force_gem_flag)
				continue;
			if((skill == AM_POTIONPITCHER ||
				skill == CR_SLIMPITCHER ||
				skill == CR_CULTIVATION) && i != x)
				continue;

			index[i] = pc_search_inventory(sd,itemid[i]);
			if(index[i] < 0 || sd->status.inventory[index[i]].amount < amount[i]) {
				if(itemid[i] == 716 || itemid[i] == 717)
					clif_skill_fail(sd,skill,(7+(itemid[i]-716)),0);
				else
					clif_skill_fail(sd,skill,0,0);
				return 0;
			}
		}
	}

	if(!(type&1))
		return 1;

	if(delitem_flag) {
		for(i=0;i<10;i++) {
			if(index[i] >= 0)
				pc_delitem(sd,index[i],amount[i],0);		// アイテム?ﾁ費
		}
		if (arrow_flag && battle_config.arrow_decrement)
			pc_delitem(sd,sd->equip_index[10],1,0);
	}

	if(type&2)
		return 1;

	if(sp > 0) {					// SP?ﾁ費
		sd->status.sp-=sp;
		clif_updatestatus(sd,SP_SP);
	}
	if(hp > 0) {					// HP?ﾁ費
		sd->status.hp-=hp;
		clif_updatestatus(sd,SP_HP);
	}
	if(zeny > 0)					// Zeny?ﾁ費
		pc_payzeny(sd,zeny);
	if(spiritball > 0)				// 氣球?ﾁ費
		pc_delspiritball(sd,spiritball,0);


	return 1;
}

/*==========================================
 * 詠?･時間計算
 *------------------------------------------
 */
int skill_castfix( struct block_list *bl, int skill_id, int skill_lv, int time)
{
	struct status_change *sc_data;
	
	nullpo_retr(0, bl);

	if (!time && bl->type != BL_MOB)
		time = skill_get_cast(skill_id, skill_lv);
	
	if (bl->type == BL_PC){
		struct map_session_data *sd = (struct map_session_data*)bl;
		nullpo_retr(0, sd);

		// calculate base cast time (reduced by dex)
		if (!skill_get_castnodex(sd->skillid, sd->skilllv) > 0) {
			int scale = battle_config.castrate_dex_scale - status_get_dex(bl);
			if (scale > 0)	// not instant cast
				time = time * scale / battle_config.castrate_dex_scale;
			else return 0;	// instant cast
		}

		// config cast time multiplier
		if (battle_config.cast_rate != 100)
			time = time * battle_config.cast_rate / 100;

		// calculate cast time reduced by card bonuses
		if (sd->castrate != 100)
			time -= time * (100 - sd->castrate) / 100;
	} else if (bl->type == BL_PET) { //Skotlex: Simple scaling
		int scale = battle_config.castrate_dex_scale - status_get_dex(bl);
		if (scale > 0)	// not instant cast
			time = time * scale / battle_config.castrate_dex_scale;
		else return 0;	// instant cast
		
		if (battle_config.cast_rate != 100)
			time = time * battle_config.cast_rate / 100;
	}

	// calculate cast time reduced by skill bonuses
	sc_data = status_get_sc_data(bl);
	/* サフラギウム */
	if (sc_data) {
		if (sc_data[SC_SUFFRAGIUM].timer != -1) {
			time -= time * (sc_data[SC_SUFFRAGIUM].val1 * 15) / 100;
			status_change_end(bl, SC_SUFFRAGIUM, -1);
		}
		/* ブラギの�? */
		if (sc_data[SC_POEMBRAGI].timer != -1)
			time -= time * sc_data[SC_POEMBRAGI].val2 / 100;
	}

	// return final cast time
	return (time > 0) ? time : 0;
}
/*==========================================
 * ディレイ計算
 *------------------------------------------
 */
int skill_delayfix( struct block_list *bl, int skill_id, int skill_lv, int time )
{
	struct status_change *sc_data;	

	nullpo_retr(0, bl);

	if (!time && bl->type != BL_MOB)
		time = skill_get_delay(skill_id, skill_lv);
	
	if (bl->type == BL_PC){
		struct map_session_data *sd = (struct map_session_data*)bl;
		nullpo_retr(0, sd);

		// instant cast attack skills depend on aspd as delay [celest]
		if (time == 0) {
			if (skill_get_type(sd->skillid) == BF_WEAPON)
				time = status_get_adelay (bl)/2;
			else
				time = 300;	// default delay, according to official servers
		} else if (time < 0)
			time = abs(time) + status_get_adelay (bl)/2;	// if set to <0, the aspd delay will be added

		if (battle_config.delay_dependon_dex &&	/* dexの影響を計算する */
			!skill_get_delaynodex(sd->skillid, sd->skilllv))	// if skill casttime is allowed to be reduced by dex
		{
			int scale = battle_config.castrate_dex_scale - status_get_dex(bl);
			if (scale < 0)
				scale = 0;
			time = time * scale / battle_config.castrate_dex_scale;
		}

		if (battle_config.delay_rate != 100)
			time = time * battle_config.delay_rate / 100;

		if (sd->delayrate != 100)
			time = time * sd->delayrate / 100;

		if (time < battle_config.min_skill_delay_limit)	// check minimum skill delay
			time = battle_config.min_skill_delay_limit;
	}

	/* ブラギの�? */
	sc_data = status_get_sc_data(bl);
	if (sc_data) {
		if (sc_data[SC_POEMBRAGI].timer != -1)
			time -= time * sc_data[SC_POEMBRAGI].val3 / 100;
		if (sc_data[SC_SPIRIT].timer != -1)
			switch (skill_id) {
				case CR_SHIELDBOOMERANG:
					if (sc_data[SC_SPIRIT].val2 == SL_CRUSADER)
						time /=2;
					break;
				case AS_SONICBLOW:
					if (!map_flag_gvg(bl->m) && sc_data[SC_SPIRIT].val2 == SL_ASSASIN)
						time /= 2;
					break;
			}
	}

	return (time > 0) ? time : 0;
}

/*==========================================
 * スキル使用?iID指定?j
 *------------------------------------------
 */
int skill_use_id (struct map_session_data *sd, int target_id, int skill_num, int skill_lv)
{
	struct map_session_data* tsd = NULL;
	struct block_list *bl = NULL;
	struct status_change *sc_data;
	int casttime, forcecast = 0;	
	unsigned int tick = gettick();

	nullpo_retr(0, sd);

	if (skill_lv <= 0)
		return 0;

	sc_data = sd->sc_data;
	switch(skill_num)
	{	//Check for skills that auto-select target
	case MO_CHAINCOMBO:
		target_id = sd->attacktarget;
		if (sc_data[SC_BLADESTOP].timer != -1){
			if ((bl=(struct block_list *)sc_data[SC_BLADESTOP].val4) == NULL) //タ?ゲットがいない？
				return 0;
			target_id = bl->id;
		}
		break;
	case MO_COMBOFINISH:
	case CH_CHAINCRUSH:
	case CH_TIGERFIST:
	case TK_STORMKICK: // Taekwon kicks [Dralnu]
	case TK_DOWNKICK:
	case TK_TURNKICK:
		target_id = sd->attacktarget;
		break;

	case TK_JUMPKICK:
	case TK_COUNTER:
		if (sc_data[SC_COMBO].timer != -1 && sc_data[SC_COMBO].val1 == skill_num)
			target_id = sc_data[SC_COMBO].val2;
		else if (skill_num == TK_COUNTER)
			return 0;
		break;
// -- moonsoul	(altered to allow proper usage of extremity from new champion combos)
//
	case MO_EXTREMITYFIST:	/*阿修羅覇鳳拳*/
		if (sc_data[SC_COMBO].timer != -1 &&
			(sc_data[SC_COMBO].val1 == MO_COMBOFINISH ||
			sc_data[SC_COMBO].val1 == CH_TIGERFIST ||
			sc_data[SC_COMBO].val1 == CH_CHAINCRUSH))
			target_id = sd->attacktarget;
		break;
	case WE_MALE:
	case WE_FEMALE:
		if (!sd->status.partner_id)
			return 0;
		tsd = map_charid2sd(sd->status.partner_id);
		bl = (struct block_list *)tsd;
		if (bl)
			target_id = bl->id;
		else
		{
			clif_skill_fail(sd,skill_num,0,0);
			return 0;
		}
		break;
	case WE_CALLBABY:
		tsd = pc_get_child(sd);
		bl = (struct block_list *)tsd;
		if (bl)
			target_id = bl->id;
		else
		{
			clif_skill_fail(sd,skill_num,0,0);
			return 0;
		}
		break;
	}
	if (bl == NULL && (bl = map_id2bl(target_id)) == NULL)
		return 0;
	
	if (bl->type == BL_PC)
		tsd = (struct map_session_data*)bl;
	
	if (bl->prev == NULL) //Prevent targeting enemies that are not in the map. [Skotlex]
		return 0;
	
	if(sd->bl.m != bl->m)
		return 0;

	if(sd->skilltimer != -1 && skill_num != SA_CASTCANCEL) //Normally not needed because clif.c checks for it, but the at/char/script commands don't! [Skotlex]
		return 0;
	
	if(skillnotok(skill_num, sd)) // [MouseJstr]
		return 0;
	if (tsd && skill_num == ALL_RESURRECTION && !pc_isdead(tsd))
		return 0;

	if(skill_get_inf2(skill_num)&INF2_NO_TARGET_SELF && sd->bl.id == target_id)
		return 0;
	if(!status_check_skilluse(&sd->bl, bl, skill_num, 0))
	{
		if(skill_num == PR_LEXAETERNA) //Eh.. assuming skill failed due to opponent frozen/stone-cursed. [Skotlex]
			clif_skill_fail(sd,skill_num,0,0);
		return 0;
	}
	
	//直前のスキルが何か?える必要のあるスキル
	switch (skill_num) {
	case SA_CASTCANCEL:
		if (sd->skillid != skill_num){ //キャストキャンセル自?は?えない
			sd->skillid_old = sd->skillid;
			sd->skilllv_old = sd->skilllv;
		}
		break;

	case BD_ENCORE:					/* アンコ?ル */
		if (!sd->skillid_dance) { //前回使用した踊りがないとだめ
			clif_skill_fail(sd,skill_num,0,0);
			return 0;
		} else {
			sd->skillid_old = skill_num;
		}
		break;

	case GD_BATTLEORDER:
	case GD_REGENERATION:
	case GD_RESTORE:
	case GD_EMERGENCYCALL:
		{
			if (!sd->status.guild_id || !sd->state.gmaster_flag)
				return 0;
			skill_lv = guild_checkskill(sd->state.gmaster_flag, skill_num);
			if (skill_lv <= 0) return 0;
		}
		break;

	case BD_LULLABY:				/* 子守歌 */
	case BD_RICHMANKIM:				/* ニヨルドの宴 */
	case BD_ETERNALCHAOS:			/* 永遠の?ｬ沌 */
	case BD_DRUMBATTLEFIELD:		/* ?太鼓の響き */
	case BD_RINGNIBELUNGEN:			/* ニ?ベルングの指輪 */
	case BD_ROKISWEIL:				/* �?キの叫び */
	case BD_INTOABYSS:				/* ?[淵の中に */
	case BD_SIEGFRIED:				/* 不死?gのジ?クフリ?ド */
	case CG_MOONLIT:				/* 月明りの?�に落ちる花びら */
		{
			if (battle_config.player_skill_partner_check)
			{
				if (skill_check_pc_partner(sd, skill_num, &skill_lv, 1, 0) < 1) //Note that skill_lv is automatically updated.
				{
					clif_skill_fail(sd,skill_num,0,0);
					return 0;
				}
			}
			break;
		}
	}

	sd->skillid = skill_num;
	sd->skilllv = skill_lv;
	if (!skill_check_condition(sd,0)) return 0;	

	if(sd->bl.id != target_id){ // Don't check range for self skills, this is useless...
		if(!battle_check_range(&sd->bl,bl,skill_get_range2(&sd->bl, skill_num,skill_lv)+1))
			return 0;
	}

	if ((skill_num != MO_CHAINCOMBO &&
		skill_num != MO_COMBOFINISH &&
		skill_num != MO_EXTREMITYFIST &&
		skill_num != CH_TIGERFIST &&
		skill_num != CH_CHAINCRUSH &&
		skill_num != TK_STORMKICK &&
		skill_num != TK_DOWNKICK &&
		skill_num != TK_TURNKICK &&
		skill_num != TK_COUNTER) ||
		(skill_num == MO_EXTREMITYFIST && sd->state.skill_flag))
		pc_stopattack(sd);

	casttime = skill_castfix(&sd->bl, skill_num, skill_lv, 0);
	sd->state.skillcastcancel = skill_get_castcancel(skill_num);

	switch (skill_num) {	/* 何か特殊な?�?が必要 */
	case ALL_RESURRECTION:	/* リザレクション */
		if (!tsd && battle_check_undead(status_get_race(bl),status_get_elem_type(bl))) {	/* 敵がアンデッドなら */
			forcecast = 1;	/* タ?ンアンデットと同じ詠?･時間 */
			casttime = skill_castfix(&sd->bl, PR_TURNUNDEAD, skill_lv, 0);
		}
		break;

	case MO_FINGEROFFENSIVE:	/* 指? */
		casttime += casttime * ((skill_lv > sd->spiritball) ? sd->spiritball : skill_lv);
		break;

// -- moonsoul	(altered to allow proper usage of extremity from new champion combos)
//
	case MO_EXTREMITYFIST:	/*阿?C羅覇鳳�?*/
		if (sc_data && sc_data[SC_COMBO].timer != -1 &&
			(sc_data[SC_COMBO].val1 == MO_COMBOFINISH ||
			sc_data[SC_COMBO].val1 == CH_TIGERFIST ||
			sc_data[SC_COMBO].val1 == CH_CHAINCRUSH))
			casttime = 0;
		forcecast = 1;
		break;

	case SA_MAGICROD:
	case SA_SPELLBREAKER:
		forcecast = 1;
		break;

	case KN_CHARGEATK:
		casttime *= distance(sd->bl.x,sd->bl.y,bl->x,bl->y);
		break;

	// parent-baby skills
	case WE_BABY:
	case WE_CALLPARENT:
		{
			struct map_session_data *f_sd = pc_get_father(sd);
			struct map_session_data *m_sd = pc_get_mother(sd);
			
			// set target as any one of the parent
			if (f_sd) target_id = f_sd->bl.id;
			else if (m_sd) target_id = m_sd->bl.id;
			else return 0;	// neither are found
		}
		break;
	case HP_BASILICA:		/* バジリカ */
		{
			// cancel Basilica if already in effect
			if (sc_data && sc_data[SC_BASILICA].timer != -1 && sc_data[SC_BASILICA].val3 == BCT_SELF) {
				status_change_end(&sd->bl,SC_BASILICA,-1);
				return 0;
			}
		}
		break;
	}

	//�?モライズ?態ならキャストタイムが1/3
	if (sc_data && sc_data[SC_MEMORIZE].timer != -1 && casttime > 0) {
		casttime = casttime/2;
		if ((--sc_data[SC_MEMORIZE].val2) <= 0)
			status_change_end(&sd->bl, SC_MEMORIZE, -1);
	}

	if (battle_config.pc_skill_log)
		ShowInfo("PC %d skill use target_id=%d skill=%d lv=%d cast=%d\n",
			sd->bl.id, target_id, skill_num, skill_lv, casttime);

	if (casttime > 0 || forcecast) {
		struct mob_data *md;
		int mode;
		if(sd->disguise) { // [Valaris]
			clif_skillcasting(&sd->bl,sd->bl.id, target_id, 0,0, skill_num,0);
			clif_skillcasting(&sd->bl,-sd->bl.id, target_id, 0,0, skill_num,casttime);
		}
		else
			clif_skillcasting(&sd->bl,sd->bl.id, target_id, 0,0, skill_num,casttime);
		/* 詠?･反?モンスタ? */
		if (bl->type == BL_MOB && (mode = status_get_mode(bl))&MD_CASTSENSOR && (md = (struct mob_data *)bl) &&
			(!md->special_state.ai || skill_get_inf(skill_num) != INF_SUPPORT_SKILL) //Avoid having summons target master from supportive skills. [Skotlex]
		) {
			switch (md->state.skillstate) {
				case MSS_ANGRY:
				case MSS_RUSH:
				case MSS_FOLLOW:
					if (!(mode&(MD_AGGRESSIVE|MD_ANGRY)))
						break; //Only Aggressive mobs change target while chasing.
				case MSS_IDLE:
				case MSS_WALK:
					md->target_id = sd->bl.id;
					md->state.targettype = ATTACKABLE;
					md->state.aggressive = (mode&MD_ANGRY)?1:0;
					md->min_chase = md->db->range3;
			}
		}
	}

	if (!(battle_config.pc_cloak_check_type&2) &&
		sc_data && sc_data[SC_CLOAKING].timer != -1 &&
		sd->skillid != AS_CLOAKING)
		status_change_end(&sd->bl,SC_CLOAKING,-1);

	sd->skilltarget = target_id;
	sd->skillx = 0;
	sd->skilly = 0;
	sd->canact_tick = tick + casttime + 100;
	//Recycling forcecast to store the skill's level. [Skotlex]
	sd->canmove_tick = tick + (casttime>0 && (forcecast = pc_checkskill(sd,SA_FREECAST)) > 0?0:casttime);

	if (casttime > 0) {
		sd->skilltimer = add_timer (tick + casttime, skill_castend_id, sd->bl.id, 0);
		if (forcecast > 0)
			status_quick_recalc_speed (sd, SA_FREECAST, forcecast, 1);
		else
			pc_stop_walking(sd,0);
	} else {
		sd->state.skillcastcancel = 0;	/* 詠?･の無いものはキャンセルされない */
		if (skill_num != SA_CASTCANCEL)
			sd->skilltimer = -1;
		skill_castend_id(sd->skilltimer,tick,sd->bl.id,0);
	}

	return 0;
}

/*==========================================
 * スキル使用?i?�?且w定?j
 *------------------------------------------
 */
int skill_use_pos (struct map_session_data *sd, int skill_x, int skill_y, int skill_num, int skill_lv)
{
	struct block_list bl;
	struct status_change *sc_data;
	int casttime, skill = 0;
	unsigned int tick = gettick();

	nullpo_retr(0, sd);

	if (pc_isdead(sd))
		return 0;
	if (skill_lv <= 0)
		return 0;
	if (sd->skilltimer != -1) //Normally not needed since clif.c checks for it, but at/char/script commands don't! [Skotlex]
		return 0;
	if (skillnotok(skill_num, sd)) // [MouseJstr]
		return 0;
	if (skill_num == WZ_ICEWALL && map[sd->bl.m].flag.noicewall && !map[sd->bl.m].flag.pvp && !map[sd->bl.m].flag.gvg)  { // noicewall flag [Valaris]
		clif_skill_fail(sd,sd->skillid,0,0);
		return 0;
	}
	if (map_getcell(sd->bl.m, skill_x, skill_y, CELL_CHKNOPASS))
	{	//prevent casting ground targeted spells on non-walkable areas. [Skotlex] 
		
		clif_skill_fail(sd,skill_num,0,0);
		return 0;
	}
	
	sc_data = sd->sc_data;

	if (!status_check_skilluse(&sd->bl, NULL, skill_num, 0))
		return 0;

	sd->skillid = skill_num;
	sd->skilllv = skill_lv;
	sd->skillx = skill_x;
	sd->skilly = skill_y;
	if (!skill_check_condition(sd,0)) return 0;

	/* 射程と?瘧Q物チェック */
	bl.type = BL_NUL;
	bl.m = sd->bl.m;
	bl.x = skill_x;
	bl.y = skill_y;

	if(!battle_check_range(&sd->bl,&bl,skill_get_range2(&sd->bl, skill_num,skill_lv)+1))
		return 0;
		
/* Previous code body, left here in case we have to rollback. [Skotlex]
	{
		int check_range_flag = 0;

		range = skill_get_range(skill_num,skill_lv);
		if(range < 0)
			range = status_get_range(&sd->bl) - (range + 1);
		// be lenient if the skill was cast before we have moved to the correct position [Celest]
		if (sd->walktimer != -1)
			range ++;
		else check_range_flag = 1;
		if(!battle_check_range(&sd->bl,&bl,range)) {
			if (check_range_flag && pc_can_move(sd) && battle_check_range(&sd->bl,&bl,range + 1)) {
				int mask[8][2] = {{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
				int dir = map_calc_dir(&sd->bl,bl.x,bl.y);
				pc_walktoxy (sd, sd->bl.x + mask[dir][0], sd->bl.y + mask[dir][1]);
			} else
				return 0;
		}
	}
*/
	pc_stopattack(sd);

	casttime = skill_castfix(&sd->bl, skill_num, skill_lv, 0);
	sd->state.skillcastcancel = skill_db[skill_num].castcancel;

	if (battle_config.pc_skill_log)
		ShowInfo("PC %d skill use target_pos=(%d,%d) skill=%d lv=%d cast=%d\n",
			sd->bl.id, skill_x, skill_y, skill_num, skill_lv, casttime);

	//�?モライズ?態ならキャストタイムが1/3
	if (sc_data && sc_data[SC_MEMORIZE].timer != -1 && casttime > 0){
		casttime = casttime/3;
		if ((--sc_data[SC_MEMORIZE].val2)<=0)
			status_change_end(&sd->bl, SC_MEMORIZE, -1);
	}
	
	if( casttime>0 ) {	/* 詠?･が必要 */
		if(sd->disguise) { // [Valaris]
			clif_skillcasting(&sd->bl,sd->bl.id, 0, skill_x,skill_y, skill_num,0);
			clif_skillcasting(&sd->bl,-sd->bl.id, 0, skill_x,skill_y, skill_num,casttime);
		}
		else
			clif_skillcasting(&sd->bl,sd->bl.id, 0, skill_x,skill_y, skill_num,casttime);
	}

	if (!(battle_config.pc_cloak_check_type&2) &&
		sc_data && sc_data[SC_CLOAKING].timer != -1)
		status_change_end(&sd->bl,SC_CLOAKING,-1);

	sd->skilltarget	= 0;
	sd->canact_tick = tick + casttime + 100;
	sd->canmove_tick = tick + (casttime>0 && (skill = pc_checkskill(sd,SA_FREECAST))>0?0:casttime);

	if (casttime > 0) {
		sd->skilltimer = add_timer(tick + casttime, skill_castend_pos, sd->bl.id, 0);
		if (skill > 0)
			status_quick_recalc_speed (sd, SA_FREECAST, skill, 1);
		else
			pc_stop_walking(sd,0);
	} else {
		sd->state.skillcastcancel = 0;	/* 詠?･の無いものはキャンセルされない */
		sd->skilltimer = -1;
		skill_castend_pos(sd->skilltimer,tick,sd->bl.id,0);
	}

	return 0;
}

/*==========================================
 * スキル詠?･キャンセル
 *------------------------------------------
 */
int skill_castcancel (struct block_list *bl, int type)
{
	int ret = 0;

	nullpo_retr(0, bl);

	if (bl->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)bl;
		unsigned long tick = gettick();
		nullpo_retr(0, sd);
		sd->canact_tick = tick;
		sd->canmove_tick = tick;
		if (sd->skilltimer != -1) {
			if ((ret = pc_checkskill(sd,SA_FREECAST)) > 0) {
				status_quick_recalc_speed(sd, SA_FREECAST, ret, 0);	//Updated to use calc_speed [Skotlex]
			}
			if (!type) {
				if (skill_get_inf( sd->skillid ) & INF_GROUND_SKILL)
					ret = delete_timer( sd->skilltimer, skill_castend_pos );
				else
					ret = delete_timer( sd->skilltimer, skill_castend_id );
				if (ret < 0)
					ShowError("delete timer error : skillid : %d\n", sd->skillid);
			} else {
				if (skill_get_inf( sd->skillid_old ) & INF_GROUND_SKILL)
					ret = delete_timer( sd->skilltimer, skill_castend_pos );
				else
					ret = delete_timer( sd->skilltimer, skill_castend_id );
				if (ret < 0)
					ShowError("delete timer error : (old) skillid : %d\n", sd->skillid_old);
			}
			sd->skillid = sd->skilllv = -1;
			sd->skilltimer = -1;
			clif_skillcastcancel(bl);
		}
		return 0;
	} else if (bl->type == BL_MOB) {
		struct mob_data *md = (struct mob_data *)bl;
		nullpo_retr(0, md);
		if (md->skilltimer != -1) {
			if (skill_get_inf( md->skillid ) & INF_GROUND_SKILL)
				ret = delete_timer( md->skilltimer, mobskill_castend_pos );
			else
				ret = delete_timer( md->skilltimer, mobskill_castend_id );
			md->skillid = md->skilllv = -1;
			md->skilltimer = -1;
			clif_skillcastcancel(bl);
		}
		if (ret < 0)
			ShowError("delete timer error : skillid : %d\n", md->skillid);
		return 0;
	} if (bl->type == BL_PET) {
		struct pet_data *pd = (struct pet_data*)bl;
		pd->state.casting_flag = 0;
		clif_skillcastcancel(bl);
		if (pd->timer != -1)
		{	//Free the data attached to casting. [Skotlex]
			struct TimerData *td = get_timer(pd->timer);
			if (td && td->data)
			{
				aFree((struct cast_end_delay*)td->data);
				td->data = 0;
			}
		}
		//The timer is not deleted as the pet's attack will be resumed.
		return 0;
	}

	return 1;
}
/*=========================================
 * ブランディッシュスピア ?炎�範?決定
 *----------------------------------------
 */
void skill_brandishspear_first(struct square *tc,int dir,int x,int y){

	nullpo_retv(tc);

	if(dir == 0){
		tc->val1[0]=x-2;
		tc->val1[1]=x-1;
		tc->val1[2]=x;
		tc->val1[3]=x+1;
		tc->val1[4]=x+2;
		tc->val2[0]=
		tc->val2[1]=
		tc->val2[2]=
		tc->val2[3]=
		tc->val2[4]=y-1;
	}
	else if(dir==2){
		tc->val1[0]=
		tc->val1[1]=
		tc->val1[2]=
		tc->val1[3]=
		tc->val1[4]=x+1;
		tc->val2[0]=y+2;
		tc->val2[1]=y+1;
		tc->val2[2]=y;
		tc->val2[3]=y-1;
		tc->val2[4]=y-2;
	}
	else if(dir==4){
		tc->val1[0]=x-2;
		tc->val1[1]=x-1;
		tc->val1[2]=x;
		tc->val1[3]=x+1;
		tc->val1[4]=x+2;
		tc->val2[0]=
		tc->val2[1]=
		tc->val2[2]=
		tc->val2[3]=
		tc->val2[4]=y+1;
	}
	else if(dir==6){
		tc->val1[0]=
		tc->val1[1]=
		tc->val1[2]=
		tc->val1[3]=
		tc->val1[4]=x-1;
		tc->val2[0]=y+2;
		tc->val2[1]=y+1;
		tc->val2[2]=y;
		tc->val2[3]=y-1;
		tc->val2[4]=y-2;
	}
	else if(dir==1){
		tc->val1[0]=x-1;
		tc->val1[1]=x;
		tc->val1[2]=x+1;
		tc->val1[3]=x+2;
		tc->val1[4]=x+3;
		tc->val2[0]=y-4;
		tc->val2[1]=y-3;
		tc->val2[2]=y-1;
		tc->val2[3]=y;
		tc->val2[4]=y+1;
	}
	else if(dir==3){
		tc->val1[0]=x+3;
		tc->val1[1]=x+2;
		tc->val1[2]=x+1;
		tc->val1[3]=x;
		tc->val1[4]=x-1;
		tc->val2[0]=y-1;
		tc->val2[1]=y;
		tc->val2[2]=y+1;
		tc->val2[3]=y+2;
		tc->val2[4]=y+3;
	}
	else if(dir==5){
		tc->val1[0]=x+1;
		tc->val1[1]=x;
		tc->val1[2]=x-1;
		tc->val1[3]=x-2;
		tc->val1[4]=x-3;
		tc->val2[0]=y+3;
		tc->val2[1]=y+2;
		tc->val2[2]=y+1;
		tc->val2[3]=y;
		tc->val2[4]=y-1;
	}
	else if(dir==7){
		tc->val1[0]=x-3;
		tc->val1[1]=x-2;
		tc->val1[2]=x-1;
		tc->val1[3]=x;
		tc->val1[4]=x+1;
		tc->val2[1]=y;
		tc->val2[0]=y+1;
		tc->val2[2]=y-1;
		tc->val2[3]=y-2;
		tc->val2[4]=y-3;
	}

}

/*=========================================
 * ブランディッシュスピア 方向判定 範??張
 *-----------------------------------------
 */
void skill_brandishspear_dir(struct square *tc,int dir,int are){

	int c;

	nullpo_retv(tc);

	for(c=0;c<5;c++){
		if(dir==0){
			tc->val2[c]+=are;
		}else if(dir==1){
			tc->val1[c]-=are; tc->val2[c]+=are;
		}else if(dir==2){
			tc->val1[c]-=are;
		}else if(dir==3){
			tc->val1[c]-=are; tc->val2[c]-=are;
		}else if(dir==4){
			tc->val2[c]-=are;
		}else if(dir==5){
			tc->val1[c]+=are; tc->val2[c]-=are;
		}else if(dir==6){
			tc->val1[c]+=are;
		}else if(dir==7){
			tc->val1[c]+=are; tc->val2[c]+=are;
		}
	}
}

/*==========================================
 * Weapon Repair [Celest/DracoRPG]
 *------------------------------------------
 */
void skill_repairweapon(struct map_session_data *sd, int idx)
{
	int material;
	int materials[4] = { 1002, 998, 999, 756 };
	struct item *item;

	nullpo_retv(sd);
	nullpo_retv(sd->repair_target);

	if(idx==0xFFFF) // No item selected ('Cancel' clicked)
		return;

    item = &sd->repair_target->status.inventory[idx];

	if(sd!=sd->repair_target && !battle_check_range(&sd->bl,&sd->repair_target->bl,skill_get_range2(&sd->bl, sd->skillid,sd->skilllv))){
		clif_item_repaireffect(sd,item->nameid,1);
		return;
	}

	if(idx >= 0 && idx < MAX_INVENTORY) {
		if(item->nameid > 0 && item->attribute == 1 ) {
			if (itemdb_type(item->nameid)==4)
				material = materials [itemdb_wlv(item->nameid)-1]; // Lv1/2/3/4 weapons consume 1 Iron Ore/Iron/Steel/Rough Oridecon
			else
				material = materials [2]; // Armors consume 1 Steel
			if (pc_search_inventory(sd,material) < 0 ) {
				clif_skill_fail(sd,sd->skillid,0,0);
				return;
			}
			item->attribute=0;
			clif_equiplist(sd->repair_target);
			pc_delitem(sd,pc_search_inventory(sd,material),1,0);
            clif_item_repaireffect(sd,item->nameid,0);
            if(sd!=sd->repair_target)
            	clif_item_repaireffect(sd->repair_target,item->nameid,0);
			sd->repair_target=NULL;
		}
	}
}

/*==========================================
 * Item Appraisal
 *------------------------------------------
 */
void skill_identify(struct map_session_data *sd,int idx)
{
	int flag=1;

	nullpo_retv(sd);

	if(idx >= 0 && idx < MAX_INVENTORY) {
		if(sd->status.inventory[idx].nameid > 0 && sd->status.inventory[idx].identify == 0 ){
			flag=0;
			sd->status.inventory[idx].identify=1;
		}
	}
	clif_item_identified(sd,idx,flag);
}

/*==========================================
 * Weapon Refine [Celest]
 *------------------------------------------
 */
void skill_weaponrefine(struct map_session_data *sd,int idx)
{
	int i = 0, ep = 0, per;
	int material[5] = { 0, 1010, 1011, 984, 984 };
	struct item *item;

	nullpo_retv(sd);

	if (idx >= 0 && idx < MAX_INVENTORY) {
		struct item_data *ditem = sd->inventory_data[idx];
		item = &sd->status.inventory[idx];

		if(item->nameid > 0 && ditem->type == 4) {
			if (item->refine >= sd->skilllv ||
				item->refine >= MAX_REFINE ||		// if it's no longer refineable
				ditem->flag.no_refine ||	// if the item isn't refinable
				(i = pc_search_inventory(sd, material [ditem->wlv])) < 0 ) { //fixed by Lupus (item pos can be = 0!)
				clif_skill_fail(sd,sd->skillid,0,0);
				return;
			}

			per = percentrefinery [ditem->wlv][(int)item->refine];
			per += (sd->status.job_level-50)/2; //Updated per the new kro descriptions. [Skotlex]

			if (per > rand() % 100) {
				item->refine++;
				pc_delitem(sd, i, 1, 0);
				if(item->equip) {
					ep = item->equip;
					pc_unequipitem(sd,idx,3);
				}
				clif_refine(sd->fd,sd,0,idx,item->refine);
				clif_delitem(sd,idx,1);
				clif_additem(sd,idx,1,0);
				if (ep)
					pc_equipitem(sd,idx,ep);
				clif_misceffect(&sd->bl,3);
				if(item->refine == MAX_REFINE && item->card[0] == 0x00ff && MakeDWord(item->card[2],item->card[3]) == sd->char_id){ // Fame point system [DracoRPG]
					switch(ditem->wlv){
						case 1:
							pc_addfame(sd,1); // Success to refine to +10 a lv1 weapon you forged = +1 fame point
							break;
						case 2:
							pc_addfame(sd,25); // Success to refine to +10 a lv2 weapon you forged = +25 fame point
							break;
						case 3:
							pc_addfame(sd,1000); // Success to refine to +10 a lv3 weapon you forged = +1000 fame point
							break;
					}
				}
			} else {
				pc_delitem(sd, i, 1, 0);
				item->refine = 0;
				if(item->equip)
					pc_unequipitem(sd,idx,3);
				clif_refine(sd->fd,sd,1,idx,item->refine);
				pc_delitem(sd,idx,1,0);
				clif_misceffect(&sd->bl,2);
				clif_emotion(&sd->bl, 23);
			}
		}
	}
}

/*==========================================
 * オ?トスペル
 *------------------------------------------
 */
int skill_autospell(struct map_session_data *sd,int skillid)
{
	int skilllv;
	int maxlv=1,lv;

	nullpo_retr(0, sd);

	skilllv = pc_checkskill(sd,SA_AUTOSPELL);
	if(skilllv <= 0) return 0;

	if(skillid==MG_NAPALMBEAT)	maxlv=3;
	else if(skillid==MG_COLDBOLT || skillid==MG_FIREBOLT || skillid==MG_LIGHTNINGBOLT){
		if(skilllv==2) maxlv=1;
		else if(skilllv==3) maxlv=2;
		else if(skilllv>=4) maxlv=3;
	}
	else if(skillid==MG_SOULSTRIKE){
		if(skilllv==5) maxlv=1;
		else if(skilllv==6) maxlv=2;
		else if(skilllv>=7) maxlv=3;
	}
	else if(skillid==MG_FIREBALL){
		if(skilllv==8) maxlv=1;
		else if(skilllv>=9) maxlv=2;
	}
	else if(skillid==MG_FROSTDIVER) maxlv=1;
	else return 0;

	if(maxlv > (lv=pc_checkskill(sd,skillid)))
		maxlv = lv;

	status_change_start(&sd->bl,SC_AUTOSPELL,skilllv,skillid,maxlv,0,	// val1:スキルID val2:使用?ﾅ大Lv
		skill_get_time(SA_AUTOSPELL,skilllv),0);// にしてみたけどbscriptが?曹ｫ易い????H
	return 0;
}

/*==========================================
 * ギャングスタ?パラダイス判定?�?(foreachinarea)
 *------------------------------------------
 */

static int skill_gangster_count(struct block_list *bl,va_list ap)
{
	int *c;
	struct map_session_data *sd;
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	c=va_arg(ap,int *);

	if(sd && c && pc_issit(sd) && pc_checkskill(sd,RG_GANGSTER) > 0)
		(*c)++;
	return 0;
}

static int skill_gangster_in(struct block_list *bl,va_list ap)
{
	struct map_session_data *sd;
 	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	if(sd && pc_issit(sd) && pc_checkskill(sd,RG_GANGSTER) > 0)
		sd->state.gangsterparadise=1;
	return 0;
}

static int skill_gangster_out(struct block_list *bl,va_list ap)
{
	struct map_session_data *sd;
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	if(sd && sd->state.gangsterparadise)
		sd->state.gangsterparadise=0;
	return 0;
}

int skill_gangsterparadise(struct map_session_data *sd ,int type)
{
	int range=1;
	int c=0;
	nullpo_retr(0, sd);

	if(pc_checkskill(sd,RG_GANGSTER) <= 0)
		return 0;

	if(type==1) {/* ?ﾀった時の?�? */
		map_foreachinarea(skill_gangster_count,sd->bl.m,
			sd->bl.x-range,sd->bl.y-range,
			sd->bl.x+range,sd->bl.y+range,BL_PC,&c);
		if(c > 1) {/*ギャングスタ??ｬ功したら自分にもギャングスタ???ｫ付?*/
			map_foreachinarea(skill_gangster_in,sd->bl.m,
				sd->bl.x-range,sd->bl.y-range,
				sd->bl.x+range,sd->bl.y+range,BL_PC);
			sd->state.gangsterparadise = 1;
		}
		return 0;
	}
	else if(type==0) {/* 立ち?繧ｪったときの?�? */
		map_foreachinarea(skill_gangster_count,sd->bl.m,
			sd->bl.x-range,sd->bl.y-range,
			sd->bl.x+range,sd->bl.y+range,BL_PC,&c);
		if(c < 2)
			map_foreachinarea(skill_gangster_out,sd->bl.m,
				sd->bl.x-range,sd->bl.y-range,
				sd->bl.x+range,sd->bl.y+range,BL_PC);
		sd->state.gangsterparadise = 0;
		return 0;
	}
	return 0;
}
/*==========================================
 * Taekwon TK_HPTIME and TK_SPTIME skills [Dralnu]
 *------------------------------------------
 */
static int skill_rest_count(struct block_list *bl,va_list ap)
{
	int *c_r;
	struct map_session_data *sd;
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	c_r=va_arg(ap,int *);

	if(sd && c_r && pc_issit(sd) && (pc_checkskill(sd,TK_HPTIME) > 0 || pc_checkskill(sd,TK_SPTIME) > 0  ))
		(*c_r)++;
	return 0;
}

static int skill_rest_in(struct block_list *bl,va_list ap)
{
	struct map_session_data *sd;
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	if(sd && pc_issit(sd) && (pc_checkskill(sd,TK_HPTIME) > 0 || pc_checkskill(sd,TK_SPTIME) > 0 )){
		sd->state.rest=1;
		status_calc_pc(sd,0);
	}		
	return 0;
}

static int skill_rest_out(struct block_list *bl,va_list ap)
{
	struct map_session_data *sd;
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	sd=(struct map_session_data*)bl;
	if(sd && sd->state.rest != 0){
		sd->state.rest=0;
		}		
	return 0;
}

int skill_rest(struct map_session_data *sd ,int type)
{
	int range=1;
	int c_r=0;
	nullpo_retr(0, sd);

	if(pc_checkskill(sd,TK_HPTIME) <= 0 && pc_checkskill(sd,TK_SPTIME) <= 0)
		return 0;

	if(type==1) {	//When you sit down
		map_foreachinarea(skill_rest_count,sd->bl.m,
			sd->bl.x-range,sd->bl.y-range,
			sd->bl.x+range,sd->bl.y+range,BL_PC,&c_r);
		if(c_r > 1) {
			map_foreachinarea(skill_rest_in,sd->bl.m,
				sd->bl.x-range,sd->bl.y-range,
				sd->bl.x+range,sd->bl.y+range,BL_PC);
			sd->state.rest = 1;
			status_calc_pc(sd,0);
		}
		return 0;
	}
	else if(type==0) {	//When you stand up
		map_foreachinarea(skill_rest_count,sd->bl.m,
			sd->bl.x-range,sd->bl.y-range,
			sd->bl.x+range,sd->bl.y+range,BL_PC,&c_r);
		if(c_r < 2)
			map_foreachinarea(skill_rest_out,sd->bl.m,
				sd->bl.x-range,sd->bl.y-range,
				sd->bl.x+range,sd->bl.y+range,BL_PC);
		sd->state.rest = 0;
		status_calc_pc(sd,0);
		return 0;
	}
	return 0;
}
/*==========================================
 * 寒いジョ?ク?スクリ?ム判定?�?(foreachinarea)
 *------------------------------------------
 */
int skill_frostjoke_scream(struct block_list *bl,va_list ap)
{
	struct block_list *src;
	int skillnum,skilllv;
	unsigned int tick;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, src=va_arg(ap,struct block_list*));

	skillnum=va_arg(ap,int);
	skilllv=va_arg(ap,int);
	if(skilllv <= 0) return 0;
	tick=va_arg(ap,unsigned int);

	if (src == bl ||	//自分には?かない
		bl->prev == NULL ||
		status_isdead(bl))
		return 0;
	if (bl->type == BL_PC) {
		struct map_session_data *sd = (struct map_session_data *)bl;
		if (sd && sd->status.option & OPTION_INVISIBLE && pc_isGM(sd) > 0)
			return 0;
	}
	//It has been reported that Scream/Joke works the same regardless of woe-setting. [Skotlex]
	if(battle_check_target(src,bl,BCT_ENEMY) > 0)
		skill_additional_effect(src,bl,skillnum,skilllv,BF_MISC,tick);
	else if(battle_check_target(src,bl,BCT_PARTY) > 0 && rand()%100 < 10)
		skill_additional_effect(src,bl,skillnum,skilllv,BF_MISC,tick);

	return 0;
}

/*==========================================
 * バジリカのセルを?ﾝ定する
 *------------------------------------------
 */
void skill_unitsetmapcell(struct skill_unit *src, int skill_num, int flag)
{
	int i,x,y,range = skill_get_unit_range(skill_num);
	int size = range*2+1;

	for (i=0;i<size*size;i++) {
		x = src->bl.x+(i%size-range);
		y = src->bl.y+(i/size-range);
		map_setcell(src->bl.m,x,y,flag);
	}
}

/*==========================================
 * Sets a map cell around the caster, according to the skill's range.
 *------------------------------------------
 */
void skill_setmapcell(struct block_list *src, int skill_num, int skill_lv, int flag)
{
	int i,x,y,range = skill_get_range2(src, skill_num, skill_lv);
	int size = range*2+1;

	for (i=0;i<size*size;i++) {
		x = src->x+(i%size-range);
		y = src->y+(i/size-range);
		map_setcell(src->m,x,y,flag);
	}
}
	
/*==========================================
 *
 *------------------------------------------
 */
int skill_attack_area(struct block_list *bl,va_list ap)
{
	struct block_list *src,*dsrc;
	int atk_type,skillid,skilllv,flag,type;
	unsigned int tick;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	atk_type = va_arg(ap,int);
	if((src=va_arg(ap,struct block_list*)) == NULL)
		return 0;
	if((dsrc=va_arg(ap,struct block_list*)) == NULL)
		return 0;
	skillid=va_arg(ap,int);
	skilllv=va_arg(ap,int);
	if(skillid > 0 && skilllv <= 0) return 0;	// celest
	tick=va_arg(ap,unsigned int);
	flag=va_arg(ap,int);
	type=va_arg(ap,int);

	if(battle_check_target(dsrc,bl,type) > 0)
		skill_attack(atk_type,src,dsrc,bl,skillid,skilllv,tick,flag);

	return 0;
}
/*==========================================
 *
 *------------------------------------------
 */
int skill_clear_element_field(struct block_list *bl)
{
	struct mob_data *md=NULL;
	struct map_session_data *sd=NULL;
	int i,max,skillid;

	nullpo_retr(0, bl);

	if (bl->type==BL_MOB) {
		max = MAX_MOBSKILLUNITGROUP;
		md = (struct mob_data *)bl;
	} else if(bl->type==BL_PC) {
		max = MAX_SKILLUNITGROUP;
		sd = (struct map_session_data *)bl;
	} else
		return 0;

	for (i=0;i<max;i++) {
		if(sd){
			skillid=sd->skillunit[i].skill_id;
			if(skillid==SA_DELUGE||skillid==SA_VOLCANO||skillid==SA_VIOLENTGALE||skillid==SA_LANDPROTECTOR)
				skill_delunitgroup(&sd->skillunit[i]);
		}else if(md){
			skillid=md->skillunit[i].skill_id;
			if(skillid==SA_DELUGE||skillid==SA_VOLCANO||skillid==SA_VIOLENTGALE||skillid==SA_LANDPROTECTOR)
				skill_delunitgroup(&md->skillunit[i]);
		}
	}
	return 0;
}

/*==========================================
 * Returns the first element field found [Skotlex]
 *------------------------------------------
 */
struct skill_unit_group *skill_locate_element_field(struct block_list *bl)
{
	struct mob_data *md=NULL;
	struct map_session_data *sd=NULL;
	int i,max,skillid;

	nullpo_retr(0, bl);

	if (bl->type==BL_MOB) {
		max = MAX_MOBSKILLUNITGROUP;
		md = (struct mob_data *)bl;
	} else if(bl->type==BL_PC) {
		max = MAX_SKILLUNITGROUP;
		sd = (struct map_session_data *)bl;
	} else
		return NULL;

	for (i=0;i<max;i++) {
		if(sd){
			skillid=sd->skillunit[i].skill_id;
			if(skillid==SA_DELUGE||skillid==SA_VOLCANO||skillid==SA_VIOLENTGALE||skillid==SA_LANDPROTECTOR)
				return &sd->skillunit[i];
		}else if(md){
			skillid=md->skillunit[i].skill_id;
			if(skillid==SA_DELUGE||skillid==SA_VOLCANO||skillid==SA_VIOLENTGALE||skillid==SA_LANDPROTECTOR)
				return &md->skillunit[i];
		}
	}
	return NULL;
}

// for graffiti cleaner [Valaris]
int skill_graffitiremover(struct block_list *bl, va_list ap)
{
	struct skill_unit *unit=NULL;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	if(bl->type!=BL_SKILL || (unit=(struct skill_unit *)bl) == NULL)
		return 0;

	if((unit->group) && (unit->group->unit_id == UNT_GRAFFITI))
		skill_delunit(unit);

	return 0;
}

int skill_greed(struct block_list *bl, va_list ap)
{
	struct block_list *src;
	struct map_session_data *sd=NULL;
	struct flooritem_data *fitem=NULL;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, src = va_arg(ap,struct block_list *));

	if(src->type == BL_PC && (sd=(struct map_session_data *)src) && bl->type==BL_ITEM && (fitem=(struct flooritem_data *)bl))
		pc_takeitem(sd, fitem);

	return 0;
}

/*==========================================
 * ランドプ�?テクタ?チェック(foreachinarea)
 *------------------------------------------
 */
int skill_landprotector(struct block_list *bl, va_list ap )
{
	int skillid;
	int *alive;
	struct skill_unit *unit;
	struct block_list *src;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	skillid = va_arg(ap,int);
	alive = va_arg(ap,int *);
	src = va_arg(ap,struct block_list *);
	
	if ((unit = (struct skill_unit *)bl) == NULL || unit->group == NULL)
		return 0;

	if (alive && skillid == SA_LANDPROTECTOR && unit->group->skill_id == SA_LANDPROTECTOR
		&& battle_check_target(bl, src, BCT_ENEMY) > 0)
	{	//Check for offensive Land Protector to delete both. [Skotlex]
		(*alive) = 0;
		skill_delunit(unit);
		return 0;
	}	
	if (skillid == SA_LANDPROTECTOR || 
		skillid == HW_GANBANTEIN)
		skill_delunit(unit);
	else if (alive && unit->group->skill_id == SA_LANDPROTECTOR)
		(*alive) = 0;
	else if  (alive && skillid == HP_BASILICA && unit->group->skill_id == HP_BASILICA)
		(*alive) = 0; //Basilica can't be placed on top of itself to avoid map-cell stacking problems. [Skotlex]
	return 0;
}

/*==========================================
 * variation of skill_landprotector
 *------------------------------------------
 */
int skill_ganbatein(struct block_list *bl, va_list ap )
{
	struct skill_unit *unit;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	if ((unit = (struct skill_unit *)bl) == NULL || unit->group == NULL)
		return 0;

	if (unit->group->skill_id == SA_LANDPROTECTOR)
		skill_delunit(unit);
	else skill_delunitgroup(unit->group);

	return 0;
}

/*==========================================
 * 指定範??でsrcに?して有?なタ?ゲットのblの?を?える(foreachinarea)
 *------------------------------------------
 */
int skill_count_target (struct block_list *bl, va_list ap)
{
	struct block_list *src;
	int *c;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);

	if ((src = va_arg(ap,struct block_list *)) == NULL)
		return 0;
	if ((c = va_arg(ap,int *)) == NULL)
		return 0;
	if (battle_check_target(src,bl,BCT_ENEMY) > 0)
		(*c)++;
	return 0;
}
/*==========================================
 * トラップ範??�?(foreachinarea)
 *------------------------------------------
 */
int skill_trap_splash (struct block_list *bl, va_list ap)
{
	struct block_list *src;
	int tick;
	int splash_count;
	struct skill_unit *unit;
	struct skill_unit_group *sg;
	struct block_list *ss;
	int i;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, src = va_arg(ap,struct block_list *));
	nullpo_retr(0, unit = (struct skill_unit *)src);
	nullpo_retr(0, sg = unit->group);
//	nullpo_retr(0, ss = map_id2bl(sg->src_id));
	if ((ss = map_id2bl(sg->src_id)) == NULL)
	{	//Temporal debug until this case is solved. [Skotlex]
		ShowDebug("skill_trap_splash: Trap's source (id: %d) not found!\n", sg->src_id);
		return 0;
	}

	tick = va_arg(ap,int);
	splash_count = va_arg(ap,int);

	if(battle_check_target(src,bl,BCT_ENEMY) > 0){
		switch(sg->unit_id){
			case UNT_SHOCKWAVE:
			case UNT_SANDMAN:
			case UNT_FLASHER:        
				skill_additional_effect(ss,bl,sg->skill_id,sg->skill_lv,BF_MISC,tick);
				break;
			case UNT_BLASTMINE:
			case UNT_CLAYMORETRAP:
				for(i=0;i<splash_count;i++){
					skill_attack(BF_MISC,ss,src,bl,sg->skill_id,sg->skill_lv,tick,(sg->val2)?0x0500:0);
				}
				break;
			case UNT_FREEZINGTRAP:
					skill_attack(BF_WEAPON,	ss,src,bl,sg->skill_id,sg->skill_lv,tick,(sg->val2)?0x0500:0);
				break;
			default:
				break;
		}
	}

	return 0;
}

/*==========================================
 * ステ?タス異?�?I了
 *------------------------------------------
 */
int skill_enchant_elemental_end (struct block_list *bl, int type)
{
	struct status_change *sc_data;

	nullpo_retr(0, bl);
	nullpo_retr(0, sc_data = status_get_sc_data(bl));

	if (type != SC_ENCPOISON && sc_data[SC_ENCPOISON].timer != -1)			/* エンチャントポイズン解?� */
		status_change_end(bl, SC_ENCPOISON, -1);
	if (type != SC_ASPERSIO && sc_data[SC_ASPERSIO].timer != -1)			/* アスペルシオ解?� */
		status_change_end(bl, SC_ASPERSIO, -1);
	if (type != SC_FIREWEAPON && sc_data[SC_FIREWEAPON].timer != -1)	/* フレイムランチャ解?� */
		status_change_end(bl, SC_FIREWEAPON, -1);
	if (type != SC_WATERWEAPON && sc_data[SC_WATERWEAPON].timer != -1)		/* フ�?ストウェポン解?� */
		status_change_end(bl, SC_WATERWEAPON, -1);
	if (type != SC_WINDWEAPON && sc_data[SC_WINDWEAPON].timer != -1)	/* ライトニング�??ダ?解?� */
		status_change_end(bl, SC_WINDWEAPON, -1);
	if (type != SC_EARTHWEAPON && sc_data[SC_EARTHWEAPON].timer != -1)	/* サイスミックウェポン解?� */
		status_change_end(bl, SC_EARTHWEAPON, -1);
	if (type != SC_SHADOWWEAPON && sc_data[SC_SHADOWWEAPON].timer != -1)
		status_change_end(bl, SC_SHADOWWEAPON, -1);
	if (type != SC_GHOSTWEAPON && sc_data[SC_GHOSTWEAPON].timer != -1)
		status_change_end(bl, SC_GHOSTWEAPON, -1);
	return 0;
}

/* ク�??キング??ｸ?i周りに移動不可能地?があるか?j */
int skill_check_cloaking(struct block_list *bl)
{
	struct map_session_data *sd = NULL;
	static int dx[] = { 0, 1, 0, -1, -1,  1, 1, -1}; //optimized by Lupus
	static int dy[] = {-1, 0, 1,  0, -1, -1, 1,  1};
	int end = 1,i;

	nullpo_retr(1, bl);

	if (bl->type == BL_PC) {
		nullpo_retr(1, sd = (struct map_session_data *)bl);
	}
	
	if ((bl->type == BL_PC && battle_config.pc_cloak_check_type&1) ||
		(bl->type != BL_PC && battle_config.monster_cloak_check_type&1))
		{	//Check for walls.
			for (i = 0; i < 8; i++)
			if (map_getcell(bl->m, bl->x+dx[i], bl->y+dy[i], CELL_CHKNOPASS))
			{
				end = 0;
				break;
			}
		} else
			end = 0; //No wall check.
			
	if(end){
		if ((sd && pc_checkskill(sd,AS_CLOAKING)<3) || bl->type == BL_MOB) {
			status_change_end(bl, SC_CLOAKING, -1);
		}
		else if (sd && sd->sc_data[SC_CLOAKING].val3 != 130) {
			status_quick_recalc_speed (sd, AS_CLOAKING, 130, 1);
		}
	}
	else {
		if (sd && sd->sc_data[SC_CLOAKING].val3 != 103) {
			status_quick_recalc_speed (sd, AS_CLOAKING, 103, 1);
		}
	}

	return end;
}

/*
 *----------------------------------------------------------------------------
 * スキルユニット
 *----------------------------------------------------------------------------
 */

/*==========================================
 * 演奏/ダンスをやめる
 * flag 1で?�奏中なら相方にユニットを任せる
 *
 *------------------------------------------
 */
void skill_stop_dancing(struct block_list *src)
{
	struct status_change* sc_data;
	struct skill_unit_group* group;
	struct map_session_data* dsd = NULL;

	nullpo_retv(src);
	nullpo_retv(sc_data = status_get_sc_data(src));

	if(sc_data[SC_DANCING].timer == -1)
		return;
	
	group = (struct skill_unit_group *)sc_data[SC_DANCING].val2;
	sc_data[SC_DANCING].val2 = 0;
	
	if (sc_data[SC_DANCING].val4)
	{
		if (sc_data[SC_DANCING].val4 != BCT_SELF)
			dsd = map_id2sd(sc_data[SC_DANCING].val4);
		sc_data[SC_DANCING].val4 = 0;
	}

	if (group)
		skill_delunitgroup(group);
		
	if (dsd)
	{
		dsd->sc_data[SC_DANCING].val4 = dsd->sc_data[SC_DANCING].val2 = 0;
		status_change_end(&dsd->bl, SC_DANCING, -1);
	}
	status_change_end(src, SC_DANCING, -1);
}

/*==========================================
 * スキルユニット?炎�化
 *------------------------------------------
 */
struct skill_unit *skill_initunit(struct skill_unit_group *group,int idx,int x,int y)
{
	struct skill_unit *unit;

	nullpo_retr(NULL, group);
	nullpo_retr(NULL, unit=&group->unit[idx]);

	if(!unit->alive)
		group->alive_count++;

	unit->bl.id=map_addobject(&unit->bl);
	unit->bl.type=BL_SKILL;
	unit->bl.m=group->map;
	unit->bl.x=x;
	unit->bl.y=y;
	unit->group=group;
	unit->val1=unit->val2=0;
	unit->alive=1;

	map_addblock(&unit->bl);
	clif_skill_setunit(unit);

	switch (group->skill_id) {
	case AL_PNEUMA:
		skill_unitsetmapcell(unit,AL_PNEUMA,CELL_SETPNEUMA);
		break;
	case MG_SAFETYWALL:
		skill_unitsetmapcell(unit,MG_SAFETYWALL,CELL_SETSAFETYWALL);
		break;
	case SA_LANDPROTECTOR:
		skill_unitsetmapcell(unit,SA_LANDPROTECTOR,CELL_SETLANDPROTECTOR);
		break;
	case HP_BASILICA:
		skill_unitsetmapcell(unit,HP_BASILICA,CELL_SETBASILICA);
		break;
	}
	return unit;
}

/*==========================================
 * スキルユニット?�?�
 *------------------------------------------
 */
int skill_delunit(struct skill_unit *unit)
{
	struct skill_unit_group *group;

	nullpo_retr(0, unit);
	if(!unit->alive)
		return 0;
	nullpo_retr(0, group=unit->group);

	/* onlimitイベント呼び?oし */
	skill_unit_onlimit( unit,gettick() );

	/* onoutイベント呼び?oし */
	if (!unit->range) {
		map_foreachincell(skill_unit_effect,unit->bl.m,
			unit->bl.x,unit->bl.y,0,&unit->bl,gettick(),4);
	}

	switch (group->skill_id) {
	case AL_PNEUMA:
		skill_unitsetmapcell(unit,AL_PNEUMA,CELL_CLRPNEUMA);
		break;
	case MG_SAFETYWALL:
		skill_unitsetmapcell(unit,MG_SAFETYWALL,CELL_CLRSAFETYWALL);
		break;
	case SA_LANDPROTECTOR:
		skill_unitsetmapcell(unit,SA_LANDPROTECTOR,CELL_CLRLANDPROTECTOR);
		break;
	case HP_BASILICA:
		skill_unitsetmapcell(unit,HP_BASILICA,CELL_CLRBASILICA);
		break;
	}

	clif_skill_delunit(unit);

	unit->group=NULL;
	unit->alive=0;
	map_delobjectnofree(unit->bl.id);
	if(--group->alive_count==0)
		skill_delunitgroup(group);

	return 0;
}
/*==========================================
 * スキルユニットグル?プ?炎�化
 *------------------------------------------
 */
static int skill_unit_group_newid = MAX_SKILL_DB;
struct skill_unit_group *skill_initunitgroup(struct block_list *src,
	int count,int skillid,int skilllv,int unit_id)
{
	int i;
	struct skill_unit_group *group=NULL, *list=NULL;
	int maxsug=0;

	if(skilllv <= 0) return 0;

	nullpo_retr(NULL, src);

	if(src->type==BL_PC){
		list=((struct map_session_data *)src)->skillunit;
		maxsug=MAX_SKILLUNITGROUP;
	}else if(src->type==BL_MOB){
		list=((struct mob_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	}else if(src->type==BL_PET){
		list=((struct pet_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	}
	if(list){
		for(i=0;i<maxsug;i++)	/* 空いているもの??� */
			if(list[i].group_id==0){
				group=&list[i];
				break;
			}

		if(group==NULL){	/* 空いてないので古いもの??� */
			int j=0;
			unsigned maxdiff=0,x,tick=gettick();
			for(i=0;i<maxsug;i++)
				if((x=DIFF_TICK(tick,list[i].tick))>maxdiff){
					maxdiff=x;
					j=i;
				}
			skill_delunitgroup(&list[j]);
			group=&list[j];
		}
	}

	if(group==NULL){
		ShowFatalError("skill_initunitgroup: error unit group !\n");
		exit(1);
	}

	group->src_id=src->id;
	group->party_id=status_get_party_id(src);
	group->guild_id=status_get_guild_id(src);
	group->group_id=skill_unit_group_newid++;
	if(skill_unit_group_newid<=0)
		skill_unit_group_newid = MAX_SKILL_DB;
	group->unit=(struct skill_unit *)aCalloc(count,sizeof(struct skill_unit));
	group->unit_count=count;
	group->val1=group->val2=0;
	group->skill_id=skillid;
	group->skill_lv=skilllv;
	group->unit_id=unit_id;
	group->map=src->m;
	group->limit=10000;
	group->interval=1000;
	group->tick=gettick();
	if (skillid == PR_SANCTUARY) //Sanctuary starts healing +1500ms after casted. [Skotlex]
		group->tick += 1500;
	group->valstr=NULL;

	i = skill_get_unit_flag(skillid); //Reuse for faster access from here on. [Skotlex]
	if (i&UF_DANCE) {
		struct map_session_data *sd = NULL;
		if(src->type==BL_PC && (sd=(struct map_session_data *)src) ){
			sd->skillid_dance=skillid;
			sd->skilllv_dance=skilllv;
		}
		status_change_start(src,SC_DANCING,skillid,(int)group,0,(i&UF_ENSEMBLE?BCT_SELF:0),skill_get_time(skillid,skilllv)+1000,0);
		//?�奏スキルは相方をダンス?�態にする
		if (sd && i&UF_ENSEMBLE &&
			battle_config.player_skill_partner_check) {
				skill_check_pc_partner(sd, skillid, &skilllv, 1, 1);
		}
	}
	return group;
}

/*==========================================
 * スキルユニットグル?プ?�?�
 *------------------------------------------
 */
int skill_delunitgroup(struct skill_unit_group *group)
{
	struct block_list *src;
	int i;

	nullpo_retr(0, group);
	if(group->unit_count<=0)
		return 0;

	src=map_id2bl(group->src_id);
	//ダンススキルはダンス?�態を解?怩ｷる
	if(src) {
		if (skill_get_unit_flag(group->skill_id)&UF_DANCE)
		{
			struct status_change* sc_data = status_get_sc_data(src);
			if (sc_data && sc_data[SC_DANCING].timer != -1)
			{
				sc_data[SC_DANCING].val2 = 0 ; //This prevents status_change_end attempting to redelete the group. [Skotlex]
				status_change_end(src,SC_DANCING,-1);
			}
		}

		if (group->unit_id == UNT_GOSPEL) { //Clear Gospel [Skotlex]
			struct status_change *sc_data = status_get_sc_data(src);
			if(sc_data && sc_data[SC_GOSPEL].timer != -1) {
				sc_data[SC_GOSPEL].val3 = 0; //Remove reference to this group. [Skotlex]
				status_change_end(src,SC_GOSPEL,-1);
			}
		}
	}

	group->alive_count=0;
	if(group->unit!=NULL){
		for(i=0;i<group->unit_count;i++)
			if(group->unit[i].alive)
				skill_delunit(&group->unit[i]);
	}
	if(group->valstr!=NULL){
		//Supposedly Free remembers the size of the original Calloc/Malloc, so this should be safe [Skotlex]
		aFree(group->valstr);
		group->valstr=NULL;
	}

	map_freeblock((struct block_list*)group->unit);	/* aFree()の替わり */
	group->unit=NULL;
	group->src_id=0;
	group->group_id=0;
	group->unit_count=0;
	return 0;
}

/*==========================================
 * スキルユニットグル?プ全?�?�
 *------------------------------------------
 */
int skill_clear_unitgroup(struct block_list *src)
{
	struct skill_unit_group *group=NULL;
	int maxsug=0;

	nullpo_retr(0, src);

	if(src->type==BL_PC){
		group=((struct map_session_data *)src)->skillunit;
		maxsug=MAX_SKILLUNITGROUP;
	} else if(src->type==BL_MOB){
		group=((struct mob_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	} else if(src->type==BL_PET){ // [Valaris]
		group=((struct pet_data *)src)->skillunit;
		maxsug=MAX_MOBSKILLUNITGROUP;
	} else
		return 0;
	if(group){
		int i;
		for(i=0;i<maxsug;i++)
			if(group[i].group_id>0 && group[i].src_id == src->id)
				skill_delunitgroup(&group[i]);
	}
	return 0;
}

/*==========================================
 * スキルユニットグル?プの被影響tick??�
 *------------------------------------------
 */
struct skill_unit_group_tickset *skill_unitgrouptickset_search(
	struct block_list *bl,struct skill_unit_group *group,int tick)
{
	int i,j=-1,k,s,id;
	struct skill_unit_group_tickset *set;

	nullpo_retr(0, bl);
	if (group->interval==-1)
		return NULL;

	if (bl->type == BL_PC)
		set = ((struct map_session_data *)bl)->skillunittick;
	else if (bl->type == BL_MOB)
		set = ((struct mob_data *)bl)->skillunittick;
	else
		return 0;

	if (skill_get_unit_flag(group->skill_id)&UF_NOOVERLAP)
		id = s = group->skill_id;
	else
		id = s = group->group_id;

	for (i=0; i<MAX_SKILLUNITGROUPTICKSET; i++) {
		k = (i+s) % MAX_SKILLUNITGROUPTICKSET;
		if (set[k].id == id)
			return &set[k];
		else if (j==-1 && (DIFF_TICK(tick,set[k].tick)>0 || set[k].id==0))
			j=k;
	}

	if (j == -1) {
		if(battle_config.error_log) {
			ShowWarning ("skill_unitgrouptickset_search: tickset is full\n");
		}
		j = id % MAX_SKILLUNITGROUPTICKSET;
	}

	set[j].id = id;
	set[j].tick = tick;
	return &set[j];
}

/*==========================================
 * スキルユニットタイマ??動?�?用(foreachinarea)
 *------------------------------------------
 */
int skill_unit_timer_sub_onplace( struct block_list *bl, va_list ap )
{
	struct skill_unit *unit;
	struct skill_unit_group *group;
	unsigned int tick;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	unit = va_arg(ap,struct skill_unit *);
	tick = va_arg(ap,unsigned int);

	if (!unit->alive || bl->prev==NULL)
		return 0;

	nullpo_retr(0, group=unit->group);

	if (map_getcell(bl->m, bl->x, bl->y, CELL_CHKLANDPROTECTOR))
		return 0; //AoE skills are ineffective. [Skotlex]

	if (!skill_unit_checktarget(group->skill_id, bl))
		return 0;
	
	if (battle_check_target(&unit->bl,bl,group->target_flag)<=0)
		return 0;

	skill_unit_onplace_timer(unit,bl,tick);

	return 0;
}

/*==========================================
 * スキルユニットタイマ??�?用(foreachobject)
 *------------------------------------------
 */
int skill_unit_timer_sub( struct block_list *bl, va_list ap )
{
	struct skill_unit *unit;
	struct skill_unit_group *group;
	int range;
	unsigned int tick;

	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, unit=(struct skill_unit *)bl);
	tick=va_arg(ap,unsigned int);

	if(!unit->alive)
		return 0;
	group=unit->group;

	nullpo_retr(0, group);
	range = unit->range;

	/* onplace_timerイベント呼び?oし */
	if (range>=0 && group->interval!=-1) {
		map_foreachinarea(skill_unit_timer_sub_onplace, bl->m,
			bl->x-range,bl->y-range,bl->x+range,bl->y+range,0,bl,tick);
		if (!unit->alive)
			return 0;
		// マグヌスは発動したユニットは?�?怩ｷる
		if (group->skill_id==PR_MAGNUS && unit->val2) {
			skill_delunit(unit);
			return 0;
		}
	}
	/* 時間?ﾘれ?�?� */
	if((DIFF_TICK(tick,group->tick)>=group->limit || DIFF_TICK(tick,group->tick)>=unit->limit)){
		switch(group->unit_id){
			case UNT_BLASTMINE:
				group->unit_id = UNT_USED_TRAPS;
				clif_changelook(bl,LOOK_BASE,group->unit_id);
				group->limit=DIFF_TICK(tick+1500,group->tick);
				unit->limit=DIFF_TICK(tick+1500,group->tick);
				break;
			case UNT_SKIDTRAP:
			case UNT_ANKLESNARE:
			case UNT_LANDMINE:
			case UNT_SHOCKWAVE:
			case UNT_SANDMAN:
			case UNT_FLASHER:
			case UNT_FREEZINGTRAP:
			case UNT_CLAYMORETRAP:
			case UNT_TALKIEBOX:
				{
					struct block_list *src=map_id2bl(group->src_id);
					if(group->unit_id == UNT_ANKLESNARE && group->val2);
					else{
						if(src && src->type==BL_PC && group->val3 != BD_INTOABYSS)
						{	//Avoid generating trap items when it did not cost to create them. [Skotlex]
							struct item item_tmp;
							memset(&item_tmp,0,sizeof(item_tmp));
							item_tmp.nameid=1065;
							item_tmp.identify=1;
							map_addflooritem(&item_tmp,1,bl->m,bl->x,bl->y,NULL,NULL,NULL,0);	// ?返還
						}
					}
					skill_delunit(unit);
				}
				break;

			case 0xc1:
			case 0xc2:
			case 0xc3:
			case 0xc4:
				{
					struct block_list *src=map_id2bl(group->src_id);
					if (src)
						group->tick = tick;
				}
				break;

			default:
				skill_delunit(unit);
		}
	}

	if(group->unit_id == UNT_ICEWALL) {
		unit->val1 -= 5;
		if(unit->val1 <= 0 && unit->limit + group->tick > tick + 700)
			unit->limit = DIFF_TICK(tick+700,group->tick);
	}

	return 0;
}
/*==========================================
 * スキルユニットタイマ??�?
 *------------------------------------------
 */
int skill_unit_timer( int tid,unsigned int tick,int id,int data)
{
	map_freeblock_lock();

	map_foreachobject( skill_unit_timer_sub, BL_SKILL, tick );

	map_freeblock_unlock();

	return 0;
}

/*==========================================
 * スキルユニット移動時?�?用(foreachinarea)
 *------------------------------------------
 */
int skill_unit_move_sub( struct block_list *bl, va_list ap )
{
	struct skill_unit *unit = (struct skill_unit *)bl;
	struct block_list *target;
	unsigned int tick,flag,result;
	int skill_id;
	
	nullpo_retr(0, bl);
	nullpo_retr(0, ap);
	nullpo_retr(0, target=va_arg(ap,struct block_list*));
	tick = va_arg(ap,unsigned int);
	flag = va_arg(ap,int);
	
	nullpo_retr(0, unit->group);
	skill_id = unit->group->skill_id; //Necessary in case the group is deleted after calling on_place/on_out [Skotlex]
	

	if (unit->group->interval!=-1 && 
		!(skill_get_unit_flag(skill_id)&UF_DUALMODE)) //Skills in dual mode have to trigger both. [Skotlex]
		return 0;

	if (!skill_unit_checktarget(skill_id, target))
		return 0;
	
	if (!unit->alive || target->prev==NULL)
		return 0;

	if (flag&1)
	{
		result = skill_unit_onplace(unit,target,tick);
		if (flag&2 && result)
		{	//Clear skill ids we have stored in onout.
			int i;
			for(i=0; i<8 && skill_unit_temp[i]!=result; i++);
			if (i<8)
				skill_unit_temp[i] = 0;
		}
	}
	else
	{
		result = skill_unit_onout(unit,target,tick);
		if (flag&2 && skill_unit_index < 7 && result) //Store this unit id.
			skill_unit_temp[skill_unit_index++] = result;
	}
	if (flag&4)
		skill_unit_onleft(skill_id,target,tick);
	return 1;
}

/*==========================================
 * Invoked when a char has moved and unit cells must be invoked (onplace, onout, onleft)
 * Flag values:
 * flag&1: invoke skill_unit_onplace (otherwise invoke skill_unit_onout)
 * flag&2: this function is being invoked twice as a bl moves, store in memory the affected
 * units to figure out when they have left a group.
 * flag&4: Force a onleft event (triggered when the bl is killed, for example)
 *------------------------------------------
 */
int skill_unit_move(struct block_list *bl,unsigned int tick,int flag)
{
	nullpo_retr(0, bl);

	if(bl->prev==NULL )
		return 0;

	if (flag&2 && !(flag&1))
	{	//Onout, clear data
		memset (&skill_unit_temp,0,sizeof(skill_unit_temp));
		skill_unit_index=0;
	}
		
	map_foreachincell(skill_unit_move_sub,
			bl->m,bl->x,bl->y,BL_SKILL,bl,tick,flag);

	if (flag&2 && flag&1)
	{ //Onplace, check any skill units you have left.
		int i;
		for (i=0; i< 8 && skill_unit_temp[i]>0; i++)
			skill_unit_onleft(skill_unit_temp[i], bl, tick);
	}

	return 0;
}

/*==========================================
 * スキルユニット自?の移動時?�?
 * 引?はグル?プと移動量
 *------------------------------------------
 */
int skill_unit_move_unit_group( struct skill_unit_group *group, int m,int dx,int dy)
{
	int i,j;
	int tick = gettick();
	int *m_flag;
	struct skill_unit *unit1;
	struct skill_unit *unit2;

	nullpo_retr(0, group);
	if (group->unit_count<=0)
		return 0;
	if (group->unit==NULL)
		return 0;

	i = skill_get_unit_flag(group->skill_id); //Check the flag...
	if (!(
		(i&UF_DANCE && !(i&UF_ENSEMBLE)) || //Only non ensemble dances and traps can be moved.
		skill_get_inf2(group->skill_id)&INF2_TRAP
	))
		return 0;
		
	m_flag = (int *) aMalloc(sizeof(int)*group->unit_count);
	memset(m_flag,0,sizeof(int)*group->unit_count);// 移動フラグ
	//    m_flag
	//		0: Neither of the following (skill_unit_onplace & skill_unit_onout are needed)
	//		1: Unit will move to a slot that had another unit of the same group (skill_unit_onplace not needed)
	//		2: Another unit from same group will end up positioned on this unit (skill_unit_onout not needed)
	//		3: Both 1+2.
	for(i=0;i<group->unit_count;i++){
		unit1=&group->unit[i];
		if (!unit1->alive || unit1->bl.m!=m)
			continue;
		for(j=0;j<group->unit_count;j++){
			unit2=&group->unit[j];
			if (!unit2->alive)
				continue;
			if (unit1->bl.x+dx==unit2->bl.x && unit1->bl.y+dy==unit2->bl.y){
				m_flag[i] |= 0x1;
			}
			if (unit1->bl.x-dx==unit2->bl.x && unit1->bl.y-dy==unit2->bl.y){
				m_flag[i] |= 0x2;
			}
		}
	}
	j = 0;
	for (i=0;i<group->unit_count;i++) {
		unit1=&group->unit[i];
		if (!unit1->alive)
			continue;
		if (!(m_flag[i]&0x2)) {
			map_foreachincell(skill_unit_effect,unit1->bl.m,
				unit1->bl.x,unit1->bl.y,0,&unit1->bl,tick,4);
		}
		//Move Cell using "smart" criteria (avoid useless moving around)
		switch(m_flag[i])
		{
			case 0:
			//Cell moves independently, safely move it.
				map_delblock(&unit1->bl);
				unit1->bl.m = m;
				unit1->bl.x += dx;
				unit1->bl.y += dy;
				map_addblock(&unit1->bl);
				clif_skill_setunit(unit1);
				break;
			case 1:
			//Cell moves unto another cell, look for a replacement cell that won't collide
			//and has no cell moving into it (flag == 2)
				for(;j<group->unit_count;j++)
				{
					if(m_flag[j]!=2 || !group->unit[j].alive)
						continue;
					//Move to where this cell would had moved.
					unit2 = &group->unit[j];
					map_delblock(&unit1->bl);
					unit1->bl.m = m;
					unit1->bl.x = unit2->bl.x + dx;
					unit1->bl.y = unit2->bl.y + dy;
					map_addblock(&unit1->bl);
					clif_skill_setunit(unit1);
					j++; //Skip this cell as we have used it.
					break;
				}
				break;
			case 2:
			case 3:
				break; //Don't move the cell as a cell will end on this tile anyway.
		}
		if (!(m_flag[i]&2)) { //We only moved the cell in 0-1
			map_foreachincell(skill_unit_effect,unit1->bl.m,
				unit1->bl.x,unit1->bl.y,0,&unit1->bl,tick,1);
		}
	}
	aFree(m_flag);
	return 0;
}

/*----------------------------------------------------------------------------
 * アイテム?�?ｬ
 *----------------------------------------------------------------------------
 */

/*==========================================
 * アイテム?�?ｬ可能判定
 *------------------------------------------
 */
int skill_can_produce_mix( struct map_session_data *sd, int nameid, int trigger )
{
	int i,j;

	nullpo_retr(0, sd);

	if(nameid<=0)
		return 0;

	for(i=0;i<MAX_SKILL_PRODUCE_DB;i++){
		if(skill_produce_db[i].nameid == nameid )
			break;
	}
	if( i >= MAX_SKILL_PRODUCE_DB )	/* デ?タベ?スにない */
		return 0;

	if(trigger>=0){
		if(trigger>20) { // Non-weapon, non-food item (itemlv must match)
			if(skill_produce_db[i].itemlv!=trigger)
				return 0;
		} else if(trigger>10) { // Food (itemlv must be higher or equal)
			if(skill_produce_db[i].itemlv<=10 || skill_produce_db[i].itemlv>trigger)
				return 0;
		} else { // Weapon (itemlv must be higher or equal)
			if(skill_produce_db[i].itemlv>trigger)
				return 0;
		}
	}
	if( (j=skill_produce_db[i].req_skill)>0 && pc_checkskill(sd,j)<=0 )
		return 0;		/* スキルが足りない */

	for(j=0;j<MAX_PRODUCE_RESOURCE;j++){
		int id,x,y;
		if( (id=skill_produce_db[i].mat_id[j]) <= 0 )	/* これ以?繧ﾍ?ﾞ料要らない */
			continue;
		if(skill_produce_db[i].mat_amount[j] <= 0) {
			if(pc_search_inventory(sd,id) < 0)
				return 0;
		}
		else {
			for(y=0,x=0;y<MAX_INVENTORY;y++)
				if( sd->status.inventory[y].nameid == id )
					x+=sd->status.inventory[y].amount;
			if(x<skill_produce_db[i].mat_amount[j]) /* アイテムが足りない */
				return 0;
		}
	}
	return i+1;
}

/*==========================================
 * アイテム?�?ｬ可能判定
 *------------------------------------------
 */
int skill_produce_mix( struct map_session_data *sd,
	int nameid, int slot1, int slot2, int slot3 )
{
	int slot[3];
	int i,sc,ele,idx,equip,wlv,make_per,flag;

	nullpo_retr(0, sd);

	if( !(idx=skill_can_produce_mix(sd,nameid,-1)) )	/* ?�?不足 */
		return 0;
	idx--;
	slot[0]=slot1;
	slot[1]=slot2;
	slot[2]=slot3;

	/* 埋め?み?�? */
	for(i=0,sc=0,ele=0;i<3;i++){
		int j;
		if( slot[i]<=0 )
			continue;
		j = pc_search_inventory(sd,slot[i]);
		if(j < 0)	/* 不?ｳパケット(アイテム存?ﾝ)チェック */
			continue;
		if(slot[i]==1000){	/* Star Crumb */
			pc_delitem(sd,j,1,1);
			sc++;
		}
		if(slot[i]>=994 && slot[i]<=997 && ele==0){	/* Flame Heart . . . Great Nature */
			static const int ele_table[4]={3,1,4,2};
			pc_delitem(sd,j,1,1);
			ele=ele_table[slot[i]-994];
		}
	}

	/* ?ﾞ料?ﾁ費 */
	for(i=0;i<MAX_PRODUCE_RESOURCE;i++){
		int j,id,x;
		if( (id=skill_produce_db[idx].mat_id[i]) <= 0 )
			continue;
		x=skill_produce_db[idx].mat_amount[i];	/* 必要な個? */
		do{	/* ２つ以?繧ﾌインデックスにまたがっているかもしれない */
			int y=0;
			j = pc_search_inventory(sd,id);

			if(j >= 0){
				y = sd->status.inventory[j].amount;
				if(y>x)y=x;	/* 足りている */
				pc_delitem(sd,j,y,0);
			}else {
				if(battle_config.error_log)
					ShowError("skill_produce_mix: material item error\n");
			}

			x-=y;	/* まだ足りない個?を計算 */
		}while( j>=0 && x>0 );	/* ?ﾞ料を?ﾁ費するか?Aエラ?になるまで繰り返す */
	}

	if((equip=itemdb_isequip(nameid)))
		wlv = itemdb_wlv(nameid);
	if(!equip) {
		switch(skill_produce_db[idx].req_skill){
			case BS_IRON:
			case BS_STEEL:
			case BS_ENCHANTEDSTONE:
				{ // Ores & Metals Refining - skill bonuses are straight from kRO website [DracoRPG]
				int skill = pc_checkskill(sd,skill_produce_db[idx].req_skill);
				make_per = sd->status.job_level*20 + sd->paramc[4]*10 + sd->paramc[5]*10; //Base chance
				switch(nameid){
					case 998: // Iron
						make_per += 4000+skill*500; // Temper Iron bonus: +26/+32/+38/+44/+50
						break;
					case 999: // Steel
						make_per += 3000+skill*500; // Temper Steel bonus: +35/+40/+45/+50/+55
						break;
					case 1000: //Star Crumb
						make_per = 100000; // Star Crumbs are 100% success crafting rate? (made 1000% so it succeeds even after penalties) [Skotlex]
						break;
					default: // Enchanted Stones
						make_per += 1000+skill*500; // Enchantedstone Craft bonus: +15/+20/+25/+30/+35
					break;
				}
				break;
			case AM_PHARMACY: // Potion Preparation - reviewed with the help of various Ragnainfo sources [DracoRPG]
				make_per = pc_checkskill(sd,AM_LEARNINGPOTION)*100
					+ pc_checkskill(sd,AM_PHARMACY)*300 + sd->status.job_level*20
					+ sd->paramc[3]*5 + sd->paramc[4]*10+sd->paramc[5]*10;
				switch(nameid){
					case 501: // Red Potion
					case 503: // Yellow Potion
					case 504: // White Potion
					case 605: // Anodyne
					case 606: // Aloevera
						make_per += 2000;
						break;
					case 505: // Blue Potion
						make_per -= 500;
						break;
					case 545: // Condensed Red Potion
					case 546: // Condensed Yellow Potion
					case 547: // Condensed White Potion
						make_per -= 1000;
					    break;
				 	case 970: // Alcohol
						make_per += 1000;
						break;
					case 7139: // Glistening Coat
						make_per -= 1000;
						break;
					case 7135: // Bottle Grenade
					case 7136: // Acid Bottle
					case 7137: // Plant Bottle
					case 7138: // Marine Sphere Bottle
					default:
						break;
				}
				if(battle_config.pp_rate != 100)
					make_per = make_per * battle_config.pp_rate / 100;
				break;
			case SA_CREATECON: // Elemental Converter Creation - skill bonuses are from kRO [DracoRPG]
				make_per = sd->status.job_level*20 + sd->paramc[3]*10 + sd->paramc[4]*10;
				switch(nameid){
					case 12114:
						make_per += pc_checkskill(sd,SA_FLAMELAUNCHER)*1000-500;
						break;
					case 12115:
						make_per += pc_checkskill(sd,SA_FROSTWEAPON)*1000-500;
						break;
					case 12116:
						make_per += pc_checkskill(sd,SA_SEISMICWEAPON)*1000-500;
						break;
					case 12117:
						make_per += pc_checkskill(sd,SA_LIGHTNINGLOADER)*1000-500;
						break;
				}
				break;
			default:
				make_per = 5000;
				break;
			}
		}
	} else { // Weapon Forging - skill bonuses are straight from kRO website, other things from a jRO calculator [DracoRPG]
		make_per = 5000 + sd->status.job_level*20 + sd->paramc[4]*10 + sd->paramc[5]*10; // Base
		make_per += pc_checkskill(sd,skill_produce_db[idx].req_skill)*500; // Smithing skills bonus: +5/+10/+15
		make_per += pc_checkskill(sd,BS_WEAPONRESEARCH)*100 +((wlv >= 3)? pc_checkskill(sd,BS_ORIDEOCON)*100:0); // Weaponry Research bonus: +1/+2/+3/+4/+5/+6/+7/+8/+9/+10, Oridecon Research bonus (custom): +1/+2/+3/+4/+5
		make_per -= (ele?2000:0) + sc*1500 + (wlv>1?wlv*1000:0); // Element Stone: -20%, Star Crumb: -15% each, Weapon level malus: -0/-20/-30
		if(pc_search_inventory(sd,989) > 0) make_per+= 1000; // Emperium Anvil: +10
		else if(pc_search_inventory(sd,988) > 0) make_per+= 500; // Golden Anvil: +5
		else if(pc_search_inventory(sd,987) > 0) make_per+= 300; // Oridecon Anvil: +3
		else if(pc_search_inventory(sd,986) > 0) make_per+= 0; // Anvil: +0?
		if(battle_config.wp_rate != 100)
			make_per = make_per * battle_config.wp_rate / 100;
	}
// - Baby Class Penalty = 80% (from adult's chance) ----//
	if (sd->class_&JOBL_BABY) //if it's a Baby Class
		make_per = (make_per * 80) / 100; //Lupus

	if(make_per < 1) make_per = 1;

	if(rand()%10000 < make_per){
		struct item tmp_item;
		memset(&tmp_item,0,sizeof(tmp_item));
		tmp_item.nameid=nameid;
		tmp_item.amount=1;
		tmp_item.identify=1;
		if(equip){
			tmp_item.card[0]=0x00ff;
			tmp_item.card[1]=((sc*5)<<8)+ele;
			tmp_item.card[2]=GetWord(sd->char_id,0); // CharId
			tmp_item.card[3]=GetWord(sd->char_id,1);
		} else if((battle_config.produce_item_name_input && skill_produce_db[idx].req_skill!=AM_PHARMACY) ||
			(battle_config.produce_potion_name_input && skill_produce_db[idx].req_skill==AM_PHARMACY)) {
			tmp_item.card[0]=0x00fe;
			tmp_item.card[1]=0;
			tmp_item.card[2]=GetWord(sd->char_id,0); // CharId
			tmp_item.card[3]=GetWord(sd->char_id,1);
		}

		if(log_config.produce > 0)
			log_produce(sd,nameid,slot1,slot2,slot3,1);

		if(equip){
			clif_produceeffect(sd,0,nameid);
			clif_misceffect(&sd->bl,3);
			if(itemdb_wlv(nameid) >= 3 && ((ele? 1 : 0) + sc) >= 3) // Fame point system [DracoRPG]
				pc_addfame(sd,10); // Success to forge a lv3 weapon with 3 additional ingredients = +10 fame point
		} else {
			switch (skill_produce_db[idx].req_skill) {
				case AM_PHARMACY:
					clif_produceeffect(sd,2,nameid);
					clif_misceffect(&sd->bl,5);
					if(nameid >= 545 && nameid <= 547) { // Fame point system [DracoRPG]
			  			switch(++sd->potion_success_counter) {
							case 3:
								pc_addfame(sd,1); // Success to prepare 3 Condensed Potions in a row = +1 fame point
								break;
							case 5:
								pc_addfame(sd,3); // Success to prepare 5 Condensed Potions in a row = +3 fame point
								break;
				  			case 7:
								pc_addfame(sd,10); // Success to prepare 7 Condensed Potions in a row = +10 fame point
								break;
							case 10:
								pc_addfame(sd,50); // Success to prepare 10 Condensed Potions in a row = +50 fame point
								sd->potion_success_counter = 0;
								break;
						}
					} else sd->potion_success_counter = 0;
					break;
				case BS_IRON:
				case BS_STEEL:
				case BS_ENCHANTEDSTONE:
					clif_produceeffect(sd,0,nameid);
					clif_misceffect(&sd->bl,3);
					break;
			}
		}

		if((flag = pc_additem(sd,&tmp_item,1))) {
			clif_additem(sd,0,0,flag);
			map_addflooritem(&tmp_item,1,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
		}
	} else {
		if(log_config.produce > 0)
			log_produce(sd,nameid,slot1,slot2,slot3,0);

		if(equip){
			clif_produceeffect(sd,1,nameid);
			clif_misceffect(&sd->bl,2);
		} else {
			switch (skill_produce_db[idx].req_skill) {
				case AM_PHARMACY:
					clif_produceeffect(sd,3,nameid);
					clif_misceffect(&sd->bl,6);
					sd->potion_success_counter = 0; // Fame point system [DracoRPG]
					break;
				case BS_IRON:
				case BS_STEEL:
				case BS_ENCHANTEDSTONE:
					clif_produceeffect(sd,1,nameid);
					clif_misceffect(&sd->bl,2);
					break;
			}
		}
	}
	return 0;
}

int skill_arrow_create( struct map_session_data *sd,int nameid)
{
	int i,j,flag,index=-1;
	struct item tmp_item;

	nullpo_retr(0, sd);

	if(nameid <= 0)
		return 1;

	for(i=0;i<MAX_SKILL_ARROW_DB;i++)
		if(nameid == skill_arrow_db[i].nameid) {
			index = i;
			break;
		}

	if(index < 0 || (j = pc_search_inventory(sd,nameid)) < 0)
		return 1;

	pc_delitem(sd,j,1,0);
	for(i=0;i<5;i++) {
		memset(&tmp_item,0,sizeof(tmp_item));
		tmp_item.identify = 1;
		tmp_item.nameid = skill_arrow_db[index].cre_id[i];
		tmp_item.amount = skill_arrow_db[index].cre_amount[i];
		if(battle_config.making_arrow_name_input) {
			tmp_item.card[0]=0x00fe;
			tmp_item.card[1]=0;
			tmp_item.card[2]=GetWord(sd->char_id,0); // CharId
			tmp_item.card[3]=GetWord(sd->char_id,1);
		}
		if(tmp_item.nameid <= 0 || tmp_item.amount <= 0)
			continue;
		if((flag = pc_additem(sd,&tmp_item,tmp_item.amount))) {
			clif_additem(sd,0,0,flag);
			map_addflooritem(&tmp_item,tmp_item.amount,sd->bl.m,sd->bl.x,sd->bl.y,NULL,NULL,NULL,0);
		}
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * ?炎�化系
 */

/*
 * 文字列?��?
 *        ',' で区?ﾘって val に戻す
 */
int skill_split_str(char *str,char **val,int num)
{
	int i;

	for (i=0; i<num && str; i++){
		val[i] = str;
		str = strchr(str,',');
		if (str)
			*str++=0;
	}
	return i;
}
/*
 * 文字列?��?
 *      ':' で区?ﾘってatoiしてvalに戻す
 */
int skill_split_atoi(char *str,int *val)
{
	int i, j, diff, step = 1;

	for (i=0; i<MAX_SKILL_LEVEL; i++) {
		if (!str) break;
		val[i] = atoi(str);
		str = strchr(str,':');
		if (str)
			*str++=0;
	}
	if(i==0) //No data found.
		return 0;
	if(i==1)
	{	//Single value, have the whole range have the same value.
		for (; i < MAX_SKILL_LEVEL; i++)
			val[i] = val[i-1];
		return i;
	}
	//Check for linear change with increasing steps until we reach half of the data acquired.
	for (step = 1; step <= i/2; step++)
	{
		diff = val[i-1] - val[i-step-1];
		for(j = i-1; j >= step; j--)
			if ((val[j]-val[j-step]) != diff)
				break;
	
		if (j>=step) //No match, try next step.
			continue;
		
		for(; i < MAX_SKILL_LEVEL; i++)
		{	//Apply linear increase
			val[i] = val[i-step]+diff;
			if (val[i] < 1 && val[i-1] >=0) //Check if we have switched from + to -, cap the decrease to 0 in said cases.
			{ val[i] = 1; diff = 0; step = 1; }
		}
		return i;
	}
	//Okay.. we can't figure this one out, just fill out the stuff with the previous value.
	for (;i<MAX_SKILL_LEVEL; i++)
		val[i] = val[i-1];
	return i;
}

/*
 * スキルユニットの配置?�報?�?ｬ
 */
void skill_init_unit_layout()
{
	int i,j,size,pos = 0;

	memset(skill_unit_layout,0,sizeof(skill_unit_layout));
	// 矩形のユニット配置を?�?ｬする
	for (i=0; i<=MAX_SQUARE_LAYOUT; i++) {
		size = i*2+1;
		skill_unit_layout[i].count = size*size;
		for (j=0; j<size*size; j++) {
			skill_unit_layout[i].dx[j] = (j%size-i);
			skill_unit_layout[i].dy[j] = (j/size-i);
		}
	}
	pos = i;
	// 矩形以外のユニット配置を?�?ｬする
	for (i=0;i<MAX_SKILL_DB;i++) {
		if (!skill_db[i].unit_id[0] || skill_db[i].unit_layout_type[0] != -1)
			continue;
		switch (i) {
			case MG_FIREWALL:
			case WZ_ICEWALL:
				// ファイア?[ウォ?[ル?Aアイスウォ?[ルは方向で変わるので別?��?
				break;
			case PR_SANCTUARY:
			{
				static const int dx[] = {
					-1, 0, 1,-2,-1, 0, 1, 2,-2,-1,
					 0, 1, 2,-2,-1, 0, 1, 2,-1, 0, 1};
				static const int dy[]={
					-2,-2,-2,-1,-1,-1,-1,-1, 0, 0,
					 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2};
				skill_unit_layout[pos].count = 21;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			case PR_MAGNUS:
			{
				static const int dx[] = {
					-1, 0, 1,-1, 0, 1,-3,-2,-1, 0,
					 1, 2, 3,-3,-2,-1, 0, 1, 2, 3,
					-3,-2,-1, 0, 1, 2, 3,-1, 0, 1,-1, 0, 1};
				static const int dy[] = {
					-3,-3,-3,-2,-2,-2,-1,-1,-1,-1,
					-1,-1,-1, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3};
				skill_unit_layout[pos].count = 33;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			case AS_VENOMDUST:
			{
				static const int dx[] = {-1, 0, 0, 0, 1};
				static const int dy[] = { 0,-1, 0, 1, 0};
				skill_unit_layout[pos].count = 5;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			case CR_GRANDCROSS:
			case NPC_GRANDDARKNESS:
			{
				static const int dx[] = {
					 0, 0,-1, 0, 1,-2,-1, 0, 1, 2,
					-4,-3,-2,-1, 0, 1, 2, 3, 4,-2,
					-1, 0, 1, 2,-1, 0, 1, 0, 0};
				static const int dy[] = {
					-4,-3,-2,-2,-2,-1,-1,-1,-1,-1,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					 1, 1, 1, 1, 2, 2, 2, 3, 4};
				skill_unit_layout[pos].count = 29;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			case PF_FOGWALL:
			{
				static const int dx[] = {
					-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2};
				static const int dy[] = {
					-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
				skill_unit_layout[pos].count = 15;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			case PA_GOSPEL:
			{
				static const int dx[] = {
					-1, 0, 1,-1, 0, 1,-3,-2,-1, 0,
					 1, 2, 3,-3,-2,-1, 0, 1, 2, 3,
					-3,-2,-1, 0, 1, 2, 3,-1, 0, 1,
					-1, 0, 1};
				static const int dy[] = {
					-3,-3,-3,-2,-2,-2,-1,-1,-1,-1,
					-1,-1,-1, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
					 3, 3, 3};
				skill_unit_layout[pos].count = 33;
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
				break;
			}
			default:
				ShowError("unknown unit layout at skill %d\n",i);
				break;
		}
		if (!skill_unit_layout[pos].count)
			continue;
		for (j=0;j<MAX_SKILL_LEVEL;j++)
			skill_db[i].unit_layout_type[j] = pos;
		pos++;
	}
	// ファイヤ?[ウォ?[ル
	firewall_unit_pos = pos;
	for (i=0;i<8;i++) {
		if (i&1) {	/* 斜め配置 */
			skill_unit_layout[pos].count = 5;
			if (i&0x2) {
				int dx[] = {-1,-1, 0, 0, 1};
				int dy[] = { 1, 0, 0,-1,-1};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			} else {
				int dx[] = { 1, 1 ,0, 0,-1}; 
				int dy[] = { 1, 0, 0,-1,-1}; 
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			}
		} else {	/* ?c横配置 */
			skill_unit_layout[pos].count = 3;
			if (i%4==0) {	/* ?繪ｺ */
				int dx[] = {-1, 0, 1};
				int dy[] = { 0, 0, 0};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			} else {			/* ?ｶ右 */
				int dx[] = { 0, 0, 0};
				int dy[] = {-1, 0, 1};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			}
		}
		pos++;
	}
	// アイスウォ?[ル
	icewall_unit_pos = pos;
	for (i=0;i<8;i++) {
		skill_unit_layout[pos].count = 5;
		if (i&1) {	/* 斜め配置 */
			if (i&0x2) {
				int dx[] = {-2,-1, 0, 1, 2};
				int dy[] = { 2,-1, 0,-1,-2};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			} else {
				int dx[] = { 2, 1 ,0,-1,-2}; 
				int dy[] = { 2, 1, 0,-1,-2}; 
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			}
		} else {	/* ?c横配置 */
			if (i%4==0) {	/* ?繪ｺ */
				int dx[] = {-2,-1, 0, 1, 2};
				int dy[] = { 0, 0, 0, 0, 0};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			} else {			/* ?ｶ右 */
				int dx[] = { 0, 0, 0, 0, 0};
				int dy[] = {-2,-1, 0, 1, 2};
				memcpy(skill_unit_layout[pos].dx,dx,sizeof(dx));
				memcpy(skill_unit_layout[pos].dy,dy,sizeof(dy));
			}
		}
		pos++;
	}
}

/*==========================================
 * スキル?係ファイル?み?み
 * skill_db.txt スキルデ?タ
 * skill_cast_db.txt スキルの詠?･時間とディレイデ?タ
 * produce_db.txt アイテム?�?ｬスキル用デ?タ
 * create_arrow_db.txt 矢?�?ｬスキル用デ?タ
 * abra_db.txt アブラカダブラ?動スキルデ?タ
 *------------------------------------------
 */
int skill_readdb(void)
{
	int i,j,k,l,m;
	FILE *fp;
	char line[1024],path[1024],*p;
	char *filename[]={"produce_db.txt","produce_db2.txt"};

	/* スキルデ?タベ?ス */
	memset(skill_db,0,sizeof(skill_db));
	sprintf(path, "%s/skill_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50];
		if(line[0]=='/' && line[1]=='/')
			continue;
		j = skill_split_str(line,split,14);
		if(j < 14 || split[13]==NULL)
			continue;

		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;

		skill_split_atoi(split[1],skill_db[i].range);
		skill_db[i].hit=atoi(split[2]);
		skill_db[i].inf=atoi(split[3]);
		skill_db[i].pl=atoi(split[4]);
		skill_db[i].nk=atoi(split[5]);
		skill_db[i].max=atoi(split[6]);
		skill_split_atoi(split[7],skill_db[i].num);

		if(strcmpi(split[8],"yes") == 0)
			skill_db[i].castcancel=1;
		else
			skill_db[i].castcancel=0;
		skill_db[i].cast_def_rate=atoi(split[9]);
		skill_db[i].inf2=atoi(split[10]);
		skill_db[i].maxcount=atoi(split[11]);
		if(strcmpi(split[12],"weapon") == 0)
			skill_db[i].skill_type=BF_WEAPON;
		else if(strcmpi(split[12],"magic") == 0)
			skill_db[i].skill_type=BF_MAGIC;
		else if(strcmpi(split[12],"misc") == 0)
			skill_db[i].skill_type=BF_MISC;
		else
			skill_db[i].skill_type=0;
		skill_split_atoi(split[13],skill_db[i].blewcount);

		for (j = 0; skill_names[j].id != 0; j++)
			if (skill_names[j].id == i) {
				skill_db[i].name = skill_names[j].name;
				skill_db[i].desc = skill_names[j].desc;
				break;
			}
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);

	sprintf(path, "%s/skill_require_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50];
		if(line[0]=='/' && line[1]=='/')
			continue;
		j = skill_split_str(line,split,30);
		if(j < 30 || split[29]==NULL)
			continue;

		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;

		skill_split_atoi(split[1],skill_db[i].hp);
		skill_split_atoi(split[2],skill_db[i].mhp);
		skill_split_atoi(split[3],skill_db[i].sp);
		skill_split_atoi(split[4],skill_db[i].hp_rate);
		skill_split_atoi(split[5],skill_db[i].sp_rate);
		skill_split_atoi(split[6],skill_db[i].zeny);
		
		p = split[7];
		for(j=0;j<32;j++){
			l = atoi(p);
			if (l==99) {
				skill_db[i].weapon = 0xffffffff;
				break;
			}
			else
				skill_db[i].weapon |= 1<<l;
			p=strchr(p,':');
			if(!p)
				break;
			p++;
		}

		if( strcmpi(split[8],"hiding")==0 ) skill_db[i].state=ST_HIDING;
		else if( strcmpi(split[8],"cloaking")==0 ) skill_db[i].state=ST_CLOAKING;
		else if( strcmpi(split[8],"hidden")==0 ) skill_db[i].state=ST_HIDDEN;
		else if( strcmpi(split[8],"riding")==0 ) skill_db[i].state=ST_RIDING;
		else if( strcmpi(split[8],"falcon")==0 ) skill_db[i].state=ST_FALCON;
		else if( strcmpi(split[8],"cart")==0 ) skill_db[i].state=ST_CART;
		else if( strcmpi(split[8],"shield")==0 ) skill_db[i].state=ST_SHIELD;
		else if( strcmpi(split[8],"sight")==0 ) skill_db[i].state=ST_SIGHT;
		else if( strcmpi(split[8],"explosionspirits")==0 ) skill_db[i].state=ST_EXPLOSIONSPIRITS;
		else if( strcmpi(split[8],"cartboost")==0 ) skill_db[i].state=ST_CARTBOOST;
		else if( strcmpi(split[8],"recover_weight_rate")==0 ) skill_db[i].state=ST_RECOV_WEIGHT_RATE;
		else if( strcmpi(split[8],"move_enable")==0 ) skill_db[i].state=ST_MOVE_ENABLE;
		else if( strcmpi(split[8],"water")==0 ) skill_db[i].state=ST_WATER;
		else skill_db[i].state=ST_NONE;

		skill_split_atoi(split[9],skill_db[i].spiritball);
		skill_db[i].itemid[0]=atoi(split[10]);
		skill_db[i].amount[0]=atoi(split[11]);
		skill_db[i].itemid[1]=atoi(split[12]);
		skill_db[i].amount[1]=atoi(split[13]);
		skill_db[i].itemid[2]=atoi(split[14]);
		skill_db[i].amount[2]=atoi(split[15]);
		skill_db[i].itemid[3]=atoi(split[16]);
		skill_db[i].amount[3]=atoi(split[17]);
		skill_db[i].itemid[4]=atoi(split[18]);
		skill_db[i].amount[4]=atoi(split[19]);
		skill_db[i].itemid[5]=atoi(split[20]);
		skill_db[i].amount[5]=atoi(split[21]);
		skill_db[i].itemid[6]=atoi(split[22]);
		skill_db[i].amount[6]=atoi(split[23]);
		skill_db[i].itemid[7]=atoi(split[24]);
		skill_db[i].amount[7]=atoi(split[25]);
		skill_db[i].itemid[8]=atoi(split[26]);
		skill_db[i].amount[8]=atoi(split[27]);
		skill_db[i].itemid[9]=atoi(split[28]);
		skill_db[i].amount[9]=atoi(split[29]);
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);

	/* キャスティングデ?タベ?ス */

	sprintf(path, "%s/skill_cast_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50];
		memset(split,0,sizeof(split));	// [Valaris] thanks to fov
		if(line[0]=='/' && line[1]=='/')
			continue;
		j = skill_split_str(line,split,5);
		if(split[4]==NULL || j<5)
			continue;

		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;

		skill_split_atoi(split[1],skill_db[i].cast);
		skill_split_atoi(split[2],skill_db[i].delay);
		skill_split_atoi(split[3],skill_db[i].upkeep_time);
		skill_split_atoi(split[4],skill_db[i].upkeep_time2);
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);

	/* スキルユニットデ?[タベ?[ス */

	sprintf(path, "%s/skill_unit_db.txt", db_path);
	fp=fopen(path,"r");
	if (fp==NULL) {
		ShowError("can't read %s\n", path);
		return 1;
	}
        k = 0;
	while (fgets(line,1020,fp)) {
		char *split[50];
		if (line[0]=='/' && line[1]=='/')
			continue;
		j = skill_split_str(line,split,8);
		if (split[7]==NULL || j<8)
			continue;

		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;
		skill_db[i].unit_id[0] = strtol(split[1],NULL,16);
		skill_db[i].unit_id[1] = strtol(split[2],NULL,16);
		skill_split_atoi(split[3],skill_db[i].unit_layout_type);
		skill_db[i].unit_range = atoi(split[4]);
		skill_db[i].unit_interval = atoi(split[5]);

		if( strcmpi(split[6],"noenemy")==0 ) skill_db[i].unit_target=BCT_NOENEMY;
		else if( strcmpi(split[6],"friend")==0 ) skill_db[i].unit_target=BCT_NOENEMY;
		else if( strcmpi(split[6],"party")==0 ) skill_db[i].unit_target=BCT_PARTY;
		else if( strcmpi(split[6],"ally")==0 ) skill_db[i].unit_target=BCT_PARTY|BCT_GUILD;
		else if( strcmpi(split[6],"all")==0 ) skill_db[i].unit_target=BCT_ALL;
		else if( strcmpi(split[6],"enemy")==0 ) skill_db[i].unit_target=BCT_ENEMY;
		else if( strcmpi(split[6],"self")==0 ) skill_db[i].unit_target=BCT_SELF;
		else if( strcmpi(split[6],"noone")==0 ) skill_db[i].unit_target=BCT_NOONE;
		else skill_db[i].unit_target = strtol(split[6],NULL,16);

		skill_db[i].unit_flag = strtol(split[7],NULL,16);
		k++;
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);
	skill_init_unit_layout();

	/* ?ｻ造系スキルデ?タベ?ス */
	memset(skill_produce_db,0,sizeof(skill_produce_db));
	for(m=0;m<2;m++){
		sprintf(path, "%s/%s", db_path, filename[m]);
		fp=fopen(path,"r");
		if(fp==NULL){
			if(m>0)
				continue;
			ShowError("can't read %s\n",path);
			return 1;
		}
		k=0;
		while(fgets(line,1020,fp)){
			char *split[6 + MAX_PRODUCE_RESOURCE * 2];
			int x,y;
			if(line[0]=='/' && line[1]=='/')
				continue;
			memset(split,0,sizeof(split));
			j = skill_split_str(line,split,(3 + MAX_PRODUCE_RESOURCE * 2));
			if(split[0]==0) //fixed by Lupus
				continue;
			i=atoi(split[0]);
			if(i<=0) continue;

			skill_produce_db[k].nameid=i;
			skill_produce_db[k].itemlv=atoi(split[1]);
			skill_produce_db[k].req_skill=atoi(split[2]);

			for(x=3,y=0; split[x] && split[x+1] && y<MAX_PRODUCE_RESOURCE; x+=2,y++){
				skill_produce_db[k].mat_id[y]=atoi(split[x]);
				skill_produce_db[k].mat_amount[y]=atoi(split[x+1]);
			}
			k++;
			if(k >= MAX_SKILL_PRODUCE_DB)
				break;
		}
		fclose(fp);
		ShowStatus("Done reading '"CL_WHITE"%d"CL_RESET"' entries in '"CL_WHITE"%s"CL_RESET"'.\n",k,path);
	}

	memset(skill_arrow_db,0,sizeof(skill_arrow_db));

	sprintf(path, "%s/create_arrow_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	k=0;
	while(fgets(line,1020,fp)){
		char *split[16];
		int x,y;
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		j = skill_split_str(line,split,13);
		if(split[0]==0) //fixed by Lupus
			continue;
		i=atoi(split[0]);
		if(i<=0)
			continue;

		skill_arrow_db[k].nameid=i;

		for(x=1,y=0;split[x] && split[x+1] && y<5;x+=2,y++){
			skill_arrow_db[k].cre_id[y]=atoi(split[x]);
			skill_arrow_db[k].cre_amount[y]=atoi(split[x+1]);
		}
		k++;
		if(k >= MAX_SKILL_ARROW_DB)
			break;
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%d"CL_RESET"' entries in '"CL_WHITE"%s"CL_RESET"'.\n",k,path);

	memset(skill_abra_db,0,sizeof(skill_abra_db));
	sprintf(path, "%s/abra_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	k=0;
	while(fgets(line,1020,fp)){
		char *split[16];
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		j = skill_split_str(line,split,13);
		if(split[0]==0) //fixed by Lupus
			continue;
		i=atoi(split[0]);
		if(i<=0)
			continue;

		skill_abra_db[i].req_lv=atoi(split[2]);
		skill_abra_db[i].per=atoi(split[3]);

		k++;
		if(k >= MAX_SKILL_ABRA_DB)
			break;
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%d"CL_RESET"' entries in '"CL_WHITE"%s"CL_RESET"'.\n",k,path);

	sprintf(path, "%s/skill_castnodex_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	while(fgets(line,1020,fp)){
		char *split[50];
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		j = skill_split_str(line,split,4);
		if(split[0]==0) //fixed by Lupus
			continue;
		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;

		skill_split_atoi(split[1],skill_db[i].castnodex);
		if (!split[2])
			continue;
		skill_split_atoi(split[2],skill_db[i].delaynodex);
		if(!split[3])
			continue;
		skill_split_atoi(split[3],skill_db[i].delaynowalk);
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);

	sprintf(path, "%s/skill_nocast_db.txt", db_path);
	fp=fopen(path,"r");
	if(fp==NULL){
		ShowError("can't read %s\n", path);
		return 1;
	}
	k=0;
	while(fgets(line,1020,fp)){
		char *split[16];
		if(line[0]=='/' && line[1]=='/')
			continue;
		memset(split,0,sizeof(split));
		j = skill_split_str(line,split,2);
		if(split[0]==0) //fixed by Lupus
			continue;
		i=atoi(split[0]);
		if (i>=10000 && i<10015) // for guild skills [Celest]
			i -= 9500;
		else if(i<=0 || i>MAX_SKILL_DB)
			continue;
		skill_db[i].nocast=atoi(split[1]);
		k++;
	}
	fclose(fp);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n",path);

	return 0;
}

/*===============================================
 * For reading leveluseskillspamount.txt [Celest]
 *-----------------------------------------------
 */
static int skill_read_skillspamount(void)
{
	char *buf,*p;
	struct skill_db *skill = NULL;
	int s, idx, new_flag=1, level=1, sp=0;

	buf=(char *) grfio_reads("data\\leveluseskillspamount.txt",&s);

	if(buf==NULL)
		return -1;

	buf[s]=0;
	for(p=buf;p-buf<s;){
		char buf2[64];

		if (sscanf(p,"%[@]",buf2) == 1) {
			level = 1;
			new_flag = 1;
		} else if (new_flag && sscanf(p,"%[^#]#",buf2) == 1) {
			for (idx=0; skill_names[idx].id != 0; idx++) {
				if (strstr(buf2, skill_names[idx].name) != NULL) {
					skill = &skill_db[ skill_names[idx].id ];
					new_flag = 0;
					break;
				}
			}
		} else if (!new_flag && sscanf(p,"%d#",&sp) == 1) {
			skill->sp[level-1]=sp;
			level++;
		}

		p=strchr(p,10);
		if(!p) break;
		p++;
	}
	aFree(buf);
	ShowStatus("Done reading '"CL_WHITE"%s"CL_RESET"'.\n","data\\leveluseskillspamount.txt");

	return 0;
}

void skill_reload(void)
{
	skill_readdb();
	if (battle_config.skill_sp_override_grffile)
		skill_read_skillspamount();
}

/*==========================================
 * スキル?係?炎�化?�?
 *------------------------------------------
 */
int do_init_skill(void)
{
	skill_readdb();
	if (battle_config.skill_sp_override_grffile)
		skill_read_skillspamount();

	add_timer_func_list(skill_unit_timer,"skill_unit_timer");
	add_timer_func_list(skill_castend_id,"skill_castend_id");
	add_timer_func_list(skill_castend_pos,"skill_castend_pos");
	add_timer_func_list(skill_timerskill,"skill_timerskill");
	add_timer_func_list(skill_castend_delay_sub,"skill_castend_delay_sub");
	
	add_timer_interval(gettick()+SKILLUNITTIMER_INVERVAL,skill_unit_timer,0,0,SKILLUNITTIMER_INVERVAL);

	return 0;
}
