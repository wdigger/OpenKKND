#include "src/kknd.h"

#include "src/_unsorted_functions.h"
#include "src/_unsorted_data.h"

#include "src/Random.h"
#include "src/Render.h"
#include "src/Script.h"
#include "src/ScriptEvent.h"
#include "src/Entity.h"


//----- (00401110) --------------------------------------------------------
void entity_401110_aircraft(Entity *a1)
{
    Entity *v1; // ebx@1
    EntityTurret *v2; // esi@2
    Sprite *v3; // eax@3
    Script *v4; // eax@3
    UnitAttachmentPoint *v5; // edx@3

    v1 = a1;
    if (a1->stats->attach)
    {
        v2 = (EntityTurret *)script_create_local_object(a1->script, 56);
        if (v2)
        {
            memset(v2, 0, sizeof(EntityTurret));
            v3 = sprite_create_scripted(
                v1->stats->attach->mobd_id,
                v1->sprite,
                UNIT_AttachHandler_DockPoint,
                SCRIPT_FUNCTION,
                v1->stru60.ptr_0);
            v2->turret_sprite = v3;
            v3->drawjob->on_update_handler = (void(*)(void *, DrawJob *))drawjob_update_handler_448580_entity_aircraft_turret;
            v2->turret_sprite->drawjob->flags |= 0x10000000u;
            v2->turret_sprite->drawjob->job_details.palette = per_player_sprite_palettes[player_sprite_color_by_player_side[v1->player_side]];
            v4 = v2->turret_sprite->script;
            v2->sprite_task = v4;
            v4->param = v2;
            v2->turret_sprite->param = v2;
            v2->turret_sprite->z_index = 0;
            v2->entity = v1;
            v2->mobd_lookup_id = v1->current_mobd_lookup_idx;
            v5 = v1->stats->attach;
            v2->handler = EntityTowerAttachment_handler_4010E0;
            v2->stats_attachment_point = v5;
            v1->turret = v2;
            v2->sprite_task->event_handler = MessageHandler_448EC0;
        }
        v1->sprite->drawjob->flags &= 0xBFFFFFFF;
    }
}

//----- (004011F0) --------------------------------------------------------
void EventHandler_Aircraft(Script *receiver, Script *sender, enum SCRIPT_EVENT event, void *param)
{
    Entity *v4; // esi@1
    int v5; // eax@7
    int v6; // eax@8

    v4 = (Entity *)receiver->param;
    if (!v4->destroyed)
    {
        switch (event)
        {
        case EVT_MSG_1511_sidebar_click_category:
            entity_410CB0_event1511(v4);
            break;
        case EVT_SHOW_UI_CONTROL:
            entity_410CD0_eventTextString(v4);
            break;
        case EVT_MSG_SHOW_UNIT_HINT:
            entity_show_hint(v4);
            break;
        case EVT_MSG_DAMAGE:
            if (param)
            {
                v5 = v4->hitpoints;
                if (v5 > 0)
                {
                    v6 = v5 - *((_WORD *)param + 71);
                    v4->hitpoints = v6;
                    if (v6 <= 0)
                    {
                        script_445370_yield_to_main_thread(v4->script, 0x80000000, 1);
                        v4->hitpoints = 0;
                        v4->mode = entity_mode_401660_aircraft;
                        v4->destroyed = 1;
                    }
                }
            }
            entity_410710_status_bar(v4);
            break;
        default:
            return;
        }
    }
}

