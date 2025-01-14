#include "src/kknd.h"

#include "src/_unsorted_functions.h"
#include "src/_unsorted_data.h"

#include "src/Random.h"
#include "src/Script.h"
#include "src/ScriptEvent.h"
#include "src/Entity.h"


//----- (004072A0) --------------------------------------------------------
void EventHandler_Prison(Script *receiver, Script *sender, enum SCRIPT_EVENT event, void *param)
{
    Entity *v4; // esi@1

    v4 = (Entity *)receiver->param;
    if (!v4->destroyed)
    {
        if (event == EVT_MSG_DAMAGE)
        {
            if (current_level_idx == LEVEL_SURV_09_RESCUE_THE_COMMANDER)
                entity_402E90_on_damage(v4, param, entity_mode_prison_on_death_surv09);
            else
                entity_402E90_on_damage(v4, param, entity_mode_prison_on_death);
            entity_410520_update_healthbar_color(v4);
        }
        else
        {
            EventHandler_DefaultBuildingsHandler(receiver, sender, event, param);
        }
    }
}

//----- (00407300) --------------------------------------------------------
void entity_mode_407300_prison(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1

    v1 = a1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->x = ((a1->stru60.ptr_C->x_offset + a1->sprite->x) & 0xFFFFE000) - a1->stru60.ptr_C->x_offset + 4096;
    a1->sprite->field_88_unused = 1;
    a1->sprite->y = ((a1->stru60.ptr_C->y_offset + a1->sprite->y) & 0xFFFFE000) - a1->stru60.ptr_C->y_offset + 4096;
    entity_40DD00_boxd(a1);
    v2 = v1->script;
    v1->mode = (void(*)(Entity *))nullsub_1;
    script_445370_yield_to_main_thread(v2, 0x80000000, 1);
}

//----- (00407390) --------------------------------------------------------
void entity_mode_407390_prison(Entity *a1)
{
    entity_load_idle_mobd(a1);
    a1->mode = entity_mode_407300_prison;
    script_445370_yield_to_main_thread(a1->script, 0x80000000, 1);
}

//----- (004073D0) --------------------------------------------------------
void UNIT_Handler_Prison(Script *a1)
{
    Entity *v1; // esi@1

    v1 = (Entity *)a1->param;
    if (!v1)
    {
        v1 = entity_list_create(a1);
        v1->script->event_handler = EventHandler_Prison;
        entity_initialize_building(v1, 5, 0, 0);
        sprite_load_mobd(a1->sprite, 0);
        v1->mode = entity_mode_407390_prison;
    }
    (v1->mode)(v1);
}

//----- (00407420) --------------------------------------------------------
void entity_mode_prison_spawn_unit(Entity *a1)
{
    Sprite *v1; // eax@2
    int v2; // edx@2
    int v3; // eax@2
    int v4; // esi@2

    if (a1->_12C_prison_bunker_spawn_type)
    {
        a1->sprite->field_88_unused = 1;
        v1 = a1->sprite;
        v2 = v1->x;
        v3 = v1->y;
        v4 = a1->_12C_prison_bunker_spawn_type - 1;
        a1->_12C_prison_bunker_spawn_type = v4;
        if (current_level_idx == LEVEL_SURV_23)   // surv_23.lvl
        {
            if (!spawn_unit(surv_prison_spawns_table[v4], v2, v3, player_side))
                show_message_ex(0, aCouldnTCreateS);
        }
        else if (!spawn_unit(mute_prison_spawns_table[v4], v2, v3, player_side))
        {
            show_message_ex(0, aCouldnTCreateS);
        }
    }
    else
    {
        a1->mode = entity_mode_403720_on_prison_death__or__prolly_any_generic_building;
    }
}

//----- (004074B0) --------------------------------------------------------
void entity_mode_prison_on_death(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1

    v1 = a1;
    entity_mode_building_default_on_death(a1);
    v2 = v1->script;
    v1->_12C_prison_bunker_spawn_type = 10;
    v1->mode = entity_mode_prison_spawn_unit;
    script_445370_yield_to_main_thread(v2, 0x80000000, 80);
}

//----- (004074E0) --------------------------------------------------------
void entity_mode_prison_spawn_unit_surv09(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1

    v1 = a1;
    a1->sprite->field_88_unused = 1;
    spawn_unit(UNIT_STATS_SURV_GENERAL, a1->sprite->x, a1->sprite->y, player_side);
    v2 = v1->script;
    v1->mode = entity_mode_403720_on_prison_death__or__prolly_any_generic_building;
    script_445370_yield_to_main_thread(v2, 0x80000000, 10);
}

