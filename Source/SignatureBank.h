#pragma once
#include "Parameters.h"
#include <array>
namespace nova {
constexpr int signaturePresetCount=48,signaturePresetStart=764;
struct SignaturePatch {
    const char* name;const char* description;int engine,palette;
    float scan,cutoff,attack,decay,sustain,release,rate,motion,colour,delay,reverb,velocity;
};
inline const std::array<SignaturePatch,48>& signaturePatches(){
 static const std::array<SignaturePatch,48> patches={{
 {"Voces de mercurio","Formantes sinteticos que respiran; toca acordes suaves.",2,1,.12f,2200,.35f,1.2f,.72f,2.4f,.17f,.62f,.08f,.12f,.23f,.6f},
 {"Coro de bolsillo","Vocal compacto con ataque lento y movimiento estrecho.",2,1,.58f,1600,.12f,.7f,.8f,.65f,.42f,.22f,.12f,.08f,.16f,.8f},
 {"Lenguaje submarino","Vocal oscuro con barrido amplio y cola sumergida.",2,1,.08f,480,.65f,2.1f,.67f,3.6f,.09f,.9f,.03f,.26f,.4f,.4f},
 {"Silabas de luz","Pluck vocal corto; cada golpe abre el filtro.",2,1,.7f,3200,.002f,.23f,.08f,.24f,3.4f,.45f,.2f,.21f,.12f,1},
 {"Vidrio que canta","Cruce vocal brillante, sostenido y limpio.",2,1,.32f,7200,.025f,1.5f,.42f,1.8f,.31f,.36f,.02f,.17f,.26f,.55f},
 {"Radar humano","Pulso de filtro vocal con movimiento rapido.",2,1,.46f,1200,.008f,.48f,.64f,.4f,5.2f,.82f,.27f,.09f,.08f,.7f},
 {"Vocal de ceniza","Ataque blando y formantes cerrados con grano.",2,1,.8f,670,.08f,.9f,.4f,1.1f,.13f,.33f,.4f,.11f,.21f,.9f},
 {"Ultima llamada","Lead vocal seco, directo y sensible a la fuerza.",2,1,.22f,4800,.004f,.36f,.75f,.16f,1.7f,.18f,.15f,.05f,.06f,1},
 {"Cristal elastico","Parciales pares que se transforman en un destello.",2,2,.05f,11000,.002f,1.4f,.02f,1.9f,.23f,.7f,.03f,.24f,.29f,.7f},
 {"Hielo encendido","Campana armonica brillante con cuerpo saturado.",2,2,.76f,8400,.001f,.64f,.03f,.75f,.7f,.28f,.48f,.1f,.19f,.9f},
 {"Prisma liquido","Tecla cristalina con espectro que cambia al sostener.",2,2,.28f,5600,.018f,.8f,.33f,1.2f,.19f,.88f,.08f,.19f,.24f,.55f},
 {"Polvo de diamante","Pluck fino y corto con repeticiones abiertas.",2,2,.52f,12500,.001f,.16f,0,.32f,4.1f,.4f,.12f,.33f,.14f,1},
 {"Catedral pixel","Acordes cristalinos de entrada lenta y cola amplia.",2,2,.1f,4200,1.2f,2.4f,.62f,4.5f,.07f,.74f,.02f,.22f,.46f,.35f},
 {"Espejo roto","Tecla seca con parciales densos y ataque marcado.",2,2,.91f,9800,.003f,.39f,.12f,.18f,2.1f,.15f,.33f,0,.05f,.8f},
 {"Gotas orbitales","Gotas armonicas con ecos de distinta profundidad.",2,2,.35f,2800,.006f,.45f,.02f,.58f,.55f,.56f,.06f,.39f,.21f,.65f},
 {"Corona de sal","Pad brillante que abre lentamente sus armonicos.",2,2,.63f,1800,.8f,1.8f,.78f,2.7f,.12f,.93f,.16f,.13f,.32f,.45f},
 {"Cuerda de carbono","Cuerda pulsada oscura con salida corta.",3,0,.12f,2400,.001f,.8f,.28f,.35f,.3f,.08f,.16f,.12f,.17f,.9f},
 {"Arco electrico","Espectro de cuerda sintetica con entrada progresiva.",2,3,.4f,4200,.28f,1.4f,.76f,1.3f,.21f,.56f,.18f,.09f,.22f,.65f},
 {"Junco de neon","Soplo resonante con articulacion suave.",4,0,.2f,3900,.09f,.65f,.73f,.28f,.6f,.2f,.05f,.12f,.17f,.9f},
 {"Madera magnetica","Pulsacion hueca con cola seca y tactil.",3,0,.67f,1300,.001f,.46f,.12f,.19f,1.1f,.16f,.3f,.04f,.08f,1},
 {"Niebla de cuerdas","Pad armonico que se ensancha sin desafinar.",2,3,.09f,1700,1.6f,2.3f,.81f,4.1f,.08f,.86f,.04f,.2f,.43f,.5f},
 {"Canal de aire","Lead de soplo resonante con ataque definido.",4,0,.64f,7300,.016f,.32f,.66f,.14f,2.7f,.36f,.21f,.08f,.1f,1},
 {"Resina digital","Tecla rugosa de caida media y fuerza expresiva.",2,3,.78f,3100,.003f,.73f,.17f,.46f,.78f,.31f,.44f,.16f,.14f,.95f},
 {"Pua fantasma","Pulsacion casi senoidal con ecos suspendidos.",3,0,.34f,860,.002f,1.1f,.42f,1.5f,.14f,.34f,.03f,.31f,.34f,.55f},
 {"Motor de tinta","Bajo con muescas armonicas y barrido de filtro.",2,4,.15f,630,.002f,.28f,.62f,.14f,2.2f,.63f,.32f,.02f,.03f,.9f},
 {"Bajo de porcelana","Bajo hueco y percusivo con armonicos limpios.",2,4,.68f,1200,.001f,.16f,.18f,.08f,.8f,.19f,.04f,0,.02f,1},
 {"Gravedad violeta","Bajo sostenido con evolucion lenta y cuerpo denso.",2,4,.3f,410,.025f,1.5f,.85f,.32f,.12f,.89f,.46f,.03f,.04f,.6f},
 {"Mordida holografica","Bajo corto de filtro abierto y grano fuerte.",2,4,.92f,3400,.001f,.21f,.3f,.09f,6.4f,.42f,.7f,.01f,.02f,1},
 {"Talon de goma","Bajo elastico con barrido descendente de timbre.",2,4,.05f,780,.004f,.48f,.15f,.2f,1.3f,.77f,.21f,.06f,.04f,.85f},
 {"Asfalto mojado","Bajo oscuro que responde a la intensidad MIDI.",2,4,.54f,290,.009f,.36f,.72f,.18f,.36f,.3f,.55f,0,.03f,1},
 {"Pulso de obsidiana","Bajo ritmico con filtro en movimiento.",2,4,.44f,1650,.001f,.6f,.64f,.15f,7.3f,.92f,.38f,.07f,.05f,.7f},
 {"Sub de origami","Bajo doblado por armonicos, sin modulacion de tono.",0,0,.21f,520,.003f,.34f,.58f,.13f,.65f,.24f,.4f,0,.02f,.8f},
 {"Memoria refractada","Tecla con huecos espectrales y eco suave.",2,5,.17f,4100,.005f,.87f,.22f,.74f,.18f,.71f,.09f,.27f,.23f,.8f},
 {"Piano de plasma","Tecla sintetica brillante con ataque firme.",2,5,.84f,9400,.001f,.56f,.14f,.29f,1.2f,.23f,.22f,.07f,.14f,1},
 {"Celesta inversa","Tecla de entrada creciente y final cristalino.",2,5,.42f,7600,.34f,.28f,.05f,1.2f,.4f,.49f,.03f,.24f,.3f,.6f},
 {"Tinta sobre seda","Tecla suave con color cerrado y tacto delicado.",2,5,.08f,1150,.018f,.98f,.26f,.9f,.11f,.37f,.12f,.09f,.18f,1},
 {"Terminal nocturna","Lead de muesca espectral y ecos breves.",2,5,.64f,3100,.007f,.42f,.73f,.21f,3.7f,.62f,.31f,.16f,.09f,.8f},
 {"Chispa de papel","Pluck delicado con filtro sensible al golpe.",2,5,.27f,6200,.001f,.12f,.02f,.19f,2.4f,.16f,.07f,.18f,.12f,1},
 {"Acorde fosil","Tecla oscura de ataque lento y decaimiento largo.",2,5,.95f,820,.11f,2.6f,.18f,2.1f,.06f,.81f,.25f,.14f,.29f,.7f},
 {"Luz entre persianas","Pad de espectro estrecho con apertura lenta.",2,5,.36f,2400,.73f,1.7f,.69f,3.2f,.16f,.94f,.04f,.22f,.38f,.4f},
 {"Maquina de luciernagas","Textura pulsante armonica con imagen cambiante.",2,6,.13f,5600,.08f,.72f,.67f,1.1f,4.7f,.94f,.18f,.28f,.25f,.65f},
 {"Terciopelo cuantico","Pad oscuro que recorre parciales espaciados.",2,6,.77f,1100,1.4f,2.7f,.82f,4.8f,.05f,.78f,.04f,.17f,.42f,.4f},
 {"Reloj sumergido","Pluck hueco con eco largo y pulso lento.",2,6,.4f,1800,.002f,.33f,.06f,.8f,.72f,.67f,.13f,.4f,.3f,.8f},
 {"Fotones en fuga","Lead rapido con barrido de espectro acentuado.",2,6,.05f,8300,.003f,.24f,.51f,.17f,8.6f,.85f,.46f,.11f,.09f,1},
 {"Horizonte plegado","Textura que abre lentamente brillo y anchura.",2,6,.54f,3300,.92f,2.1f,.74f,3.6f,.09f,1,.08f,.25f,.39f,.5f},
 {"Ritual de silicio","Pulso grave con grano y eco contenido.",2,6,.88f,780,.006f,.64f,.59f,.34f,3.1f,.73f,.63f,.14f,.11f,.9f},
 {"Mapa de estrellas","Destellos sostenidos con ecos y espectro abierto.",2,6,.24f,12000,.019f,1.2f,.39f,1.8f,.27f,.57f,.02f,.35f,.31f,.6f},
 {"Ultimo holograma","Pad brillante de entrada suave y final largo.",2,6,.66f,6700,.48f,1.6f,.7f,2.9f,.14f,.91f,.21f,.19f,.37f,.55f}
 }};return patches;
}
inline Params signaturePreset(int index){
 index=std::clamp(index,0,47);const auto& s=signaturePatches()[index];Params p;
 p.engine=s.engine;p.palette=s.palette;p.fm=s.scan;p.cutoff=s.cutoff;p.attack=s.attack;p.decay=s.decay;p.sustain=s.sustain;p.release=s.release;
 p.lfoRate=s.rate;p.movement=s.motion;p.character=s.colour;p.delay=s.delay;p.reverb=s.reverb;p.velocityTone=s.velocity;
 p.mix=.32f;p.detune=0;p.fineTune=0;p.unison=false;p.sub=0;p.noise=s.engine==4?.025f:0;p.envAmount=index>=24&&index<32?2.1f:.55f;
 p.resonance=s.engine==3?.82f:s.engine==4?.71f:.12f;p.master=index>=24&&index<32?-13.f:-9.f;
 static constexpr float levelTrim[]={0.719f,0.794f,-0.936f,6.000f,2.965f,1.099f,1.954f,1.635f,-1.125f,4.175f,1.895f,6.000f,-4.052f,6.000f,4.686f,-2.511f,6.000f,-4.557f,-5.573f,6.000f,-4.304f,-4.829f,3.037f,5.764f,3.973f,6.000f,1.895f,6.000f,6.000f,4.340f,4.353f,2.184f,-0.638f,1.740f,4.178f,-0.452f,-4.697f,6.000f,-5.771f,-5.175f,-4.814f,-5.513f,2.567f,-3.378f,-6.000f,-5.711f,-2.241f,-6.000f};
 p.master=std::clamp(p.master+levelTrim[index],-30.f,0.f);
 p.spread=.85f;p.room=s.release>2?.8f:.48f;p.delayTime=index%3==0?.375f:index%3==1?.25f:.1875f;p.feedback=.28f;
 if(s.engine==0){p.waveA=0;p.waveB=3;p.sub=.4f;p.transpose=12;}
 return p;
}
inline const char* signatureFamily(int i){static const char* n[]={"Vocal / Formantes","Cristal / Percusion","Organico / Hibridos","Bajo / Materia","Teclas / Refraccion","Textura / Movimiento"};return n[std::clamp(i/8,0,5)];}
}
