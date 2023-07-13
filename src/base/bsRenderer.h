//+++HDR+++
//======================================================================
//   This file is part of the BRAIN software framework.
//   Copyright (C) 2009-2022 by CRS4, Pula, Italy.
//
//   For more information, visit the CRS4 Visual Computing Group
//   web pages at http://www.crs4.it/vic/
//
//   This file may be used under the terms of the GNU General Public
//   License as published by the Free Software Foundation and appearing
//   in the file LICENSE included in the packaging of this file.
//
//   CRS4 reserves all rights not expressly granted herein.
//  
//   This file is provided AS IS with NO WARRANTY OF ANY KIND, 
//   INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS 
//   FOR A PARTICULAR PURPOSE.
//
//======================================================================
//---HDR---//
// .NAME bsRenderer

#ifndef _BS_RENDERER_H_
#define _BS_RENDERER_H_

#include "bsGeometryHelpers.h"
#include <sl/fixed_size_vector.hpp>
#include <vector>
#include <cassert>

#define signf(x) ((x>=0.0f) ? 1.0f : -1.0f)

template <class T>
class bsRenderer {

public:

  bsRenderer() {
    resetDefaults();
  }

  virtual ~bsRenderer() {
  }
  
public: // render

  virtual void render(const T* s) = 0; 

public: // defaults

  void resetDefaults() {
    resetEnableDefaults();
    resetColorDefaults();
  }

  void resetEnableDefaults() {
    setEnableInternal(true);
    setEnableExternal(true);
    setEnableSubRegions(true);
    setEnableNeuronTypes(true);
  }

  void resetColorDefaults() {
    assignDefaultExternalColor();
    assignDefaultInternalColor();
    assignDefaultSubRegionColors();
    assignDefaultNeuronTypeColors();
  }
  
public: // enable_external
  
  inline bool enableExternal() const {
    return(m_enable_external);
  }

  inline void setEnableExternal(bool b) {
    m_enable_external = b;
  }

public: // enable_internal

  inline bool enableInternal() const {
    return(m_enable_internal);
  }
  
  inline void setEnableInternal(bool b) {
    m_enable_internal = b;
  }
  
public: // enable_subregions
  
  inline bool enableSubRegions() const {
    return(m_enable_subregions);
  }
  
  inline void setEnableSubRegions(bool b) {
    m_enable_subregions = b;
  }
    
public: // enable_neuron_types
  
  inline bool enableNeuronTypes() const {
    return(m_enable_neuron_types);
  }
  
  inline void setEnableNeuronTypes(bool b) {
    m_enable_neuron_types = b;
  }

public: // m_external_color
  
  inline sl::color3f externalColor() const {
    return(m_external_color);
  }
  
  inline void setExternalColor(const sl::color3f& c) {
    m_external_color = c;
  }

  inline void assignDefaultExternalColor() {
      sl::color3f c; c= 0.5f, 0.5f, 0.4f;
      setExternalColor(c);
  }

public: // m_internal_color
  
  inline sl::color3f internalColor() const {
    return(m_internal_color);
  }
  
  inline void setInternalColor(const sl::color3f& c) {
    m_internal_color = c;
  }

  inline void assignDefaultInternalColor() {
      sl::color3f c; c = 0.8f, 0.8f, 0.7f;
      setInternalColor(c);
  }

public: // subregions

  inline bool goodSubRegionColorIndex(int i) const {
    return i>=0 && i<subRegionColorCount();
  }

  inline int subRegionColorCount() const {
    return m_subregion_colors.size();
  }

  inline sl::color3f subRegionColor(int i) const {
    assert(goodSubRegionColorIndex(i));
    return m_subregion_colors[i];
  }
  
  inline void setSubRegionColor(int i, const sl::color3f& c) {
    assert(goodSubRegionColorIndex(i));
    m_subregion_colors[i] = c;
  }
  
  inline void setSubRegionColors(const std::vector<sl::color3f>& c) {
    m_subregion_colors = c;
  }

  void resizeSubRegionColors(int new_count) {
    assert(new_count >= 0);
    int old_count = subRegionColorCount();
    m_subregion_colors.resize(new_count);

    for (int i= old_count; i< new_count; i++) {
      m_subregion_colors[i]=createNewColor(m_subregion_colors,i,old_count,new_count);
    }
  }

  void assignDefaultSubRegionColors() {
    int old_count = subRegionColorCount();
    resizeSubRegionColors(0);
    resizeSubRegionColors(old_count);
  }

public: // neuronTypes

  inline bool goodNeuronTypeColorIndex(int i) const {
    return i>=0 && i<neuronTypeColorCount();
  }

  inline int neuronTypeColorCount() const {
    return m_neuron_type_colors.size();
  }

  inline sl::color3f neuronTypeColor(int i) const {
    assert(goodNeuronTypeColorIndex(i));
    return m_neuron_type_colors[i];
  }