//----- (004012D0) --------------------------------------------------------
void UNIT_Handler_Aircraft(Script *a1)
{
    Script *v1; // edi@1
    Entity *v2; // esi@1
    DrawJobDetails *v3; // ebx@2
    stru31 *v4; // eax@2
    Sprite *v5; // eax@7
    unsigned int v6; // edi@8
    int v7; // edi@8
    int v8; // ebp@9
    unsigned int v9; // edi@11
    Sprite *v10; // edx@14
    int a1a; // [sp+Ch] [bp+4h]@2

    v1 = a1;
    v2 = (Entity *)a1->param;
    if (!v2)
    {
        v3 = &_47A010_mapd_item_being_drawn[0]->draw_job->job_details;
        a1a = kknd_rand_debug(__FILE__, __LINE__) & 3;
        v2 = entity_list_create(v1);
        v4 = stru31_list_free_pool;                 // INLINED entity_401070_stru31
        if (stru31_list_free_pool)
            stru31_list_free_pool = stru31_list_free_pool->next;
        else
            v4 = 0;
        if (v4)
        {
            v4->param__entity__int = v2;
            v4->next = stru31_list_477300;
            v4->prev = (stru31 *)&stru31_list_477300;
            stru31_list_477300->prev = v4;
            stru31_list_477300 = v4;
        }
        v2->sprite->field_88_unused = 1;
        v5 = v2->sprite;
        v2->sprite_x = v5->x;
        v2->sprite_y = v5->y;
        if (a1a > 1)
        {
            v9 = kknd_rand_debug(__FILE__, __LINE__);
            v8 = (v9 % (render_call_draw_handler_mode2(v3) - 64) + 32) << 8;
            if (a1a & 1)
                v7 = -8192;
            else
                v7 = (render_call_draw_handler_mode1(v3) + 32) << 8;
        }
        else
        {
            v6 = kknd_rand_debug(__FILE__, __LINE__);
            v7 = (v6 % (render_call_draw_handler_mode1(v3) - 64) + 32) << 8;
            if (a1a & 1)
                v8 = -8192;
            else
                v8 = (render_call_draw_handler_mode2(v3) + 32) << 8;
        }
        v2->sprite->field_88_unused = 1;
        v2->sprite->x = v7;
        v2->sprite->field_88_unused = 1;
        v2->sprite->y = v8;
        v2->sprite->drawjob->on_update_handler = (void(*)(void *, DrawJob *))drawjob_update_handler_448510_aircraft;
        v2->sprite->z_index = 46080;
        v2->script->event_handler = EventHandler_Aircraft;
        v10 = v2->sprite;
        v2->mode = entity_mode_401800_aircraft;
        v10->x_speed = 0;
        v2->sprite->y_speed = 0;
        v2->_128_spawn_param = (void *)2;
        entity_401110_aircraft(v2);
        entity_410CF0_aircraft(v2);
    }
    (v2->mode)(v2);
}

//----- (00401480) --------------------------------------------------------
void entity_mode_401480_aircraft(Entity *a1)
{
    Entity *v1; // edi@1
    Sprite *v2; // esi@1
    Sprite *v3; // eax@2
    Script *v4; // eax@5

    v1 = a1;
    v2 = a1->sprite;
    if (v2->z_index <= 0)
    {
        v2->z_index = 255;
        sprite_408800_play_sound(v2, SOUND_GENERIC_PROJECTILE_DMG, _4690A8_unit_sounds_volume, 0);
        v2->mobd_id = MOBD_EXPLOSIONS;
        sprite_load_mobd(v2, SPRITE_EXPLOSION_MOBD_OFFSET_BIG);
        v2->_60_mobd_anim_speed = 0x20000000;
        v2->z_index = 768;
        sprite_40D8B0_dmg(v2, 64);
        v4 = v1->script;
        v1->mode = entity_mode_401600_aircraft_stru31;
        script_445370_yield_to_main_thread(v4, 0x80000000, 20);
    }
    else
    {
        v3 = entity_4390F0_add_explosion(a1);
        if (v3)
        {
            v3->z_index = v1->sprite->z_index + 256;
            v3->drawjob->on_update_handler = (void(*)(void *, DrawJob *))drawjob_update_handler_448510_aircraft;
        }
        script_445370_yield_to_main_thread(v1->script, 0x80000000, 20);
    }
}

