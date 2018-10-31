#include "../Config.h"
#include "Glow.h"
#include "../Memory.h"

void renderGlow()
{
    constexpr auto& glow = config.glow;

    for (int i = 0; i < memory.glowObjectManager->glowObjectDefinitions.size; i++) {
        GlowObjectDefinition& glowobject = memory.glowObjectManager->glowObjectDefinitions[i];

        if (memory.glowObjectManager->glowObjectDefinitions.elements->isUnused() || !glowobject.entity)
            continue;

        if (glowobject.entity->IsDormant())
            continue;

        glowobject.renderWhenOccluded = false;

        if (glow.enabled) {

            glowobject.alpha = glow.alpha;
            glowobject.renderWhenUnoccluded = false;
            glowobject.glowStyle = glow.style;
            glowobject.fullBloomRender = false;
            glowobject.fullBloomStencilTestValue = 0;
            glowobject.bloomAmount = glow.thickness;

            switch (glowobject.entity->GetClientClass()->m_ClassID) {
            case ClassId_CCSPlayer:
                if (glow.players) {
                    bool isEnemy = reinterpret_cast<BaseEntity*>(glowobject.entity)->getTeamNumber() != (*memory.localPlayer)->getTeamNumber();

                    if (isEnemy) {
                        glowobject.glowColor = glow.enemiesColor;
                        glowobject.renderWhenOccluded = true;
                    }
                    else if (!glow.enemiesOnly) {
                        glowobject.glowColor = glow.alliesColor;
                        glowobject.renderWhenOccluded = true;
                    }
                }
                break;
            case ClassId_CChicken:
                if (glow.chickens) {
                    glowobject.glowColor = glow.chickensColor;
                    glowobject.renderWhenOccluded = true;
                }
                break;
            case ClassId_CC4:
                if (glow.C4) {
                    glowobject.glowColor = glow.C4Color;
                    glowobject.renderWhenOccluded = true;
                }
                break;
            case ClassId_CPlantedC4:
                if (glow.plantedC4) {
                    glowobject.glowColor = glow.plantedC4Color;
                    glowobject.renderWhenOccluded = true;
                }
                break;
            default:
                if (glowobject.entity->isWeapon()) {
                    if (glow.weapons) {
                        glowobject.glowColor = glow.weaponsColor;
                        glowobject.renderWhenOccluded = true;
                    }
                }
            }
        }
    }
}