//----- (00407530) --------------------------------------------------------
void entity_mode_prison_on_death_surv09(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1

    v1 = a1;
    entity_mode_building_default_on_death(a1);
    v2 = v1->script;
    v1->mode = entity_mode_prison_spawn_unit_surv09;
    script_445370_yield_to_main_thread(v2, 0x80000000, 80);
}

//----- (00407560) --------------------------------------------------------
void EventHandler_TechBunker(Script *receiver, Script *sender, enum SCRIPT_EVENT event, void *param)
{
    Entity *v4; // esi@1

    v4 = (Entity *)receiver->param;
    if (!v4->destroyed)
    {
        switch (event)
        {
        case EVT_MSG_DAMAGE:
            entity_402E90_on_damage(v4, param, entity_mode_407C60_on_death_tech_bunker);
            entity_410710_status_bar(v4);
            break;
        case EVT_MSG_1511_sidebar_click_category:
            entity_410CB0_event1511(v4);
            break;
        case EVT_SHOW_UI_CONTROL:
            entity_410CD0_eventTextString(v4);
            break;
        case EVT_MSG_SHOW_UNIT_HINT:
            entity_show_hint(v4);
            break;
        default:
            return;
        }
    }
}

//----- (004075F0) --------------------------------------------------------
void entity_4075F0_techbunker(Entity *a1)
{
    Entity *v1; // esi@1

    v1 = a1;
    entity_load_move_mobd(a1);
    script_445370_yield_to_main_thread(v1->script, 0x80000000, 1);
}

//----- (00407630) --------------------------------------------------------
void entity_mode_407630_bunker(Entity *a1)
{
    Entity *v1; // esi@1

    v1 = a1;
    a1->sprite->field_88_unused = 1;
    spawn_unit(
        UNIT_STATS_SURV_EL_PRESIDENTE,
        a1->stru60.pstru4->x_offset + a1->sprite->x,
        a1->sprite->y + a1->stru60.pstru4->y_offset - 1280,
        player_side);
    v1->_12C_prison_bunker_spawn_type = 0;
    v1->mode = entity_4075F0_techbunker;
}

//----- (00407690) --------------------------------------------------------
void entity_407690_techbunker_spawn(Entity *a1)
{
    Entity *v1; // esi@1
    enum PLAYER_SIDE v2; // edi@1
    int v3; // edx@5
    int v4; // eax@8
    const char *v5; // edx@19
    Script *v6; // ST00_4@24

    v1 = a1;
    v2 = (enum PLAYER_SIDE)(int)a1->_128_spawn_param;
    if (!single_player_game || _47C6D8_use__466098_cost_multipliers)
        a1->_12C_prison_bunker_spawn_type = 9;
    entity_load_move_mobd(a1);
    if (v1->_12C_prison_bunker_spawn_type == 9)
    {
        v3 = kknd_rand_debug(__FILE__, __LINE__) % 6;
        v1->_12C_prison_bunker_spawn_type = v3;
        if (_47C6D8_use__466098_cost_multipliers)
        {
            if (v3 == 4)
                v1->_12C_prison_bunker_spawn_type = 3;
        }
    }
    v4 = v1->_12C_prison_bunker_spawn_type;
    if (v4 < 4)
    {
        v1->sprite->field_88_unused = 1;
        spawn_unit(
            techbunker_spawns_table[v1->_12C_prison_bunker_spawn_type],
            v1->sprite->x + v1->stru60.pstru4->x_offset,
            v1->sprite->y + v1->stru60.pstru4->y_offset,
            v2);
        goto LABEL_23;
    }
    if (v4 == 4)
    {
        if (single_player_game)
        {
            if (player_side == 1)
            {
                v1->sprite->field_88_unused = 1;
            LABEL_16:
                spawn_unit(
                    UNIT_STATS_SURV_TANKER,
                    v1->sprite->x + v1->stru60.pstru4->x_offset,
                    v1->sprite->y + v1->stru60.pstru4->y_offset,
                    v2);
                goto LABEL_23;
            }
        }
        else if (!netz_47A740[v1->player_side + 1].field_A)
        {
            v1->sprite->field_88_unused = 1;
            goto LABEL_16;
        }
        v1->sprite->field_88_unused = 1;
        spawn_unit(
            UNIT_STATS_MUTE_TANKER,
            v1->sprite->x + v1->stru60.pstru4->x_offset,
            v1->sprite->y + v1->stru60.pstru4->y_offset,
            v2);
    }
    else
    {
        if (v4 == 5)
        {
            add_integer((int *)&game_globals_per_player + v2, 5000);
            v5 = a5000ResourceUn;
        }
        else
        {
            if (v4 != 6)
                goto LABEL_23;
            add_integer((int *)&game_globals_per_player + v2, 1000);
            v5 = a1000ResourceUn;
        }
        show_message_ex(v1, v5);
    }
LABEL_23:
    if (v1->_12C_prison_bunker_spawn_type == 10)
    {
        v6 = v1->script;
        v1->_12C_prison_bunker_spawn_type = 5;
        script_445370_yield_to_main_thread(v6, 0x80000000, 1);
        v1->mode = entity_mode_407630_bunker;
    }
    else
    {
        script_445370_yield_to_main_thread(v1->script, 0x80000000, 1);
        v1->mode = entity_4075F0_techbunker;
    }
}