//----- (00401530) --------------------------------------------------------
void entity_401530_aircraft(Entity *a1, int a2)
{
    Entity *v2; // esi@1
    int v3; // edi@1
    Script *v4; // eax@1
    EntityTurret *v5; // eax@1
    void(*v6)(Entity *); // eax@4
    Script *v7; // ST00_4@5

    v2 = a1;
    v3 = a2;
    v4 = a1->script;
    a1->destroyed = 1;
    v4->field_24 &= 0xEFFFFFFF;
    script_trigger_event(a1->script, EVT_SHOW_UI_CONTROL, 0, task_mobd17_cursor);
    v5 = v2->turret;
    if (v5)
    {
        sprite_list_remove(v5->turret_sprite);
        script_deinit(v2->turret->sprite_task);
        script_free_local_object(v2->script, v2->turret);
    }
    script_trigger_event_group(v2->script, EVT_SHOW_UI_CONTROL, v2, SCRIPT_TYPE_39030);
    v2->script->script_type = SCRIPT_TYPE_INVALID;
    v2->sprite->z_speed = 0;
    v2->sprite->z_speed_limit = 512;
    v2->sprite->z_speed_factor_2 = -10;
    if (v3)
    {
        v6 = v2->mode;
        v2->mode = entity_mode_401480_aircraft;
        v2->mode_return = v6;
    }
    else
    {
        v7 = v2->script;
        v2->mode = entity_mode_401600_aircraft_stru31;
        script_445370_yield_to_main_thread(v7, 0x80000000, 60);
    }
}

//----- (00401600) --------------------------------------------------------
void entity_mode_401600_aircraft_stru31(Entity *a1)
{
    stru31 *v1; // eax@1
    Entity *v2; // esi@1
    enum UNIT_ID v3; // ecx@1

    v1 = stru31_list_477300;
    v2 = a1;
    v3 = a1->unit_id;
    if ((stru31 **)stru31_list_477300 != &stru31_list_477300)
    {
        while (v1->param__entity__int->entity_id != v3)
        {
            v1 = v1->next;
            if ((stru31 **)v1 == &stru31_list_477300)
                goto LABEL_6;
        }
        v1->next->prev = v1->prev;
        v1->prev->next = v1->next;
        v1->next = stru31_list_free_pool;
        stru31_list_free_pool = v1;
    }
LABEL_6:
    sprite_list_remove(v2->sprite);
    script_yield(v2->script);
    entity_list_remove(v2);
}

//----- (00401660) --------------------------------------------------------
void entity_mode_401660_aircraft(Entity *a1)
{
    entity_401530_aircraft(a1, 1);
}

//----- (00401670) --------------------------------------------------------
void entity_401670_aircraft(Entity *a1)
{
    entity_401530_aircraft(a1, 0);
}

//----- (00401680) --------------------------------------------------------
void entity_401680(Entity *a1)
{
    a1->sprite_x = (render_call_draw_handler_mode1(&_47A010_mapd_item_being_drawn[0]->draw_job->job_details) + 32) << 8;
}

//----- (004016B0) --------------------------------------------------------
void entity_mode_4016B0_aircraft(Entity *a1)
{
    Entity *v1; // esi@1
    int v2; // eax@1
    UnitDamageSource *v3; // edi@4
    Sprite *v4; // eax@6
    int v5; // edx@6
    Sprite *v6; // ebx@6
    DrawJob *v7; // edx@8
    Script *v8; // eax@8
    int v9; // eax@9

    v1 = a1;
    v2 = a1->_12C_prison_bunker_spawn_type;
    if (v2)
    {
        a1->_12C_prison_bunker_spawn_type = v2 - 1;
    }
    else if (a1->_134_param__unitstats_after_mobile_outpost_plant)
    {
        v3 = a1->stats->dmg_source;
        if (v3 && _47C048_unit_bomberdmg < 200)
        {
            ++_47C048_unit_bomberdmg;
            v4 = sprite_create_scripted(v3->mobd_id, a1->sprite, v3->dmg_handler, SCRIPT_FUNCTION, a1->stru60.ptr_0);
            v5 = v3->mobd_offset;
            v6 = v4;
            if (v5 != -1)
                sprite_load_mobd(v4, v5);
            v7 = v6->drawjob;
            v6->z_index = v1->sprite->z_index;
            v7->on_update_handler = (void(*)(void *, DrawJob *))drawjob_update_handler_448580_entity_aircraft_turret;
            v8 = v6->script;
            v6->param = v3;
            v8->param = 0;
            v6->_80_entity__stru29__sprite__initial_hitpoints = v1;
            v6->field_84 = v1->entity_id;
            v6->field_8C_infantry_damage = LOWORD_HEXRAYS(v3->damage_infantry)
                + (v3->damage_infantry
                    * _465610_damage_multipliers[v1->_98_465610_accuracy_dmg_bonus_idx] >> 8);
            v6->field_8E_vehicle_damage = LOWORD_HEXRAYS(v3->damage_vehicle)
                + (v3->damage_vehicle
                    * _465610_damage_multipliers[v1->_98_465610_accuracy_dmg_bonus_idx] >> 8);
            v6->field_90_building_damage = LOWORD_HEXRAYS(v3->damage_building)
                + (v3->damage_building
                    * _465610_damage_multipliers[v1->_98_465610_accuracy_dmg_bonus_idx] >> 8);
        }
        v9 = v1->_134_param__unitstats_after_mobile_outpost_plant;
        v1->_12C_prison_bunker_spawn_type = 15;
        v1->_134_param__unitstats_after_mobile_outpost_plant = v9 - 1;
    }
    else
    {
        a1->mode = entity_mode_401800_aircraft;
    }
}
// 47C048: using guessed type int _47C048_unit_bomberdmg;

