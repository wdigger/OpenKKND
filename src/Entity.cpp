#include "src/kknd.h"

#include "src/_unsorted_functions.h"
#include "src/_unsorted_data.h"

#include "src/Entity.h"
#include "src/Script.h"



Entity *entity_list_get()
{
    if (entity_list_free_pool)
    {
        Entity *entity = entity_list_free_pool;
        entity_list_free_pool = entity_list_free_pool->next;

        return entity;
    }
    return nullptr;
}

Entity *entity_list_end()
{
    return (Entity *)&entity_list_head;
}



bool entity_is_tower(Entity *entity)
{
    return entity->unit_id >= UNIT_STATS_SURV_GUARD_TOWER && entity->unit_id <= UNIT_STATS_MUTE_ROTARY_CANNON;
}

bool entity_is_moveable(Entity *entity)
{
    return entity->unit_id <= UNIT_STATS_MUTE_MISSILE_CRAB || entity->unit_id >= UNIT_STATS_GORT;

}

bool entity_is_attacker(Entity *entity)
{
    return
        entity_is_moveable(entity)
        && (entity->unit_id < UNIT_STATS_SURV_MOBILE_DERRICK || entity->unit_id > UNIT_STATS_TANKER_CONVOY)
        && (entity->unit_id < UNIT_STATS_SURV_TECHNICIAN || entity->unit_id > UNIT_STATS_MUTE_MEKANIK)
        || entity_is_tower(entity);
}



bool is_21st_century(UNIT_ID unit_id)
{
    return unit_id >= UNIT_STATS_GORT && unit_id <= UNIT_STATS_MECH;
}

bool entity_is_21st_century(Entity *entity)
{
    return is_21st_century(entity->unit_id);
}



int entity_get_dx(Entity *entity)
{
    if (entity->stats->field_30_hp_regen_condition)
        return entity_40F0A0_get_dx(entity, entity->field_A4);
    else
        return entity->stats->field_4C != 128 ? 7424 : 4096;
}

int entity_get_dy(Entity *entity)
{
    if (entity->stats->field_30_hp_regen_condition)
        return entity_40F100_get_dy(entity, entity->field_A4);
    else
        return entity->stats->field_4C != 128 ? 7424 : 4096;
}


int entity_transform_x(Entity *entity, int x)
{
    return entity_get_dx(entity) + (x & 0xFFFFE000);
}

int entity_transform_y(Entity *entity, int y)
{
    return entity_get_dy(entity) + (y & 0xFFFFE000);
}


//----- (0041A270) --------------------------------------------------------
void entity_move(Entity *a1, _47CAF0_task_attachment1_move_task *a2)
{
    int v4; // eax@5
    int v5; // eax@9
    int v6; // eax@11
    Entity *v8; // ecx@13
    int v9; // edx@13

    if (a1->player_side == a2->player_side)
    {
        v4 = a1->field_DC == 1
            && !((a2->dst_x ^ a1->sprite_width_2) & 0xFFFFE000)
            && !((a2->dst_y ^ a1->sprite_height_2) & 0xFFFFE000);
        if (!v4 || a1->entity_8)
        {
            v5 = a2->dst_x;
            if (v5 >= 0 && v5 < _4793F8_map_width << 13)
            {
                v6 = a2->dst_y;
                if (v6 >= 0 && v6 < _478AAC_map_height << 13)
                {
                    a1->stru224.field_54 = 0;
                    a1->stru224.field_50 = 0;
                    script_445370_yield_to_main_thread(a1->script, 0x80000000, 1);
                    v8 = a1->_E0_current_attack_target;
                    v9 = a1->_E0_current_attack_target_entity_id;
                    a1->field_DC = 1;
                    a1->_E4_entity = v8;
                    a1->_E4_entity_id = v9;
                    a1->_E0_current_attack_target = 0;
                    a1->_134_param__unitstats_after_mobile_outpost_plant = 600;
                    a1->sprite_width_2 = entity_transform_x(a1, a2->dst_x);
                    a1->sprite_height_2 = entity_transform_y(a1, a2->dst_y);
                    entity_414440_boxd(a1, &a1->sprite_width_2, &a1->sprite_height_2);
                    a1->entity_8 = 0;
                    entity_40DF50_boxd(a1, 1);
                    a1->mode = entity_mode_move_attack;
                }
            }
        }
    }
}


//----- (00419F00) --------------------------------------------------------
void entity_attack(Entity *a1, _47CAF0_task_attachment1_attack_task *param)
{
    Entity *v3; // esi@1
    enum PLAYER_SIDE v4; // ecx@1
    Entity *v5; // edx@2
    enum PLAYER_SIDE v6; // eax@3
    int v7; // eax@7

    v3 = a1;
    v4 = a1->player_side;
    if (v4 == param->player_side)
    {
        v5 = param->target;
        if (v5 == v3->_E0_current_attack_target)
        {
            v3->_E4_entity = 0;
            v3->_134_param__unitstats_after_mobile_outpost_plant = 600;
            if (v3->entity_8 != v3->_E0_current_attack_target)
                v3->entity_8 = 0;
        }
        else
        {
            v6 = v5->player_side;
            if (v6 && v6 != v4 && !is_enemy(v4, v5))
            {
                if (!single_player_game)
                {
                    v7 = param->target->player_side;
                    if (player_side == v7 || player_side == v3->player_side)
                    {
                        sprintf(
                            byte_479B00,
                            aAllegianceBroken,
                            netz_47A740[v7 + 1].player_name,
                            netz_47A740[v3->player_side + 1].player_name);
                        show_message_ex(0, byte_479B00);
                    }
                }
                game_globals_cpu[param->target->player_side].cash[v3->player_side] = 0;
                game_globals_cpu[v3->player_side].cash[param->target->player_side] = 0;
            }
            v3->stru224.field_54 = 0;
            v3->stru224.field_50 = 0;
            script_445370_yield_to_main_thread(v3->script, 0x80000000, 1);
            v3->field_DC = 2;
            v3->_E0_current_attack_target = param->target;
            v3->_E0_current_attack_target_entity_id = param->target->entity_id;
            v3->_E4_entity = 0;
            v3->_134_param__unitstats_after_mobile_outpost_plant = 600;
            v3->entity_8 = 0;
            v3->mode = entity_mode_move_attack;
        }
    }
}