//----- (00407870) --------------------------------------------------------
void entity_mode_407870_techbubker(Entity *a1)
{
    entity_load_attack_mobd(a1);
    script_445370_yield_to_main_thread(a1->script, 0x10000000, 0);
    a1->mode = entity_407690_techbunker_spawn;
}

//----- (004078B0) --------------------------------------------------------
Entity *entity_4078B0_techbunker_find_entity(Entity *a1, int a2)
{
    Sprite *v2; // ecx@1
    Entity *result; // eax@1
    int v4; // ebx@1
    Sprite *v5; // ecx@4
    int v6; // esi@4
    int v7; // esi@4
    bool v8; // zf@4
    bool v9; // sf@4
    Sprite *v10; // esi@4
    int v11; // ecx@5
    int v12; // esi@7
    int v13; // edi@7
    int v14; // [sp+10h] [bp-4h]@1

    a1->sprite->field_88_unused = 1;
    v2 = a1->sprite;
    result = entity_list_head;
    v4 = v2->x;
    v14 = v2->y;
    if ((Entity **)entity_list_head == &entity_list_head)
    {
    LABEL_11:
        result = 0;
    }
    else
    {
        while (1)
        {
            if (!result->destroyed && result->player_side)
            {
                result->sprite->field_88_unused = 1;
                v5 = result->sprite;
                v6 = v5->x;
                v5->field_88_unused = 1;
                v7 = v6 - v4;
                v8 = v7 == 0;
                v9 = v7 < 0;
                v10 = result->sprite;
                v11 = v9 || v8 ? v4 - v10->x : v10->x - v4;
                v12 = v10->y;
                v13 = v12 - v14;
                if (v12 - v14 <= 0)
                    v13 = v14 - v12;
                if (v11 + v13 < a2)
                    break;
            }
            result = result->next;
            if ((Entity **)result == &entity_list_head)
                goto LABEL_11;
        }
    }
    return result;
}

//----- (00407950) --------------------------------------------------------
void entity_mode_407950_techbunker_spawn_generic(Entity *a1)
{
    Entity *v1; // esi@1
    int v2; // eax@1
    Entity *v3; // eax@2
    int v4; // eax@4
    EntityTurret *v5; // eax@5

    v1 = a1;
    entity_load_idle_mobd(a1);
    v2 = v1->_134_param__unitstats_after_mobile_outpost_plant;
    if (v2 > 0)
    {
        v4 = v2 - 1;
        v1->_134_param__unitstats_after_mobile_outpost_plant = v4;
        if (v4 <= 0)
        {
            v5 = v1->turret;
            if (v5)
                v5->turret_sprite->drawjob->flags &= 0xBFFFFFFF;
        }
    }
    else
    {
        v3 = entity_4078B0_techbunker_find_entity(v1, (int)v1->_128_spawn_param);
        if (v3)
        {
            v1->_128_spawn_param = (void *)v3->player_side;
            sprite_408800_play_sound(v1->sprite, SOUND_MobileOutpost_ClanhallWagon_Planted, _4690A8_unit_sounds_volume, 0);
            v1->mode = entity_mode_407870_techbubker;
        }
    }
    script_445370_yield_to_main_thread(v1->script, 0x80000000, 1);
}