  inline bool neuronTypeAligned(int i) const {
    assert(goodNeuronTypeColorIndex(i));
    return m_neuron_type_aligned[i];
  }

  inline float neuronTypeDimension(int i) const {
    assert(goodNeuronTypeColorIndex(i));
    return m_neuron_type_dimension[i];
  }

  inline bool neuronTypeEnabled(int i) const {
    assert(goodNeuronTypeColorIndex(i));
    return m_neuron_type_enabled[i];
  }
  
  inline void setNeuronTypeColor(int i, const sl::color3f& c) {
    assert(goodNeuronTypeColorIndex(i));
    m_neuron_type_colors[i] = c;
  }

  inline void setNeuronTypeAligned(int i, const bool b) {
    assert(goodNeuronTypeColorIndex(i));
    m_neuron_type_aligned[i] = b;
  }
  
  inline void setNeuronTypeEnabled(int i, const bool b) {
    assert(goodNeuronTypeColorIndex(i));
    m_neuron_type_enabled[i] = b;
  }

  inline void setNeuronTypeDimension(int i, const float value) {
    assert(goodNeuronTypeColorIndex(i));
    m_neuron_type_dimension[i] = value;
  }

  inline bool isNeuronDimensionIndependent() const {
    bool result = false;
    for (int i=0; i< (int)m_neuron_type_dimension.size() && !result; i++) {
      result = result || (m_neuron_type_dimension[i] != 0.0f);
    }
    return result;
  }
  
  inline void setNeuronTypeColors(const std::vector<sl::color3f>& c) {
    m_neuron_type_colors = c;
  }

  inline void setNeuronTypeAlignedFlags(const std::vector<bool>& b) {
    m_neuron_type_aligned = b;
  }

  inline void setNeuronTypeEnabledFlags(const std::vector<bool>& b) {
    m_neuron_type_enabled = b;
  }

  inline void setNeuronTypeDimensions(const std::vector<float>& b) {
    m_neuron_type_dimension = b;
  }

  void resizeNeuronTypeColors(int new_count) {
    assert(new_count >= 0);
    int old_count = neuronTypeColorCount();
    m_neuron_type_colors.resize(new_count);
    m_neuron_type_aligned.resize(new_count);
    m_neuron_type_enabled.resize(new_count);
    m_neuron_type_dimension.resize(new_count);
    for (int i= old_count; i< new_count; i++) {
      m_neuron_type_colors[i]=createNewColor(m_neuron_type_colors,i,old_count,new_count);
      m_neuron_type_aligned[i]=false;
      m_neuron_type_enabled[i]=true;
      m_neuron_type_dimension[i]=0.0f;
    }
  }

  void assignDefaultNeuronTypeColors() {
    int old_count = neuronTypeColorCount();
    resizeNeuronTypeColors(0);
    resizeNeuronTypeColors(old_count);
  }

protected:

  inline sl::color3f createNewColor(const std::vector<sl::color3f>& v, int i, int old_count, int new_count) {
    float value = 1.0f;
    float hue = 12.38f;
    float saturation = 1.0f;
    float step =0.0f;
    float lum = 1.0f;
    if (new_count > old_count) {
      step = 360.0f/(new_count-old_count);
      if (old_count > 0) {
	value = bsGeometryHelpers::rgb2hsv(v[old_count-1])[2];
	hue = bsGeometryHelpers::rgb2hsv(v[old_count-1])[0];
      }
    }
    value = value;
    hue= (i%2==0) ? hue+step*(i-old_count+1) :  hue+step*(i-old_count+1)+180.0f;
    while ( hue >= 360.0f ) {
      hue = hue - 360.0f;
    }

    // hue NOT red
    if (std::fabs(hue) <= 5.0f) {
      hue = hue+signf(hue)*5.0f;
    }

    // hue NOT yellow
    if (std::fabs(hue-60.0f) <= 30.0f) {
      hue = hue-30.0f;
    }
    
    sl::color3f hsv; hsv = hue,saturation,value;
    sl::color3f rgb; rgb = bsGeometryHelpers::hsv2rgb(hsv);

    // setting luminance
    lum = bsGeometryHelpers::luminance(rgb);
    value = (lum*0.90f+(1.0f-lum)*0.10f)*value;
    hsv = hue,saturation,value;
    rgb = bsGeometryHelpers::hsv2rgb(hsv);
    return rgb;
  }

  bool m_enable_external;
  bool m_enable_internal;
  bool m_enable_subregions;
  bool m_enable_neuron_types;
  
  sl::color3f m_external_color;
  sl::color3f m_internal_color;
    
  std::vector<sl::color3f> m_subregion_colors;
  std::vector<sl::color3f> m_neuron_type_colors;    
  std::vector<bool> m_neuron_type_aligned;    
  std::vector<bool> m_neuron_type_enabled;
  std::vector<float> m_neuron_type_dimension;
    
  
}; // bsRenderer


#endif // _BS_RENDERER_H_
