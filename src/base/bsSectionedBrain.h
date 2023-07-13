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
// .NAME bsSectionedBrain


#ifndef _BS_SECTIONEDBRAIN_H_
#define _BS_SECTIONEDBRAIN_H_

#include <sl/fixed_size_point.hpp>
#include <sl/projective_map.hpp>
#include <sl/axis_aligned_box.hpp>
#include <vector>
#include <string>
#include <cassert>
class bsSection;


class bsSectionedBrain {

public: // Creation & Destruction

  bsSectionedBrain();

  virtual ~bsSectionedBrain();

  virtual bool invariant() const;

  void clear(); 

public: // Queries -- Box

  sl::aabox3f boundingBox() const;

public: // Queries -- Filename list

  inline const std::string& filenameItem(int i) const {
    assert(goodSectionIndex(i));
    return m_filenames[i];
  }


public: // Queries -- Region Label list

  inline int regionLabelCount() const { 
    return m_region_labels.size(); 
  }

  inline int regionVolumeCount() const {
    return m_sub_region_volumes.size(); 
  }

  inline bool goodRegionLabelIndex(int i) const {
    return i>=0 && i<regionLabelCount();
  }

  inline const std::string& regionLabelItem(int i) const {
    assert(goodRegionLabelIndex(i));
    return m_region_labels[i];
  }

  int  maxSubRegionId() const;

  int searchRegionLabel(const std::string& label) const;


public: // Queries -- Neuron Type Label list

  inline int neuronTypeLabelCount() const { 
    return m_neuron_type_labels.size(); 
  }

  inline bool goodNeuronTypeLabelIndex(int i) const {
    return i>=0 && i<neuronTypeLabelCount();
  }

  inline const std::string& neuronTypeLabelItem(int i) const {
    assert(goodNeuronTypeLabelIndex(i));
    return m_neuron_type_labels[i];
  }

  int  maxNeuronType() const;

  int searchNeuronTypeLabel(const std::string& label) const;


public: // Queries -- Sections
  
  bool regularSection(int i) const;

  inline int sectionCount() const { 
    return m_brain_2d.size(); 
  }

  inline bool goodSectionIndex(int i) const {
    return i>=0 && i<sectionCount();
  }

  inline const bsSection* sectionItem(int i) const {
    assert(goodSectionIndex(i));
    return m_brain_2d[i];
  }

  inline bsSection* sectionItem(int i) {
    assert(goodSectionIndex(i));
    return m_brain_2d[i];
  }

  float sectionQuota(int i) const;

public: // Queries Volumes

  float brainVolume() const;
  float cortexRegionVolume(int c) const;
  float subRegionVolume(int id) const;

public: // Commands Volumes

  void computeSubRegionVolumes();


public: // Queries -- Region Colors

  inline const sl::color3f regionColorItem(int i) const {
    assert(goodRegionLabelIndex(i));
    return m_region_colors[i];
  }

public: // Queries -- Neuron Colors

  inline const sl::color3f neuronTypeColorItem(int i) const {
    assert(goodNeuronTypeLabelIndex(i));
    return m_neuron_type_colors[i];
  }

public: // Neuron Type Aligned

  inline bool neuronTypeAligned(int i) const {
    assert(goodNeuronTypeLabelIndex(i));
    return m_neuron_type_aligned[i];
  }

  void setNeuronTypeAligned(int i, bool b);

public: // Neuron Type Enabled

  inline bool neuronTypeEnabled(int i) const {
    assert(goodNeuronTypeLabelIndex(i));
    return m_neuron_type_enabled[i];
  }

  void setNeuronTypeEnabled(int i, bool b);


public: // Neuron Type Dimension

  inline float neuronTypeDimension(int i) const {
    assert(goodNeuronTypeLabelIndex(i));
    return m_neuron_type_dimension[i];
  }

  std::vector<float> neuronTypeDimensions() const;

  void setNeuronTypeDimension(int i, float value);

  bool isNeuronDimensionIndependent() const;

public: // Create color
  sl::color3f createRegionColor(int i);
  sl::color3f createNeuronColor(int i);


public: // Commands -- Region Label list

  void assignDefaultRegionLabels(int nmax);
  void assignDefaultRegionLabels();

  // void assignRegionLabel(int i, const std::string& label);
  // void assignRegionLabel(int i, const std::string& label, const  sl::color3f& color);
  // void assignRegionLabel(int i, const sl::color3f& color);

  void  createRegionLabelColor(const std::string& label);
  void  assignRegionLabel(int i, const std::string& label);
  void  assignRegionColor(int i, const  sl::color3f& color);
  void  assignRegionLabelColor(int i, const std::string& label, const  sl::color3f& color);


public: // Commands -- Neuron Label list

  void assignDefaultNeuronTypeLabels(int nmax);
  void assignDefaultNeuronTypeLabels();

  // void assignNeuronTypeLabel(int i, const std::string& label, const bool b=false);
  // void assignNeuronTypeLabel(int i, const std::string& label, const  sl::color3f& color, const bool b=false);
  // void assignNeuronTypeLabel(int i, const  sl::color3f& color, const bool b=false);

void createNeuronTypeLabel(const std::string& label, const bool b=false);
void assignNeuronTypeLabel(int i, const std::string& label, const bool b=false);
void assignNeuronTypeColor(int i, const  sl::color3f& color, const bool b=false);
void assignNeuronTypeLabelColor(int i, const std::string& label,
					     const  sl::color3f& color, 
					     const bool b=false);

public: // Commands -- section

  void appendSection(bsSection* s);
  void appendSection(const std::string& fname, bsSection* s);
  void sort();
  void assignThickness();

protected:
  void appendVolume(int id, float volume);


protected:

  std::vector<std::string>        m_filenames;
  std::vector<bsSection*>   m_brain_2d;

  std::vector<std::string>        m_region_labels;
  std::vector<sl::color3f>  m_region_colors;
  std::vector<float>        m_sub_region_volumes;

  std::vector<std::string>        m_neuron_type_labels;
  std::vector<bool>         m_neuron_type_aligned;
  std::vector<bool>         m_neuron_type_enabled;
  std::vector<sl::color3f>  m_neuron_type_colors;
  std::vector<float>        m_neuron_type_dimension;

};

#endif // _BS_SECTIONEDBRAIN_H_


