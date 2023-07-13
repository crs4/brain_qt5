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
// .NAME bsSectionRenderer


#ifndef _BS_SECTION_RENDERER_H_
#define _BS_SECTION_RENDERER_H_

#include "bsRenderer.h"

class bsSection;
class bsCortexRegion;
class bsSubRegion;

class bsSectionRenderer: public bsRenderer<bsSection> {

public:

  bsSectionRenderer();

  virtual ~bsSectionRenderer();
  
public: // render

  virtual void render(const bsSection* s); 
  void setNeuronSize(float neuronsize);
  void setBorderSize(float bordersize);

  inline bool stripeRendering() const {
    return m_stripe_rendering;
  }

  inline bool fillRendering() const {
    return m_fill_rendering;
  }

  inline void setStripeRendering(bool b) {
    m_stripe_rendering = b;
  }

  inline void setFillRendering(bool b) {
    m_fill_rendering = b;
  }

  inline int stepSize() const {
    return m_step_size;
  }

  inline void setStepSize(int val) {
    m_step_size = val;
  }
 
public:
  virtual void renderNeurons(const bsSection* s);

protected:
  void renderVertex(const sl::column_vector3f& vdown,
		    const sl::column_vector3f& vup,
		    const sl::point3f& pA, 
		    const sl::point3f& p, 
		    const sl::point3f& pB);
  virtual void renderBoundary(const bsCortexRegion* s);
  virtual void renderHole(const bsCortexRegion* s, int nh);
  virtual void renderSubRegion(const bsSubRegion* s);
  
  bool m_lighted_rendering;
  bool m_fill_rendering;
  bool m_stripe_rendering;
  int m_step_size;

}; // bsSectionRenderer


#endif // _BS_SECTION_RENDERER_H_