//----- (004079F0) --------------------------------------------------------
void entity_mode_4079F0_techbunker_spawn10_surv18_lvl(Entity *a1)
{
    Entity *v1; // esi@1
    int v2; // eax@1
    Entity *v3; // eax@2
    int v4; // eax@4
    EntityTurret *v5; // eax@5

    v1 = a1;
    entity_load_idle_mobd(a1);
    v2 = v1->_134_param__unitstats_after_mobile_outpost_plant;
    if (v2 > 0)
    {
        v4 = v2 - 1;
        v1->_134_param__unitstats_after_mobile_outpost_plant = v4;
        if (v4 <= 0)
        {
            v5 = v1->turret;
            if (v5)
                v5->turret_sprite->drawjob->flags &= 0xBFFFFFFF;
        }
    }
    else
    {
        v3 = entity_4078B0_techbunker_find_entity(v1, (int)v1->_128_spawn_param);
        if (v3)
        {
            v1->_12C_prison_bunker_spawn_type = 10;
            v1->_128_spawn_param = (void *)v3->player_side;
            sprite_408800_play_sound(v1->sprite, SOUND_MobileOutpost_ClanhallWagon_Planted, _4690A8_unit_sounds_volume, 0);
            v1->mode = entity_mode_407870_techbubker;
        }
    }
    script_445370_yield_to_main_thread(v1->script, 0x80000000, 1);
}

//----- (00407A90) --------------------------------------------------------
void entity_mode_407A90_techbunker(Entity *a1)
{
    Entity *v1; // esi@1

    v1 = a1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->x = ((a1->stru60.ptr_C->x_offset + a1->sprite->x) & 0xFFFFE000) - a1->stru60.ptr_C->x_offset + 4096;
    a1->sprite->field_88_unused = 1;
    a1->sprite->y = ((a1->stru60.ptr_C->y_offset + a1->sprite->y) & 0xFFFFE000) - a1->stru60.ptr_C->y_offset + 4096;
    entity_40DD00_boxd(a1);
    if (current_level_idx == LEVEL_SURV_18)
        v1->mode = entity_mode_4079F0_techbunker_spawn10_surv18_lvl;
    else
        v1->mode = entity_mode_407950_techbunker_spawn_generic;
    if (!single_player_game || _47C6D8_use__466098_cost_multipliers)
        v1->_134_param__unitstats_after_mobile_outpost_plant = kknd_rand_debug(__FILE__, __LINE__) % 25200 + 28800;
    else
        v1->_134_param__unitstats_after_mobile_outpost_plant = 5;
    script_445370_yield_to_main_thread(v1->script, 0x80000000, 1);
}

//----- (00407B70) --------------------------------------------------------
void entity_mode_407B70_techbunker(Entity *a1)
{
    entity_load_idle_mobd(a1);
    a1->mode = entity_mode_407A90_techbunker;
    script_445370_yield_to_main_thread(a1->script, 0x80000000, 1);
}

//----- (00407BB0) --------------------------------------------------------
void UNIT_Handler_TechBunker(Script *a1)
{
    Entity *v1; // esi@1
    Entity *v2; // esi@3
    Script *v3; // eax@3

    v1 = (Entity *)a1->param;
    if (!v1)
    {
        if (_441240_techbunker_should_create())
        {
            v2 = entity_list_create(a1);
            entity_set_draw_handlers(v2);
            v3 = v2->script;
            v2->_128_spawn_param = (void *)24576;
            v3->event_handler = EventHandler_TechBunker;
            entity_attach_docking_point(v2);
            v2->mode = entity_mode_407B70_techbunker;
            (v2->mode)(v2);
            return;
        }
        sprite_list_remove(a1->sprite);
        script_yield(a1);
    }
    (v1->mode)(v1);
}

//----- (00407C20) --------------------------------------------------------
void entity_mode_407C20_on_death_tech_bunker(Entity *a1)
{
    Entity *v1; // esi@1

    v1 = a1;
    sound_play(SOUND_23, 0, _4690A8_unit_sounds_volume, 16, 0);
    entity_40DDD0_boxd(v1);
    sprite_list_remove(v1->sprite);
    script_yield(v1->script);
    entity_list_remove(v1);
}

//----- (00407C60) --------------------------------------------------------
void entity_mode_407C60_on_death_tech_bunker(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1
    Script **v3; // eax@1
    Sprite *v4; // eax@3
    Script *v5; // edx@3

    v1 = a1;
    entity_439120_add_explosion(a1);
    sub_4083D0();
    v2 = v1->script;
    v1->destroyed = 1;
    v2->field_24 &= 0xEFFFFFFF;
    v3 = &v1->turret->sprite_task;
    if (v3)
        script_trigger_event(0, EVT_MSG_1500, 0, *v3);
    script_trigger_event(v1->script, EVT_SHOW_UI_CONTROL, 0, task_mobd17_cursor);
    script_trigger_event_group(v1->script, EVT_SHOW_UI_CONTROL, v1, SCRIPT_TYPE_39030);
    v1->script->script_type = SCRIPT_TYPE_INVALID;
    v4 = v1->sprite;
    v1->entity_id = 0;
    v4->x_speed = 0;
    v1->sprite->y_speed = 0;
    entity_439150_add_explosion(v1);
    entity_438D90_on_death_explosion(v1);
    v5 = v1->script;
    v1->mode = entity_mode_407C20_on_death_tech_bunker;
    script_445370_yield_to_main_thread(v5, 0x80000000, 185);
}

