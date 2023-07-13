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
#ifndef _BSNEURONRENDERER_H
#define _BSNEURONRENDERER_H

#include <sl/axis_aligned_box.hpp>


class bsDocument;
class bsSectionRenderer;
class bsBrainViewer;

typedef  sl::fixed_size_vector< sl::column_orientation, 3, unsigned char> sl_color3ub;


class bsNeuronRenderer {

public:

  bsNeuronRenderer (const bsBrainViewer *viewer);

  virtual ~bsNeuronRenderer();


public:

  void qualityNeuronRendering();
  void fastNeuronRendering();

  void renderNeurons();

  void updateNeuronColors(const std::vector<sl_color3ub>* rgb_shell,
			  const std::vector<unsigned short>* z_shell,
			  std::vector<sl_color3ub>* rgb_neuron_and_shell,
			  const std::vector<unsigned short>* z_neuron_and_shell,
			  float tau);

  inline float zetaExtinctionThresholdStart() const {
    return m_zeta_extinction_threshold[0];
  }

  inline float zetaExtinctionThresholdEnd() const {
    return m_zeta_extinction_threshold[1];
  }

  inline void setZetaExtinctionThreshold(const float v0, const float v1) {
    if (v0 >= 0.0f && v1 > v0) {
      m_zeta_extinction_threshold[0] = v0;
      m_zeta_extinction_threshold[1] = v1;
    }
  }

protected:
  const bsBrainViewer* m_viewer;
  float m_zeta_extinction_threshold[2];
};


#endif // _BSNEURONRENDERER_H

