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
#ifndef _BSDOCUMENT_H
#define _BSDOCUMENT_H

#include <qobject.h>
#include <qstring.h> 
#include <sl/affine_map.hpp>
#include <sl/oriented_box.hpp>

class bsSectionedBrain;
class qtCommandHistory;
class bsChooseOptions;

class bsDocument : public QObject {

    Q_OBJECT

public:

    bsDocument(QObject* parent = NULL, qtCommandHistory* cmd_processor = NULL);
    ~bsDocument();

    bsSectionedBrain* getBrain2D();
    void clear();

    void setErrorMessage(int error, const char* message);

    inline QString lastMessage() const {
	return m_message;
    }

    inline int lastError() const {
	return m_error;
    }
   
    void transformSection(int i, const sl::affine_map2f& t);

    void setRegionId(int section, int i, int j, int id);
    void emitBrain2DChanged(int i);

    inline bool brain2DModified() const {
	return m_brain2D_modified;
    }

    inline bool brain2DSaved() const {
	return m_brain2D_saved;
    }
    
    inline bool hasBrain2D() const {
	return m_brain2D != NULL;
    }

    inline void setBorderSize(const float f) {
      m_bordersize = f;
    }

    inline float getBorderSize() const {
      return m_bordersize;
    }

    inline void setNeuronSize(const float f) {
      m_neuronsize = f;
    }

    inline float getNeuronSize() const {
      return m_neuronsize;
    }

    void setNeuronDimension(int i, float value);

    float getNeuronDimension(int i) const;

    bool isNeuronDimensionIndependent() const;

    // Current filename

    inline QString currentFilename() const {
      return m_current_filename;
    }

    inline void setCurrentFilename(const QString s) {
      m_current_filename=s;
    }

    //Cutting Box Query

    sl::obox3f cuttingBox() const {
      return m_cutting_box;
    }


    // Retriangulate section i
    void retriangulate(int i);

    void setRegionOptions(const bsChooseOptions* r);

    void setNeuronTypeOptions(const bsChooseOptions* r);


public slots:
    void setBrain2D(bsSectionedBrain* b);
    void loadInside(QString filename);
    void saveInside(QString filename);
    void loadXml(QString filename);
    void saveXml(QString filename);
    void loadSettings(QString filename);
    void saveSettings(QString filename);
    void computeSubRegionVolumes();
    void setCuttingBox(sl::obox3f& cbox);
    void setGhostSection(int n, bool b);

signals:
    void errorOn();
    void newBrain2D(bool);
    void brain2DChanged(int);
    void regionOptionsChanged();
    void neuronTypeOptionsChanged();
    void retriangulateSection(int);
    void newCuttingBoxLoaded();

 
private:
    bsSectionedBrain* m_brain2D;
    bool m_brain2D_modified;
    bool m_brain2D_saved;
    qtCommandHistory* m_cmd_processor;

    QString m_current_filename;

    QString m_message;
    int m_error;

    float m_bordersize;
    float m_neuronsize;

    sl::obox3f m_cutting_box;

};


#endif