//----- (00407D10) --------------------------------------------------------
void entity_mode_407D10(Entity *a1)
{
    Entity *v1; // esi@1

    v1 = a1;
    script_yield(a1->script);
    entity_list_remove(v1);
}

//----- (00407D30) --------------------------------------------------------
void entity_mode_hut_on_death(Entity *a1)
{
    entity_load_mobd_4(a1);
    a1->mode = entity_mode_407D10;
    script_trigger_event(a1->script, EVT_SHOW_UI_CONTROL, 0, task_mobd17_cursor);

    a1->destroyed = 1;
    entity_439120_add_explosion(a1);
    entity_438D90_on_death_explosion(a1);
    script_445370_yield_to_main_thread(a1->script, 0xC0000000, 1);
}

//----- (00407DA0) --------------------------------------------------------
void entity_mode_407DA0(Entity *a1)
{
    entity_load_idle_mobd(a1);
    script_445370_yield_to_main_thread(a1->script, 0xC0000000, 1);
}

//----- (00407DE0) --------------------------------------------------------
void EventHandler_Hut(Script *receiver, Script *sender, enum SCRIPT_EVENT event, void *param)
{
    Entity *v4; // esi@1

    v4 = (Entity *)receiver->param;
    if (!v4->destroyed)
    {
        switch (event)
        {
        case EVT_MSG_DAMAGE:
            entity_402E90_on_damage(v4, param, entity_mode_hut_on_death);
            entity_410710_status_bar(v4);
            break;
        case EVT_MSG_1511_sidebar_click_category:
            entity_410CB0_event1511(v4);
            break;
        case EVT_SHOW_UI_CONTROL:
            entity_410CD0_eventTextString(v4);
            break;
        case EVT_MSG_SHOW_UNIT_HINT:
            entity_show_hint(v4);
            break;
        default:
            return;
        }
    }
}

//----- (00407E70) --------------------------------------------------------
void entity_mode_407E70_hut(Entity *a1)
{
    Entity *v1; // esi@1
    Script *v2; // eax@1

    v1 = a1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->field_88_unused = 1;
    a1->sprite->x = ((a1->stru60.ptr_C->x_offset + a1->sprite->x) & 0xFFFFE000) - a1->stru60.ptr_C->x_offset + 4096;
    a1->sprite->field_88_unused = 1;
    a1->sprite->y = ((a1->stru60.ptr_C->y_offset + a1->sprite->y) & 0xFFFFE000) - a1->stru60.ptr_C->y_offset + 4096;
    entity_40DD00_boxd(a1);
    v2 = v1->script;
    v1->mode = entity_mode_407DA0;
    script_445370_yield_to_main_thread(v2, 0x80000000, 1);
}

//----- (00407F00) --------------------------------------------------------
void entity_mode_407F00_hut(Entity *a1)
{
    entity_load_idle_mobd(a1);
    a1->mode = entity_mode_407E70_hut;
    script_445370_yield_to_main_thread(a1->script, 0x80000000, 1);
}

//----- (00407F40) --------------------------------------------------------
void UNIT_Handler_Hut(Script *a1)
{
    Entity *v1; // esi@1
    int v2; // eax@2

    v1 = (Entity *)a1->param;
    if (!v1)
    {
        v1 = entity_list_create(a1);
        entity_set_draw_handlers(v1);
        v2 = v1->_12C_prison_bunker_spawn_type;
        v1->_128_spawn_param = (void *)24576;
        switch (v2)
        {
        case 1:
            v1->current_mobd_lookup_idx = 16;
            break;
        case 2:
            v1->current_mobd_lookup_idx = 32;
            break;
        case 3:
            v1->current_mobd_lookup_idx = 48;
            break;
        case 4:
            v1->current_mobd_lookup_idx = 64;
            break;
        default:
            v1->current_mobd_lookup_idx = 0;
            break;
        }
        v1->script->event_handler = EventHandler_Hut;
        v1->mode = entity_mode_407F00_hut;
    }
    (v1->mode)(v1);
}