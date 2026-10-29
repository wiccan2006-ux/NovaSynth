#pragma once
#include "Presets.h"
namespace nova {
inline int presetRole(int i){int c=presetCategory(i);if(c==0||c==10||c==27)return 0;if(c==2||c==13||c==14||c==15||c==29)return 3;if(c==3||c==9||c==5||c==17||c==25)return 2;if(c==4||c==11||c==18||c==28)return 4;if(c==7||c==19)return 5;if(i>=364&&i<764){int shape=(i-364)%10;if(shape==3)return 0;if(shape==4||shape==8)return 3;if(shape==1)return 4;if(shape==2||shape==5)return 2;}return 1;}
inline const char* roleName(int role){static const char* names[]={"Bajo","Lead","Pluck / Campana","Pad / Textura","Teclas","Efecto"};return names[std::clamp(role,0,5)];}
inline std::string presetTags(int i){auto p=preset(i);std::string tags=roleName(presetRole(i));tags+=p.cutoff<1600?" oscuro":" brillante";tags+=p.attack>.18f?" suave":" marcado";tags+=p.release>1.1f?" largo":" corto";if(p.lfoDepth>.15f||p.movement>.15f)tags+=" evolutivo";if(p.drive>.2f||p.character>.3f)tags+=" agresivo";if(p.unison||p.chorus>.1f||p.movement>.3f)tags+=" amplio";return tags;}
}
