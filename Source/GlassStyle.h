#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace nova::glass {
// iOS-inspired internal material; the cached blurred scene belongs to Nova.
// It never captures the desktop or other applications.
inline void halo(juce::Graphics& g,juce::Rectangle<float> area,juce::Colour colour,float opacity=.22f){
    if(area.isEmpty())return;juce::Graphics::ScopedSaveState state(g);g.addTransform(juce::AffineTransform::scale(area.getWidth(),area.getHeight()).translated(area.getX(),area.getY()));
    juce::ColourGradient glow(colour.withAlpha(opacity),.5f,.5f,colour.withAlpha(0.f),1.f,.5f,true);glow.addColour(.35,colour.withAlpha(opacity*.36f));glow.addColour(.7,colour.withAlpha(opacity*.09f));g.setGradientFill(glow);g.fillEllipse(0.f,0.f,1.f,1.f);
}
inline const juce::Image& backdrop(juce::Colour accent){
    struct Cached {juce::uint32 key=0;juce::Image image;};static std::array<Cached,7> cache;
    const juce::uint32 palette[]={0xff40a9ff,0xffffcf44,0xff4be291,0xffb789ff,0xffff4eb6,0xfff5fbff,0xffff8b24};int index=0;for(int i=0;i<7;++i)if(accent.getARGB()==palette[i])index=i;
    auto& entry=cache[index];if(entry.key==accent.getARGB()&&entry.image.isValid())return entry.image;
    juce::Image source(juce::Image::ARGB,280,200,true);{
        juce::Graphics g(source);g.fillAll(juce::Colour(0xff05080f));
        halo(g,{-82,40,244,240},accent,.77f);halo(g,{110,-87,220,245},juce::Colour(0xff818be4),.57f);
        halo(g,{175,82,176,180},accent,.38f);halo(g,{12,-35,210,170},juce::Colour(0xff75d8e7),.14f);halo(g,{51,44,175,102},juce::Colour(0xffe0f0ff),.10f);
    }
    entry.image=juce::Image(juce::Image::ARGB,280,200,true);juce::ImageConvolutionKernel blur(13);blur.createGaussianBlur(4.f);blur.applyToImage(entry.image,source,source.getBounds());entry.key=accent.getARGB();return entry.image;
}
inline void background(juce::Graphics& g,juce::Rectangle<float> area,juce::Colour accent){
    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);g.drawImage(backdrop(accent),area);
}
inline juce::Point<float> materialOrigin(const juce::Component* component){
    #if defined(NOVA_GLASS_DRAWING_PREVIEW)
    (void)component;return {};
    #else
    if(!component)return {};auto* root=component;while(root&&root->getName()!="NovaGlassCanvas")root=root->getParentComponent();
    return root?root->getLocalPoint(component,juce::Point<int>{}).toFloat():juce::Point<float>{};
    #endif
}
inline float setting(const juce::Component* component,const char* key,float fallback){
    #if !defined(NOVA_GLASS_DRAWING_PREVIEW)
    for(auto* c=component;c;c=c->getParentComponent())if(c->getName()=="NovaGlassCanvas")return static_cast<float>(c->getProperties().getWithDefault(key,fallback));
    #endif
    return fallback;
}
inline void panel(juce::Graphics& g,juce::Rectangle<float> area,juce::Colour accent,float corner=16,const juce::Component* component=nullptr){
    if(area.isEmpty())return;corner=std::min(corner,area.getHeight()*.5f);auto edge=area.reduced(.65f);
    for(int i=3;i>0;--i){g.setColour(juce::Colours::black.withAlpha(.045f));g.drawRoundedRectangle(area.translated(0,2).expanded(i*.75f),corner+i*.5f,i*1.8f);}
    {
        juce::Graphics::ScopedSaveState clip(g);juce::Path mask;mask.addRoundedRectangle(area,corner);g.reduceClipRegion(mask);
        auto origin=materialOrigin(component);g.setImageResamplingQuality(juce::Graphics::mediumResamplingQuality);g.drawImageTransformed(backdrop(accent),juce::AffineTransform::scale(4.f).translated(-origin.x,-origin.y));
        float tint=setting(component,"novaTint",.35f);g.setColour(juce::Colour(0xff080d17).withAlpha(.16f+.74f*tint));g.fillRect(area);
        juce::ColourGradient frost(juce::Colours::white.withAlpha(.19f),area.getX(),area.getY(),juce::Colours::white.withAlpha(.022f),area.getRight(),area.getBottom(),false);frost.addColour(.42,juce::Colours::white.withAlpha(.044f));g.setGradientFill(frost);g.fillRect(area);
        halo(g,{area.getX()-area.getWidth()*.18f,area.getY()-area.getHeight()*.88f,area.getWidth()*.82f,area.getHeight()*1.25f},juce::Colours::white,.17f);
        g.setColour(accent.withAlpha(.045f));g.fillRoundedRectangle(area.reduced(2),std::max(1.f,corner-2));
    }
    juce::ColourGradient rim(juce::Colours::white.withAlpha(.53f),edge.getX(),edge.getY(),juce::Colours::white.withAlpha(.055f),edge.getRight(),edge.getBottom(),false);rim.addColour(.48,juce::Colours::white.withAlpha(.14f));rim.addColour(.82,accent.withAlpha(.18f));g.setGradientFill(rim);g.drawRoundedRectangle(edge,corner,.95f);
    g.setColour(juce::Colours::white.withAlpha(.07f));g.drawRoundedRectangle(edge.reduced(1.3f),std::max(1.f,corner-1.3f),.7f);
    juce::Path reflection;reflection.startNewSubPath(edge.getX()+corner,edge.getY()+2);reflection.cubicTo(edge.getX()+area.getWidth()*.32f,edge.getY()+5,edge.getX()+area.getWidth()*.54f,edge.getY()+1,edge.getRight()-corner,edge.getY()+2);g.setColour(juce::Colours::white.withAlpha(.24f));g.strokePath(reflection,juce::PathStrokeType(1.f));
    juce::Path caustic;caustic.startNewSubPath(edge.getX()+corner,edge.getBottom()-1.4f);caustic.quadraticTo(edge.getCentreX(),edge.getBottom()-4,edge.getRight()-corner,edge.getBottom()-1.4f);g.setColour(juce::Colours::white.withAlpha(.10f));g.strokePath(caustic,juce::PathStrokeType(.8f));
}
inline void strokeGlow(juce::Graphics& g,const juce::Path& path,juce::Colour colour,float width=2,const juce::Component* component=nullptr){
    float strength=setting(component,"novaGlow",.7f);
    for(int layer=4;layer>0;--layer){g.setColour(colour.withMultipliedAlpha(strength*(.035f+(.035f*(4-layer)))));g.strokePath(path,juce::PathStrokeType(width+layer*4.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));}
    g.setColour(colour.withMultipliedAlpha(.45f*strength));g.strokePath(path,juce::PathStrokeType(width+2.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));g.setColour(colour.brighter(.35f));g.strokePath(path,juce::PathStrokeType(width,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
}
}