//----- (004017E0) --------------------------------------------------------
void entity_4017E0(Entity *a1)
{
    script_445370_yield_to_main_thread(a1->script, 0x80000000, 40);
}

//----- (00401800) --------------------------------------------------------
void entity_mode_401800_aircraft(Entity *a1)
{
    Entity *v1; // esi@1
    char v2; // al@1
    int v3; // ebx@1
    Sprite *v4; // ecx@1
    int v5; // ebp@1
    int v6; // eax@1
    int v7; // ecx@1
    int v8; // edi@1
    int v9; // ebp@1
    int v10; // eax@5

    v1 = a1;
    v2 = _42D560_get_mobd_lookup_id_rotation(a1->sprite_x - a1->sprite->x, a1->sprite_y - a1->sprite->y);
    entity_advance_mobd_rotation(&v1->current_mobd_lookup_idx, v2 & 0xF0, 5);
    sprite_4273B0_load_mobd_item_sound(
        v1->sprite,
        v1->stats->mobd_lookup_offset_move,
        _47D3C4_entity_mobd_lookup_ids[v1->current_mobd_lookup_idx + 1]);
    v1->sprite->x_speed = v1->stats->speed * _4731A8_speeds[__47CFC4_mobd_lookup_speeds[v1->current_mobd_lookup_idx + 1]] >> 6;
    v1->sprite->y_speed = -(v1->stats->speed * _4731A8_speeds[8 + __47CFC4_mobd_lookup_speeds[v1->current_mobd_lookup_idx + 1]]) >> 6;
    v3 = v1->sprite_x;
    v4 = v1->sprite;
    v5 = v1->sprite_y;
    v6 = v4->x >> 13;
    v7 = v4->y >> 13;
    v8 = (v5 - 2048) >> 13;
    v9 = (v5 + 2048) >> 13;
    if (v6 >= (v3 - 2048) >> 13 && v6 <= (v3 + 2048) >> 13 && v7 >= v8 && v7 <= v9)
    {
        v10 = (int)v1->_128_spawn_param - 1;
        v1->_128_spawn_param = (void *)v10;
        if (v10 >= 0)
        {
            if (v10)
            {
                v1->_12C_prison_bunker_spawn_type = 0;
                v1->_134_param__unitstats_after_mobile_outpost_plant = 1;
                v1->mode = entity_mode_4016B0_aircraft;
            }
            else
            {
                script_445370_yield_to_main_thread(v1->script, 0x80000000, 40);
            }
        }
        else
        {
            if (v10 == -2)
                script_445370_yield_to_main_thread(v1->script, 0x80000000, 40);
            if (v1->_128_spawn_param == (void *)-1)
                v1->sprite_x = (render_call_draw_handler_mode1(&_47A010_mapd_item_being_drawn[0]->draw_job->job_details) + 32) << 8;
            else
                entity_401530_aircraft(v1, 0);
        }
    }
}